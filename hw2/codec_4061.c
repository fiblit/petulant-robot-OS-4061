#include "codec_4061.h"

int main( int argc, char *argv[] ) {
	if (argc != 4 || strlen( argv[ 1 ] ) != 2 || argv[ 1 ][ 0 ] != '-') {
		printf( "Usage: %s -[ed] <input_dir> <output_dir>", argv[ 0 ] );
		return 1;
	}
	bool isEncode = ( argv[ 1 ][ 1 ] == 'e' );

	int lenIn = strlen( argv[ 2 ] );
	char *input; 
	if (argv[ 2 ][ lenIn - 1 ] == '/') {
		input = (char *) malloc( sizeof( char ) * (lenIn - 1) );
		strncpy( input, argv[ 2 ], lenIn - 1);
		input[ lenIn ] = '\0';//overwrite the dumb final '/' 
	}
	else {
		input = (char *) malloc( sizeof( char ) * lenIn );
		strcpy( input, argv[ 2 ]);
	}
	int lenOut = strlen( argv[ 3 ] );
	char *output;
	if (argv[ 3 ][ lenOut - 1 ] == '/') {
		output = (char *) malloc( sizeof( char ) * (lenOut - 1) );
		strncpy( output, argv[ 3 ], lenOut - 1);
		output[ lenOut ] = '\0';//overwrite the dumb final '/'
	}
	else {
		output = (char *) malloc( sizeof( char ) * lenOut );
		strcpy( output, argv[ 3 ]);
	}
	char *output_input = (char *) malloc( sizeof( char ) * (lenOut + 1 + lenIn + 1) );//output/input not output_input :p
	output_input[ 0 ] = '\0';
	strcat( strcat( strcat( output_input, output ), "/"), input);
	if(mkdir_r( output_input ) == -1)
		return -1;

	char *reportName = (char *) malloc( sizeof( char ) * (lenOut + 1 + lenIn + 11 + 1));//the final plus one is for \0
	reportName[ 0 ] = '\0';
   	strcat( strcat( reportName, output_input), "_report.txt");
	FILE *freport = fopen( reportName, "a+" );
	free( output );

	if (codeDir( input, output_input, isEncode, freport ) != 0) {
		return -1;
	}
	free( input );
	free( output_input );

	char *s = fqsort( freport );
	fclose( freport );
	freport = fopen( reportName, "w+" );
	fputs( s, freport );
	free( reportName );
	fclose( freport );
	
	return 0;
}

static int pstrcompare( const void *p1, const void *p2 ) { //static since only the below qsort needs this.
	return strcmp( *(char * const *)p1, *(char * const *)p2 );
}

char *fqsort( FILE *f ) {
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

	//read in lines from file
	char **lines = (char **)malloc( sizeof( char * ) * numLine );
	int totalLen = 0;
	for (int i = 0; i < numLine; i++) {
		char c = fgetc( f );
		int lineLen = 1;
		while (c != '\n' && c != EOF) {
			c = fgetc( f );
			lineLen++;
		}
		if (c == EOF) {
			lineLen--; //include '\n' in line
		}
		totalLen += lineLen;

		lines[ i ] = (char *)malloc( sizeof( char ) * lineLen );
		fseek( f, -lineLen , SEEK_CUR);
		fgets( lines[ i ], lineLen + 1, f );
		printf( "%s", lines[ i ] );
	}
	totalLen += 1;//1 for the EOF

	qsort( lines, numLine, sizeof( char * ), pstrcompare );

	char *s = (char *)malloc( sizeof( char ) * totalLen );
	s[ 0 ] = '\0';
	for (int i = 0; i < numLine; i++) {
		strcat( s, lines[ i ] );
		free( lines[ i ] );
	}
	return s;
	free( lines );
}

/* This function will encode or decode the files of the directory input */
int codeDir( char *input, char *output, bool isEncode, FILE* report ) {//TODO: list of inodes for EC
	struct stat buf;
	if (stat( input, &buf ) == -1) {
		fprintf( stderr, "There was an error reading info about %s\n\t%s\n", input, strerror( errno ) );
		return -1;
	}
	DIR *indir = opendir( input );
	
	//TODO: further error checking (e.g. mkdir_r)
	struct dirent *entry = readdir( indir );
	while (entry != NULL) {
		if ((strncmp( entry->d_name, ".", 1) == 0) || (strncmp( entry->d_name, "..", 2) == 0)) {
			entry = readdir( indir );
			continue;
		}

		char *inputFile = (char *) malloc( sizeof( char ) * (strlen( input ) + 1 + strlen( entry->d_name ) + 1));
		char *outputFile = (char *) malloc( sizeof( char ) * (strlen( output ) + 1 + strlen( entry->d_name ) + 1));
		inputFile[ 0 ] = '\0';
		outputFile[ 0 ] = '\0';
		strcat( strcat( strcat( inputFile, input ), "/" ), entry->d_name );
		strcat( strcat( strcat( outputFile, output ), "/" ), entry->d_name );

		struct stat ebuf;
		if (stat( inputFile, &ebuf ) == -1) {//TODO: change stat to lstat for EC
			fprintf( stderr, "There was an error in reading information about %s/%s:\n\t%s\n", input, entry->d_name, strerror( errno ));
			return -1;
		}
		
		if (S_ISDIR( ebuf.st_mode )) {
			
			if(mkdir_r( outputFile ) == -1)//in this case the "file" is specifically a dir
				return -1;
			if (codeDir( inputFile, outputFile, isEncode, report ) != 0)
				return -1;
			
			//write to report
			fprintf( report, "%s, directory, 0, 0\n", entry->d_name);
		}
		else {
			FILE *in = fopen( inputFile, "r");
			FILE *out = fopen( outputFile, "w");
		    struct stat inbuf;
			stat( inputFile, &inbuf );//to find filesize of inputFile
			int inSize = (int)inbuf.st_size;
			//TODO: encode/decode the file input/entry->d_name
			fclose( in );
			struct stat outbuf;//TODO: maybe change this to just a return from encodeFile/decodeFile
			stat( outputFile, &outbuf );//to find filesize of outputFile
			int outSize = (int)outbuf.st_size;
			fclose( out );

			//write to report
			fprintf( report, "%s, regular file, %d, %d\n", entry->d_name, inSize, outSize);
		}

		free( inputFile );
		free( outputFile );
		entry = readdir( indir );
	}
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

