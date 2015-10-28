#include "codec_4061.h"

int main( int argc, char *argv[] ) {

    /* Input validation and reading */
    if (argc != 4 || strlen( argv[ 1 ] ) != 2 || argv[ 1 ][ 0 ] != '-') {
        printf( "Usage: %s -[ed] <input_dir> <output_dir>", argv[ 0 ] );
        return 1;
    }

    /* read encoding parameter */
    bool isEncode = ( argv[ 1 ][ 1 ] == 'e' );

    /* Read Input parameter */
    int lenIn = strlen( argv[ 2 ] );
    char *input;
    if (argv[ 2 ][ lenIn - 1 ] == '/') { //The case where "dir/" is entered (removes the /)
        input = (char *) malloc( sizeof( char ) * lenIn );
        if ( input == NULL ) { //malloc error checking
            perror( "Call to malloc failed in main" );
            return -1;
        }
        input = (char *) memset( input, '\0', lenIn );//valgrind wouldn't shut up about uninitalized memory
        strncpy( input, argv[ 2 ], lenIn - 1);//get rid of the final '/'
    }
    else { // the case where "dir" is entered
        input = (char *) malloc( sizeof( char ) * (lenIn + 1) );
        if ( input == NULL ) { //malloc error checking
            perror( "Call to malloc failed in main" );
            return -1;
        }
        strcpy( input, argv[ 2 ]);
    }

    /* read output parameter */
    int lenOut = strlen( argv[ 3 ] );
    char *output;
    if (argv[ 3 ][ lenOut - 1 ] == '/') { //the case where "dir/" is entered (removes the /)
        output = (char *) malloc( sizeof( char ) * lenOut );
        if ( output == NULL ) { //malloc error checking
            perror( "Call to malloc failed in main" );
            return -1;
        }
        output = (char *) memset( output, '\0', lenOut );//valgrind wouldn't shut up
        strncpy( output, argv[ 3 ], lenOut - 1);//get rid of the final '/'
    }
    else { // the case where "dir" is entered
        output = (char *) malloc( sizeof( char ) * (lenOut + 1) );
        if ( output == NULL ) { //malloc error checking
            perror( "Call to malloc failed in main" );
            return -1;
        }
        strcpy( output, argv[ 3 ]);
    }

    /* build (true) output directory string */
    int lenOutput_input = lenOut + 1 + lenIn;
    char *output_input = (char *) malloc( sizeof( char ) * (lenOutput_input + 1) );//output/input not output_input :p
    if ( output_input == NULL ) { //malloc error checking
        perror( "Call to malloc failed in main" );
        return -1;
    }
    output_input[ 0 ] = '\0';
    strcat( strcat( strcat( output_input, output ), "/"), input);
    if(mkdir_r( output_input ) == -1)
        return -1;

    /* build report file */
    //the +11 is the length of "_report.txt"
    char *reportName = (char *) malloc( sizeof( char ) * (lenOutput_input + 11 + 1));//the final plus one is for \0
    if ( reportName == NULL ) { //malloc error checking
        perror( "Call to malloc failed in main" );
        return -1;
    }
    reportName[ 0 ] = '\0';
       strcat( strcat( reportName, output_input), "_report.txt");
    FILE *freport = fopen( reportName, "a+" );
    free( output );

    /* construct linked list of inodes */
    inodeLL_t fileInodes = inodeLL_construct();

    /* Do the actual recursion */
    if (codeDir( input, output_input, isEncode, freport, fileInodes ) != 0) {
        return -1;
    }

    /* cleanup */
    inodeLL_destruct( fileInodes );
    free( input );
    free( output_input );

    /* sort report file */
    char *s = fqsort( freport );

    /* further cleanup */
    if (ferror ( freport )) {
        perror("Error in handling of freport");
        return -1;
    }
    fclose( freport );

    /* finish sorting report file */
    freport = fopen( reportName, "w+" );
    fputs( s, freport );
    free( s );
    free( reportName );
    if (ferror( freport )) {
        perror("Error in handling of freport");
        return -1;
    }
    fclose( freport );

    return 0;
}

static int pstrcompare( const void *p1, const void *p2 ) { //static since only the below qsort needs this.
    return strcmp( *(char * const *)p1, *(char * const *)p2 );
}

