#/* CSci4061 Assignment 3
#* name: Dalton Hildreth, Timothy Kohn
#* id: hildr039, kohnx074 */

CC = gcc
CVER = -std=c99
CWARN = -Wall -Werror
DEBUG = -g #-DDEBUG
CFLAGS = $(CVER) $(CWARN) $(DEBUG) -c
LDFLAGS = -pthread

all: twitterTrend_server twitterTrend_client

twitterTrend_client:
	echo "client is currently unimplemented :("

twitterTrend_server: twitterTrend_server.o queue.o twitterDBMem.o errorFunction.o
	$(CC) $(LDFLAGS) -o $@ $^
twitterTrend_server.o: twitterTrend_server.c twitterTrend_server.h twitterDBMem.h queue.h errorFunction.h
	$(CC) $(CFLAGS) $<
twitterDBMem.o: twitterDBMem.c twitterDBMem.h errorFunction.h
	$(CC) $(CFLAGS) $<
queue.o: queue.c queue.h errorFunction.h
	$(CC) $(CFLAGS) $<
errorFunction.o: errorFunction.c errorFunction.h
	$(CC) $(CFLAGS) $<

clean:
	rm -f *.o twitterTrend_server twitterTrend_client
