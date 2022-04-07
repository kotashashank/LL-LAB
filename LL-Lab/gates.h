#ifndef _GATES_H_
#define _GATES_H_

#include <stdarg.h>
#include <stdbool.h>

// TODOS
// Need global variable keeping track of all malloc that we can unmalloc at the end

#define TRUE 1
#define FALSE 0

// The delay of a gate.
extern unsigned delay;
extern unsigned t;


// The different kinds of ports.
typedef enum ptype {
    PTYPE_IN,
    PTYPE_OUT,
    PTYPE_EXT_IN,
    PTYPE_EXT_OUT,
    PTYPE_CLK,
    PTYPE_ERR = -1
} ptype_t;


typedef struct linked_list_node {
    void * data;
    struct linked_list_node *next;
} node, *linked_list;


extern void freeLL(linked_list node );

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
    linked_list port_inputs;
    port_t port_output;
    int delay;
} *gate_t;

typedef struct port_data {
    bool value;
    bool is_valid;
    linked_list gates;
    //gate_t nextGate;//null if no gate
    linked_list ports;
    //port_t ports[]; 
} *pdata_t;

typedef struct return_from_logic {
    bool is_valid;
    bool value;
} logic_return;

// The next three routines allow a test routine to specify the circuit structure.
extern port_t port(const ptype_t pt, const char *name);
extern void gate(const op_t op, const port_t out, const unsigned num_in, ...);
extern void wire(const port_t src, const port_t dst);

void process_gate(gate_t g);

// The remaining routines allow a test routine to simulate a given circuit for a number of timesteps.
// A timestep is of an arbitrary and unspecified number of seconds.
extern port_t clock(const unsigned hi, const unsigned lo);

extern void set_port(port_t p, bool val);
extern bool get_port(port_t p);
extern unsigned get_sim_time(void);

extern void sim_init(void);
extern void sim_run(const unsigned nsteps);
extern void sim_exit(void);

#endif 