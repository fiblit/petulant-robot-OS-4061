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
			exec (nodeij.prog)
		else
			while (r_wait > 0)
	}
	free rowlist
	*/
	node_t ns[ 5 ];
	ns[ 0 ] = readNode("ls -a:1 3:blank-file.txt:blank-out.txt");
	ns[ 1 ] = readNode("ls -a:2 4:blank-file.txt:blank-out.txt");
	ns[ 2 ] = readNode("ls -a:4:blank-file.txt:blank-out.txt");
	ns[ 3 ] = readNode("ls -a:4:blank-file.txt:blank-out.txt");
	ns[ 4 ] = readNode("ls -a:none:blank-file.txt:blank-out.txt");
	for (int j = 0; j < 5; j++){
		printf("\n%dth node\n", j);
		printf("prog: %s\ninput: %s\noutput %s\n", ns[ j ].prog, ns[ j ].input, ns[ j ].output);
		printf( "test7\n" );
		printf("numkids: %d\n", ns[ j ].num_children);
		printf( "test6\n" );
		for (int i = 0; i < ns[ j ].num_children; i++){
			printf( "test5\n" );
			printf("%dth child: %d\n", i, ns[ j ].children[ i ]);
		}
		printf( "test4\n" );
	}
	
	printf( "test3" );
	rowlist_t rl = buildRowList( ns, 5 );
	for (int i = 0; i < 5; i++){
		printf("\n%dth row\n", i);
		for (int j = 0; j < 5; j++){
			printf("\n%dth node\n", j);
			printf("prog: %s\ninput: %s\noutput %s\n", rl[ i ][ j ].prog, rl[ i ][ j ].input, rl[ i ][ j ].output);
			printf("numkids: %d\n", rl[ i ][ j ].num_children);
			for (int i = 0; i < rl[ i ][ j ].num_children; i++){
				printf("%dth child: %d\n", i, rl[ i ][ j ].children[ i ]);
			}
		}
	}
	
	return 0;
}
