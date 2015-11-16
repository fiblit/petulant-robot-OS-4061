/* CSci4061 Assignment 3
* name: Dalton Hildreth, Timothy Kohn
* id: hildr039, kohnx074 */

#include "twitterTrend.h"

/* does initial processing, in particular, it starts the threads */
int main( int argc, char *argv[] ) {

	/* get args */
	if (argc != 3) {
		fprintf( stderr, "Incorrect usage.\n" );
		fprintf( stderr, "\tUsage: %s <*.in file> <num_threads>\n", argv[ 0 ] );
		return 1;
	}
	char *inFileName = argv[ 1 ];
	int num_threads = atoi( argv[ 2 ] );

	/* init data structures */
	openInFile( inFileName );
	readTwitterDB();
	queue = queue_construct();
	globalQueue = false;

	/* init semaphores */
	if (sem_init(&full_slots, 0, 0) != 0) {
		perror( "Semaphore initialization failed" );
		return 1;
	}
	if (sem_init(&empty_slots, 0, num_threads) != 0) {//the buffer/queue is bounded by num_threads
		perror( "Semaphore initialization failed" );
		return 1;
	}
	if (sem_init(&mut, 0, 1) != 0) {
		perror( "Semaphore initialization failed" );
		return 1;
	}

	/* make and run threads */
	pthread_t *processerThreads = (pthread_t *) malloc( sizeof( pthread_t ) * num_threads );
	pthread_t queueerThread;
	int *ids = (int *) malloc( sizeof( int ) * (num_threads + 1) );
	for (int i = 0; i < num_threads + 1; i++) {
		ids[ i ] = i + 1;
	}
	int id;
	for (id = 0; id < num_threads; id++) {
		if (pthread_create( &processerThreads[ id ], NULL, processer, &ids[ id ]) != 0 ) {
			fprintf( stderr, "Failed to create processer thread, ID:%d : %s\n", ids[ id ], strerror( errno ) );
		}
		else {
			fprintf( stderr, "Created thread %d\n", ids[ id ] );
		}
	}
	if (pthread_create( &queueerThread, NULL, queueer, &ids[ id ]) != 0 ) {
		fprintf( stderr, "Failed to create queueer thread, ID:%d : queueer : %s\n", num_threads + 1, strerror( errno ) );
	}
	else {
		fprintf( stderr, "Created thread %d : queueer\n", num_threads + 1);
	}

	/* join threads */
	if (pthread_join( queueerThread, NULL ) != 0) {
		fprintf( stderr, "Failed to join queueer thread, ID:%d : %s\n", num_threads + 1, strerror( errno ) );
	}
	else {
		fprintf( stderr, "Joined thread %d : queueer\n", num_threads + 1);
	}
	for (id = 0; id < num_threads; id++) {
		if (pthread_join( processerThreads[ id ], NULL) != 0) {
			fprintf( stderr, "Failed to join processer thread, ID:%d : %s\n", ids[ id ], strerror( errno ) );
		}
		else {
			fprintf( stderr, "Joined thread %d\n", ids[ id ] );
		}
	}

	fclose( inFile );
	TwitterDBMem_destruct( tdbm );
	queue_destruct( queue );

	return 0;
}

void readTwitterDB() {
	FILE *twitterFile = fopen( TWITTERDB_FILENAME, "r" );
	tdbm = TwitterDBMem_construct( twitterFile );
	fclose( twitterFile );
}

