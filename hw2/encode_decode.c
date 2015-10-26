#include "encode_decode.h"

int encode ( FILE * inFile, FILE* outFile, int inputSize)  {
    char *in = ( char * ) malloc ( sizeof ( char ) * inputSize + 3); // +3 for possible added zeroes and \n
    char *out = ( char * ) malloc (( sizeof ( char ) * inputSize ) * 2 ); //not sure how much to allocate, this works currently

    fread( in, sizeof(char), inputSize, inFile );

    //ZERO padding below
    int inputSizeModThree = inputSize % 3;
    uint8_t chZero = 0;  // padded zero char
    if ( inputSizeModThree != 0 ) {  //makes sure its always a multiple of 3 being passed to encode_block
        memset ( in + ( inputSize ), chZero, inputSizeModThree );
    }

    int i = 0;
    int j = 0; //bytes written / for out

    //encode loop
    //attempt to handle non multiple of 3 below...
    while ( i < ( inputSize - inputSizeModThree )) { //goes up to the last chars
        encode_block ( in + i, out + j, 3 );
        i = i + 3;
        j = j + 4;
    }

    if ( inputSizeModThree != 0 ) { //if it needs special treatment
        encode_block ( in + i, out + j, inputSizeModThree ); //inputSizeModThree will be 1 or 2
        j = j + 4; // writes inputSizeModThree bytes
    }

    fwrite( out, sizeof(char), j, outFile );

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
    char *valid_in = ( char * ) malloc ( sizeof ( char ) * inputSize );
    char *out = ( char * ) malloc (( sizeof ( char ) * inputSize ) * 2 ); //not sure how much to allocate, this works currently

    fread( in, sizeof(char), inputSize, inFile );

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
        decode_block( valid_in + i, out + j);
        i = i + 4;
        j = j + 3; //bytes written
    }

    fwrite( out, sizeof(char), j, outFile );

    free ( in );
    free ( valid_in );
    free ( out );

    return j; //number of bytes written
}
