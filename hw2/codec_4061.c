#include "codec_4061.h"

int main( int argc, char *argv[] ) {
	if (argc != 4 || strlen( argv[ 1 ] ) != 2 || argv[ 1 ][ 0 ] != '-') {
		printf( "Usage: %s -(e|d) (input_dir) (output_dir)", argv[ 0 ] );
		return 1;
	}
	bool isEncode = (argv[1][1] == 'e');
	
	/*
	 mk reportFile
	 fopen reportfile
	 //reportFile parameter, so that codeDIR can update it. (Not the name of it, but the actual FILE* is passed)
	 codeDir(input, output, e/d, reportFile)//Otherwise input and output are just const char *
	 //codeDir could perhaps be split into two different functions encodeDIR and decodeDIR
	 //but it is such a trivial difference in the function that I don't see why you would. :P
	*/

	return 0;
}

/*
codeDir(input, output, e/d, reportfile) //input and output should be just plain const char * They should NOT be FILE* or fd.
	DIR *indir = opendir(input)
	dirent = readDir(indir);
	while dirent != NULL
		if dirent is dir
			mk output/dir, then recurse on codeDir(input/dir, output/dir, e/d)
		else
			encode/decode file
			save to output/file
			write to reportFile
		dirent = readDir(indir);
*/
