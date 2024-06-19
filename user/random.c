#include "stdio.h"
#include "builtin.h"
unsigned int seed = 0;

unsigned int rand_int(){
    seed = ((seed + 4566) * current_clock() + 7891)%1000;
    return seed;
}
