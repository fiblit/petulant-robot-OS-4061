#include "IO.h"

node_t readNode( char *line ){
		node_t nod;
		char **arg;
		makeargv( line, ":", &arg );
		
		strcpy( nod.prog, arg[ 0 ] );
		strcpy( nod.input, arg[ 2 ] );
		strcpy( nod.output, arg[ 3 ] );
		
		char **childt;
		makeargv( arg[ 1 ], " ", &childt );
		if ( strcmp( childt[ 0 ], "none") ){
			nod.children[ 0 ] = -1;
			nod.num_children = 0;
		}		
		
		return nod;
}
