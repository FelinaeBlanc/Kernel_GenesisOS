#ifndef PROCESSUS_H
#define PROCESSUS_H

#include <stdint.h>

#define MAX_PROCESS 2
#define SIZE_PILE_EXEC 512

enum Etat { ELU, ACTIVABLE, ENDORMI };

struct Processus {
    uint32_t pid;
    char nom[25];
    enum Etat etat;
    int32_t contexte[5];  // Array to hold the saved registers (ebx, esp, ebp, esi, edi)
    int32_t pile[SIZE_PILE_EXEC];  // Stack for the process
};

// 0 - ebx
// 1 - esp
// 2 - ebp
// 3 - esi
// 4 - edi


extern struct Processus * tableDesProcs[MAX_PROCESS];
extern struct Processus * processusElu;

void ctx_sw(int *, int *);

#endif
