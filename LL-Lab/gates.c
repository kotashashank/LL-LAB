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

// Frees memory used by nodes in linked list
void freeLL(linked_list list) {
    while (list != NULL) {
        linked_list next = list->next;
        free(list->data);
        free(list);
        list = next;
    }
}

// Initilizes a new port
port_t port(const ptype_t pt, const char *name)
{
    //TODO: might need to add EXT_IN to global for memory management
    port_t new_port = malloc(sizeof(struct port));
    new_port->pt = pt;
    new_port->name = name;
    // Create port data
    pdata_t misc_info = malloc(sizeof(struct port_data));
    misc_info->is_valid = 0;
    misc_info->gates = 0;
    misc_info->ports = 0;

    // Add port to port struct
    void * misc = (void *) (misc_info);
    new_port->misc = misc; // Every port starts undefined 

    //printf("port added of type %i and of name %s\n", new_port->pt, new_port->name);

    return new_port;

}


// Adds ports to gate's input and outputs and and gates to port's gate linked list
void gate(const op_t op, const port_t out, const unsigned num_in, ...)
{

    // Make new gate, except for input ports
    gate_t new_gate = malloc(sizeof(struct gate));
    new_gate->op = op;
    new_gate->port_output = out;
    new_gate->delay = delay;
    
    va_list va;
    va_start(va, num_in);

    // Iterate though list of inputs
    for (int i = 0; i < num_in; i++) {
        port_t current = va_arg(va, port_t);
        
        // Create and add port node for gate's linked list
        linked_list newPortNode = malloc(sizeof(linked_list));
        newPortNode->data = current;
        newPortNode->next = new_gate->port_inputs;
        new_gate->port_inputs = newPortNode;

        // Create and add gate node for port's linked list
        linked_list newGateNode = malloc(sizeof(linked_list));
        newGateNode->data = new_gate;
        newGateNode->next = ((pdata_t) current->misc)->gates;
        ((pdata_t) current->misc)->gates = newGateNode;

       // printf("adding port %d: port %s to this gate\n", i, current->name);
    }

    // Debugging code
    // linked_list currentPort = new_gate->port_inputs;
    // printf("ports in gate: ");
    // while(currentPort != NULL)
    // {
    //     printf("%s, ", ((port_t) currentPort->data)->name);
    //     currentPort = currentPort->next;
    // }
    // printf("\n");

    va_end(va);
    
}


// Returns the output of an AND operation from a list of ports
logic_return logical_AND(linked_list inputs) {
    bool all_inputs_valid = TRUE;
    bool output_value = TRUE;
    logic_return output;
    
    // Iterate through each input
    while (inputs != NULL) {
        pdata_t pdata = (pdata_t)(((port_t) (inputs->data))->misc);

        all_inputs_valid = all_inputs_valid && pdata->is_valid;
        output_value = pdata->value && output_value;
        inputs = inputs->next;
    }
    
    output.value = output_value;
    output.is_valid = all_inputs_valid;
    return output;
}

// Returns the output of an OR operation from a list of ports
logic_return logical_OR(linked_list inputs) {
    
    bool all_inputs_valid = TRUE;
    logic_return output;
    bool output_value = FALSE;
    // Iterate through each input
    while (inputs != NULL) {
        pdata_t pdata = (pdata_t)(((port_t) (inputs->data))->misc);

        all_inputs_valid = all_inputs_valid && pdata->is_valid;
        // Short circuit OR operation
        output_value = output_value | pdata->value;
        inputs = inputs->next;
    }


    output.value = output_value;
    output.is_valid = all_inputs_valid;
    
    return output;
}

// Returns the output of an XOR operation from a list of ports. Returns true if the number of TRUE values are odd.
logic_return logical_XOR(linked_list inputs) {
    bool all_inputs_valid = TRUE;
    bool return_value = FALSE;
    logic_return output;

    // Iterate through each input
    while (inputs != NULL) {
        pdata_t pdata = (pdata_t)(((port_t) (inputs->data))->misc);
        // Input is invalid, exit immediatly
        all_inputs_valid = all_inputs_valid && pdata->is_valid;
        if (pdata->value) return_value = !return_value;
        inputs = inputs->next;
    }

    output.value = return_value;
    output.is_valid = all_inputs_valid;
    return output;
}



// Returns the output of an NAND operation from a list of ports.
logic_return logical_NAND(linked_list inputs) {
    logic_return output = logical_AND(inputs);
    output.value = !output.value;
    return output;
}

// Returns the output of an NOR operation from a list of ports.
logic_return logical_NOR(linked_list inputs) {
    logic_return output = logical_OR(inputs);
    output.value = !output.value;
    return output;
}

logic_return logical_NOT(linked_list inputs) {
    assert(inputs->next == NULL); // Make sure input list is size 1

    logic_return output;
    //pdata_t input_data = (pdata_t) inputs ->data;
    pdata_t input_data = (pdata_t)(((port_t) (inputs->data))->misc);
    output.value = !input_data->value;
    output.is_valid = input_data->is_valid;
}


// Add to list to process after current timestamp
void delayed_process_gate(gate_t g) { 

    if (!gates_delayed_processing) {
        linked_list startNode = malloc(sizeof(node));
        startNode->data = (void *)(g);
        startNode->next = 0;
        gates_delayed_processing = startNode;
        return;
    }

    linked_list current_place = gates_delayed_processing;
    while (current_place) {//if it already exists, skip
        if ((gate_t)current_place->data == g) return;
        current_place = current_place->next;
    }
    

    // We've reached end and nothing there
    linked_list newGate = malloc(sizeof(node));
    newGate->data = (void *)(g);
    newGate->next = gates_delayed_processing;
    gates_delayed_processing = newGate;

    // Append this gate pointer to gates_delayed_processing
}

