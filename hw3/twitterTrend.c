/* CSci4061 Assignment 3
* name: Dalton Hildreth, Timothy Kohn
* id: hildr039, kohnx074 */

#include "twitterTrend.h"

/* does initial processing, in particular, it starts the threads */
int main( int argc, char *argv[] ) {

	/* get args */
	if ( argc != 3 ) {
		fprintf( stderr, "Incorrect usage.\n" );
		fprintf( stderr, "\tUsage: %s <*.in file> <num_threads>\n", argv[ 0 ] );
		return 1;
	}
	char *inFileName = argv[ 1 ];
	int num_threads = atoi( argv[ 2 ] );
	if ( num_threads <= 0) {  //num_threads should be > 0
		errno = EINVAL;
		errorFunction ( "num_threads is less than or equal to 0, exiting");
	}

	/* init data structures */
	openInFile( inFileName );
	readTwitterDB();
	queue = queue_construct();
	globalQueue = false;

	/* init semaphores */
	if ( sem_init( &full_slots, 0, 0 ) != 0 ) {
		perror( "Semaphore initialization failed" );
		return 1;
	}
	if ( sem_init( &empty_slots, 0, num_threads ) != 0 ) {//the buffer/queue is bounded by num_threads
		perror( "Semaphore initialization failed" );
		return 1;
	}
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
	int cityLength;
	int lineAfterCityNameLength;
	int lastCharOfCity;
	int semValue;
	FILE *cityFile;
	FILE *resultFile;
	char *cityBuf = ( char * ) malloc ( sizeof ( char ) * 16 ); //cityNames will be less than 15 characters
	if ( cityBuf == NULL ) { //malloc error checking
		errorFunction ( "Call to malloc failed in processer" );
	}

	char *cityLine = ( char * ) malloc ( sizeof ( char ) * 100 ); //every line in TwitterDB is less than 100 characters
	if ( cityLine == NULL ) { //malloc error checking
		errorFunction ( "Call to malloc failed in processer" );
	}

	char *lineAfterCityName = ( char * ) malloc ( sizeof ( char ) * 85 ); //will be contents of cityLine after the cityName
	if ( lineAfterCityName == NULL ) { //malloc error checking
		errorFunction ( "Call to malloc failed in processer" );
	}

	char *processerFileName = ( char * ) malloc ( sizeof ( char ) * 15); //should be big enough for the name "clientX.txt"
	if ( processerFileName == NULL ) { //malloc error checking
		errorFunction ( "Call to malloc failed in processer" );
	}

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

		processerFileName = queue_dequeue ( queue ); //store the file name of the client

		//release access to queue
		if ( sem_post( &mut ) != 0 ) {
			perror( "Error occured while releasing semaphore lock" );
			exit( EXIT_FAILURE );
		}

		strcpy ( originalFileName, processerFileName ); //to makesure finished message outputs correctly
;		printf( "Thread %d is handling client %s\n", id, processerFileName );

		//open file to get city name
		if ( processerFileName == NULL ) { //check to make sure something is in processerFileName
			perror( "ProcesserFileName is NULL, fopen would have segfaulted" );
			exit( EXIT_FAILURE );
		}
		else {
			if ( access ( processerFileName, F_OK ) != -1 ) {  //check if file exists, if it does open it
				cityFile = fopen ( processerFileName, "r" );
			} else { //it does not exist
				errorFunction ( "Error, attempting to open nonexistent file" );
			}
		}
		fgets ( cityBuf, MAXCITYNAMELENGTH, cityFile );
		fclose ( cityFile );
		lastCharOfCity = strlen( cityBuf );
		if (cityBuf[ lastCharOfCity - 1 ] == '\n') {
			cityBuf[ lastCharOfCity - 1 ] = '\0';
		}


		//will stick the city's line in cityLine if it exists
		cityLine = TwitterDBMem_getCityKwd( tdbm, cityBuf );
		if ( !( cityLine ) ) {
			fprintf ( stderr, "City %s does not exist\n", cityBuf );
			exit( EXIT_FAILURE );
		}
		cityLength = strlen ( cityBuf );
		strncpy ( lineAfterCityName, cityLine + cityLength + 1, ( 100 - cityLength )); //+1 for the extra comma

		//free( cityLine );

		//create result file
		strcat( processerFileName, ".result" ); //create name of result file
		resultFile = fopen ( processerFileName, "w+" ); //w+ mode will create the file

		//write the city name, then put the : and two spaces in, then put the lineAfterCityName in, then add \n
		lineAfterCityNameLength = strlen ( lineAfterCityName );
		fwrite ( cityBuf, sizeof ( char ), cityLength, resultFile );
		fputc ( ' ', resultFile );
		fputc ( ':', resultFile );
		fputc ( ' ', resultFile );
		fwrite ( lineAfterCityName, sizeof ( char ), lineAfterCityNameLength, resultFile );
		fputc ( '\n', resultFile );
		fclose ( resultFile );

		printf( "Thread %d is finished handling client %s\n", id, originalFileName );

		//post that there is another empty slot
		if ( sem_post( &empty_slots ) != 0 ) {
			perror( "Error occured while posting to a semaphore" );
			exit( EXIT_FAILURE );
		}
	}
	
	return NULL;
}

