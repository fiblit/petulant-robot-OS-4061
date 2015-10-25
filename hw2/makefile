CC = gcc
DEBUG = -g
COPT = -Wall $(DEBUG) -c
CVER = -std=c99

all: codec_4061

codec_4061: codec_4061.o codec.o
	$(CC) -o $@ $^
codec_4061.o: codec_4061.c codec_4061.h
	$(CC) $(CVER) $(COPT) $<
codec.o: codec.c codec.h
	$(CC) $(CVER) $(COPT) $<
	
clean:
	rm -f *.o codec_4061
