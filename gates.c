#include <stdarg.h>
#include <stdbool.h>
#include "gates.h"

// The delay of a gate.
extern unsigned delay;

// The next three routines allow a test routine to specify the circuit structure.
extern port_t port(const ptype_t pt, int num_args, ...)
{

    port_t new;
    va_list valist;
    int num = num_args;
    va_start(valist, num);
    for(int i = 0; i < num; i++)
    {
        new->name = va_arg(valist, char*);
    }
    va_end(valist);
    new->pt = pt;
    return new;
}
extern void gate(const op_t op, const port_t out, ...)
{

}
extern void wire(const port_t src, const port_t dst)
{
    dst->val = src->val;
    dst->pt = src->pt;
}

// The remaining routines allow a test routine to simulate a given circuit for a number of timesteps.
// A timestep is of an arbitrary and unspecified number of seconds.
extern void clock(const unsigned hi, const unsigned lo)
{

}

extern void set_port(port_t p, bool val, unsigned t)
{
    p->val = val;
}
extern bool get_port(port_t p)
{
    return p->val;
}
extern unsigned get_sim_time(void)
{

}

extern void sim_init(void)
{

}
extern void sim_run(const unsigned nsteps)
{

}