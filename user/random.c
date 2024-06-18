#include "stdio.h"

unsigned int seed = 0;

void srand(unsigned int newSeed){
    seed = newSeed;
}

unsigned int rand_int(){
    seed = seed * 1103515245 + 44523457;
    return (unsigned int)(seed/65536) % 32768;
}
