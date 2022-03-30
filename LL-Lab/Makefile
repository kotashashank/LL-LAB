# Definitions

CC = gcc
CC_FLAGS = -Wall -ggdb 

all: gates.h pq
gates.o: gates.c gates.h
fulladder.o: fulladder.c

runner: fulladder.o gates.o
	$(CC) $(CFLAGS) -o ll gates.o fulladder.o

pq: priorityqueue.c gates.h
	$(CC) $(CFLAGS) -o pq.o gates.h priorityqueue.c priorityqueue.h

clean:
	rm -f *.o