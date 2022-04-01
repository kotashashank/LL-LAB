#ifndef _PRIORITY_QUEUE_H_
#define _PRIORITY_QUEUE_H_

#include <stdbool.h>
#include <stdarg.h>

#include "gates.h"

extern int size;

typedef struct pq_node {
  port_t port;
  unsigned int t;
  bool new_value;
} *node_t; 

typedef struct pq_linked_list {
  node_t node;
  struct pq_linked_list * next;
} * ll;

extern struct pq_linked_list * pq_global_ll;
extern node_t heap_array[10];

extern void insert(node_t array[], node_t new_port); 

extern struct pq_node * seeFirst();

extern void deleteRoot(node_t array[]);

extern void printArray(node_t array[], int size);

#endif