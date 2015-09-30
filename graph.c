#include "graph.h"

/* 
 * Builds the "rowlist" data structure from the graph nodes
 * Requires that there be no cycles in the graph 
 */
rowlist_t buildRowList( node_t nodes[], int len ){	
	/*
	 *	for each node
	 * 		tell kdis +1 parent
	 *	for each remaining node
	 * 		if has parent
	 * 			incrememnt depth/row
	 * 		else
	 * 			tell kids -1 parent
	 * 			swap with end of remaining
	 * 			-1 remaining
	 * 			add to rowlist at row "depth". 
	 */
	//Setup
	for (int i = 0; i < len; i++){//for each node
		for (int j = 0; j < 10; j++){//for each of their children
			if (nodes[ i ].children[ j ] == -1){
				break;
			}
			nodes[ nodes[ i ].children[ j ] ].num_parents++;
		}
	}
	rowlist_t rowlist;
	for (int i = 0; i < 50; i++){
		rowlist[ i ] = malloc ( sizeof(node_t) * 50 );//way overallocating
	}
	node_t active[ len ];//so I can swap things around while still have children refs
	for (int i = 0; i < len; i++){
		active[ i ] = nodes[ i ];
	}
	
	//Build rowlist
	for (int i = 0, rem = len - 1; i < rem; i++){
		if (nodes[ i ].num_parents > 0){//has parents
			active[ i ].temp++;//temp in this case means depth/row
		}
		else{
			for (int j = 0; j < 10; j++){
				if (nodes[ i ].children[ j ] == -1){
					break;
				}
				nodes[ active[ i ].children[ j ] ].num_parents--;
			}
			node_t t = active[ i ];
			active[ i ] = active[ rem ];
			active[ rem ] = t;
			rem--;
			*rowlist[ active[ i ].temp ] = active[ rem ];   
			rowlist[ active[ i ].temp ]++;
		}
	}
	free(active);
	return rowlist;
}
