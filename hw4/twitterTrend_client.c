/* CSci4061 Assignment 4
* name: Dalton Hildreth, Timothy Kohn
* id: hildr039, kohnx074 */

/*Bulk of my connection code at the beginning is heavily influenced by
  the client example from Beej's Guide to Network Programming, at
  www.retran.com/beej/clientserver.html#simpleclient
*/

#include "twitterTrend_client.h"

int main( int argc, char *argv[] ) {
    int sockfd, getaddrinfo_rv;
    struct addrinfo hints;
    struct addrinfo *serv_info, *p;
    char *portno = ( char * ) malloc ( sizeof ( char ) * MAXPORTNOSIZE );
    char *filepath = ( char * ) malloc ( sizeof ( char ) * MAXFILEPATHSIZE );
    char *host_name = ( char * ) malloc ( sizeof ( char ) * HOST_NAME_MAX );

    if ( argc != 4 ) {
        fprintf( stderr, "Usage : %s <hostname> <port number> <file_path>\n", argv[ 0 ] );
        return 1;
    }

    //set hints struct to 0 to ensure no garbage values
    memset( &hints, 0, sizeof ( hints ) );
    portno = argv[ 2 ];
    filepath = argv[ 3 ];
    host_name = argv[ 1 ];

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    getaddrinfo_rv = getaddrinfo( host_name, portno, &hints, &serv_info );
    if ( getaddrinfo_rv != 0 ) {
        fprintf( stderr, "getaddrinfo error: %s\n", gai_strerror( getaddrinfo_rv ) );
        exit( EXIT_FAILURE );
    }

    //loop through the addresses trying until one works
    for ( p = serv_info; p != NULL; p = p->ai_next ) {
        sockfd = socket( p->ai_family, p->ai_socktype, p->ai_protocol );
        if ( sockfd == -1 ) {
            perror( "Client, error creating socket, trying again" );
            continue;
        }

        if ( connect( sockfd, p->ai_addr, p->ai_addrlen ) == -1 ) {
            close ( sockfd );
            perror( "Client, error connecting, trying again" );
            continue;
        }

        break;
    }

    if ( p == NULL ) {
        errorFunction( "Client failed to connect" );
    }

    //if connection succeeds print client connects to stdout
    printf( "client connects\n" ); //displayed when connection successful

    clientHandShake( sockfd );

    //could have an array of the 2d arrays cityNames, one for each filepath
    char **cityNames = getCityNames( filepath );
    int i;
    char * cityName = ( char * ) malloc ( sizeof ( char ) * MAXCITYSIZE );
    for (i = 0; cityNames[ i ] != NULL; i++ ) {
        cityName = cityNames[ i ];
        printf( "Here is cityname currently at %d, %s", i, cityNames[ i ] );
        twitterTrendRequest( sockfd, cityName );
        message_t response_msg = waitForResponse( sockfd );
        if ( response_msg->id == ERRMSG ) {
            printf( "Received error message, closing connection\n" );
            close( sockfd );
        }

        writeReportFile( filepath, cityName, response_msg );
    }

    //finally, end the request and close the connection (?)
    endRequest( sockfd );
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
        cityNames[ lineCounter ] = buffer;  //segfaults when I try to use strcpy, and this only grabs the last city for all the entries
        //TODO: FIX THE ABOVE LINE to correctly grab our cityNames
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
    reportFile = fopen( reportFileName, "a+" ); //a+ mode will create the file and append further entries onto it

    lineAfterCityNameLength = response_msg->length; //length of payload
    cityLength = strlen( cityName );
    if (cityName[ cityLength - 1 ] == '\n') {
        cityName[ cityLength - 1 ] = '\0';
    }

    fwrite( cityName, sizeof ( char ), cityLength, reportFile );
    fputc( ' ', reportFile );
    fputc( ':', reportFile );
    fputc( ' ', reportFile );
    fwrite( response_msg->payload, sizeof ( char ), lineAfterCityNameLength, reportFile );
    fputc( '\n', reportFile );
    fclose( reportFile );
}
