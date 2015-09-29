#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <string.h>
#include "makeargv.h"
#include "graph.h"

node_t readNode(char *line);

char *getLine(FILE *in);

#endif
