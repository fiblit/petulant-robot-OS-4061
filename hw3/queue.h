/* CSci4061 Assignment 3
* name: Dalton Hildreth, Timothy Kohn
* id: hildr039, kohnx074 */

/* This file implements the thinly disguised Linked-List a.k.a. queue  */
struct queueNode {
	struct queueNode *next;
	char *item;
};

struct queue {
	struct queueNode *head;
	struct queueNode *tail;
	int size;
}

typedef struct queue* queue_t;

/* adds item to the tail of the queue
 * returns int for any error */
int enqueue(queue_t q, char *item);

/* places front item into ret,
 * returns int for any error
 * NOTE: change return type to char* if there are no possible errors */
int dequeue(queue_t q, char **ret);

