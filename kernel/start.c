#include "debugger.h"
#include "cpu.h"
#include "stdio.h"
#include "processus.h"
#include "stdlib.h"
#include "string.h"
#include "horloge.h"
#include "test.h"

int idle() {
  printf("JE SUIS ENTREE DANS IDLE\n");
  for (;;) {
    sti(); // usage de sti et cli avant la phase 5 (possible après)
    hlt(); // autre exception possible: le code de vos tests
    cli(); // JAMAIS de sti ou de cli dans le reste de votre kernel
  }
}

// Fonction pour afficher l'état de chaque processus
int proc_test_enfant(){
    printf("proc_test enfant [temps = %lu] processus %s pid = %i\n", current_clock(), mon_nom(),
    getpid());
    return 4;
}
int proc_test_a(){
    printf("proc_test a [temps = %lu] processus %s pid = %i\n", current_clock(), mon_nom(),
          getpid());
    int pid = start(&proc_test_enfant, SIZE_PILE_EXEC, 2, "proc_test_enfant", NULL);
    int retval;
    pid = waitpid(pid, &retval);
    printf("enfant pid = %i et %d\n", pid,retval);
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
    //start(&test_run, SIZE_PILE_EXEC, 128, "idle", arg);
    start(&proc_test_a, SIZE_PILE_EXEC, 1, "proc_test_a", NULL);

    init_traitant_IT(32, traitant_IT_32);

    idle();

    return;
}
