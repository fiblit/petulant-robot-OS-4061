#include "messager.h"

message_t construct_message() {
	message_t msg = (message_t) malloc( sizeof(message) );
	msg->id = ERRMSG;
	msg->length = 0;
	msg->payload = NULL;
}

void destruct_message( message_t msg ) {
	if (msg->payload != NULL)
		free( msg->payload );
	free( msg );	
}



