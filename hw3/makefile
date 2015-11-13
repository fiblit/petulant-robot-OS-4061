#/* CSci4061 Assignment 3
#* name: Dalton Hildreth, Timothy Kohn
#* id: hildr039, kohnx074 */

CC = gcc
DEBUG = -g
COPT = -Wall $(DEBUG) -c
CVER = -std=c99

all: twitterTrend

twitterTrend: twitterTrend.o queue.o twitterDBMem.o
	$(CC) -o $@ $^
twitterTrend.o: twitterTrend.c twitterTrend.h twitterDBMem.h queue.h
	$(CC) $(CVER) $(COPT) $<
twitterDBMem.o: twitterDBMem.c twitterDBMem.h
	$(CC) $(CVER) $(COPT) $<
queue.o: queue.c queue.h
	$(CC) $(CVER) $(COPT) $<

clean:
	rm -f *.o twitterTrend
