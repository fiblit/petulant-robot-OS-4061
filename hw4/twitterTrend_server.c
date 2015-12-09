/* CSci4061 Assignment 4
* name: Dalton Hildreth, Timothy Kohn
* id: hildr039, kohnx074 */

#include "twitterTrend_server.h"

/* does initial processing, in particular, it starts the threads */
int main( int argc, char *argv[] ) {

	/* get args */
	int num_threads;
	int publicServerPort;
	if ( argc != 3 && argc != 2) { // invalid range
		fprintf( stderr, "Incorrect usage.\n" );
		fprintf( stderr, "\tUsage: %s <port_number> [num_threads = 5]\n", argv[ 0 ] );
		return 1;
	}
	else {
		if (argc == 2) { //optional param excluded
			num_threads = 5; //default value
		}
		else { //optional param included
			num_threads = atoi( argv[ 2 ] );
			//num_threads should be > 0
			if ( num_threads <= 0) {
				errno = EINVAL;
				errorFunction( "num_threads is less than or equal to 0, exiting" );
			}
		}
		publicServerPort = atoi( argv[ 1 ] );
	}

	/* init public server socket */
	serverSocket = socket( AF_INET, SOCK_STREAM, 0 );//SOCK_STREAM = TCP protocol
	if (serverSocket == -1)
		errorFunction( "Failed to create server socket" );//TODO: change error____ to handleError(errno,"___",void *args), which is in a file that handles errors

	/* bind socket to port and listen */
	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons((short)publicServerPort);
	serverAddr.sin_addr.s_addr = htonl( INADDR_ANY );
	if ( bind( serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr) ) == -1)
		errorFunction( "Failed to bind to given port" );
	if ( listen( serverSocket, 50 )  == -1)//TODO: change backlog amount to whatever it needs to be. (100?)
		errorFunction( "Failed to listen on socket" );
	printf( "Listening on port %d\n", publicServerPort);

	/* init data structures */
	readTwitterDB();
	queue = queue_construct();
	globalQueue = false;//TODO: Rename this dumb variable

	/* init semaphores */
	if ( sem_init( &full_slots, 0, 0 ) != 0 ) {
		perror( "Semaphore initialization failed" );
		return 1;
	}
	/*deprecated: unbounded
	if ( sem_init( &empty_slots, 0, num_threads ) != 0 ) {//the buffer/queue is bounded by num_threads
		perror( "Semaphore initialization failed" );
		return 1;
	}
	*/
	if ( sem_init(&mut, 0, 1) != 0) {
		perror( "Semaphore initialization failed" );
		return 1;
	}

	/* make and run threads */
	pthread_t *processerThreads = ( pthread_t * ) malloc( sizeof( pthread_t ) * num_threads );
	if ( processerThreads == NULL ) { //malloc error checking
		errorFunction ( "Call to malloc failed in main" );
	}

	pthread_t queueerThread;
	int *ids = ( int * ) malloc( sizeof( int ) * ( num_threads + 1 ) );
	if ( ids == NULL ) { //malloc error checking
		errorFunction ( "Call to malloc failed in main" );
	}

	for ( int i = 0; i < num_threads + 1; i++ ) {
		ids[ i ] = i + 1;
	}
	int id;
	for (id = 0; id < num_threads; id++) {
		if ( pthread_create( &processerThreads[ id ], NULL, processer, &ids[ id ]) != 0 ) {
			fprintf( stderr, "Failed to create processer thread, ID:%d : %s\n", ids[ id ], strerror( errno ) );
		}
		else {
			//fprintf( stderr, "Created thread %d\n", ids[ id ] );
		}
	}
	if ( pthread_create( &queueerThread, NULL, queueer, &ids[ id ]) != 0 ) {
		fprintf( stderr, "Failed to create queueer thread, ID:%d : queueer : %s\n", num_threads + 1, strerror( errno ) );
	}
	else {
		//fprintf( stderr, "Created thread %d : queueer\n", num_threads + 1);
	}

	/* join threads */
	if ( pthread_join( queueerThread, NULL ) != 0 ) {
		fprintf( stderr, "Failed to join queueer thread, ID:%d : %s\n", num_threads + 1, strerror( errno ) );
	}
	else {
		//fprintf( stderr, "Joined thread %d : queueer\n", num_threads + 1);
	}
	for ( id = 0; id < num_threads; id++ ) {
		if ( pthread_join( processerThreads[ id ], NULL) != 0 ) {
			fprintf( stderr, "Failed to join processer thread, ID:%d : %s\n", ids[ id ], strerror( errno ) );
		}
		else {
			//fprintf( stderr, "Joined thread %d\n", ids[ id ] );
		}
	}

	fclose( inFile );
	TwitterDBMem_destruct( tdbm );
	queue_destruct( queue );
	free( processerThreads );
	free( ids );

	return 0;
}

