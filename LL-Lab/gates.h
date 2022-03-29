#ifndef _GATES_H_
#define _GATES_H_

#include <stdarg.h>


// troolean false/true/undefined

#define UNDEFINED 2
#define TRUE 1
#define FALSE 0

// The delay of a gate.
extern unsigned delay;

// The different kinds of ports.
typedef enum ptype {
    PTYPE_IN,
    PTYPE_OUT,
    PTYPE_EXT_IN,
    PTYPE_EXT_OUT,
    PTYPE_ERR = -1
} ptype_t;

typedef struct port_data {
    bool value;
    bool isValid;
    gate_t nextGate;//null if no gate
    port_t ports[]; 
} *pdata_t;

// The port object.
typedef struct port {
    char *name; // Name of port (optional)
    void *misc; // Anything else you want
    ptype_t pt;
} *port_t;

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

// The gate object.
typedef struct gate {
    op_t op;
    port_t port_inputs[];
    port_t port_outputs[];
    int delay;
} *gate_t;

// The next three routines allow a test routine to specify the circuit structure.
extern port_t port(const ptype_t pt, const char *name);
extern void gate(const op_t op, const port_t out, const unsigned num_in, ...);
extern void wire(const port_t src, const port_t dst);

// The remaining routines allow a test routine to simulate a given circuit for a number of timesteps.
// A timestep is of an arbitrary and unspecified number of seconds.
extern void clock(const unsigned hi, const unsigned lo);

extern void set_port(port_t p, bool val);
extern bool get_port(port_t p);
extern unsigned get_sim_time(void);

extern void sim_init(void);
extern void sim_run(const unsigned nsteps);

#endif 