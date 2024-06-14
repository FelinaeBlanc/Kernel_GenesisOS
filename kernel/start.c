#include "debugger.h"
#include "cpu.h"
#include "stdio.h"
#include "processus.h"
#include "stdlib.h"
#include "string.h"
#include "horloge.h"

int idle() {
  // printf("JE SUIS ENTREE DANS IDLE\n");
  for (;;) {
    sti(); // usage de sti et cli avant la phase 5 (possible après)
    hlt(); // autre exception possible: le code de vos tests
    cli(); // JAMAIS de sti ou de cli dans le reste de votre kernel
  }
}

void kernel_start(void)
{
  printf("\f");

  init_ordonnanceur(); // Init l'ordonnanceur
  idle();
  
  return;
}