void openInFile(char *inFileName) {
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
	char *cityLine = ( char * ) malloc ( sizeof ( char ) * 100 ); //every line in TwitterDB is less than 100 characters
	char *lineAfterCityName = ( char * ) malloc ( sizeof ( char ) * 85 ); //will be contents of cityLine after the cityName
	char *processerFileName = ( char * ) malloc ( sizeof ( char ) * 100); //should be big enough for the name "clientX.txt"
	char *originalFileName = ( char * ) malloc ( sizeof ( char ) * 100); //should be big enough for the name "clientX.txt"

	sem_getvalue ( &full_slots, &semValue );
	while (! (globalQueue && semValue) ) { //test if the queue has anything in it

		//keep dequeueing until we break
		if ( sem_wait ( &full_slots ) != 0 ) {
			perror( "Error occured while processer was waiting" );
		}
		//lock access to queue
		if (sem_wait( &mut ) != 0) {
			perror( "Error occured while waiting for semaphore lock" );
			exit( EXIT_FAILURE );
		}

		processerFileName = queue_dequeue ( queue ); //store the file name of the client

		//release access to queue
		if (sem_post( &mut ) != 0) {
			perror( "Error occured while releasing semaphore lock" );
			exit( EXIT_FAILURE );
		}

;		printf( "Thread %d is handling client %s\n", id, processerFileName );

		//open file to get city name
		cityFile = fopen ( processerFileName, "r" );
		fread ( cityBuf, sizeof ( char ), MAXCITYNAMELENGTH, cityFile );
		fclose ( cityFile );
		lastCharOfCity = strlen( cityBuf );
		if (cityBuf[ lastCharOfCity - 1 ] == '\n') {
			cityBuf[ lastCharOfCity - 1 ] = '\0';
		}


		//will stick the city's line in cityLine if it exists
		cityLine = TwitterDBMem_getCityKwd( tdbm, cityBuf );
		if ( !( cityLine ) ) {
			fprintf ( stderr, "City %s does not exist", cityBuf );
		}
		cityLength = strlen ( cityBuf );
		//printf( "Length of city %s is %d\n", cityBuf, cityLength ); //testing printf
		strncpy ( lineAfterCityName, cityLine + cityLength + 1, ( 100 - cityLength )); //+1 for the extra comma

		//create result file
		originalFileName = processerFileName;
		strcat( processerFileName, ".result" ); //create name of result file
		resultFile = fopen ( processerFileName, "a+" ); //a+ mode will create the file

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

		fprintf( stderr, "1:Hi! I am ID:%d\n", id);

		//post that there is another empty slot
		if (sem_post( &empty_slots ) != 0) {
			perror( "Error occured while posting to a semaphore" );
			exit( EXIT_FAILURE );
		}

		sem_getvalue ( &full_slots, &semValue ); //to keep while loop updated

		fprintf( stderr, "2:Hi! I am ID:%d\n", id);
	}

	return NULL;
}

void *queueer( void *args ) {
	int id = *((int *) args);

	//Keep enqueueing until we break from having no more items
	while ( 1 ) {

		//test if the queue has slots to fill
		if (sem_trywait( &empty_slots ) != 0 && errno == EAGAIN ) {
			if (errno == EAGAIN) { //we're just at 0
				fprintf( stderr, "Thread %d is waiting to add items to the full queue...\n", id );
				if (sem_wait( &empty_slots ) != 0) {
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

		/* put next item on queue */
		char *newItem = (char *) malloc( sizeof( char ) * (MAXCITYNAMELENGTH + 1) );

		//get next line or EOF
		if (fgets( newItem, MAXCITYNAMELENGTH + 1, inFile ) == NULL) {

			//release access to queue
			if (sem_post( &mut ) != 0) {
				perror( "Error occured while releasing semaphore lock" );
				exit( EXIT_FAILURE );
			}
			globalQueue = true;
			break;//no more items to add
		}

		//TODO: <insert semaphore post to inFile>

		//delete \n char
		int newItemLen = strlen( newItem );
		if (newItem[ newItemLen - 1 ] == '\n') {
			newItem[ newItemLen - 1 ] = '\0';
		}

		//Lock access to queue
		if (sem_wait( &mut ) != 0 ) {//TODO: turn these validators into functions
			perror( "Error occured while acquiring semaphore lock" );
			exit( EXIT_FAILURE );
		}
		queue_enqueue( queue, newItem );

		//release access to queue
		if (sem_post( &mut ) != 0) {
			perror( "Error occured while releasing semaphore lock" );
			exit( EXIT_FAILURE );
		}

		//post that there is another full slot
		if (sem_post( &full_slots ) != 0) {
			perror( "Error occured while posting to a semaphore" );
			exit( EXIT_FAILURE );
		}

	}

	return NULL;//Nothing to return
}
