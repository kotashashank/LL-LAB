# Definitions

CC = gcc
CC_FLAGS = -Wall -ggdb 

all: runner
gates.o: gates.c gates.h
fulladder.o: fulladder.c

hashmap: hashmap.c
	$(CC) $(CFLAGS) -o hashmap hashmap.c

runner: fulladder.o gates.o
	$(CC) $(CFLAGS) -o ll gates.o fulladder.o

	
clean:
	rm -f *.o