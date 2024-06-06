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

/**TEST FNC**/
//Fonction pour afficher l'état de chaque processus
int proc_test_enfant(){
    printf("proc_test enfant [temps = %u] processus %s pid = %i\n", nbr_secondes(), mon_nom(),
    getpid());
    dors(2);
    printf("Finittt proc test enfant\n ");
    return 0;
}
int proc_test_a(){
    printf("proc_test_a [temps = %u] processus %s pid = %i\n", nbr_secondes(), mon_nom(),
          getpid());
    int pid = start(&proc_test_enfant, SIZE_PILE_EXEC, 4, "proc_test_enfant", NULL);
    int retval;
    printf("Attend enfant...");
    pid = waitpid(pid, &retval);
    printf("enfant finit!!! pid = %i\n", pid);
          dors(2);
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
    //start(&proc1, SIZE_PILE_EXEC, 1, "proc1", NULL);
    start(&proc_test_a, SIZE_PILE_EXEC, 1, "proc_test_a", NULL);
    //start(&proc3, SIZE_PILE_EXEC, 2, "proc3", NULL);

    init_traitant_IT(32, traitant_IT_32);


    idle();

    while(1)
        hlt();

    return;
}
