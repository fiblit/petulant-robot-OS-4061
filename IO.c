#include "IO.h"

/* Turns a string (line) into a node */
node_t readNode( char *line ){
		node_t nod;
		char **arg;
		makeargv( line, ":", &arg );
		
		strcpy( nod.prog, arg[ 0 ] );
		strcpy( nod.input, arg[ 2 ] );
		strcpy( nod.output, arg[ 3 ] );
		
		char **childt;
		int nchild = makeargv( arg[ 1 ], " ", &childt );
		if ( strcmp( childt[ 0 ], "none") ){
			nod.children[ 0 ] = -1;
			nod.num_children = 0;
		}		
		else{
			for (int i = 0; i < nchild; i++){
				nod.children [ i ] = atoi( childt[ i ] );
			}
			nod.num_children = nchild;
		}
		return nod;
}

/* Returns a line from the file in */
char *getLine( FILE *in, int buf ){
	char c;
	char *line = malloc(sizeof(char) * buf);
	int i;
	for (i = 0; (i < buf - 1) && ((c = getc(in)) != EOF) && (c != '\n'); i++ )
		line[ i ] = c;
	line [ i ] = '\0';
	return line;
}
