/* CSci4061 Assignment 4
* name: Dalton Hildreth, Timothy Kohn
* id: hildr039, kohnx074 */

#ifndef MESSAGER_H
#define MESSAGER_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
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

message_t construct_message_blank();

message_t construct_message( int msg_id, char *msg_payload );

char * build_string_message( message_t msg );

char * build_string_request_message( message_t msg );

//preps for a recv
void clean_message( message_t msg);

void destruct_message( message_t msg );

int sendMessage( int sock_fd, message_t send );
void handle_pipeDeath( int signo );

int recvMessage( int sock_fd, message_t recv );

//from restart library
ssize_t r_read( int fd, void *buf, size_t size );
ssize_t r_write( int fd, void *buf, size_t size );

int clientHandShake( int sock_fd, bool verboseDebug );

int serverHandShake( int sock_fd, char * addr, bool verboseDebug );

void twitterTrendRequest( int sock_fd, char * cityName, bool verboseDebug );

message_t waitForResponse( int sock_fd );

int acknowledgeEndOfResponse( int sock_fd );

void endRequest( int sock_fd, bool verboseDebug );

#endif
