/* CSci4061 Assignment 4
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
	queueClientInfo_t qci = (queueClientInfo_t) malloc( sizeof( queueClientInfo ) );
	qn->client = qci;
	qn->client->socket = -1;
	memset(&(qn->client->address), 0, sizeof( struct sockaddr_in ) );
	return qn;
}

/* destruct qn */
void queueNode_destruct( queueNode_t qn ) {
	if (qn == NULL)
		return;
	queueNode_destruct( qn->next );

	if (qn->client != NULL)
		free( qn->client );

	free( qn );
}

/* adds item to the tail of the queue */
void queue_enqueue( queue_t q, int socket, struct sockaddr_in address ) {
	queueNode_t qn = queueNode_construct();
	qn->client->socket = socket;
	qn->client->address = address;
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
queueClientInfo_t queue_dequeue( queue_t q ) {
	if (q->head != NULL) {

		/* get old head */
		queueNode_t oldHead = q->head;

		/* get retval */
		queueClientInfo_t info = (queueClientInfo_t) malloc ( sizeof( queueClientInfo ) );
		info->socket = q->head->client->socket;
		info->address = q->head->client->address;

		/* move head forward */
		if (q->head->next == NULL) {
			q->tail = NULL;
		}
		q->head = q->head->next;

		/* kill old head */
		oldHead->next = NULL;
		queueNode_destruct( oldHead );
		q->size--;

		return info;
	}
	else {
		fprintf( stderr, "Attempted to dequeue an empty queue\n" );
		exit( EXIT_FAILURE );//TODO: return/check for errno so we can print the culprit thread
	}
	return NULL;
}
