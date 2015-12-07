/* CSci4061 Assignment 4
* name: Dalton Hildreth, Timothy Kohn
* id: hildr039, kohnx074 */

#ifndef MESSAGER_H
#define MESSAGER_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "errorFunction.h"

#define MAXLINESIZE 110

enum ID {HANDSHAKE=100,
	 HAND_RESPONSE,
	 REQUEST,
	 RESPONSE,
	 ENDREQ,
	 ENDRES,
	 ERRMSG};
typedef struct message {
	int id;//should be enum ID, but isn't due to requirements
	int length;
	char* payload;
} message;

typedef message *message_t;

message_t construct_message( int msg_id, char *msg_payload );

char * build_string_message( message_t msg );

void destruct_message( message_t msg );

//TODO: these need to have some network headers included
/* Note: sorry I basically did nothing I meant to get more done but I
   got distracted */

int sendMessage( int sock_fd, message_t send );

int recvMessage( int sock_fd, message_t recv );

void clientHandShake( int sock_fd );

void twitterTrendRequest( int sock_fd, char * cityName );

message_t waitForResponse( int sock_fd );

void endRequest( int sock_fd );

#endif
