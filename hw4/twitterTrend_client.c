/* CSci4061 Assignment 4
* name: Dalton Hildreth, Timothy Kohn
* id: hildr039, kohnx074 */

/* outline of this file (and its header)
 *
 * TODO: okay I think I want to add a "message" struct of some sort to handle the messages
 *
 * main:
 *  put files to read in array (for ec) (not hard so might as well)
 * 	socket(): create socket
 *	handshake(): do two-way handshake with server
 *		: handshake includes the sending of message (#100) and message (#101)
 *	for each file to read: (for ec) (not hard so mgiht as well)
 *		report = createReportFile(): ...
 *		for each line in file:
 *			sendMessage(twitterTrendRequest): send linei request (#102) to server
 *			waitMessage(): wait for response
 *			error(reply): quit if reply was error message (#106)
 *			write(kwd reply): write kwd's of reply to this file's .report
 *	sendMessage(endOfRequest): send end of request message (#104)
 *
 */

#include "twitterTrend_client.h"

int main( int argc, char *argv[] ) {
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    char buffer[ 100 ]; //everyline in TwitterDB is less than 100 chars
    char * filepath = ( char * ) malloc ( sizeof ( char ) * MAXFILEPATHSIZE );

    if( argc != 4 ) {
        fprintf( stderr, "Usage : %s <hostname> <port number> <file_path>\n", argv[ 0 ] );
        return 1;
    }

    //set buffer and serv_addr struct to 0 to ensure no garbage values
    memset( buffer, '0', sizeof( buffer ) );
    memset( &serv_addr, '0', sizeof( serv_addr ) );

    portno = atoi( argv[ 2 ] );
    filepath = argv[ 3 ];
    sockfd = socket( AF_INET, SOCK_STREAM, 0 );
    if( sockfd == -1 ) {
        errorFunction( "Socket creation failed" );
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons( portno ); //change byte ordering to network compatible

    //not sure if this is how to get the connection set up properly...
    if ( inet_pton( AF_INET, argv[ 1 ], &serv_addr.sin_addr ) <= 0) { //might need to change argv[1] to ip from host_to_ip func
        errorFunction( "Error occured in inet_pton on client" );
    }

    //if connection succeeds print client connects to stdout
    if( connect( sockfd, ( struct sockaddr * ) &serv_addr, sizeof( serv_addr )) == -1 ) {
        errorFunction( "Connection attempt failed" );
    }

    printf( "client connects\n" ); //displayed when connection successful

    clientHandShake( sockfd );

    //start while loop here to get all the cities/files
    char **cityNames = getCityNames( filepath );
    int n = 0;
    char * cityName = ( char * ) malloc ( sizeof ( char ) * MAXCITYSIZE );
    while ( cityNames[ n ] != NULL ) {
        cityName = cityNames[ n ];
        printf( "Here is cityname currently at %d, %s", n, cityNames[ n ] );
        n++;
        twitterTrendRequest( sockfd, cityName );
        message_t response_msg = waitForResponse( sockfd );
        if ( response_msg->id == ERRMSG ) {
            printf( "Received error message, closing connection\n" );
            close( sockfd );
        }

        writeReportFile( filepath, cityName, response_msg );
        //repeat above in while loop until finished
    }

    //finally, end the request and close the connection (?)
    endRequest( sockfd );
    //close( sockfd ); //might need to do this? probably not, I think server does

}

char **getCityNames( char *filepath ) {
    FILE *cityFile;
    int lineCounter = 0;
    if ( filepath == NULL ) { //check to make sure something is in filepath
		errorFunction( "ERROR : filepath is NULL, fopen would have segfaulted" );
	}
	else {
		if ( access ( filepath, F_OK ) != -1 ) {  //check if file exists, if it does open it
			cityFile = fopen ( filepath, "r" );
		} else { //it does not exist
			errorFunction ( "Error, attempting to open nonexistent file" );
		}
	}
    char **cityNames = ( char ** ) malloc ( sizeof ( char ) * MAXCITYSIZE * MAXCITIES );
    char *buffer = ( char * ) malloc ( sizeof ( char ) * MAXCITYSIZE );
    while ( fgets( buffer, MAXCITYSIZE, cityFile ) != NULL ) {
        strcpy( cityNames[ lineCounter ], buffer);  //segfaults at this line...am clueless why...
        printf( "%d: %s", lineCounter, cityNames[ lineCounter ] );
        lineCounter++;
    }

    //for debugging purposes
    for(int i = 0; i < lineCounter; i++ ) {
        printf( "%d, %s", i, cityNames[ i ] );
    }

    fclose( cityFile );
    return cityNames;
}

void writeReportFile( char *filepath, char *cityName, message_t response_msg ) {
    FILE *reportFile;
    int lineAfterCityNameLength;
    int cityLength;
    char *reportFileName = ( char * ) malloc ( sizeof ( char ) * MAXFILEPATHSIZE );
    strcpy( reportFileName, filepath ); //copy filepath into filePathCopy so filepath isn't altered incase we need it later
    strcat( reportFileName, ".result" ); //create name of result file
    reportFile = fopen( reportFileName, "w+" ); //w+ mode will create the file

    lineAfterCityNameLength = response_msg->length; //length of payload
    cityLength = strlen( cityName );
    fwrite( cityName, sizeof ( char ), cityLength, reportFile );
    fputc( ' ', reportFile );
    fputc( ':', reportFile );
    fputc( ' ', reportFile );
    fwrite( response_msg->payload, sizeof ( char ), lineAfterCityNameLength, reportFile );
    fputc( '\n', reportFile );
    fclose( reportFile );
}
