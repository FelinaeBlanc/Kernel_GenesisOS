#include "debugger.h"
#include "cpu.h"
#include "stdio.h"
#include "processus.h"
#include "stdlib.h"
#include "string.h"
#include "horloge.h"

void idle() {
  // printf("JE SUIS DANS IDLE\n");
  for (;;) {
    sti(); // usage de sti et cli avant la phase 5 (possible après)
    hlt(); // autre exception possible: le code de vos tests
    cli(); // JAMAIS de sti ou de cli dans le reste de votre kernel
  }
}

struct Processus * ProcIdle;
void kernel_start(void)
{
    printf("\f");
    /* Afichage de l'horloge */
    reg_frequence(); 
    sign_clock(); // active l'interruption de l'horloge

    init_ordonnanceur(); // Init l'ordonnanceur
    
    int pid_idle = cree_processus(&idle,IDLE,"idle");
    ProcIdle = tableDesProcs[pid_idle];

    init_traitant_IT(32, traitant_IT_32);

    idle();

    while(1)
        hlt();

    return;
}
