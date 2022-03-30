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
linked_list gates_delayed_processing;

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
    misc_info->gates = 0;
    misc_info->ports = 0;
    //misc_info->ports = malloc(sizeof(port_t) * 3);//value doesn't matter b/c currently invalid
    void * misc = (void *) (misc_info);
    new_port->misc = misc; // every port starts undefined

    printf("port added of type %i and of name %s\n", new_port->pt, new_port->name);


    // TODO: ADD TO HASHMAP

    return new_port;

}


void gate(const op_t op, const port_t out, const unsigned num_in, ...)
{

    //make new gate, except for input ports
    gate_t new_gate = malloc(sizeof(struct gate));
    new_gate->op = op;
    new_gate->port_output = out;
    new_gate->delay = delay;
    


    // create one-dimensional input_port array
    va_list va;
    va_start(va, num_in);

    //create input list
    for (int i = 0; i < num_in; i++) {
        port_t current = va_arg(va, port_t);
        
        linked_list newPortNode = malloc(sizeof(linked_list));
        newPortNode->data = current;
        newPortNode->next = new_gate->port_inputs;
        new_gate->port_inputs = newPortNode;


        linked_list newGateNode = malloc(sizeof(linked_list));
        newGateNode->data = new_gate;
        newGateNode->next = ((pdata_t) current->misc)->gates;
        ((pdata_t) current->misc)->gates = newGateNode;

        printf("adding port %d: port %s to this gate\n", i, current->name);
    }

    linked_list currentPort = new_gate->port_inputs;
    printf("ports in gate: ");
    while(currentPort != NULL)
    {
        printf("%s, ", ((port_t) currentPort->data)->name);
        currentPort = currentPort->next;
    }
    printf("\n");
    va_end(va);
    
}

logic_return AND(linked_list inputs) {//if valid 0, return false; otherwise indeterminant or true if all valid and ture
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

logic_return OR(linked_list inputs) {//if there is a valid 1, its true; else it is either invalid or if all false then false
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

void delayed_process_gate(gate_t g) { // add to list to process after current time stamp

    if (!gates_delayed_processing) {
        linked_list startNode = malloc(sizeof(node));
        startNode->data = (void *)(g);
        startNode->next = 0;
        return;
    }
    linked_list current_place = gates_delayed_processing;
    while (current_place) {//while its not null
        if ((gate_t)current_place->data == g) {
            return;
        }

    }//its not already in there

    //we've reached end and ntohing there
    linked_list newGate = malloc(sizeof(node));
    newGate->data = (void *)(g);
    newGate->next = gates_delayed_processing;
    gates_delayed_processing = newGate;
    //append this gate pointer to gates_delayed_processing
}

void process_delayed_gates() {//process all gates we left for later
    linked_list current_place = gates_delayed_processing;
    while (current_place) {
        gate_t g = (gate_t)current_place->data;
        process_gate(g);
        linked_list old_place = current_place;
        current_place = current_place->next;
        free(old_place);//freeing node b./c were done w/ it now

    }
    gates_delayed_processing = 0;
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
};

//init wire
void wire(const port_t src, const port_t dst)
{
    // dst->val = src->val;
    // dst->pt = src->pt;

    //add to front of linkedlist
    if (((pdata_t)(src->misc))->ports ==0) {//if no ports, create linked list
        linked_list portList = malloc(sizeof(node));
        portList->next = 0; 
        portList->data = (void *)(dst);
        ((pdata_t)(src->misc))->ports = portList;
        return;
    }
    //if linked list of ports, add this to front
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
    //if this is already true, quit
    if (((pdata_t) p->misc)->value == val && ((pdata_t) p->misc)->isValid ==1)//all is already fine
        return;

    //its being set so now valid
    ((pdata_t) p->misc)->value = val;
    ((pdata_t) p->misc)->isValid = 1;

    //need to push children/gates/ports that are needed
    linked_list port_children = ((pdata_t)(p->misc))->ports;
    while (port_children) {
        port_t current_port = (port_t)(port_children->data);
        //its directly connected so need to set this port too
        set_port(current_port, val);

        //port_t children_port
        port_children = port_children->next;
    }
    
    linked_list theGates = ((pdata_t)(p->misc))->gates;

    //gate_t * theGate = &(((pdata_t)(p->misc))->nextGate);
    while (theGates) {//there is a gate we need to process
        //TIME COMPLEXITY REDUCTION: wait til done with all setting values before doing this
        gate_t theGate = (gate_t)theGates->data;
        delayed_process_gate(theGate);//push to process after done w/ all of current time stamp

        theGates = theGates->next;
    }

    //insert(heap_array, port);
    //printArray(heap_array, size);
}
bool get_port(port_t p)
{
    //if(p->misc == UNDEFINED) assert(false);
    return ((pdata_t)p->misc)->value;
}
unsigned get_sim_time(void) {
    return t;
}

void sim_init(void) {
    t=0;
    gates_delayed_processing = 0;
}
void sim_run(const unsigned nsteps) {
   t = t + nsteps;
   unsigned int lastTime = heap_array[0]->t;
   while (heap_array[0]->t < t) {//while stuff to pop, pop it
       if (heap_array[0]->t != lastTime) {//process delayed gates if there's a time difference
           process_delayed_gates();
           //process all delayed gates
       }
       node_t node_pointer = heap_array[0];
       if (node_pointer->new_value != ((pdata_t)(node_pointer->port->misc))->value) {
           set_port(node_pointer->port, node_pointer->new_value);
           //go ahead and process this
       }
       else {if (!((pdata_t)(node_pointer->port->misc))->isValid) {
           //go ahead and process it
           set_port(node_pointer->port, node_pointer->new_value);
       }}
    
   }
   if (gates_delayed_processing!=0) {//if gates left to process process then run again
       process_delayed_gates();
       sim_run(0);
   }
   
}