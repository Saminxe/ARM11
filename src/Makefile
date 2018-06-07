CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate music

assemble: assemble.o

emulate: emulate.o

music: music.o

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