void *queueer( void *args ) {
	int id = *((int *) args);

	//Keep enqueueing until we break from having no more items
	while ( 1 ) {

		//test if the queue has slots to fill
		if ( sem_trywait( &empty_slots ) != 0 && errno == EAGAIN ) {
			if ( errno == EAGAIN ) { //we're just at 0
				fprintf( stderr, "Waiting to add items to the full queue\n" );
				if (sem_wait( &empty_slots ) != 0 ) {
					fprintf( stderr, "ID:%d", id);//just so id is used once
					perror( "An error occured while the queueer was waiting" );
					exit( EXIT_FAILURE );
				}
			}
			else { //real error occured
				perror( "An error occured while the queueer was trying to wait" );
				exit( EXIT_FAILURE );
			}
		}

		// TODO: <insert semaphore wait to inFile>

		char c = fgetc( inFile );
		int itemLength = 1;
		while( c != '\n' && c != EOF ) {
			c = fgetc( inFile );
			itemLength++;
		}
		if(c != EOF)
			fseek( inFile, -itemLength, SEEK_CUR);

		/* put next item on queue */
		char *newItem = ( char * ) malloc( sizeof( char ) * ( itemLength + 1 ) );
		if ( newItem == NULL ) { //malloc error checking
			errorFunction ( "Call to malloc failed in queueer" );
		}

		//get next line or EOF
		if ( fgets( newItem, itemLength + 1, inFile ) == NULL ) {
			globalQueue = true;

			//release access to queue
			if ( sem_post( &mut ) != 0 ) {
				perror( "Error occured while releasing semaphore lock" );
				exit( EXIT_FAILURE );
			}

			//this is a nasty hack
			if ( sem_post( &full_slots ) != 0 ) {
				perror( "Error occurred while posting to a semaphore" );
				exit( EXIT_FAILURE );
			}
			break;//no more items to add
		}

		//TODO: <insert semaphore post to inFile>

		//delete \n char
		int newItemLen = strlen( newItem );
		if (newItem[ newItemLen - 1 ] == '\n') {
			newItem[ newItemLen - 1 ] = '\0';
		}

		//Lock access to queue
		if ( sem_wait( &mut ) != 0 ) {//TODO: turn these validators into functions
			perror( "Error occured while acquiring semaphore lock" );
			exit( EXIT_FAILURE );
		}
		queue_enqueue( queue, newItem );

		free( newItem );

		//release access to queue
		if ( sem_post( &mut ) != 0 ) {
			perror( "Error occured while releasing semaphore lock" );
			exit( EXIT_FAILURE );
		}

		//post that there is another full slot
		if ( sem_post( &full_slots ) != 0 ) {
			perror( "Error occured while posting to a semaphore" );
			exit( EXIT_FAILURE );
		}
	}

	return NULL;//Nothing to return
}
