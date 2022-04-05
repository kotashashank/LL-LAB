#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {


    char *file = argv[1];

    if(file == NULL) {
        printf("Missing test file!\n");
        return -1;
    }
    printf("%s", file);
    
}


void *run(char *filename) {
    FILE *file;

    file = fopen(filename, ".c");
}