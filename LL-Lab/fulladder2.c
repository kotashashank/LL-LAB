#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "gates.h"

void halfadder(port_t ain, port_t bin, port_t sum, port_t cout) {
    // assert(PTYPE_EXT_IN == ain->pt);
    // assert(PTYPE_EXT_IN == bin->pt);
    // assert(PTYPE_EXT_OUT == sum->pt);
    // assert(PTYPE_EXT_OUT == cout->pt);

    port_t XORia = port(PTYPE_IN, NULL),
           XORib = port(PTYPE_IN, NULL),
           XORo = port(PTYPE_OUT, NULL),
           ANDia = port(PTYPE_IN, NULL),
           ANDib = port(PTYPE_IN, NULL),
           ANDo = port(PTYPE_OUT, NULL);
    
    wire(ain, XORia); wire(ain, ANDia);
    wire(bin, XORib); wire(bin, ANDib);
    wire(XORo, sum); wire(ANDo, cout);

    gate(OP_XOR, XORo, 2, XORia, XORib);
    gate(OP_AND, ANDo, 2, ANDia, ANDib);
}

void fulladder(port_t ain, port_t bin, port_t cin, port_t sum, port_t cout) {
    assert(PTYPE_EXT_IN == ain->pt);
    assert(PTYPE_EXT_IN == bin->pt);
    assert(PTYPE_EXT_IN == cin->pt);
    assert(PTYPE_EXT_OUT == sum->pt);
    assert(PTYPE_EXT_OUT == cout->pt);

    printf("FA 2");

    port_t HA1a = port(PTYPE_IN, "_HA1a");
    port_t HA1b = port(PTYPE_IN, "_HA1b");
    port_t HA1s = port(PTYPE_OUT, "_HA1s");
    port_t HA1d = port(PTYPE_OUT, "_HA1d");
    port_t HA2a = port(PTYPE_IN, "_HA2a");
    port_t HA2b = port(PTYPE_IN, "_HA2b");
    port_t HA2s = port(PTYPE_OUT, "_HA2s");
    port_t HA2d = port(PTYPE_OUT, "_HA2d");
    port_t Cout1 = port(PTYPE_IN, "_Cout1");
    port_t Cout2 = port(PTYPE_IN, "_Cout2");
    port_t Cout = port(PTYPE_OUT, "_Cout");
    
    wire(ain, HA1a); wire(bin, HA1b);
    halfadder(HA1a, HA1b, HA1s, HA1d);
    wire(HA1s, HA2a); wire(cin, HA2b);
    halfadder(HA2a, HA2b, HA2s, HA2d);
    wire(HA1d, Cout1); wire(HA2d, Cout2);
    gate(OP_OR, Cout, 2, Cout1, Cout2);
    wire(HA2s, sum); wire(Cout, cout);
}

int main(void) {
    sim_init();

    port_t A = port(PTYPE_EXT_IN, NULL);
    port_t B = port(PTYPE_EXT_IN, NULL);
    port_t C = port(PTYPE_EXT_IN, NULL);
    port_t S = port(PTYPE_EXT_OUT, NULL);
    port_t D = port(PTYPE_EXT_OUT, NULL);

    delay = 2;
    fulladder(A, B, C, S, D);
    unsigned t = get_sim_time();
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                set_port(A, i%2 == 0);
                set_port(B, j%2 == 0);
                set_port(C, k%2 == 0);
                for (int n = 0; n < 10; n++) {
                    printf("[%du] %d %d %d %d %d\n", 
                        t, get_port(A), get_port(B), get_port(C), get_port(S), get_port(D));
                    sim_run(1); t++;
                    assert(get_sim_time() == t);
                }
                printf("\n");
            }
        }
    }
    return EXIT_SUCCESS;
}