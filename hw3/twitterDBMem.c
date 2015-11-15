/* CSci4061 Assignment 3
* name: Dalton Hildreth, Timothy Kohn
* id: hildr039, kohnx074 */

#include "twitterDBMem.h"

/* construct a TwitterDBMem from FILE */
TwitterDBMem_t TwitterDBMem_construct(FILE* twitterDB) {
	
	//allocate tdbm
	TwitterDBMem_t tdbm = (TwitterDBMem_t) malloc( sizeof( TwitterDBMem ) );
	
	/* the following is basically what will be "int fgetlines(FILE* f, char ***lines)" */
	//count number of lines in twitterDB
	rewind( twitterDB );
	char c = fgetc( twitterDB );
	tdbm->numLines = 0;
	while (c != EOF) {
		if (c == '\n') {
			tdbm->numLines++;
		}
		c = fgetc( twitterDB );
	}

	//allocate space for the lines
	tdbm->lines = (char **) malloc( sizeof( char * ) * tdbm->numLines );
	if (tdbm->lines == NULL) {
		perror( "Call to malloc failed in TwitterDBMem_construct" );
		exit( EXIT_FAILURE );
	}

	//read the lines
	rewind( twitterDB );
	for (int i = 0; i < tdbm->numLines; i++) {
		char c = fgetc( twitterDB );
		int lineLen = 1;
		while(c != '\n' && c != EOF) {//not at end-of-line
			c = fgetc( twitterDB );
			lineLen++;
		}
		if (c == EOF) {
			lineLen--; //exclude EOF from the line count
		}
		
		tdbm->lines[ i ] = (char *) malloc( sizeof( char ) * (lineLen + 1) );
		if ( tdbm->lines[ i ] == NULL ) { 
			perror( "Call to malloc failed in TwitterDBMem_construct" );
			exit( EXIT_FAILURE );
		}
		fseek( twitterDB, -lineLen, SEEK_CUR );//move back the # of chars in the line
		fgets( tdbm->lines[ i ], lineLen + 1, twitterDB );//+1 for the '\0' char
	}
	
	return tdbm;
}

/* destruct tdbm, int is for errors */
void TwitterDBMem_destruct(TwitterDBMem_t tdbm) {
	for (int i = 0; i < tdbm->numLines; i++) {
		free( tdbm->lines[ i ] );
	}
	free( tdbm->lines );
	free( tdbm );
}

/* points cline to the city's line or NULL if city does not exist. 
 * Returns false if city DNE, otherwise true. */
bool TwitterDBMem_getCityKwd(TwitterDBMem_t tdbm, const char *city, char **cline) {
	
	//Linear search. TODO: change it to a binary search
	int cityLen = strlen( city );
	*cline = NULL;
	for (int i = 0; i < tdbm->numLines; i++) {
		if ( strncmp( tdbm->lines[ i ], city, cityLen ) == 0 ) {
			*cline = tdbm->lines[ i ];
			return true;
		}
	}
	return false;
}

