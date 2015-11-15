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
	int id;//TODO: fix ID bug, the problem is that the address at which id is stored gets overwritten before the processer thread can access it.
	for (id = 0; id < num_threads; id++) {
		if (pthread_create( &processerThreads[ id ], NULL, processer, &id) != 0 ) {
			fprintf( stderr, "Failed to create processer thread, ID:%d : %s\n", id, strerror( errno ) );
		}
		else {
			fprintf( stderr, "Created thread %d\n", id );
		}
	}
	if (pthread_create( &queueerThread, NULL, queueer, &id) != 0 ) {
		fprintf( stderr, "Failed to create queueer thread, ID:%d : queueer : %s\n", num_threads, strerror( errno ) );
	}
	else {
		fprintf( stderr, "Created thread %d : queueer\n", num_threads );
	}

	/* join threads */
	if (pthread_join( queueerThread, NULL ) != 0) {
		fprintf( stderr, "Failed to join queueer thread, ID:%d : %s\n", num_threads, strerror( errno ) );
	}
	else {
		fprintf( stderr, "Joined thread %d : queueer\n", num_threads);
	}
	for (id = 0; id < num_threads; id++) {
		if (pthread_join( processerThreads[ id ], NULL) != 0) {
			fprintf( stderr, "Failed to join processer thread, ID:%d : %s\n", id, strerror( errno ) );
		}
		else {
			fprintf( stderr, "Joined thread %d\n", id);
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
	
	/* stuff goes here */
	fprintf( stderr, "Hi! I am ID:%d\n", id);

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
			break;//no more items to add
		}

		//TODO: <insert semaphore post to inFile>

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

