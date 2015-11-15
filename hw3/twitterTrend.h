/* CSci4061 Assignment 3
* name: Dalton Hildreth, Timothy Kohn
* id: hildr039, kohnx074 */

/* this file implements the main execution, input entry, and the threads */
/* NOTE: We may want to move the threads to a different file, I just thought this would
 * be convenient */

#ifndef TWITTERTREND_H_GUARD
#define TWITTERTREND_H_GUARD

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>

#include "twitterDBMem.h"
#include "queue.h"

#define TWITTERDB_FILENAME "TwitterDB.txt"
#define MAXCITYNAMELENGTH 15

/* global variables */
sem_t full_slots;
sem_t empty_slots;
sem_t mut;
queue_t queue;
TwitterDBMem_t tdbm;
FILE *inFile;
/* <insert global variables> */

/* Reads TwitterDB.txt into the tdbm struct */
void readTwitterDB();

/* opens/validates *.in file */
void openInFile(char *inFile);

/* a thread which processes clients/items on the queue
 * args is nothing AFAIK (global "arguments")
 * returns pointer to int (error) */
void *processer( void *args );

/* a thread which adds clients/items to the queue
 * args is nothing AFAIK (global "arguments")
 * returns pointer to int (error) */
void *queueer( void *args );

#endif //TWITTERTREND_H_GUARD

