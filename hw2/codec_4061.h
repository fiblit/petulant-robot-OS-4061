/*
 * name: Dalton Hildreth, Timothy Kohn
 * x500: hildr039, kohnx074
 * Student ids: 5117575, 4050267
 * CSELabs machine: csel-kh4250-05.cselabs.umn.edu
 */

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
#include "encode_decode.h"

char *fqsort( FILE *f );
int codeDir( char *input, char *output, bool isEncode, FILE* report, inodeLL_t fileInodes );
int mkdir_r( char *dir );

#endif //CODEC_4061_H
