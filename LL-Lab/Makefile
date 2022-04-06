# Definitions

CC = gcc
CC_FLAGS = -Wall -ggdb -g

all: runner
gates.o: gates.c gates.h priorityqueue.h
priorityqueue.o: priorityqueue.c priorityqueue.h gates.h

runner: gates.o priorityqueue.o fulladder.c
	$(CC) $(CFLAGS) -o runner fulladder.c  gates.o priorityqueue.o 

runner2: gates.o priorityqueue.o fulladder.c
	$(CC) $(CFLAGS) -o runner2 fulladder2.c  gates.o priorityqueue.o 

clean:
	rm -f *.o runner r2 