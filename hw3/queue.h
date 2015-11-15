/* CSci4061 Assignment 3
* name: Dalton Hildreth, Timothy Kohn
* id: hildr039, kohnx074 */

/* This file implements the thinly disguised Linked-List a.k.a. queue  */

#ifndef QUEUE_H_GUARD
#define QUEUE_H_GUARD

#include <stdlib.h>
#include <string.h>

typedef struct queueNode {
	struct queueNode *next;
	char *item;
} queueNode;

typedef queueNode* queueNode_t;

typedef struct queue {
	queueNode_t head;
	queueNode_t tail;
	int size;
} queue_queue;

typedef queue_queue* queue_t;

/* construct a queue_t */
queue_t queue_construct();

/* destruct q */
void queue_destruct( queue_t q );

/* construct a queueNode_t */
queueNode_t queueNode_construct();

/* destruct qn */
void queueNode_destruct( queueNode_t qn );

/* adds item to the tail of the queue */
void queue_enqueue( queue_t q, char *item );

/* returns item from the head of q and removes the item from head of q */
char *queue_dequeue( queue_t q );

#endif //QUEUE_H_GUARD

