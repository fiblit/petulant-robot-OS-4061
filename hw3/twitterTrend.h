/* CSci4061 Assignment 3
* name: Dalton Hildreth, Timothy Kohn
* id: hildr039, kohnx074 */

/* this file implements the main execution, input entry, and the threads */
/* NOTE: We may want to move the threads to a different file, I just thought this would
 * be convenient */

#include <stdio.h>

#include "twitterDBMem.h"
#include "queue.h"

#define TWITTERDB_FILENAME "TwitterDB.txt"

/*
 * <insert global variables> which I did not really spend time making declarations about
 * (there are definitely a few semaphores here)
 */

/* a thread which processes clients/items on the queue
 * args is nothing AFAIK (global "arguments")
 * returns pointer to int (error) */
void *processer( void *args );

/* a thread which adds clients/items to the queue
 * args is nothing AFAIK (global "arguments")
 * returns pointer to int (error) */
void *queueer( void *args );

/* Reads TwitterDB.txt into the tdbm struct
 * returns int for any errors */
int readTwitterDB();

/* reads input arguments, and processes them into a form to be fed to the program 
 * returns int for any errors 
 * NOTE: May or may not actually be implemented... (It might be easier to just be in main) */
int readInputArguments( int argc, char *argv[] );

