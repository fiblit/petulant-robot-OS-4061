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
    char **fileArray = ( char ** ) malloc ( sizeof ( char ) * MAXFILEPATHSIZE * (argc - 3) ); //argc-3 is amount of files given
    char *host_name = ( char * ) malloc ( sizeof ( char ) * HOST_NAME_MAX );

    if ( argc < 4 ) {
        fprintf( stderr, "Usage : %s <hostname> <port number> <file_path(s)>\n", argv[ 0 ] );
        return 1;
    }

    port_check = atoi( argv[ 2 ] );
    if ( port_check < 0 || port_check > 65535 ) {
        fprintf( stderr, "Error, port number must be above 0 and below 65535\n" );
        return 1;
    }

    host_name = argv[ 1 ];
    if ( strlen( host_name ) > HOST_NAME_MAX ) {
        fprintf( stderr, "Error, host name is too long, must be 255 or less characters\n" );
        return 1;
    }

    for (i = 0; i < ( argc - 3 ); i++ ) {
        fileArray[ i ] = ( char * ) malloc ( sizeof ( char ) * MAXFILEPATHSIZE );
        fileArray[ i ] = argv[ 3 + i ]; //3+i is all of the file arguments
        //printf( "Here is fileArray[%d] : %s\n", i, fileArray[ i ] );
    }

    //set hints struct to 0 to ensure no garbage values
    memset( &hints, 0, sizeof ( hints ) );
    portno = argv[ 2 ];
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

    printf( "client connects\n" ); //displayed when connection successful

    if ( clientHandShake( sockfd ) == -1 ) {
        printf( "Error: an error message was sent or recieved during handshake, exiting\n" );
        exit( EXIT_FAILURE );
    }

    char *fileName = ( char * ) malloc ( sizeof ( char ) * MAXFILEPATHSIZE );
    char **cityNames = ( char ** ) malloc ( sizeof ( char * ) * MAXCITYSIZE * MAXCITIES );
    char *cityName = ( char * ) malloc ( sizeof ( char ) * MAXCITYSIZE );
    message_t response_msg;
    for( int k = 0; k < ( argc - 3 ); k++ ) {
        fileName = fileArray[ k ];
        cityNames = getCityNames( fileName );

        for (n = 0; cityNames[ n ] != NULL; n++ ) {
            memset( cityName, 0, ( sizeof ( char ) * MAXCITYSIZE ) );
            cityName = cityNames[ n ];
            twitterTrendRequest( sockfd, cityName );
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
            //printf("DEBUGCLIENT1: response_msg->payload : %s\n", response_msg->payload );

            writeReportFile( fileName, cityName, response_msg );
            //free( cityName );
        }
    }

    endRequest( sockfd );
}

char **getCityNames( char *filepath ) {
    FILE *cityFile;
    int lineCounter = 0;
    int cityLength;
    if ( filepath == NULL ) { //check to make sure something is in filepath
        errorFunction( "Error : filepath doesn't exist" );
    }
    else {
        if ( access ( filepath, F_OK ) != -1 ) {  //check if file exists, if it does open it
            cityFile = fopen ( filepath, "r" );
        } else { //it does not exist
            errorFunction ( "Error, attempting to open nonexistent file" );
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