void readTwitterDB() {
	FILE *twitterFile = fopen( TWITTERDB_FILENAME, "r" );
	tdbm = TwitterDBMem_construct( twitterFile );
	fclose( twitterFile );
}

void openInFile( char *inFileName ) {
	inFile = fopen( inFileName, "r" );
	if (inFile == NULL) {
		perror( "Failed to open *.in file" );
		exit( EXIT_FAILURE );
	}
}

void *processer( void *args ) {
	int id = *((int *) args);
	//int cityLength;
	//int lastCharOfCity;
	int semValue;
	//FILE *cityFile;
/*
	char *lineAfterCityName = ( char * ) malloc ( sizeof ( char ) * 85 ); //will be contents of cityLine after the cityName
 	if ( lineAfterCityName == NULL ) { //malloc error checking
		errorFunction ( "Call to malloc failed in processer" );
	}
*/

	char *cityLine = ( char * ) malloc ( sizeof ( char ) * 100 ); //every line in TwitterDB is less than 100 characters
	if ( cityLine == NULL ) { //malloc error checking
		errorFunction ( "Call to malloc failed in processer" );
	}

	queueClientInfo_t processerClient;

	char *originalFileName = ( char * ) malloc ( sizeof ( char ) * 15); //should be big enough for the name "clientX.txt"
	if ( originalFileName == NULL ) { //malloc error checking
		errorFunction ( "Call to malloc failed in processer" );
	}

	while ( 1 ) { //test if the queue has anything in it

		//keep dequeueing until we break
		if ( sem_wait ( &full_slots ) != 0 ) {
			perror( "Error occured while processer was waiting" );
		}

		if ( sem_getvalue ( &full_slots, &semValue ) != 0 ) {
			perror ( "Error occured during sem_getvalue in processer" );
		}
		if ( globalQueue && (semValue == 0) ) {

			//continue ending threads
			if ( sem_post( &full_slots ) != 0 ) {
				perror( "Error occured while posting to a semaphore" );
			}
			break;
		}

		//lock access to queue
		if ( sem_wait( &mut ) != 0 ) {
			perror( "Error occured while waiting for semaphore lock" );
			exit( EXIT_FAILURE );
		}

		//get clientInfo
		processerClient = queue_dequeue ( queue );

		//release access to queue
		if ( sem_post( &mut ) != 0 ) {
			perror( "Error occured while releasing semaphore lock" );
			exit( EXIT_FAILURE );
		}

		// get convenient strings for various output. clientAddrPort is the 127.0.0.1,XXXXX format
		uint32_t clientAddrInt = htonl( processerClient->address.sin_addr.s_addr );
		uint16_t clientPortInt = processerClient->address.sin_port;
		char * clientAddr = inet_ntoa( *(struct in_addr *)&clientAddrInt );
		char clientPort[ 6 ]; //6 since largest 16 bit unsigned is 65535 which is 5 chars + 1 for null
		sprintf( clientPort, "%u", clientPortInt );
		char * clientAddrPort = (char *) malloc( sizeof( char ) * (strlen(clientAddr) + 1 + 6) );//the 6 includes a null char, the 1 is a comma
		clientAddrPort = strcat( strcat( strcat( clientAddrPort, clientAddr ), ","), clientPort );

		/* handshake with client STEP 5/6 */
		if (serverHandShake( processerClient->socket, clientAddrPort ) == -1) {
			fprintf(stderr, "server failed to complete handshake\n" );
			break;//TODO: wait, I need to take the client off of the queue
		}

		//strcpy ( originalFileName, processerFileName );//#EXPECTED TO BREAK. NOT DEALT WITH YET# //to makesure finished message outputs correctly;

		/* I am handling */
		printf( "Thread %d is handling client %s\n", id, clientAddrPort );

		while ( 1 ) {//while requests

			/* Step #7: wait for request*/
			message_t request = construct_message_blank();
			if (recvMessage( processerClient->socket, request ) == -1) {
				destruct_message(request);
				break;
			}

			message_t response = construct_message_blank();
			if (request->id == ERRMSG) {
				printf( "server detected a client error: %s, from client %s\n\tclosing connection\n", request->payload, clientAddrPort);
				destruct_message(request);
				destruct_message(response);
				break;
				//goes on to close client
			}
			else if (request->id != ENDREQ) {
				destruct_message(request);
				destruct_message(response);
				break;
				//goes on to close client
			}
			else if (request->id != REQUEST) {
				printf( "server detected client malfunction from client %s\n\tclosing connection\n", clientAddrPort);
				response->payload = "Invalid message type: please request";
				response->length = strlen(response->payload) + 1;
				if(sendMessage( processerClient->socket, response ) == -1) {
					printf( "server failed to notify client %s of its malfunction, continuing with close\n", clientAddrPort );
				}
				destruct_message(request);
				destruct_message(response);
				break;
				//goes on to close client
			}
			//otherwise successfull request

			//store city request
			char * cityBuf = (char *) malloc( sizeof( char ) * request->length );

			/* Step #8: find keywords */
			cityLine = TwitterDBMem_getCityKwd( tdbm, cityBuf );

			response->id = RESPONSE;
			if (cityLine == NULL) {
				response->payload = (char *) malloc( sizeof( char ) * 3 );
				strcpy( response->payload, "NA");
				response->payload[2] = '\0';
				response->length = 3;//+1 for \0
			}
			else {
				response->length = strlen( cityLine ) + 1;
				response->payload = (char *) malloc( sizeof( char ) * response->length );
				strcpy( response->payload, cityLine );
			}

			/* Step #9: send back 3 keywords followed by end of response message */
			//message response
			if(sendMessage( processerClient->socket, response ) == -1) {
				printf( "server malfunction during response send to client %s\b", clientAddrPort );
				break;
				//goes on to close client
			}
			printf("server sends twitterTrend response: %s\n", build_string_message( response ) );

			//message ENDOFRES
			clean_message( response );
			response->id = ENDRES;
			response->length = 0;
			if(sendMessage( processerClient->socket, response ) == -1) {
				printf("server malfunction during end of response send to client %s\n", clientAddrPort);
				break;
				//goes on to close client
			}
			printf("server sends end of response: %s\n", build_string_message( response ) );
		}

		/* Step #11: close the connection with client */
		printf( "Thread %d is finished handling client %s\n", id, clientAddrPort );
		printf( "server closes the connection from client %s\n", clientAddrPort );
		close( processerClient->socket );
	}

	return NULL;
}

