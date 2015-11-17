/* CSci4061 Assignment 3
* name: Dalton Hildreth, Timothy Kohn
* id: hildr039, kohnx074 */

#include "queue.h"


/* construct q, int is for errors */
queue_t queue_construct() {
	queue_t q = (queue_t) malloc( sizeof( queue_queue ) );
	if ( q == NULL ) { //malloc error checking
		errorFunction ( "Call to malloc failed in queue_construct" );
	}
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
	if ( qn == NULL ) { //malloc error checking
		errorFunction ( "Call to malloc failed in queueNode_construct" );
	}
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
	if ( qn->item == NULL ) { //malloc error checking
		errorFunction ( "Call to malloc failed in queue_enqueue" );
	}
	strcpy( qn->item, item );
	if (q->head == NULL) {
		q->head = qn;
		q->tail = qn;
	}
	else {
		q->tail->next = qn;
		q->tail = qn;
	}
	q->size++;
}

/* returns item from the head of q and removes the item from head of q */
char *queue_dequeue( queue_t q ) {

	if (q->head != NULL) {

		/* get old head */
		queueNode_t oldHead = q->head;

		/* get retval */
		char *ret = (char *) malloc( sizeof( char ) * strlen( q->head->item ) );
		if ( ret == NULL ) { //malloc error checking
			errorFunction ( "Call to malloc failed in queue_dequeue" );
		}
		strcpy( ret, q->head->item );

		/* move head forward */
		if (q->head->next == NULL) {
			q->tail = NULL;
		}
		q->head = q->head->next;

		/* kill old head */
		oldHead->next = NULL;
		queueNode_destruct( oldHead );
		q->size--;

		return ret;
	}
	else {
		fprintf( stderr, "Attempted to dequeue an empty queue\n" );
		exit( EXIT_FAILURE );//TODO: return/check for errno so we can print the culprit thread
	}
}
