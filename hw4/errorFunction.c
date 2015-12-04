/* CSci4061 Assignment 3
* name: Dalton Hildreth, Timothy Kohn
* id: hildr039, kohnx074 */

#include "errorFunction.h"

void errorFunction ( char *msg ) {
	perror ( msg );
	exit( EXIT_FAILURE );
}
