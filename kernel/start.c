#include "debugger.h"
#include "cpu.h"
#include "stdio.h"
#include "processus.h"
#include "stdlib.h"
#include "string.h"
#include "horloge.h"
#include "files.h"
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
  // printf("proc runner !\n");
  test_run(14);
  printf("Proc runner fini!!!!!!!!!!!!!!!!!!!!!\n");
  return 0;
}

void kernel_start(void)
{
  printf("\f");

  init_ordonnanceur(); // Init l'ordonnanceur
  start(&proc_runner, 4000, 128, "proc_runner", NULL);
  // printf("C'est bon!");
  idle();
  
  return;
}
