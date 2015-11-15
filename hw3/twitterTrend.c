/* CSci4061 Assignment 3
* name: Dalton Hildreth, Timothy Kohn
* id: hildr039, kohnx074 */

#include "twitterTrend.h"

/* does initial processing, in particular, it starts the threads */
int main( int argc, char *argv[] ) {
	
	if (argc != 3) {
		fprintf( stderr, "Incorrect usage.\n" );
		fprintf( stderr, "\tUsage: %s <*.in file> <num_threads>\n", argv[ 0 ] );
		return 1;
	}

	char *inFileName = argv[ 1 ];	
	int num_threads = atoi( argv[ 2 ] );

	openInFile( inFileName );
	readTwitterDB();
	queue = queue_construct();

	/* make and run threads */
	pthread_t *processerThreads = (pthread_t *) malloc( sizeof( pthread_t ) * num_threads );
	pthread_t queueerThread;
	int id;
	for (id = 0; id < num_threads; id++) {
		if (pthread_create( &processerThreads[ id ], NULL, processer, &id) != 0 ) {
			fprintf( stderr, "Failed to create processer thread, ID:%d : %s", id, strerror( errno ) );
		}
	}
	if (pthread_create( &queueerThread, NULL, queueer, &id) != 0 ) {
		fprintf( stderr, "Failed to create queueer thread, ID:%d : %s", id, strerror( errno ) );
	}

	/* join threads */
	if (pthread_join( queueerThread, NULL ) != 0) {
		fprintf( stderr, "Failed to join queueer thread, ID:%d : %s", id, strerror( errno ) );
	}
	for (id = 0; id < num_threads; id++) {
		if (pthread_join( processerThreads[ id ], NULL) != 0) {
			fprintf( stderr, "Failed to join processer thread, ID:%d : %s", id, strerror( errno ) );
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
		perror( "failed to open *.in file ");
		exit( EXIT_FAILURE );	
	}
}

void *processer( void *args ) {
	return NULL;
}

void *queueer( void *args ) {
	return NULL;
}

