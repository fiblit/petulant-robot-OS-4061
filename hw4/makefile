#/* CSci4061 Assignment 3
#* name: Dalton Hildreth, Timothy Kohn
#* id: hildr039, kohnx074 */

CC = gcc
CVER = -std=c99
CWARN = -Wall -Werror
DEBUG = -g
CFLAGS = $(CVER) $(CWARN) $(DEBUG) -c
LDFLAGS = -pthread

all: twitterTrend

twitterTrend: twitterTrend.o queue.o twitterDBMem.o errorFunction.o
	$(CC) $(LDFLAGS) -o $@ $^
twitterTrend.o: twitterTrend.c twitterTrend.h twitterDBMem.h queue.h errorFunction.h
	$(CC) $(CFLAGS) $<
twitterDBMem.o: twitterDBMem.c twitterDBMem.h errorFunction.h
	$(CC) $(CFLAGS) $<
queue.o: queue.c queue.h errorFunction.h
	$(CC) $(CFLAGS) $<
errorFunction.o: errorFunction.c errorFunction.h
	$(CC) $(CFLAGS) $<

clean:
	rm -f *.o twitterTrend
