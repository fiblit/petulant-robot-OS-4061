/* login: hildr039, kohnx074
 * date: 08/07/2015
 * name: Dalton Hildreth, Timothy Kohn
 * id: 5117575, 4050267
 */
#include "graphexec.h"

int main(int argc, char *argv[]){

	//making sure there are correct # of arguments
	if ( argc != 2 ) {
		fprintf ( stderr, "Usage %s some-graph-file.txt \n", argv[0] );
		return 1;
	}

	//opening the given graph file in "r" read mode
	FILE  *graphFile = fopen( argv[1] , "r" );

	//error check that it opened a file
	if ( graphFile == NULL ) { //file failed to open
		perror ( "Failed to open file" );
		return 1;
	}

	char line[ 1024 ]; //max length of a line
	node_t *(nodes[ 50 ]); //50 max amt of lines/nodes
	int fileLineCount = 0;

	//this while loop will read all the lines of the file until it the file is complete
	while ( fgets( line, 1024, graphFile ) != NULL && fileLineCount < 50 ) {
		node_t *n = readNode( line );
		if (n != NULL){//blank line was NOT read
			nodes[ fileLineCount ] = n;
			fileLineCount++;  //increment fileLineCount
		}
	}

	//buildRowList from our group of nodes here
	rowlist_t rl = buildRowList ( nodes, fileLineCount );

	if (rl == NULL){//there was a cycle
		fprintf(stderr, "There was a cycle in the graph\n");
		return 1;
	}

	for (int row = 0; row < fileLineCount; row++){
		int numInRow = 0;
		for (int j = 0; j < fileLineCount; j++){
			if (rl[ row ][ j ]==NULL){
				break;
			}
			numInRow++;
		}

		int i;
		pid_t childpid;
		for ( i = 0; i < numInRow; i++ ) {
			if (( childpid = fork()) <= 0 ) { //fan creation
				break;
			}
		}
		if ( childpid == 0 ) {
			char **nodeArgs;
			makeargv(rl[ row ][ i ]->prog, " ", &nodeArgs);
			redirect( rl[ row ][ i ] );
			execvp( nodeArgs[0], &nodeArgs[0] );
			perror("Child failed to execvp the command"); //execvp error check
			return 1;
		}
		else if ( childpid == -1 ) {  //fork() error check
			perror( "fork failed to execute" );
			return 1;
		}
		else {  //is parent
			while( r_wait( NULL ) > 0 ) { //waiting for children
				;//do nothing
			}
		}
	}

	return 0;
}
