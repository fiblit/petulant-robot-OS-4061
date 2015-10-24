#include "codec_4061.h"

int main( int argc, char *argv[] ) {
	if (argc != 4 || strlen( argv[ 1 ] ) != 2 || argv[ 1 ][ 0 ] != '-') {
		printf( "Usage: %s -(e|d) (input_dir) (output_dir)", argv[ 0 ] );
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
	mkdir_r( output_input );

	char *reportName = (char *) malloc( sizeof( char ) * (lenOut + 1 + lenIn + 11 + 1));//the final plus one is for \0
	reportName[ 0 ] = '\0';
   	strcat( strcat( reportName, output_input), "_report.txt");
	FILE *freport = fopen( reportName, "a+");
	free( reportName );
	free( output );

	if (codeDir( input, output_input, isEncode, freport ) != 0) {
		exit(EXIT_FAILURE);
	}
	free( input );
	free( output_input );
	//TODO: sort report file :P
	fclose( freport );
	
	return 0;
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
		if (stat( inputFile, &ebuf ) == -1) {
			fprintf( stderr, "There was an error in reading information about %s/%s:\n\t%s\n", input, entry->d_name, strerror( errno ));
			return -1;
		}
		
		if (S_ISDIR( ebuf.st_mode )) {
			mkdir_r( outputFile );//in this case the "file" is specifically a dir
			if (codeDir(inputFile, outputFile, isEncode, report) != 0)
				return -1;
			//TODO: add report statement for directory
		}
		else {
			FILE *in = fopen( inputFile, "r");
			FILE *out = fopen( outputFile, "w");
			//TODO: encode/decode the file input/entry->d_name
			fclose( in );
			fclose( out );

			//write to report
			fputs(entry->d_name, report );
			//TODO: write more info to report based on coding
			fputc('\n', report );
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
		exit(EXIT_FAILURE);//TODO: get rid of this :P
	}
	return retval;
}

