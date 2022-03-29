# Definitions

CC = gcc
CC_FLAGS = -Wall -ggdb 

all: runner
gates.o: gates.c gates.h
fulladder.o: fulladder.c

hashtable: ht.c ht.h
	$(CC) $(CFLAGS) -o hashtable ht.c ht.h

runner: fulladder.o gates.o
	$(CC) $(CFLAGS) -o ll gates.o fulladder.o

pq: priorityqueue.c gates.o
    $(CC) $(CFLAGS) -o pq priorityqueue.c  gates.c gates.h

clean:
	rm -f *.o