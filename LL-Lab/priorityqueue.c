
//  Priority Queue implementation in C

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "gates.h"
#include "priorityqueue.h"


int size = 0;
unsigned t;
node_t heap_array[];
struct pq_linked_list * pq_global_ll = 0;


void swap(node_t *a, node_t *b) {
  node_t temp = *b;
  *b = *a;
  *a = temp;
}

// Function to heapify the tree
void heapify(node_t array[], int size, int i) {
  if (size == 1) {
    printf("Single element in the heap");
  } else {
    // Find the largest among root, left child and right child
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if (l < size && array[l]->t < array[largest]->t )
      largest = l;
    if (r < size && array[r]->t < array[largest]->t)
      largest = r;

    // Swap and continue heapifying if root is not largest
    if (largest != i) {
      swap(&array[i], &array[largest]);
      heapify(array, size, largest);
    }
  }
}

struct pq_node  * seeFirst() {

   //return (pq_global_ll->node);
  return heap_array[0];
}

void assert_low_priority() {
   struct pq_linked_list * current = pq_global_ll;
   struct pq_linked_list * prev = 0;
   while (current && current->next && current->node->t > current->next->node->t) {
      struct pq_linked_list * hold = current->next->next;
      if (prev) {
      
        prev->next = current->next;//set prev to next to be one over
        current->next->next = current; //se 1 over to point to current
        prev = current;//set previous to be current value
        current->next = hold;//set current to point 2 after
      }
      else {
         pq_global_ll = current->next; 
         current->next->next = current;
         prev = current;//set previous to be current value
         current->next = hold;
      }
      current = current->next;
   }

}
// Function to insert an element into the tree
void insert(node_t array[], node_t new_port) {

  /*
  ll newLL = malloc(sizeof(struct pq_linked_list));//using linked list priority queue instead so can test everything else
  newLL->node = new_port;
  if (pq_global_ll==0) {
    
    newLL->next = 0;
    
  }
  else {
    newLL->next = pq_global_ll;
  }
  pq_global_ll = newLL; 
  assert_low_priority();
  return;*/
  /*node_t new_node = malloc(sizeof(node_t));
  new_node->port = new_port;
  new_node->t = t;*/

  node_t new_node = new_port;

  if (size == 0) {
    array[0] = new_node;
    size += 1;
  } else {
    array[size] = new_node;
    size += 1;
    for (int i = size / 2 - 1; i >= 0; i--) {
      heapify(array, size, i);
    }
  }
}

// Function to delete an element from the tree
void deleteRoot(node_t array[]) {

  /*
  if (pq_global_ll) {
    ll prev = pq_global_ll;
    pq_global_ll = pq_global_ll->next;
    free(prev);
    return;
  }
  else {
    assert(0);
    return;
  }*/




  int i;
  for (i = 0; i < size; i++) {
    if (&array[0] == &array[i])
      break;
  }

  swap(&array[i], &array[size - 1]);
  size -= 1;
  for (int i = size / 2 - 1; i >= 0; i--) {
    heapify(array, size, i);
  }
}

// Print the array
void printArray(node_t array[], int size) {
  for (int i = 0; i < size; ++i)
    {
      if (array[i])
        printf("[name: %s time: %d]", array[i]->port->name, array[i]->t);
    }
  printf("\n");
}

// testing code
/*int main() {
  
  

  port_t HA1a = port(PTYPE_IN, "t0");

  port_t HA1b = port(PTYPE_IN, "t2");

  port_t HA1c = port(PTYPE_IN, "t4");

  insert(heap_array, HA1a);
  t = 2;
  insert(heap_array, HA1b);
  t = 4;
  insert(heap_array, HA1c);

  printf("Max-Heap array: ");
  printArray(heap_array, size);

  deleteRoot(heap_array);
  printf("After deleting an element: ");

  printArray(heap_array, size);

}

*/