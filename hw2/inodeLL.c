#include "inodeLL.h"

inodeLL_t inodeLL_construct() {
    inodeLL_t list = (inodeLL_t)malloc( sizeof( inodeLLNode ) );
    list->inode = -1;
    list->next = NULL;
    return list;    
}

void inodeLL_destruct( inodeLL_t list ) {
    if (list == NULL) {
        return;
    }
    inodeLL_destruct( list->next );
    free( list );
}

void inodeLL_add( inodeLL_t list, ino_t inode ) {
    if (list == NULL) {
        list = inodeLL_construct();
        list->inode = inode;
    }
    else if (list->next == NULL) {
        list->next = inodeLL_construct();
        list->next->inode = inode;
    }
    else {
        fprintf( stderr, "Attempt to add inode %d in middle of list", (int)inode);
        exit(EXIT_FAILURE);
    }
}

bool inodeLL_search( inodeLL_t list, ino_t k ) {
    while (list != NULL) {
        if (list->inode == k) {
            return true;
        }
        list = list->next;
    }
    return false;
}

inodeLL_t inodeLL_tail( inodeLL_t list ) {
    inodeLL_t prev = list;
    while (list != NULL) {
        prev = list;
        list = list->next;
    }
    return prev;
}

void inodeLL_append( inodeLL_t list, ino_t inode ) {
    inodeLL_add( inodeLL_tail( list ), inode );
}
