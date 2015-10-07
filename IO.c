#include "IO.h"

//TODO: add redirect.c code here. (turn into function)

/* Turns a string (line) into a node */
node_t *readNode( char *line ){
		node_t *nod = (node_t *) malloc(sizeof(node_t));
		char **arg;
		makeargv( line, ":\n", &arg );

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
        nod->status = INELIGIBLE;
        nod->pid = -1;
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


void redirect(node_t *n){
	int input_file = open("blank-file.txt", O_RDONLY);
	if (input_file < 0){
		perror("Error opening input file in child after fork! Exiting.");
		exit(0);
	} else {
		dup2(input_file, STDIN_FILENO);
		close(input_file);
	}

	int output_file = open(n->output, O_WRONLY|O_CREAT|O_TRUNC, 0644);

	if (output_file < 0){
		perror("Error opening output file in child after fork! Exiting.");
		exit(0);
	} else{
		dup2(output_file, STDOUT_FILENO);
		close(output_file);
	}
}
