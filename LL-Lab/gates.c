#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "priorityqueue.h"
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
  
    port_t new_port = malloc(sizeof(struct port));
    new_port->pt = pt;
    new_port->name = name;
    pdata_t misc_info = malloc(sizeof(struct port_data));
    misc_info->isValid = 0;
    misc_info->nextGate = 0;
    misc_info->ports = malloc(sizeof(port_t) * 3);//value doesn't matter b/c currently invalid
    void * misc = (void *) (misc_info);
    new_port->misc = misc; // every port starts undefined

    printf("port added of type %i and of name %s\n", new_port->pt, new_port->name);


    // TODO: ADD TO HASHMAP

    return new_port;

}

/* irrelevant unless Chat changes the code again (so like 90% chance its useful)
void updateVals(linked_list listOfPorts, linked_list listOfValues) {
    while (listOfValues) {
         //void -> port_t * -> pdata_t -> value, isValid
         ((pdata_t)(((port_t)(listOfPorts->data))->misc))->value = *((bool *)(listOfValues->data));
         ((pdata_t)(((port_t)(listOfPorts->data))->misc))->isValid = 1;

         listOfValues = listOfValues->next;
         listOfPorts = listOfPorts->next;
         
         assert(0);//still need to push these new ports to the priority queue

    }
}*/

void gate(const op_t op, const port_t out, const unsigned num_in, ...)
{

    // create one-dimensional input_port array
    va_list va;
    va_start(va, num_in);

    linked_list port_ins = malloc(sizeof(node));
    port_t data = va_arg(va, port_t);
    port_ins->data = (void *)(data);
    port_ins->next = 0;

    linked_list prevNode = port_ins;
    for (int i = 1; i < num_in; i++) {
        port_t portPointer = va_arg(va, port_t);
        linked_list nextNode = malloc(sizeof(node));
        nextNode->next = 0;
        nextNode->data = (void *)(portPointer);

        prevNode->next = nextNode;
        prevNode = nextNode;
        printf("adding port %d: port %s to this gate\n", i, portPointer->name);
    }

    va_end(va);

    gate_t new_gate = malloc(sizeof(struct gate));

    // set gate struct values
    new_gate->op = op;
    new_gate->port_inputs = port_ins;
    new_gate->port_output = out;
    new_gate->delay = delay;

    for (int i = 0; i < num_in; i++) {
        ((pdata_t)(port_ins->data))->nextGate = new_gate;
    }
    
}

logic_return AND(linked_list inputs) {
    bool allInputValid = 1;
    while (inputs) {
        if( ((pdata_t)(inputs->data))->value == 0) {
            if (((pdata_t)(inputs->data))->isValid ) {
                logic_return definitely_false;
                definitely_false.value = 0;
                definitely_false.isValid = 1;
                return definitely_false;
            }
        }
        if (!((pdata_t)(inputs->data))->isValid) {//if its not valid
            allInputValid = 0;
        }
        inputs = inputs->next;
    }
    logic_return ret;
    ret.value = 1;
    ret.value = allInputValid;
    return ret;
    //return 1;
}

logic_return OR(linked_list inputs) {
    bool allInputValid = 1;
    while (inputs) {
        if( ((pdata_t)(inputs->data))->value == 1) {//we only need 1 valid 1 for it to be truly valid
            if (((pdata_t)(inputs->data))->isValid ) {
                logic_return definitely_false;
                definitely_false.value = 1;
                definitely_false.isValid = 1;
                return definitely_false;
            }
        }
        if (!((pdata_t)(inputs->data))->isValid) {//if its not valid
            allInputValid = 0;
        }
        inputs = inputs->next;
    }
    logic_return ret;
    ret.value = 0;
    ret.value = allInputValid;
    return ret;
    
}

logic_return XOR(linked_list inputs) {//are there an odd number of ones
    bool valReturn = 0;
    logic_return log;
    while (inputs) {
        if (!(((pdata_t)(inputs->data))->isValid)) {
            log.isValid = 0;
            return log;//if its not valid it doesn't matter
        }
        if ((((pdata_t)(inputs->data))->value)) {
            valReturn = !valReturn;
        }
        inputs = inputs->next;
    }
    log.value = valReturn;
    log.isValid = 1;
    return log;
}
//process gate
void process_gate(gate_t g) {
    //first make sure all values are valid
    op_t op = g->op;
    logic_return out;
    switch (op) {
        case OP_AND:
            out = AND(g->port_inputs);
            break;
        case OP_ERROR:
            printf("ERROR HERE gates.c\n");
            assert(0);
            break; 
        case OP_NAND:
            out = AND(g->port_inputs);
            out.value = !out.value;
            break;
        case OP_NOT:
            out.value = !(((pdata_t)(g->port_inputs->data))->value);
            out.isValid = (((pdata_t)(g->port_inputs->data))->isValid);
            //out = (((pdata_t)(g->port_inputs->data))->value);//this operation assumes it is valid b/c only 1 input 
            break;
        case OP_OR:
            out = OR(g->port_inputs);
            break;
        case OP_NOR:
            out = OR(g->port_inputs);
            out.value = !out.value;
            break;
        case OP_XOR:
            out = XOR(g->port_inputs); 
            break;
        default:
            printf("FUCK, op is off \n");
            assert(0);
            break;
    }
    if (!out.isValid) {
        return;//no use processing further, output is invalid
    }
    if (out.value == ((pdata_t)g->port_output->misc)->value) {//and statement on two lines
        bool isValid = ((pdata_t)(g->port_output->misc))->isValid;
        if (isValid)
         return;//value is always valid and not changing
    }
    //value is changing either val or isValid
    node_t node = malloc(sizeof(struct pq_node));
    node->port = g->port_output;
    node->new_value = out.value;
    node->t = t + g->delay;
    insert(heap_array, node);
    //heap_array.insert()

    /*

typedef struct gate {
    op_t op;
    linked_list port_inputs;
    port_t port_output;
    int delay;
} *gate_t;

    */
};

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
    ((pdata_t) p->misc)->value = val;
    insert(heap_array, port);
    printArray(heap_array, size);
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