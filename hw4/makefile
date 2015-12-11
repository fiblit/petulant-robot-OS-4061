#/* CSci4061 Assignment 4
#* name: Dalton Hildreth, Timothy Kohn
#* id: hildr039, kohnx074 */

CC = gcc
CVER = -std=gnu99
CWARN = -Wall -Werror
DEBUG = -g #-DDEBUG
CFLAGS = $(CVER) $(CWARN) $(DEBUG) -c
LDFLAGS = -pthread

all: twitterTrend_client twitterTrend_server

twitterTrend_client: twitterTrend_client.o errorFunction.o messager.o
	$(CC) -o $@ $^
twitterTrend_server: twitterTrend_server.o queue.o twitterDBMem.o errorFunction.o messager.o
	$(CC) $(LDFLAGS) -o $@ $^

twitterTrend_server.o: twitterTrend_server.c twitterTrend_server.h twitterDBMem.h queue.h errorFunction.h messager.h
	$(CC) $(CFLAGS) $<
twitterTrend_client.o: twitterTrend_client.c twitterTrend_client.h messager.h errorFunction.h
	$(CC) $(CFLAGS) $<
twitterDBMem.o: twitterDBMem.c twitterDBMem.h errorFunction.h
	$(CC) $(CFLAGS) $<
queue.o: queue.c queue.h errorFunction.h
	$(CC) $(CFLAGS) $<
messager.o: messager.c messager.h errorFunction.h
	$(CC) $(CFLAGS) $<
errorFunction.o: errorFunction.c errorFunction.h
	$(CC) $(CFLAGS) $<

clean:
	rm -f *.o twitterTrend_server twitterTrend_client

cleanr:
	rm -f *.result

