#include "debugger.h"
#include "cpu.h"
#include "stdio.h"
#include "processus.h"
#include "stdlib.h"
#include "string.h"
#include "horloge.h"
//#include "files.h"
#include "test.h"

int idle() {
  // printf("JE SUIS ENTREE DANS IDLE\n");
  for (;;) {
    sti(); // usage de sti et cli avant la phase 5 (possible après)
    hlt(); // autre exception possible: le code de vos tests
    cli(); // JAMAIS de sti ou de cli dans le reste de votre kernel
  }
}

int proc_runner(){
  printf("Proc runner !\n");
  test_run(4);
  printf("Proc runner fini\n");
  return 0;
}


void kernel_start(void)
{
  //void * arg =  (int *) 1;
  printf("\f");
  /* Afichage de l'horloge */
  reg_frequence(); 
  sign_clock(); // active l'interruption de l'horloge

  init_ordonnanceur(); // Init l'ordonnanceur
  //init_files(); // Init les files !

  start(&proc_runner, SIZE_PILE_EXEC, 128, "proc_runner", NULL);
  init_traitant_IT(32, traitant_IT_32);

  idle();
  
  return;
}
