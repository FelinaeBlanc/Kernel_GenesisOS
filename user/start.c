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

    // en mode superviseur : CS = 0x10, autres registres de segments = 0x18
    // en mode utilisateur : CS = 0x43, autres registres de segments = 0x4b
    // printf("Ok\n");
    getpid();
    while(1) {}

    return;
}
