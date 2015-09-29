#ifndef GRAPH_H
#define GRAPH_H

//I feel like using an enum is a better idea but whatevs.
#define INELIGIBLE 0
#define READY 1
#define RUNNING 2
#define FINISHED 3
typedef struct node {
	//id seems unnessecary, IO.c should track that.
	int id; //corresponds to line number in graph text file
	int time; //for bfs. It's this node's incidence #.
	char prog[ 1024 ]; //prog+arguments
	char input[ 1024 ]; //filename
	char output[ 1024 ]; //filename
	int children[ 10 ]; //children IDs
	int num_children;
	int status;
	pid_t pid; //track it when it's running
} node_t;

//potentially could have "int detectLoop(..."
void BFS( node_t[] );//For detecting loops

typedef node_t** rowlist_t;//really it's *node_t[]...

rowlist_t buildRowList( node_t[] );

#endif
