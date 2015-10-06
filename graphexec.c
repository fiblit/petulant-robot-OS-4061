#include "graphexec.h"

int main(int argc, char *argv[])
{
	/*Main structure
	fopen("in");
	while ((line = getLine()) != NULL)
		getNode(line);
	//buildGraph(Nodes); //not needed...
	bfs/detectLoops(nodes);
	buildRowList(nodes);
	for ( int i = 0; i < rowList.size(); i++ ){
		fan ( rowList[i].size() )
		if childpid == 0
			redirect (nodeij.files)
			exec (nodeij.prog)
		else
			while (r_wait > 0)
	}
	free rowlist
	*/

	//Dalton's test code below
	/*


	node_t *(ns[ 5 ]);
	ns[ 0 ] = readNode("echo 1:1 3:blank-file.txt:blank-out.txt");
	ns[ 1 ] = readNode("echo 2:2 4:blank-file.txt:blank-out.txt");
	ns[ 2 ] = readNode("echo 3:4:blank-file.txt:blank-out.txt");
	ns[ 3 ] = readNode("echo 4:4:blank-file.txt:blank-out.txt");
	ns[ 4 ] = readNode("echo 5:none:blank-file.txt:blank-out.txt");

	for (int j = 0; j < 5; j++){
		printf("\n%dth node\n", j);
		printf("prog: %s\ninput: %s\noutput %s\n", ns[ j ]->prog, ns[ j ]->input, ns[ j ]->output);
		printf("numkids: %d\n", ns[ j ]->num_children);
		for (int i = 0; i < ns[ j ]->num_children; i++){
			printf("%dth child: %d\n", i, ns[ j ]->children[ i ]);
		}
	}

	rowlist_t rl = buildRowList( ns, 5 );
	for (int i = 0; i < 5; i++){
		printf("\n\n%dth row\n", i);
		for (int j = 0; j < 5; j++){
            if (rl[ i ][ j ]==NULL){
                printf("\nNULL - i: %d, j: %d\n", i, j);
                break;
            }
			printf("\n%dth node\n", j);
			printf("prog: %s\ninput: %s\noutput %s\n", rl[ i ][ j ]->prog, rl[ i ][ j ]->input, rl[ i ][ j ]->output);
			printf("numkids: %d\n", rl[ i ][ j ]->num_children);
			for (int k = 0; k < rl[ i ][ j ]->num_children; k++){
				printf("%dth child: %d\n", i, rl[ i ][ j ]->children[ k ]);
			}
		}
	} */

	//Tim's test code below
	/*
	node_t *(testNode[ 5 ]);
	testNode[ 0 ] = readNode("ls -a:1:blank-file.txt:blank-out.txt");
	testNode[ 1 ] = readNode("echo hi hi:2:blank-file.txt:blank-out.txt");
	testNode[ 2 ] = readNode("echo toodles:none:blank-file.txt:blank-out.txt");

	testNode[ 0 ]->status = READY;

	pid_t testchildpid;

	char **testNodeArgs;
	makeargv(testNode[ 0 ]->prog, " ", &testNodeArgs);

	printf("%s \n", testNode[ 0 ]->prog );

	if ( testNode[ 0 ]->status == READY ) {
		testchildpid = fork();
		if ( testchildpid == -1 ) {
			perror("failed to fork");
			return 1;
		}

		if ( testchildpid == 0 ) {  //child will perform
			execvp( testNodeArgs[0], &testNodeArgs[0] );
			perror("Child failed to execvp the command");
			return 1;
		}
		if ( testchildpid != wait(NULL) ) {
			perror("Parent failed to wait");
			return 1;
		}
	} */

	//making sure there are correct # of arguments
	if ( argc != 2 ) {
		fprintf ( stderr, "Usage %s some-graph-file.txt \n", argv[0] );
		return 1;
	}

	//opening the given graph file in "r" read mode
	FILE  *graphFile = fopen( argv[1] , "r" );

	//error check that it opened a file
	if (graphFile == NULL) { //file failed to open
		perror ( "Failed to open file" );
		return 1;
	}

	char line[ 1024 ]; //max length of a line
	node_t *(nodes[ 50 ]); //50 max amt of lines/nodes
	int fileLineCount = 0;

	//code below causing a seg fault atm, will fix after night class
	//this while loop will read all the lines of the file until it the file is complete
	while ( fgets( line, 1024, graphFile ) != NULL && fileLineCount < 50 ) {
		nodes[fileLineCount] = readNode( line );
		printf("nodes stuff: %d \n", nodes[ fileLineCount ]->num_children);
		printf("fileLineCount: %d \n", fileLineCount);
		fileLineCount++;  //increment fileLineCount
	}


	//detectLoops function should be here



	//buildRowList()
	//buildRowList from our group of nodes here
	printf( "File line count : %d \n", fileLineCount );
	rowlist_t rl = buildRowList ( nodes, fileLineCount );

	for (int row = 0; row < fileLineCount; row++){
		int numInRow;
		for (int j = 0; j < fileLineCount; j++){
            if (rl[ row ][ j ]==NULL){
                numInRow++;
                break;
            }
		}
		int i;
		for ( i = 0; i < numInRow; i++ ) {
			if (( childpid == fork()) <= 0 ) { //fan creation
				break;
			}
		}
		if ( childpid == 0 ) {

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
