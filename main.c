#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    for (int i = 0; i < argc; i++) {
        printf("index: %d, argv: %s\n", i, argv[i]);
    }
    
    return 0;
}