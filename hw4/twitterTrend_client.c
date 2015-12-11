/* CSci4061 Assignment 4
* name: Dalton Hildreth, Timothy Kohn
* id: hildr039, kohnx074 */

/*Bulk of my connection code at the beginning is heavily influenced by
  the client example from Beej's Guide to Network Programming, at
  www.retran.com/beej/clientserver.html#simpleclient
*/

#include "twitterTrend_client.h"

int main( int argc, char *argv[] ) {
    int sockfd, getaddrinfo_rv, i, n, port_check;
    struct addrinfo hints;
    struct addrinfo *serv_info, *p;
    char *portno = ( char * ) malloc ( sizeof ( char ) * MAXPORTNOSIZE );
	char **fileArray;
    char *host_name = ( char * ) malloc ( sizeof ( char ) * HOST_NAME_MAX );

    if ( argc < 4 ) {
        fprintf( stderr, "Usage : %s <hostname> <port number> <file_path(s)> [-v]\n", argv[ 0 ] );
        return 1;
    }

    port_check = atoi( argv[ 2 ] );
    if ( port_check < 0 || port_check > 65535 ) {
        errno = EINVAL;
        errorFunction( "port number is less than 0 or greater than 65535, exiting" );
    }

    host_name = argv[ 1 ];

    if ( strlen( host_name ) > HOST_NAME_MAX ) {
        fprintf( stderr, "Error: host name is too long, must be 255 or less characters\n" );
        return 1;
    }

	int fileArrayEnd;
	if (strlen( argv[ argc - 1  ] ) == 2 &&
		strcmp( argv[ argc - 1  ], "-v") == 0) {// -v used
		verboseDebug = true;
		fileArrayEnd = argc - 4; //there are this many files
	}
	else {
		verboseDebug = false;
		fileArrayEnd = argc - 3;
	}
	
	fileArray = (char **) malloc( sizeof( char * ) * MAXFILEPATHSIZE * fileArrayEnd );
    for (i = 0; i < fileArrayEnd; i++ ) {
        fileArray[ i ] = ( char * ) malloc ( sizeof ( char ) * MAXFILEPATHSIZE );
        fileArray[ i ] = argv[ 3+i ]; //3 is the start of file arguments
    }

    //set hints struct to 0 to ensure no garbage values
    memset( &hints, 0, sizeof ( hints ) );
    portno = argv[ 2 ];

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
            perror( "Client failed to connect, trying again" );
            continue;
        }

        break;
    }

    if ( p == NULL ) {
        errorFunction( "Client failed to connect" );
    }

	if (verboseDebug)
    	printf( "client connects\n" ); //displayed when connection successful

    if ( clientHandShake( sockfd, verboseDebug ) == -1 ) {
        printf( "Error: an error message was sent or recieved during handshake, exiting\n" );
        exit( EXIT_FAILURE );
    }

	/* do communication */
    char *fileName;
    char **cityNames = ( char ** ) malloc ( sizeof ( char * ) * MAXCITYSIZE * MAXCITIES );
    char *cityName = ( char * ) malloc ( sizeof ( char ) * MAXCITYSIZE );
    message_t response_msg;
    for( int k = 0; k < fileArrayEnd; k++ ) {
        fileName = fileArray[ k ];
        cityNames = getCityNames( fileName );
		if (cityNames == NULL) {
			printf("client skipping file \"%s\" due to error\n", fileName);
			continue;
		}

    	FILE *reportFile;
    	char *reportFileName = ( char * ) malloc ( sizeof ( char ) * MAXFILEPATHSIZE );
    	strcpy( reportFileName, fileName );
		strcat( reportFileName, ".result" ); //create name of result file
    	reportFile = fopen( reportFileName, "w+" ); //w+ is truncate read&write

		/* do the kth files communication */
        for (n = 0; cityNames[ n ] != NULL; n++ ) {
            memset( cityName, 0, ( sizeof ( char ) * MAXCITYSIZE ) );
            cityName = cityNames[ n ];
            twitterTrendRequest( sockfd, cityName, verboseDebug );
            response_msg = waitForResponse( sockfd );
            if ( acknowledgeEndOfResponse( sockfd ) == -1 ) {
                printf( "Error: an error message was sent or recieved during end of response, exiting\n" );
                exit( EXIT_FAILURE );
            }
            if ( response_msg->id == ERRMSG ) { //need to output payload, close the connection, then exit
                fprintf( stderr, "Received error message from server, closing connection. Payload: %s\n", response_msg->payload );
                close( sockfd );
                exit(EXIT_FAILURE);
            }

            writeReportFile( reportFile, cityName, response_msg );
        }

		//some cleanup....
		fclose( reportFile );
		free( reportFileName );
    }

    endRequest( sockfd, verboseDebug );
}

char **getCityNames( char *filepath ) {
    FILE *cityFile;
    int lineCounter = 0;
    int cityLength;
    if ( filepath == NULL ) { //check to make sure something is in filepath
        perror( "Error : filepath doesn't exist, avoided seg fault\n" );
		return NULL;
    }
    else {
        if ( access ( filepath, F_OK ) != -1 ) {  //check if file exists, if it does open it
            cityFile = fopen ( filepath, "r" );
        } else { //it does not exist
             printf( "Error: tried to open file \"%s\", which does not exist\n", filepath );
			 return NULL;
        }
    }

    char **cityNames = ( char ** ) malloc ( sizeof ( char * ) * MAXCITYSIZE * MAXCITIES );
    char *buffer = ( char * ) malloc ( sizeof ( char ) * MAXCITYSIZE );
    while ( fgets( buffer, MAXCITYSIZE, cityFile ) != NULL ) {
        cityNames[ lineCounter ] = ( char * ) malloc ( sizeof ( char ) * MAXCITYSIZE );
        cityLength = strlen( buffer );
        if (buffer[ cityLength - 1 ] == '\n') {
            buffer[ cityLength - 1 ] = '\0';
        }
        strcpy( cityNames[ lineCounter ], buffer );
        lineCounter++;
    }

    fclose( cityFile );
    return cityNames;
}

void writeReportFile( FILE* reportFile, char *cityName, message_t response_msg ) {
    int lineAfterCityNameLength;
    int cityLength;

    lineAfterCityNameLength = response_msg->length; //length of payload
    cityLength = strlen( cityName );
    if (cityName[ cityLength - 1 ] == '\n') {
        cityName[ cityLength - 1 ] = '\0';
    }

    fwrite( cityName, sizeof ( char ), cityLength, reportFile );
    fputc( ' ', reportFile );
    fputc( ':', reportFile );
    fputc( ' ', reportFile );
    fwrite( response_msg->payload, sizeof ( char ), lineAfterCityNameLength-1, reportFile );
    fputc( '\n', reportFile );
}
