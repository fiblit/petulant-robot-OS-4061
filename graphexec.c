#include "graphexec.h"

int main(int argc, char *argv[])
{

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
		nodes[fileLineCount] = readNode( line );
		printf("nodes stuff: %d \n", nodes[ fileLineCount ]->num_children);
		printf("fileLineCount: %d \n", fileLineCount);
		fileLineCount++;  //increment fileLineCount
	}


	//TODO: add detectLoops here

	//buildRowList()
	//buildRowList from our group of nodes here
	printf( "File line count : %d \n", fileLineCount );
	rowlist_t rl = buildRowList ( nodes, fileLineCount );

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
			fprintf(stderr, "row:%d col:%d\n", row, i);
			if (( childpid = fork()) <= 0 ) { //fan creation
				break;
			}
		}
		if ( childpid == 0 ) {
			char **nodeArgs;
			makeargv(rl[ row ][ i ]->prog, " ", &nodeArgs);
			redirect( rl[ row ][ i ] );
			execvp( nodeArgs[0], &nodeArgs[0] );
			perror("Child failed to execvp the command");
			return 1;
		}
		else if ( childpid == -1 ) {
			perror( "fork failed to execute" );
			return 1;
		}
		else {  //is parent
			while ( r_wait( NULL ) > 0 ) {
				; //Waiting for children
			}
		}
	}

	/*
	for ( int i = 0; i < rowList.size(); i++ ){
		fan ( rowList[i].size() )
		if childpid == 0
			redirect (nodeij.files)
			exec (nodeij.prog)
		else
			while (r_wait > 0)
	*/





	//then execution loop

	return 0;
}
