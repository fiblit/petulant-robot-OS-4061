/* CSci4061 Assignment 3
* name: Dalton Hildreth, Timothy Kohn
* id: hildr039, kohnx074 */

/* This file implements the Data structure to store the twitter database */
/* Currently it is an array, but the plan is to turn it into a hashtable */

#ifndef TWITTERDBMEM_H_GUARD
#define TWITTERDBMEM_H_GUARD

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TwitterDBMem {
	char **lines;//Array of strings (lines, potentially sorted)
	int numLines;
} TwitterDBMem;

typedef TwitterDBMem* TwitterDBMem_t;

/* construct a TwitterDBMem from FILE */
TwitterDBMem_t TwitterDBMem_construct(FILE* twitterDB);

/* destruct tdbm */
void TwitterDBMem_destruct(TwitterDBMem_t tdbm);

/* points cline to the city's line, or NULL if city does not exist. 
 * Returns false if city DNE, otherwise true. */
bool TwitterDBMem_getCityKwd(TwitterDBMem_t tdbm, const char *city, char **cline);

#endif //TWITTERDBMEM_H_GUARD

