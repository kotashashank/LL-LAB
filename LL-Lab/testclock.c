#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "gates.h"


int main(void) {
   sim_init();

   port_t A = clock(9, 3);
   
   port_t B = port(PTYPE_IN, "Constant Port");

   port_t C = port(PTYPE_OUT, "Value of A");

   delay = 2;
   gate(OP_AND, C, 2, A, B);

    
    
   unsigned t = get_sim_time();
   for (int n = 0; n < 100; n++) {
      set_port(B, 1);
      printf("[%du] %d %d %d\n", t, get_port(A), get_port(B), get_port(C));
      sim_run(1); t++;
      assert(get_sim_time() == t);
   }
    
   return EXIT_SUCCESS;
}
