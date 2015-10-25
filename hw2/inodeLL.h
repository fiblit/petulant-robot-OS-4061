#ifndef inodeLL_H
#define inodeLL_H

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct inodeLLNode {
	ino_t inode;
	struct inodeLLNode *next;
} inodeLLNode;
typedef inodeLLNode* inodeLL_t;

inodeLL_t inodeLL_construct( );
void inodeLL_destruct( inodeLL_t list );
void inodeLL_add( inodeLL_t list, ino_t inode );
bool inodeLL_search( inodeLL_t list, ino_t k );
inodeLL_t inodeLL_tail( inodeLL_t list );
void inodeLL_append( inodeLL_t list, ino_t inode );

#endif //inodeLL_H
