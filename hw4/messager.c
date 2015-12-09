/* CSci4061 Assignment 4
* name: Dalton Hildreth, Timothy Kohn
* id: hildr039, kohnx074 */

#include "messager.h"

message_t construct_message_blank() {
	message_t msg = (message_t) malloc( sizeof(message) );
	msg->payload = NULL;
	msg->id = ERRMSG;
	msg->length = 0;
	return msg;
}

message_t construct_message( int msg_id, char *msg_payload ) {
	message_t msg = (message_t) malloc( sizeof(message) );
	if (msg_payload != NULL) {
		msg->payload = ( char * ) malloc ( sizeof ( char ) * (strlen(msg_payload) + 1) );
		strcpy( msg->payload, msg_payload );
	}
	else {
		msg->payload = NULL;
	}
	msg->id = msg_id;
	if ( msg_id == ERRMSG || msg_id == REQUEST || msg_id == RESPONSE ) {
		msg->length = strlen( msg_payload ) + 1;//strlen doesn't count \0 IIRC
	}
	else {
		msg->length = 0;
	}
	return msg;
}

char * build_string_message( message_t msg ) {
	char * buffer = ( char * ) malloc ( sizeof ( char ) * MAXLINESIZE );
	if (msg->payload != NULL)
		snprintf( buffer, MAXLINESIZE, "(%d,%d,%s)", msg->id, msg->length, msg->payload );
	else
		snprintf( buffer, MAXLINESIZE, "(%d,%d,)", msg->id, msg->length );
	return buffer;
}

void clean_message( message_t msg ) {
	if ( msg->payload != NULL ) {
		free( msg->payload );
	}
}

void destruct_message( message_t msg ) {
	if ( msg->payload != NULL ) {
		free( msg->payload );
	}
	free( msg );
}

//TODO: fix the payload nullification
int sendMessage( int sock_fd, message_t send ) {
	int bytesSent_id, bytesSent_length, bytesSent_payload;
	char *id_string = ( char * ) malloc ( sizeof ( char ) * 4 ); //3 chars + '\0'
	char *length_string = ( char * ) malloc ( sizeof ( char ) * 4 ); //3 chars + '\0'
	sprintf( id_string, "%d", send->id ); //convert id to string for transmission
	sprintf( length_string, "%d", send->length ); //convert length to string for transmission

	bytesSent_id = write( sock_fd, id_string, sizeof( id_string ) );
	if ( bytesSent_id < 0 ) {
		errorFunction( "Error sending id message" );
	}

	bytesSent_length = write( sock_fd, length_string, sizeof( length_string ) );
	if ( bytesSent_length < 0 ) {
		errorFunction( "Error sending length message" );
	}

	bytesSent_payload = write( sock_fd, send->payload, sizeof( send->payload ) );
	if ( bytesSent_payload < 0 ) {
		errorFunction( "Error sending payload message" );
	}

	return bytesSent_id + bytesSent_length + bytesSent_payload;
}

//TODO: fix the payload nullification
int recvMessage( int sock_fd, message_t recv ) {
	int bytesRecv_id, bytesRecv_length, bytesRecv_payload;
	char *id_string = ( char * ) malloc ( sizeof ( char ) * 4 ); //3 chars + '\0'
	char *length_string = ( char * ) malloc ( sizeof ( char ) * 4 ); //3 chars + '\0'
	char *payload_string = ( char * ) malloc ( sizeof ( char ) * MAXLINESIZE );
	clean_message( recv );
	recv = construct_message_blank();

	bytesRecv_id = read( sock_fd, id_string, sizeof( id_string ) );
	if ( bytesRecv_id < 0 ) {
		errorFunction( "Error receiving id message" );
	}
	recv->id = atoi( id_string );

	bytesRecv_length = read( sock_fd, length_string, sizeof( length_string ) );
	if ( bytesRecv_length < 0 ) {
		errorFunction( "Error receiving length message" );
	}
	recv->id = atoi( length_string );

	bytesRecv_payload = read( sock_fd, payload_string, sizeof( payload_string ) );
	if ( bytesRecv_payload < 0 ) {
		errorFunction( "Error receiving payload message" );
	}
	recv->payload = ( char * ) malloc ( sizeof ( char ) * MAXLINESIZE );
	recv->payload = payload_string;

	return bytesRecv_id + bytesRecv_length + bytesRecv_payload;
}

int clientHandShake( int sock_fd ) {
	message_t msg = construct_message_blank();
	recvMessage( sock_fd, msg );
	if ( msg->id != HANDSHAKE ) {
		printf( "Expected handshake from server, returning error message to server\n" );
		msg->id = ERRMSG;
		sendMessage( sock_fd, msg );
		close( sock_fd ); //maybe close it here? or could make it return a message_t and check the id in main and close it there
		return -1;
	}
	else {  //handshake successful, give response
		msg->id = HAND_RESPONSE;
		sendMessage( sock_fd, msg );
		printf( "client sends handshake response: %s\n", build_string_message( msg ) );
	}
	return 0;
}

int serverHandShake( int sock_fd, char *addr ) {
	message_t msg = construct_message( HANDSHAKE, NULL );
	sendMessage( sock_fd, msg );
	printf( "server sends handshaking: %s to client %s", build_string_message( msg ), addr );
	recvMessage( sock_fd, msg );
	if (msg->id == ERRMSG) {
		printf( "server detected that client %s experienced an error during handshake, closing connection\n", addr );
		close( sock_fd );
		return -1;
	}
	else if (msg->id != HAND_RESPONSE) {
		printf( "server detected that client %s incorrectly responded to handshake, closing connection\n", addr );
		close( sock_fd );
		return -1;
	}
	else { //success
		printf( "server received handshake response: %s from client %s\n", build_string_message( msg ), addr );
	}
	return 0;
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
