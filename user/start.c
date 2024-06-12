#include "start.h"
#include "stdio.h"
#include "builtin.h"

/*
@ hautes
^
|              | <- ESP, adresse de base de la pile kernel, valeur prise dans la TSS
+--------------+
|     SS       |
+--------------+
|     ESP      |
+--------------+
|    EFLAGS    |
+--------------+
|     CS       |
+--------------+
|     EIP      | <- ESP après
+--------------+
|
0 
*/
void user_start(void) {

    while(1) {}

    return;
}
