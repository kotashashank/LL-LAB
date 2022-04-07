# Definitions

CC = gcc
CC_FLAGS = -Wall -Werror -ggdb

all: runner
gates.o: gates.c gates.h priorityqueue.h
priorityqueue.o: priorityqueue.c priorityqueue.h gates.h

runner: gates.o priorityqueue.o fulladder.c
	$(CC) $(CFLAGS) -o runner fulladder.c gates.o priorityqueue.o 

runner2: gates.o priorityqueue.o fulladder2.c
	$(CC) $(CFLAGS) -o runner2 fulladder2.c gates.o priorityqueue.o 

clock: gates.o priorityqueue.o testclock.c
	$(CC) $(CFLAGS) -o clock testclock.c  gates.o priorityqueue.o 

clean:
	rm -f *.o runner runner2  clock