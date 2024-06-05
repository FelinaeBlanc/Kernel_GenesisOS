#include "debugger.h"
#include "cpu.h"
#include "stdio.h"
#include "processus.h"
#include "stdlib.h"
#include "string.h"
#include "horloge.h"

void idle() {
  printf("JE SUIS DANS IDLE\n");
  for (;;) {
    sti(); // usage de sti et cli avant la phase 5 (possible après)
    hlt(); // autre exception possible: le code de vos tests
    cli(); // JAMAIS de sti ou de cli dans le reste de votre kernel
  }
}

void proc1(void) {
      printf("JE SUIS DANS PROC1\n");
  for (;;) {
    if (!stop){
      printf("proc1 [temps = %u] processus %s pid = %i\n", nbr_secondes(), mon_nom(),
      mon_pid());
      dors(2);
    }
  }
}

void proc2(void) {
  printf("JE SUIS DANS PROC2\n");
  for (;;) {
    if (!stop){

    printf("proc2 [temps = %u] processus %s pid = %i\n", nbr_secondes(), mon_nom(),
           mon_pid());
    dors(3);
    }
  }
}

void proc3(void) {
  printf("JE SUIS DANS PROC3\n");
  for (;;) {
    if (!stop){
      printf("proc3 [temps = %u] processus %s pid = %i\n", nbr_secondes(), mon_nom(),
            mon_pid());
      dors(5);
    }
  }
}

void kernel_start(void)
{
    printf("\f");
    /* Afichage de l'horloge */
    reg_frequence(); 
    sign_clock(); // active l'interruption de l'horloge

    init_ordonnanceur(); // Init l'ordonnanceur
    
    cree_processus(&idle,"idle");
    cree_processus(&proc1,"proc1");
    cree_processus(&proc2,"proc2");
    cree_processus(&proc3,"proc3");

    init_traitant_IT(32, traitant_IT_32);

    idle();

    while(1)
        hlt();

    return;
}
