# Definitions

CC = gcc
CC_FLAGS = -Wall -ggdb 

all: runner
gates.o: gates.c gates.h priorityqueue.h
priorityqueue.o: priorityqueue.c priorityqueue.h gates.h

runner: fulladder.c gates.o priorityqueue.o
	$(CC) $(CFLAGS) -o runner fulladder.c gates.o priorityqueue.o

clean:
	rm -f *.o