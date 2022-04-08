# Definitions

CC = gcc
CC_FLAGS = -g -Wall

GUI = -g


all: runner
gates.o: gates.c gates.h priorityqueue.h
	$(CC) $(CC_FLAGS) $(GUI) -c gates.c -o gates.o
priorityqueue.o: priorityqueue.c priorityqueue.h gates.h
	$(CC) $(CC_FLAGS) $(GUI) -c priorityqueue.c -o priorityqueue.o

runner: gates.o priorityqueue.o fulladder.c
	$(CC) $(GUI) -o runner fulladder.c gates.o priorityqueue.o

runner2: gates.o priorityqueue.o fulladder2.c
	$(CC) $(CC_FLAGS) $(GUI) -o runner2 fulladder2.c gates.o priorityqueue.o 
	

clock: gates.o priorityqueue.o testclock.c
	$(CC) $(GUI) -o clock testclock.c  gates.o priorityqueue.o 

clean:
	rm -f *.o runner runner2  clock