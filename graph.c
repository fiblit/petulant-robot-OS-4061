#include "graph.h"

/* 
 * Builds the "rowlist" data structure from the graph nodes
 * Requires that there be no cycles in the graph 
 */
rowlist_t buildRowList( node_t *(nodes[]), int len ){
    /*
    for each node
        tell kids + 1 parent
    while nodes remain
        for each remaining node
            if has parent
                increment depth/row
        for each remaining node
            if has no parent
                tell kids -1 parent
                swap with end of remaining
                rem - 1
                add to rowlist at row "depth".
    */
    //setup
    for (int i = 0; i < len; i++){
        for (int j = 0; j < len; j ++){
            if (nodes[ i ]->children[ j ] == -1){
                break;
            }
            nodes[ nodes[ i ]->children[ j ] ]->num_parents++;
        }
    }
    int active[ len ];
    for (int i = 0; i < len; i++){
        active[ i ] = i;
    }
    rowlist_t rl;
    rl = (rowlist_t) malloc( sizeof( node_t ** ) * len);
    for (int i = 0; i < len; i++){
        rl[ i ] = (node_t **) malloc( sizeof(node_t *) * len);
    }
    for (int i = 0; i < len; i++){
        for (int j = 0; j < len; j++){
            rl[ i ][ j ] = NULL;
        }
    }
    
    /*
    //Build
    for (int row = 0, rem = len; row < len; row++){
        int col = 0;
        for (int i = 0; i < rem; i++){
            if (nodes[ active [ i ] ]->num_parents == 0){
                for (int j = 0; j < 10; j++){//tell kids -1 parent
                    if (nodes[ active[ i ] ]->children[ j ] == -1){
                        break;
                    }
                    nodes[ nodes[ active[ i ] ]->children[ j ] ]->num_parents--;
                }
                int t = active[ i ];
                active[ i ] = active[ rem - 1 ];
                active[ rem - 1 ] = t;
                rem--;
                rl[ row ][ col ] = nodes[ active [ rem ] ];
                col++;
            }
        }
    } */
    return rl;
}

/*
rowlist_t buildRowList( node_t nodes[], int len ){	
	/*
	 *	for each node
	 * 		tell kids +1 parent
	 *	for each remaining node
	 * 		if has parent
	 * 			increment depth/row
	 * 		else
	 * 			tell kids -1 parent
	 * 			swap with end of remaining
	 * 			-1 remaining
	 * 			add to rowlist at row "depth". 
	 *
	//Setup	
	for (int i = 0; i < len; i++){
		nodes[ i ].num_parents = 0;
	}
	for (int i = 0; i < len; i++){//for each node
		for (int j = 0; j < 10; j++){//for each of their children
			if (nodes[ i ].children[ j ] == -1){
				break;
			}
			nodes[ nodes[ i ].children[ j ] ].num_parents++;
		}
	}
	rowlist_t rowlist;
	rowlist = (rowlist_t) malloc( sizeof( node_t * ) * len);
	//rowlist[0] = (node_t *) malloc( sizeof( node_t ) * len);
	for (int i = 0; i < len; i++){
		rowlist[ i ] = (node_t *) malloc ( sizeof(node_t) * len );//way overallocating
	}
	//node_t active[ len ];//so I can swap things around while still have children refs
	//for (int i = 0; i < len; i++){
	//	active[ i ] = nodes[ i ];
	//}
    int active[ len ];
    for (int i = 0; i < len; i++){
        active[ i ] = i;
    }
	
	//Build rowlist
    for (int rem = len; rem > 0;)
    {
        for (int i = 0; i < rem; i++){
            if (nodes[ active[ i ] ].num_parents > 0){//has parents
                nodes[ active[ i ] ].temp++;
            }
        }
        for (int i = 0; i < rem; i++){
            if (nodes[ active[ i ] ].num_parents == 0){//has no parents
                for (int j = 0; j < 10; j++){
                    if (nodes[ active[ i ] ].children[ j ] == -1){
                        break;
                    }
                    nodes[ nodes[ active[ i ] ].children[ j ] ].num_parents--;//tell kids -1 parent
                }
                int t = active[ i ];
                active[ i ] = active[ rem-1 ];
                active[ rem-1 ] = t;
                rem--;
                *rowlist[ nodes[ active[ i ] ].temp ] = nodes[ active[ rem ] ];   
                rowlist[ nodes[ active[ i ] ].temp ]++;
            }
        }
    }
	return rowlist;
}
*/