CC = gcc
DEBUG = -g
COPT = -Wall $(DEBUG) -c

all: codec_4061

codec_4061: codec_4061.o codec.o
	$(CC) -o $@ $^
codec_4061.o: codec_4061.c codec_4061.h
	$(CC) $(COPT) $^
codec_4061.o: codec.c codec.h
	$(CC) $(COPT) $^
	
clean:
	rm -f *.o codec_4061