// Process all gates we left for later
void process_delayed_gates() {
    linked_list current_place = gates_delayed_processing;
    while (current_place) {
        gate_t g = (gate_t)current_place->data;
        process_gate(g);
        linked_list old_place = current_place;
        current_place = current_place->next;
        free(old_place); // Free node because we are done with it now

    }
    gates_delayed_processing = 0;
}

// Apply operations to gate inputs and change output
void process_gate(gate_t g) {

    //printf("at time %08x, processing gate of value %02x\n", t, g->op);
    op_t op = g->op;
    logic_return output;

    // Execute operations on inputs
    switch (op) {
        case OP_NOT: output = logical_NOT(g->port_inputs); break;
        case OP_AND: output = logical_AND(g->port_inputs); break;
        case OP_OR: output = logical_OR(g->port_inputs); break;
        case OP_XOR: output = logical_XOR(g->port_inputs); break;
        case OP_NAND: output = logical_NAND(g->port_inputs); break;
        case OP_NOR: output = logical_NOR(g->port_inputs); break;
        case OP_ERROR: printf("Error in gates.c\n"); assert(0);
    }

    if (!output.is_valid) {
        //printf("Output is invalid\n");
        return;
    }

    //printf("output is valid\n");

    // Note: ?
    if (output.value == ((pdata_t)g->port_output->misc)->value) { //and statement on two lines
        bool is_valid = ((pdata_t)(g->port_output->misc))->is_valid;
        if (is_valid) return; //value is always valid and not changing
    }

    // Add output to priorty queue
    node_t node = malloc(sizeof(struct pq_node));
    node->port = g->port_output;
    node->new_value = output.value;
    node->t = t + g->delay;
    //printf("adding node to priority queue with time of %08x\n", node->t);
    //printArray(heap_array, size);
    assert(node!=0);
    insert(heap_array, node);
    //printArray(heap_array, size);
};


//init wire
void wire(const port_t src, const port_t dst)
{
    pdata_t pdata = ((pdata_t) src->misc);

    // Add to front of linkedlist
    if (pdata->ports == 0) { // If there are no ports, create a list
        linked_list portList = malloc(sizeof(node));
        portList->next = 0; 
        portList->data = (void *)(dst);
        pdata->ports = portList;
        return;
    }
    //if linked list of ports, add this to front
    linked_list next = pdata->ports;
    linked_list newData = malloc(sizeof(node));
    newData->data = (void *)(dst);
    newData->next = next;
    pdata->ports = newData;
    

}

// The remaining routines allow a test routine to simulate a given circuit for a number of timesteps.
// A timestep is of an arbitrary and unspecified number of seconds.
port_t clock(const unsigned hi, const unsigned lo)
{
    // TODO
}

void set_port(port_t p, bool val)
{
    //printf("updating port %s with value %01x\n", p->name, val);
    pdata_t pdata = ((pdata_t) (p->misc));

    if(p->pt == PTYPE_EXT_IN) {
        printf("port \n");
    }

    // If this is already true, quit
    if (pdata->value == val && pdata->is_valid == TRUE) // Everything is already fine
        
        
        {
            //printf("nothing changing with port %s\n", p->name);
            return;
        }
            

    // Its being set so now valid
    pdata->value = val;
    pdata->is_valid = TRUE;

    // Need to push children/gates/ports that are needed
    linked_list port_children = pdata->ports;
    while (port_children) {
        port_t current_port = (port_t)(port_children->data);
        //its directly connected so need to set this port too
        set_port(current_port, val);

        //port_t children_port
        port_children = port_children->next;
    }
    
    linked_list theGates = pdata->gates;

    //gate_t * theGate = &pdata->nextGate;
    while (theGates) { // There is a gate we need to process
        // TIME COMPLEXITY REDUCTION: wait until finished with all setting values before doing this
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
    return ((pdata_t)(p->misc))->value;
}

unsigned get_sim_time(void) {
    return t;
}

void sim_init(void) {
    t=0;
    gates_delayed_processing = 0;
}

int min(int one, int two) {
    return one < two ? one : two;
}


void sim_run(const unsigned nsteps) {
   //t = t + nsteps;
   int initial_t = t;
   if (gates_delayed_processing!=0) {//if gates left to process process then run again
   
       process_delayed_gates();
   }
   if (gates_delayed_processing==0) {
       int iii = 0 ;//printf("no gates to process\n");
   }
   if (!seeFirst())
        {
            //printf("heap array is empty\n");
            t = initial_t + nsteps;
            return;
        }
   unsigned int lastTime = seeFirst()->t;
   while (size!=0 && seeFirst() && seeFirst()->t <= initial_t+nsteps) {//while stuff to pop, pop it
        //printf("in sim_run while for time of %08x\n", seeFirst()->t);
       if (seeFirst()->t != lastTime) {//process delayed gates if there's a time difference
           process_delayed_gates();
           //process all delayed gates
       }
       //printf("processing port\n");
       node_t node_pointer = seeFirst();
       deleteRoot(heap_array);
       set_port(node_pointer->port, node_pointer->new_value);


       if (seeFirst())
         t = min(initial_t + nsteps, seeFirst()->t);
       else 
         {
             t = initial_t + nsteps;
         }
        
    
   }
   if (gates_delayed_processing!=0) {//if gates left to process process then run again
       //printf("processing gates\n");
       process_delayed_gates();
       sim_run(0);
   }

    t = initial_t + nsteps;
   
}