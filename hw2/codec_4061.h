#ifndef CODEC_4061_H
#define CODEC_4061_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>//requires C99
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

#include "inodeLL.h"

char *fqsort( FILE *f );
int codeDir( char *input, char *output, bool isEncode, FILE* report, inodeLL_t fileInodes );
int mkdir_r( char *dir );

#endif //CODEC_4061_H
