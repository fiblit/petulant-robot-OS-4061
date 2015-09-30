#TODO: add debug option
all: graphexec

graphexec: graphexec.o IO.o graph.o makeargv.o
	gcc graphexec.o IO.o graph.o makeargv.o -o graphexec

graphexec.o: graphexec.c graphexec.h
	gcc -g -std=c99 -c graphexec.c
IO.o: IO.c IO.h makeargv.h graph.h
	gcc -g -std=c99 -c IO.c
graph.o: graph.c graph.h
	gcc -g -std=c99 -c graph.c
makeargv.o: makeargv.c makeargv.h
	gcc -g -std=c99 -c makeargv.c
#I usually like having a bin and obj folder, but this project is so small that it can be added it later.
#obj/:
#	mkdir obj/
#bin/:
#	mkdir bin/

clean:
	rm -f *.o *.out graphexec
