/*
 * name: Dalton Hildreth, Timothy Kohn
 * x500: hildr039, kohnx074
 * Student ids: 5117575, 4050267
 * CSELabs machine: csel-kh4250-05.cselabs.umn.edu
 */

#ifndef ENCODE_DECODE_H
#define ENCODE_DECODE_H


#include "codec.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int encode ( FILE *inputFile, FILE *outputFile, int size);
int decode ( FILE *inputFile, FILE *outputFile, int size);

#endif //ENCODE_DECODE_H
