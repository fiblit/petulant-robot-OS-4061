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

	for (int id = 0; id < num_threads; id++);
	/* make and run threads */

	/* join threads */

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