char *fqsort( FILE *f ) {
    /* read all lines from f */
    rewind( f );
    char c = fgetc( f );
    int numLine = 0;
    while (c != EOF) {
        if (c == '\n') {
            numLine++;
        }
        c = fgetc( f );
    }
    rewind( f );

    //read into lines from file
    char **lines = (char **)malloc( sizeof( char * ) * numLine );
    if ( lines == NULL ) { //malloc error checking
        perror( "Call to malloc failed in fqsort" );
        exit ( EXIT_FAILURE );
    }
    int totalLen = 0;
    for (int i = 0; i < numLine; i++) {
        char c = fgetc( f );
        int lineLen = 1;
        while (c != '\n' && c != EOF) {//Not at EOL
            c = fgetc( f );
            lineLen++;
        }
        if (c == EOF) {
            lineLen--; //exclude EOF from the file
        }
        totalLen += lineLen;

        lines[ i ] = (char *)malloc( sizeof( char ) * (lineLen + 1) );
        if ( lines[ i ] == NULL ) { //malloc error checking
            perror( "Call to malloc failed in fqsort" );
            exit ( EXIT_FAILURE );
        }
        fseek( f, -lineLen , SEEK_CUR);//move back the # of characters in the line so we can accurately read them
        fgets( lines[ i ], lineLen + 1, f );//1 for the null character
    }
    totalLen += 1;//1 for the \0 character

    /* sort, cleanup, and return */
    qsort( lines, numLine, sizeof( char * ), pstrcompare );

    char *retstr = (char *)malloc( sizeof( char ) * totalLen );
    if ( retstr == NULL ) { //malloc error checking
        perror( "Call to malloc failed in fqsort" );
        exit ( EXIT_FAILURE );
    }
    retstr[ 0 ] = '\0';
    for (int i = 0; i < numLine; i++) {
        strcat( retstr, lines[ i ] );
        free( lines[ i ] );
    }
    free( lines );
    return retstr;
}

/* This function will encode or decode the files of the directory input */
int codeDir( char *input, char *output, bool isEncode, FILE* report, inodeLL_t fileInodes ) {
    struct stat buf;
    if (stat( input, &buf ) == -1) {
        fprintf( stderr, "There was an error reading info about %s\n\t%s\n", input, strerror( errno ) );
        return -1;
    }
    DIR *indir = opendir( input );

    /* for each dir entry */
    struct dirent *entry = readdir( indir );
    while (entry != NULL) {
        if ((strncmp( entry->d_name, ".", 1) == 0) || (strncmp( entry->d_name, "..", 2) == 0)) {
            entry = readdir( indir );
            continue;//skip '.' and '..'
        }

        /* build the input and output "file" strings */
        char *inputFile = (char *) malloc( sizeof( char ) * (strlen( input ) + 1 + strlen( entry->d_name ) + 1));
        if ( inputFile == NULL ) { //malloc error checking
            perror( "Call to malloc failed in codeDir" );
            return -1;
        }
        char *outputFile = (char *) malloc( sizeof( char ) * (strlen( output ) + 1 + strlen( entry->d_name ) + 1));
        if ( outputFile == NULL ) { //malloc error checking
            perror( "Call to malloc failed in codeDir" );
            return -1;
        }
        inputFile[ 0 ] = '\0';//strcat concats onto the null
        outputFile[ 0 ] = '\0';
        strcat( strcat( strcat( inputFile, input ), "/" ), entry->d_name );
        strcat( strcat( strcat( outputFile, output ), "/" ), entry->d_name );

        /* determine filetype */
        struct stat ebuf;
        if (lstat( inputFile, &ebuf ) == -1) {
            fprintf( stderr, "There was an error in reading information about %s/%s:\n\t%s\n", input, entry->d_name, strerror( errno ));
            return -1;
        }

        if (S_ISDIR( ebuf.st_mode )) {

            if(mkdir_r( outputFile ) == -1) {//in this case the "file" is specifically a dir
                return -1;
            }
            if (codeDir( inputFile, outputFile, isEncode, report, fileInodes ) != 0) {
                return -1;
            }

            //write to report
            fprintf( report, "%s, directory, 0, 0\n", entry->d_name );
        }
        else if(S_ISLNK( ebuf.st_mode )) {
            //write to report
            fprintf( report, "%s, sym link, 0, 0\n", entry->d_name );
        }
        else { //is hard link
            FILE *in = fopen( inputFile, "r");
            struct stat inbuf;
            stat( inputFile, &inbuf );
            int inSize = (int)inbuf.st_size;
            ino_t inInode = inbuf.st_ino;
            if (inodeLL_search( fileInodes, inInode )) { //is duplicate hard link
                //write to report
                fprintf( report, "%s, hard link, 0, 0\n", entry->d_name );
            }
            else { //new uncoded file (you can call it hard link)
                inodeLL_append( fileInodes, inInode );//mark this inode as seen

                FILE *out = fopen( outputFile, "w");
                int outSize = 0;
                if ( isEncode ) { // encode
                    outSize = encode ( in, out, inSize );
                }
                else {  // decode
                    outSize = decode ( in, out, inSize );
                }

                if (ferror ( out )) {
                    perror("Error in handling of output file");
                    return -1;
                }
                fclose( out );

                //write to report
                fprintf( report, "%s, regular file, %d, %d\n", entry->d_name, inSize, outSize );
            }
            if (ferror ( in )) {
                perror("Error in handling of input file");
                return -1;
            }
            fclose( in );
        }

        free( inputFile );
        free( outputFile );
        entry = readdir( indir );
    }

    closedir( indir );
    return 0; //Nothing bad happened
}

int mkdir_r( char *dir ) {
    int retval = mkdir((const char*)dir, 0755 );
    // if retval is 0, then dir was created successfuly
    if (retval == -1) {
        fprintf( stderr, "mkdir() failed: %s\n", strerror( errno ));
    }
    return retval;
}
