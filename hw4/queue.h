/* CSci4061 Assignment 4
* name: Dalton Hildreth, Timothy Kohn
* id: hildr039, kohnx074 */

/* This file implements the thinly disguised Linked-List a.k.a. queue  */

#ifndef QUEUE_H_GUARD
#define QUEUE_H_GUARD

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <arpa/inet.h>

#include "errorFunction.h"

typedef struct clientInfo {
	int socket;
	struct in_addr address;
} queueClientInfo;

typedef queueClientInfo* queueClientInfo_t;

typedef struct queueNode {
	struct queueNode *next;
	queueClientInfo_t client;
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
void queue_enqueue( queue_t q, int client, struct in_addr address );

/* returns item from the head of q and removes the item from head of q */
queueClientInfo_t queue_dequeue( queue_t q );

#endif //QUEUE_H_GUARD
