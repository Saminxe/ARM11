CC      = gcc
CFLAGS  = -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o 

.PHONY: all clean

all: music tracker tracker_cl

music: music.o

tracker: tracker.o -lm -lcurses

tracker_cl: tracker_cl.o -lm 

clean:
	rm -f .o
	rm -f music
	rm -f tracker
	rm -f tracker_cl

cleanall:
	rm -f .wav
	rm -f .o
	rm -f music
	rm -f tracker
	rm -f tracker_cl
