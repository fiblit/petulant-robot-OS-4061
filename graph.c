#include "graph.h"

/* 
 * Builds the "rowlist" data structure from the graph nodes
 * Requires that there be no cycles in the graph 
 */
rowlist_t buildRowList( node_t *(nodes[]), int len ){
    
    //Setup
    for (int i = 0; i < len; i++){
        for (int j = 0; j < nodes[ i ]->num_children; j ++){
            nodes[ nodes[ i ]->children[ j ] ]->num_parents++;
        }
    }
    int remain[ len ];
    for (int i = 0; i < len; i++){
        remain[ i ] = i;
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
    
    //Build
    for (int row = 0, rem = len; row < len && rem > 0; row++){
        int numOrphans = 0;
        int orphans[ len ];
        for (int i = 0; i < len; i++){
            orphans[ i ] = -1;
        }
        
        for (int i = rem - 1; i < rem && i >= 0; i--){
            if ( nodes[ remain[ i ] ]->num_parents == 0 ){
                orphans[ numOrphans++ ] = remain[ i ];
                int t = remain[ i ];
                remain[ i ] = remain[ rem - 1 ];
                remain[ rem - 1 ] = t;
                rem--;
                //i--;
            }
        }
        int col = 0;
        for (int i = 0; i < numOrphans; i++){
            for (int j = 0; j < nodes[ orphans [ i ] ]->num_children; j++){
                nodes[ nodes[ orphans[ i ] ]->children[ j ] ]->num_parents--;
            }
            rl [ row ][ col++ ] = nodes[ orphans[ i ] ];
        }
    }
    return rl;
}