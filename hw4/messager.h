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

typedef message_t message*;

message_t construct_message();

void destruct_message( message_t msg );

//TODO: these need to have some network headers included
/* Note: sorry I basically did nothing I meant to get more done but I
   got distracted */

int sendMessage( message_t send );

int recvMessage( message_t recv );

