#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "gates.h"

void fulladder(port_t ain, port_t bin, port_t cin, port_t sum, port_t cout) {
    assert(PTYPE_EXT == ain->pt);
    assert(PTYPE_EXT == bin->pt);
    assert(PTYPE_EXT == cin->pt);
    assert(PTYPE_EXT == sum->pt);
    assert(PTYPE_EXT == cout->pt);

    port_t HA1a = port(PTYPE_IN, "_HA1a");
    port_t HA1b = port(PTYPE_IN, "_HA1b");
    port_t HA1c = port(PTYPE_IN, "_HA1c");
    port_t HA1s = port(PTYPE_OUT, "_HA1s");
    port_t HA1d = port(PTYPE_OUT, "_HA1d");
    port_t HA2a = port(PTYPE_IN, "_HA2a");
    port_t HA2b = port(PTYPE_IN, "_HA2b");
    port_t HA2c = port(PTYPE_IN, "_HA2c");
    port_t HA2s = port(PTYPE_OUT, "_HA2s");
    port_t HA2d = port(PTYPE_OUT, "_HA2d");
    port_t Cout1 = port(PTYPE_IN, "_Cout1");
    port_t Cout2 = port(PTYPE_IN, "_Cout2");
    port_t Cout = port(PTYPE_OUT, "_Cout");
    
    gate(OP_XOR, HA1s, HA1a, HA1b); gate(OP_AND, HA1d, HA1a, HA1b);
    gate(OP_XOR, HA2s, HA2a, HA2b); gate(OP_AND, HA2d, HA2a, HA2b);
    gate(OP_OR, Cout, Cout1, Cout2);

    wire(ain, HA1a); wire(bin, HA1b);
    wire(HA1s, HA2a); wire(cin, HA2b);
    wire(HA1d, Cout1); wire(HA2d, Cout2);
    wire(HA2s, sum); wire(Cout, cout);
}

int main(void) {
    sim_init();

    port_t A = port(PTYPE_EXT);
    port_t B = port(PTYPE_EXT);
    port_t C = port(PTYPE_EXT);
    port_t S = port(PTYPE_EXT);
    port_t D = port(PTYPE_EXT);

    delay = 2;
    fulladder(A, B, C, S, D);
    unsigned t = get_sim_time();
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                set_port(A, i%2 == 0, t);
                set_port(B, j%2 == 0, t);
                set_port(C, k%2 == 0, t);
                for (int n = 0; n < 4; n++) {
                    printf("[%du] %d %d %d %d %d\n", 
                        t, get_port(A), get_port(B), get_port(C), get_port(S), get_port(D));
                    sim_run(1); t++;
                    assert(get_sim_time() == t);
                }
            }
        }
    }
    return EXIT_SUCCESS;
}