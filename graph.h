#ifndef GRAPH_H
#define GRAPH_H

#include "stdlib.h"
#include "unistd.h"
#include "sys/types.h"
#include <stdio.h>

//I feel like using an enum is a better idea but whatevs.
#define INELIGIBLE 0
#define READY 1
#define RUNNING 2
#define FINISHED 3
typedef struct node {
	//id seems unnessecary, IO.c should track that.
	int id; //corresponds to line number in graph text file
	int temp; //For any graph algorithm with a weight function
	char prog[ 1024 ]; //prog+arguments
	char input[ 1024 ]; //filename
	char output[ 1024 ]; //filename
	int children[ 10 ]; //children IDs
	int num_children;
	int num_parents; // for buildRowList
	int status;
	pid_t pid; //track it when it's running
} node_t;

//potentially could have "int detectLoop(..."
void BFS( node_t nodes[] );//For detecting loops

typedef node_t*** rowlist_t;//2d array of node_t pointers

rowlist_t buildRowList( node_t *(nodes[]), int len );

#endif
