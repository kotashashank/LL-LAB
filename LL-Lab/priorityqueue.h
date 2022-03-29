#ifndef _PRIORITY_QUEUE_H_
#define _PRIORITY_QUEUE_H_

#include <stdbool.h>
#include <stdarg.h>

#include "gates.h"

extern int size;

typedef struct node {
  port_t port;
  unsigned int t;
  bool new_value;
} *node_t; 

extern node_t heap_array[10];

extern void insert(node_t array[], port_t new_port); 

extern void deleteRoot(node_t array[]);

extern void printArray(node_t array[], int size);

#endif