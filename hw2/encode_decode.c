/*
 * name: Dalton Hildreth, Timothy Kohn
 * x500: hildr039, kohnx074
 * Student ids: 5117575, 4050267
 * CSELabs machine: csel-kh4250-05.cselabs.umn.edu
 */

#include "encode_decode.h"

int encode ( FILE * inFile, FILE* outFile, int inputSize)  {
    char *in = ( char * ) malloc ( sizeof ( char ) * inputSize + 3); // +3 for possible added zeroes and \n
    if ( in == NULL ) { //malloc error checking
        perror( "Call to malloc failed in encode" );
        return -1;
    }
    char *out = ( char * ) malloc (( sizeof ( char ) * inputSize ) * 2 ); //not sure how much to allocate, this works currently
    if ( out == NULL ) { //malloc error checking
        perror( "Call to malloc failed in encode" );
        return -1;
    }

    fread( in, sizeof(char), inputSize, inFile );

    //ZERO padding below
    int inputSizeModThree = inputSize % 3;
    uint8_t chZero = 0;  // padded zero char
    if ( inputSizeModThree != 0 ) {  //add padded zeroes
        memset ( in + ( inputSize ), chZero, inputSizeModThree );
    }

    int i = 0;
    int j = 0; //bytes written / for out

    //encode loop
    while ( i < ( inputSize - inputSizeModThree )) { //goes up to the last chars
        encode_block ( (uint8_t *) in + i, (uint8_t *) out + j, 3 );
        i = i + 3;
        j = j + 4;
    }

    if ( inputSizeModThree != 0 ) { //if it needs special treatment, non-multiple of 3
        encode_block ( (uint8_t *) in + i, (uint8_t *) out + j, inputSizeModThree ); //inputSizeModThree will be 1 or 2
        j = j + 4; // writes 4 bytes
    }

    fwrite( out, sizeof( char ), j, outFile );

    unsigned char chNewLine = 0x0a; // a \n

    //if it wrote anything, append a \n
    if ( j != 0 ) {
        fputc( chNewLine, outFile );
        j = j + 1; //wrote another byte
    }

    free ( in );
    free ( out );

    return j; //number of bytes written
}

int decode ( FILE *inFile, FILE *outFile, int inputSize ) {
    char *in = ( char * ) malloc ( sizeof ( char ) * inputSize );
    if ( in == NULL ) { //malloc error checking
        perror( "Call to malloc failed in decode" );
        return -1;
    }
    char *valid_in = ( char * ) malloc ( sizeof ( char ) * inputSize );
    if ( valid_in == NULL ) { //malloc error checking
        perror( "Call to malloc failed in decode" );
        return -1;
    }
    char *out = ( char * ) malloc (( sizeof ( char ) * inputSize ) * 2 ); //not sure how much to allocate, this works currently
    if ( out == NULL ) { //malloc error checking
        perror( "Call to malloc failed in decode" );
        return -1;
    }

    fread( in, sizeof( char ), inputSize, inFile );

    //loop variables
    int x = 0;
    int y = 0;
    int i = 0;
    int j = 0;

    //validation loop
    while ( x < inputSize ) {
        char test_char = in[ x ];
        if ( is_valid_char ( test_char )) {
            valid_in[ y ] = test_char;
            y++; // size of valid_in
        }
        x++;
    }

    while ( i < y ) {
        decode_block( (uint8_t *) valid_in + i, (uint8_t *) out + j);
        i = i + 4;
        j = j + 3; //bytes written
    }

    fwrite( out, sizeof( char ), j, outFile );

    free ( in );
    free ( valid_in );
    free ( out );

    return j; //number of bytes written
}
