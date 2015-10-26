CC = gcc
DEBUG = -g
COPT = -Wall $(DEBUG) -c
CVER = -std=gnu99 #gnu due to lstat in codec_4061.c::codeDIR

all: codec_4061

codec_4061: codec_4061.o codec.o inodeLL.o encode_decode.o
	$(CC) -o $@ $^
inodeLL.o: inodeLL.c inodeLL.h
	$(CC) $(CVER) $(COPT) $<
encode_decode.o: encode_decode.c encode_decode.h
	$(CC) $(CVER) $(COPT) $<
codec_4061.o: codec_4061.c codec_4061.h
	$(CC) $(CVER) $(COPT) $<
codec.o: codec.c codec.h
	$(CC) $(CVER) $(COPT) $<

clean:
	rm -f *.o codec_4061
