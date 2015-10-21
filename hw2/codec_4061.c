#include "codec_4061.h"

int main( int argc, char *argv[] ) {
	if (argc != 4 || strlen( argv[ 1 ] ) != 2 || argv[ 1 ][ 0 ] != '-') {
		printf( "Usage: %s -(e|d) (input_dir) (output_dir)", argv[ 0 ] );
		return 1;
	}
	bool isEncode = (argv[1][1] == 'e');
	
	/*
	 //could perhaps be split into two different functions encodeDIR and decodeDIR
	 //but it is such a trivial difference in the function that I don't see why you would. :P
	 codeDIR(input, output, e/d)
	 mkreport??? //(this should go before the call to codeDir. CodeDir then gets a reference to the
	 //report file, so that it can update it. (Not the name of it, but the actual FILE*)
	 */

	return 0;
}

/*
codeDir(input, output, e/d) <- input and output should probably be FILE* (or int fd due to opendir)
	DIR *indir = opendir(input)
	dirent = readDir(indir);
	while dirent != NULL
		if dirent is dir
			mk output/dir, then recurse on codeDir(input/dir, output/dir, e/d)
		else
			encode/decode file
			save to output/file
		dirent = readDir(indir);
 */
