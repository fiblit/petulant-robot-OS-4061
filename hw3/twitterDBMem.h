/* CSci4061 Assignment 3
* name: Dalton Hildreth, Timothy Kohn
* id: hildr039, kohnx074 */

/* This file implements the Data structure to store the twitter database */
/* Currently it is an array, but the plan is to turn it into a hashtable */

#include <stdbool.h>

struct TwitterDBMem {
	char **cities;//Array of strings (ordered by lines/potentially sorted)
	char **keywords;//Array of strings
	int numCities;
	int numKeywords;
};

typedef struct TwitterDBMem* TwitterDBMem_t;

/* construct tdbm from FILE, int is for errors */
int TwitterDBMem_construct(TwitterDBMem_t tdbm, FILE* twitterDB);

/* destruct tdbm, int is for errors */
int TwitterDBMem_destruct(TwitterDBMem_t tdbm);

/* points kwds to the city's keywords, or NULL if city does not exist. 
 * Returns false if city DNE, otherwise true. */
bool TwitterDBMem_getCityKwd(TwitterDBMem_t tdbm, const char *city, char **kwds);

