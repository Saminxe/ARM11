CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: music instrument

music: music.o

clean:
	rm -f $(wildcard *.o)
	rm -f $(wildcard *.wav)
	rm -f music
