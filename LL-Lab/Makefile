# Definitions

CC = gcc
CC_FLAGS = -Wall -ggdb 

all: runner
gates.o: gates.c gates.h
fulladder.o: fulladder.c

ht: ht.c ht.h
	$(CC) $(CFLAGS) -o ht ht.c ht.h

runner: fulladder.o gates.o
	$(CC) $(CFLAGS) -o ll gates.o fulladder.o

	
clean:
	rm -f *.o