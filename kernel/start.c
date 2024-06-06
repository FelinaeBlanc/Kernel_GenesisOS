#include "debugger.h"
#include "cpu.h"
#include "stdio.h"
#include "processus.h"
#include "stdlib.h"
#include "string.h"
#include "horloge.h"

int idle() {
  printf("JE SUIS ENTREE DANS IDLE\n");
  is_idle_started = true;
  for (;;) {
    sti(); // usage de sti et cli avant la phase 5 (possible après)
    hlt(); // autre exception possible: le code de vos tests
    cli(); // JAMAIS de sti ou de cli dans le reste de votre kernel
  }
}


int proc_a(){
    printf("proc_a [temps = %lu] processus %s pid = %i\n", current_clock(), mon_nom(),
        getpid());
    wait_clock(500);
    return 0;
}

int proc_b(){
  printf("proc_b [temps = %lu] processus %s pid = %i\n", current_clock(), mon_nom(),
        getpid());
  wait_clock(1);
  return 0;
}


void kernel_start(void)
{
    printf("\f");
    /* Afichage de l'horloge */
    reg_frequence(); 
    sign_clock(); // active l'interruption de l'horloge

    init_ordonnanceur(); // Init l'ordonnanceur
    
    start(&idle, SIZE_PILE_EXEC, 0, "idle", NULL);
    start(&proc_a, SIZE_PILE_EXEC, 1, "proc_a", NULL);
    start(&proc_b, SIZE_PILE_EXEC, 1, "proc_b", NULL);
    //start(&proc3, SIZE_PILE_EXEC, 2, "proc3", NULL);

    init_traitant_IT(32, traitant_IT_32);

    idle();

    while(1)
        hlt();

    return;
}
