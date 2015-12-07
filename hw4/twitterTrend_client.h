/* CSci4061 Assignment 4
* name: Dalton Hildreth, Timothy Kohn
* id: hildr039, kohnx074 */

#ifndef TWITTERTREND_CLIENT_H
#define TWITTERTREND_CLIENT_H

#define MAXCITYSIZE 15
#define MAXFILEPATHSIZE 19 //clientXX.in , clientXX.txt, and clientXX.in.result are not longer than 19 chars
#define MAXCITIES 50 //according to PA3 forums, max lines in a .in file is 50

#include "errorFunction.h"
#include "messager.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

char **getCityNames( char *filepath );

void writeReportFile( char *filepath, char *cityName, message_t response_msg );

#endif
