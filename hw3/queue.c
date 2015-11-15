/* CSci4061 Assignment 3
* name: Dalton Hildreth, Timothy Kohn
* id: hildr039, kohnx074 */

#include "queue.h"


/* construct q, int is for errors */
queue_t queue_construct() {
	queue_t q = (queue_t) malloc( sizeof( queue ) );
	q->head = NULL;
	q->tail = NULL;
	q->size = 0;
	return q;
}

/* destruct q */
void queue_destruct( queue_t q ) {
	queueNode_destruct( q->head );
	free( q );
}

/* construct a queueNode_t */
queueNode_t queueNode_construct() {
	queueNode_t qn = (queueNode_t) malloc( sizeof( queueNode ) );
	qn->next = NULL;
	qn->item = NULL;	
	return qn;
}

/* destruct qn */
void queueNode_destruct( queueNode_t qn ) {
	if (qn == NULL)
		return;
	queueNode_destruct( qn->next );
	
	if (qn->item != NULL)
		free( qn->item );

	free( qn );
}

/* adds item to the tail of the queue */
void queue_enqueue( queue_t q, char *item ) {
	queueNode_t qn = queueNode_construct();
	qn->item = (char *) malloc( sizeof( char ) * strlen( item ) );
	strcpy( qn->item, item );
	
	q->tail->next = qn;
}

/* returns item from the head of q and removes the item from head of q */
char *queue_dequeue( queue_t q ) {
	queueNode_t head = q->head;
	q->head = q->head->next;

	char *ret = (char *) malloc( sizeof( char ) * strlen( head->item ) );
	strcpy( ret, q->head->item );
	
	queueNode_destruct( head );

	return ret;
}

