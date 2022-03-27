#ifndef _GATES_H_
#define _GATES_H_
#include <stdarg.h>

// The delay of a gate.
extern unsigned delay;

// The operations performed by a gate.
typedef enum op {
    OP_NOT,
    OP_AND,
    OP_OR,
    OP_XOR,
    OP_NAND,
    OP_NOR,
    OP_ERROR = -1
} op_t;

// The different kinds of ports.
typedef enum ptype {
    PTYPE_IN,
    PTYPE_OUT,
    PTYPE_EXT,
    PTYPE_ERR = -1
} ptype_t;

// The port object.
typedef struct port {
    char *name; // Name of port (optional)
    bool val;   // Value of port (optional)
    ptype_t pt;
} *port_t;

// The next three routines allow a test routine to specify the circuit structure.
extern port_t port(const ptype_t pt, ...);
extern void gate(const op_t op, const port_t out, ...);
extern void wire(const port_t src, const port_t dst);

// The remaining routines allow a test routine to simulate a given circuit for a number of timesteps.
// A timestep is of an arbitrary and unspecified number of seconds.
extern void clock(const unsigned hi, const unsigned lo);

extern void set_port(port_t p, bool val, unsigned t);
extern bool get_port(port_t p);
extern unsigned get_sim_time(void);

extern void sim_init(void);
extern void sim_run(const unsigned nsteps);
#endif