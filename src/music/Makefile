CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: music tracker

music: music.o

tracker: tracker.o -lm -lcurses

clean:
	rm -f .o
	rm -f *.wav
	rm -f music
	rm -f tracker
