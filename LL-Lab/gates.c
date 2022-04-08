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
    
    #ifdef LGUI
        printf("\ncreating port:\n"); printf("%08x,(%s)\n", new_port, new_port->name); 
    #endif

    return new_port;

}


// Adds ports to gate's input and outputs and and gates to port's gate linked list
void gate(const op_t op, const port_t out, const unsigned num_in, ...)
{
    #ifdef LGUI
        printf("\ncreating gate:\n");
        const char * these_enums[6] = {"NOT", "AND", "OR", "XOR", "NAND", "NOR"};
        printf("%s,", these_enums[op]);
        printf("%08x,",num_in);  
        printf("%08x,",out);
        if (out->name) printf("(%s);", out->name);
        else printf("(no name);");
    #endif

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
        
        #ifdef LGUI
            printf("[%08x,", current);
            if (current->name) printf("(%s)", current->name);
            else printf("(no name)");
            printf("]");
        #endif

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

    }

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

    return output;
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

    if (!output.is_valid) return;

    if (output.value == ((pdata_t)g->port_output->misc)->value) { 
        bool is_valid = ((pdata_t)(g->port_output->misc))->is_valid;
        if (is_valid) return; 
    }

    // Add output to priorty queue
    node_t node = malloc(sizeof(struct pq_node));
    node->port = g->port_output;
    node->new_value = output.value;
    node->t = t + g->delay;

    assert(node != NULL);
    insert(heap_array, node);
};


// Initlizes wire 
void wire(const port_t src, const port_t dst)
{
    pdata_t pdata = ((pdata_t) src->misc);

    #ifdef LGUI
        printf("\ncreating wire:\n");
        printf("%08x,(%s);",src, src->name);
        printf("%08x,(%s)",dst, dst->name);
    #endif

    // Add to front of linkedlist
    if (pdata->ports == NULL) { // If there are no ports, create a list
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

    port_t return_port = malloc(sizeof(struct port));
    return_port->name = "clock";
    return_port->pt = PTYPE_CLK;
    pdata_t misc = malloc(sizeof(struct port_data));

    misc->value = FALSE;
    misc->is_valid = TRUE;
    misc->gates = NULL;
    misc->ports = NULL;

    clock_data_t clock_info = malloc(sizeof(struct clock_data));
    clock_info->time_high = hi;
    clock_info->time_low = lo;

    misc->clock_data = clock_info;

    return_port->misc = (void *)(misc);

    node_t node = malloc(sizeof(struct pq_node));
    node->port = return_port;
    node->new_value = TRUE;

    clock_data_t clock_information = ((pdata_t)(node->port->misc))->clock_data;
    node->t = t + clock_information->time_low;

    insert(heap_array, node);

    #ifdef LGUI
        printf("\ncreating port:\n"); printf("%08x,(%s)\n",return_port,return_port->name);
    #endif

    return return_port;
}


void set_port(port_t p, bool val)
{
    pdata_t pdata = ((pdata_t) (p->misc));

    #ifdef GUI
        if(p->pt == PTYPE_EXT_IN) printf("port \n");
    #endif

    if (pdata->value == val && pdata->is_valid == TRUE) return;
        
            

    // Make port valid because it is being set
    pdata->value = val;
    pdata->is_valid = TRUE;

    // Push children/gates/ports that are needed
    linked_list port_children = pdata->ports;
    while (port_children != NULL) {
        port_t current_port = (port_t)(port_children->data);

        // Set dirrectly connected ports
        set_port(current_port, val);
        
        port_children = port_children->next;
    }
    

    // Process any gate connected to set port
    linked_list connected_gates = pdata->gates;
    while (connected_gates != NULL) { 
        gate_t connected_gate = (gate_t)connected_gates->data;
        delayed_process_gate(connected_gate);

        connected_gates = connected_gates->next;
    }

}

// Get boolean value of port
bool get_port(port_t p)
{
    return ((pdata_t)(p->misc))->value;
}

// Return current simulation time
unsigned get_sim_time(void) {
    return t;
}

// Initlize global variables
void sim_init(void) {
    t = 0;
    gates_delayed_processing = NULL;
}

// Helper function
int min(int one, int two) {
    return one < two ? one : two;
}


// Run simulation for nsteps time intervals
void sim_run(const unsigned nsteps) {
   int initial_t = t;
   // Process gates inital
   if (gates_delayed_processing != NULL) process_delayed_gates();
   

   unsigned int last_time = seeFirst()->t;

   // Process ports added to priority queue
   while (size != 0 && seeFirst()->t <= initial_t + nsteps) {
       // Process gates if there is time difference
       if (seeFirst()->t != last_time) process_delayed_gates();
       
       node_t node_pointer = seeFirst();
       deleteRoot(heap_array);

        // Process clock special case
        if (node_pointer->port->pt == PTYPE_CLK) {
           pdata_t pdata = ((pdata_t)(node_pointer->port->misc));
           int time_delay;
           // Change clock interval 
           if (pdata->value == TRUE)
                time_delay = ((clock_data_t) pdata->clock_data)->time_low;
            else
                time_delay = ((clock_data_t) pdata->clock_data)->time_high;

           time_delay++;
           time_delay += t;

           // Add clock port to priority queue for processing
           node_t node = malloc(sizeof(struct pq_node));
           node->port = node_pointer->port;
           node->new_value = (((pdata_t)(node_pointer->port->misc))->value);
           node->t = time_delay;
           insert(heap_array, node);
       }

       set_port(node_pointer->port, node_pointer->new_value);


       if (seeFirst())
            t = min(initial_t + nsteps, seeFirst()->t);
       else 
            t = initial_t + nsteps;
         
        
    
   }

   // Process any additional gates
   if (gates_delayed_processing != NULL) {
       process_delayed_gates();
       sim_run(0);
   }

    t = initial_t + nsteps;
   
}


void sim_exit(void) {
    // TODO
}