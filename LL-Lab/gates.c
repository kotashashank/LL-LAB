#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <priorityqueue.h>
#include "gates.h"

// The delay of a gate.
unsigned delay;

void freeLL(linked_list ll) {
    while (ll) {
        linked_list next = ll->next;
        free(ll->data);
        free(ll);
        ll = next;
    }
}
// Initilizes a new port
port_t port(const ptype_t pt, const char *name)
{//in the future, we might need to add EXT_IN to global for memory management
  
    port_t new_port = malloc(sizeof(port_t));
    new_port->pt = pt;
    new_port->name = name;
    pdata_t misc_info = malloc(sizeof(pdata_t));
    misc_info->isValid = 0;
    misc_info->nextGate = 0;
    misc_info->ports = malloc(sizeof(port_t) * 3);//value doesn't matter b/c currently invalid
    void * misc = (void *) (misc_info);
    new_port->misc = misc; // every port starts undefined

    printf("port added of type %i and of name %s\n", new_port->pt, new_port->name);


    // TODO: ADD TO HASHMAP

    return new_port;

}

void updateVals(linked_list listOfPorts, linked_list listOfValues) {
    while (listOfValues) {
         //void -> port_t * -> pdata_t -> value, isValid
         ((pdata_t)(((port_t)(listOfPorts->data))->misc))->value = *((bool *)(listOfValues->data));
         ((pdata_t)(((port_t)(listOfPorts->data))->misc))->isValid = 1;

         listOfValues = listOfValues->next;
         listOfPorts = listOfPorts->next;
         
         assert(0);//still need to push these new ports to the priority queue

    }
}

void gate(const op_t op, const port_t out, const unsigned num_in, ...)
{

    
    // create one-dimensional input_port array
    va_list va;
    va_start(va, num_in);

    port_t port_inputs[num_in];
    port_t *port_input_start = port_inputs;

    for (int i = 0; i < num_in; i++) {
        port_inputs[i] = va_arg(va, port_t);
        printf("adding port %d: port %s to this gate\n", i, port_inputs[i]->name);
    }

    gate_t new_gate = malloc(sizeof(gate_t) + sizeof(port_inputs) * 2); // idk i hate c

    // set gate struct
    new_gate->op = op;
    new_gate->port_inputs = port_input_start;
    new_gate->port_outputs = out;
    new_gate->delay = 2; // FIX
    

    // ADD TO HASHMAP
}

//init wire
void wire(const port_t src, const port_t dst)
{
    // dst->val = src->val;
    // dst->pt = src->pt;

    //add to front of linkedlist
    linked_list next = ((pdata_t) src->misc)->ports;
    linked_list newData = malloc(sizeof(node));
    newData->data = (void *)(dst);
    newData->next = next;
    ((pdata_t) src->misc)->ports = newData;
    

}

// The remaining routines allow a test routine to simulate a given circuit for a number of timesteps.
// A timestep is of an arbitrary and unspecified number of seconds.
void clock(const unsigned hi, const unsigned lo)
{

}

void set_port(port_t p, bool val)
{

    p->misc = (short*) val;

}
bool get_port(port_t p)
{
    if(p->misc == UNDEFINED) assert(false);
    return p->misc;
}
unsigned get_sim_time(void) {
    return -1;
}

void sim_init(void) {
    return;
}
void sim_run(const unsigned nsteps) {
    return;
}