CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: MINi

MINi: MINi.o

clean:
	rm -f *.o
	rm -f MINi

cleanall:
	rm -f *.damn
	rm -f *.o
	rm -f MINi


