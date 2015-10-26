#include "encode_decode.h"

int encode ( FILE * inFile, FILE* outFile, int inputSize)  {
    char *in = ( char * ) malloc ( sizeof ( char ) * inputSize + 3); // +3 for possible added zeroes and \n
    char *out = ( char * ) malloc (( sizeof ( char ) * inputSize ) * 2 ); //not sure how much to allocate, this works currently

    int i = 0;
    int j = 0; //bytes written / for out

    fread( in, 1, inputSize, inFile );

    //ZERO padding below
    /*
    int inputSizeModThree = inputSize % 3;
    uint8_t chZero = 0;  // padded zero char

    if ( inputSizeModThree != 0 ) {  //makes sure its always a multiple of 3 being passed to encode_block
        int k;
        for ( k = 0; k < inputSizeModThree; k++) {
            in[k+inputSize] = chZero;
        }
        //memset ( in + inputSize, chZero, inputSizeModThree );
    }*/

    //encode loop
    while ( i < inputSize ) {
        encode_block ( in + i, out + j, 3 );
        i = i + 3;
        j = j + 4;
    }

    fwrite( out, 1, j, outFile );

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
