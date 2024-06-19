#include "stdio.h"
#include "builtin.h"

unsigned int seed = 42;

const unsigned int a = 1664525; // Multiplier
const unsigned int c = 1013904223; // Increment
const unsigned int m = 4294967295; // Modulus (2^32)

unsigned int rand_int(){
    seed = (a * seed + c + current_clock() ) % m;
    return seed;
}
