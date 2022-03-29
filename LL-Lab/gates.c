#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include "gates.h"

// The delay of a gate.
unsigned delay;

// Initilizes a new port
port_t port(const ptype_t pt, const char *name)
{
  
    port_t new_port = malloc(sizeof(port_t));
    new_port->pt = pt;
    new_port->name = name;

    printf("port added of type %d and of name  %s\n", new_port->pt, new_port->name);



    // TODO: ADD TO HASHMAP

    return new_port;

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
void wire(const port_t src, const port_t dst)
{
    // dst->val = src->val;
    // dst->pt = src->pt;
}

// The remaining routines allow a test routine to simulate a given circuit for a number of timesteps.
// A timestep is of an arbitrary and unspecified number of seconds.
void clock(const unsigned hi, const unsigned lo)
{

}

void set_port(port_t p, bool val)
{
    //p->val = val;
}
bool get_port(port_t p)
{
    //return p->val;
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