void *queueer( void *args ) {
	//int id = *((int *) args);//note: id is never used

	//Keep enqueueing until we break from having no more clients? (<- not sure when this happens)
	while ( 1 ) {

		/*accept client*/
		struct sockaddr_in client;
		int nextClientSocket;
		int lenOfClientAddr = sizeof( struct sockaddr * );

		int acceptRet;
		while ((acceptRet = accept( serverSocket, (struct sockaddr *)&client, (socklen_t *)&lenOfClientAddr) == -1) &&
			(errno == EINTR))//this loop was influenced by the book p.637. I wouldn't have checked for EINTR and looped before
		if (acceptRet == -1) {

			//failed to accept try again
			continue;
		}
		else {
			nextClientSocket = acceptRet;
		}

		/*output message for acceptance*/
		{
			uint32_t ip = ( client.sin_addr.s_addr );//I just wanted to localize this temporary variable
			printf( "server accepts connection from %s\n", inet_ntoa( *(struct in_addr *)&ip));
		}
		/* note to self: how to break out
			//this is a nasty hack
			if ( sem_post( &full_slots ) != 0 ) {
				errorFunction( "Error occurred while posting to a semaphore" );
			}
			break;//no more items to add
		*/

		//Lock access to queue
		if ( sem_wait( &mut ) != 0 ) {//TODO: turn these validators into functions (very very very low priority)
			errorFunction( "Error occured while acquiring semaphore lock" );
		}

		//place client on queue
		queue_enqueue( queue, nextClientSocket, client );

		//release access to queue
		if ( sem_post( &mut ) != 0 ) {
			errorFunction( "Error occured while releasing semaphore lock" );
		}

		//post that there is another full slot
		if ( sem_post( &full_slots ) != 0 ) {
			errorFunction( "Error occured while posting to a semaphore" );
		}
	}

	return NULL;//Nothing to return
}
