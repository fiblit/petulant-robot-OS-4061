#include "IO.h"

/* Turns a string (line) into a node */
node_t *readNode( char *line ){
		node_t *nod = (node_t *) malloc(sizeof(node_t));
		char **arg;
		makeargv( line, ":\n", &arg );

		if(*arg == NULL){//blank line hit
			return NULL;//no node to be read here!
		}

			strcpy( nod->prog, arg[ 0 ] );
			strcpy( nod->input, arg[ 2 ] );
			strcpy( nod->output, arg[ 3 ] );

		char **childt;
		int nchild = makeargv( arg[ 1 ], " ", &childt );
		if ( strcmp( childt[ 0 ], "none") == 0 ){
			nod->children[ 0 ] = -1;
			nod->num_children = 0;
		}
		else{
			int i;
			for (i = 0; i < nchild; i++){
				nod->children [ i ] = atoi( childt[ i ] );
			}
			if (nchild < 10){
				nod->children[ i ] = -1;
			}
			nod->num_children = nchild;
		}
			nod->num_parents = 0;
			nod->temp = 0;
			nod->pid = -1;
		return nod;
}

/* redirects stdin and stdout to *n's input and output files, respectively */
void redirect(node_t *n){
	int input_file = open(n->input, O_RDONLY);
	if (input_file < 0){
		fprintf(stderr, "Error opening input file in %dth node. ", n->id);
		perror("Exiting with status 1 ");
		//perror("Error opening input file in child after fork! Exiting.");
		exit(1);
	} else {
		dup2(input_file, STDIN_FILENO);
		close(input_file);
	}

	int output_file = open(n->output, O_WRONLY|O_CREAT|O_TRUNC, 0644);

	if (output_file < 0){
		fprintf(stderr, "Error opening output file in %dth node. ", n->id);
		perror("Exiting with status 1 ");
		//perror("Error opening output file in child after fork! Exiting.");
		exit(1);
	} else{
		dup2(output_file, STDOUT_FILENO);
		close(output_file);
	}
}
