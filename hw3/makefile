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

twitterTrend: twitterTrend.o queue.o twitterDBMem.o
	$(CC) $(LDFLAGS) -o $@ $^
twitterTrend.o: twitterTrend.c twitterTrend.h twitterDBMem.h queue.h
	$(CC) $(COPT) $<
twitterDBMem.o: twitterDBMem.c twitterDBMem.h
	$(CC) $(COPT) $<
queue.o: queue.c queue.h
	$(CC) $(COPT) $<

clean:
	rm -f *.o twitterTrend
