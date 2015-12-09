/* CSci4061 Assignment 4
* name: Dalton Hildreth, Timothy Kohn
* id: hildr039, kohnx074 */

#include "messager.h"

message_t construct_message( int msg_id, char *msg_payload ) {
	message_t msg = (message_t) malloc( sizeof(message) );
	msg->payload = ( char * ) malloc ( sizeof ( char ) * MAXLINESIZE );
	msg->id = msg_id;
	if ( msg_id == ERRMSG || msg_id == REQUEST || msg_id == RESPONSE ) {
		msg->length = strlen( msg_payload );
	}
	else {
		msg->length = 0;
	}
	msg->payload = msg_payload;
	return msg;
}

char * build_string_message( message_t msg ) {
	char * buffer = ( char * ) malloc ( sizeof ( char ) * MAXLINESIZE );
	snprintf( buffer, MAXLINESIZE, "(%d,%d,%s)", msg->id, msg->length, msg->payload );
	return buffer;
}

void destruct_message( message_t msg ) {
	if ( msg->payload != NULL ) {
		free( msg->payload );
	}
	free( msg );
}

//may not be correct, have yet to be able to test it
int sendMessage( int sock_fd, message_t send ) {
	int bytesSent;
	bytesSent = write( sock_fd, send, sizeof( send ) );
	if ( bytesSent < 0 ) {
		errorFunction( "Error sending message" );
	}
	return bytesSent;
}

//may not be correct, have yet to be able to test it
int recvMessage( int sock_fd, message_t recv ) {
	int bytesRecv;
	bytesRecv = read( sock_fd, recv, MAXLINESIZE );
	if ( bytesRecv < 0 ) {
		errorFunction( "Error receiving message" );
	}
	return bytesRecv;
}

void clientHandShake( int sock_fd ) {
	message_t msg = ( message_t ) malloc ( sizeof ( message ) );
	recvMessage( sock_fd, msg );
	if ( msg->id != HANDSHAKE ) {
		printf( "Expected handshake from server, returning error message to server\n" );
		msg->id = ERRMSG;
		sendMessage( sock_fd, msg );
		close( sock_fd ); //maybe close it here? or could make it return a message_t and check the id in main and close it there
	}
	else {  //handshake successful, give response
		msg->id = HAND_RESPONSE;
		sendMessage( sock_fd, msg );
		printf( "client sends handshake response: %s\n", build_string_message( msg ) );
	}
}

void twitterTrendRequest( int sock_fd, char * cityName ) {
	//message_t msg = ( message_t ) malloc ( sizeof ( message ) );
	//TODO: add quotations to the cityName message being printed, they need to have it according to the forums
	message_t msg = construct_message( REQUEST, cityName );
	sendMessage( sock_fd, msg );
	printf( "client sends twitterTrend request: %s\n", build_string_message( msg ) );
}

message_t waitForResponse( int sock_fd ) {
	message_t msg = ( message_t ) malloc ( sizeof ( message ) );
	recvMessage( sock_fd, msg );
	if ( msg->id == ERRMSG ) {
		printf( "Received error message from server, canceling request\n" );
		printf( "Error message payload: %s\n", msg->payload );
		return msg; //have to still return
	} else if ( msg->id == RESPONSE ) { //everything works correctly
		return msg;
	} else { //any other id is an error
		printf( "Expected twitterTrend response from server, returning error message to server\n" );
		msg->id = ERRMSG;
		sendMessage( sock_fd, msg );
		return msg; //return the malformed msg anyways, where we will cancel the connection if server hasn't already
	}
}

void endRequest( int sock_fd ) {
	message_t msg = construct_message( ENDREQ, "" ); //build msg with empty payload
	sendMessage( sock_fd, msg ); //send endRequest message to server
	printf( "client sends end of request: %s\n", build_string_message( msg ) );
}
