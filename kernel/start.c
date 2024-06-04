#include "debugger.h"
#include "cpu.h"
#include "stdio.h"
#include "processus.h"
#include "stdlib.h"
#include "string.h"
#include "horloge.h"

struct Processus * tableDesProcs[MAX_PROCESS];
struct Processus * processusElu = NULL;

// Déclaration statique des processus
struct Processus p0_idle;
struct Processus p1_proc1;

int fact(int n)
{
    if (n < 2)
        return 1;

    return n * fact(n-1);
}

void idle(void) {
  for (;;) {
    printf("[%s] pid = %i\n", mon_nom(), mon_pid());
    for (int32_t i = 0; i < 100000000; i++)
      ;
    ordonnance();
  }
}

void proc1(void) {
  for (;;) {
    printf("[%s] pid = %i\n", mon_nom(), mon_pid());
    for (int32_t i = 0; i < 100000000; i++)
      ;
    ordonnance();
  }
}

void kernel_start(void)
{

    /* Afichage de l'horloge */
    // reg_frequence();
    // sign_clock();
    // init_traitant_IT(32, traitant_IT_32);
    // sti();

    /* PROCESSUS 1 */
    // Initialiser p0_idle
    /*printf("\f");
    p0_idle.pid = 0;
    strcpy(p0_idle.nom, "idle");
    p0_idle.etat = ELU;
    p0_idle.contexte[1] = (int32_t)&p0_idle.pile[SIZE_PILE_EXEC - 1]; // ESP pointant sur le sommet de la pile
    p0_idle.contexte[2] = (int32_t)idle;  // EBP pointant sur la fonction idle

    p0_idle.pile[SIZE_PILE_EXEC - 1] = (int32_t) &idle;
    

    // Initialiser p1_proc1
    p1_proc1.pid = 1;
    strcpy(p1_proc1.nom, "Proc1");
    p1_proc1.etat = ACTIVABLE;
	p1_proc1.contexte[0] = 0;
    p1_proc1.contexte[1] = (int32_t)&p1_proc1.pile[SIZE_PILE_EXEC - 1]; // ESP pointant sur le sommet de la pile
    p1_proc1.contexte[2] = 0;
	p1_proc1.contexte[3] = 0;
	p1_proc1.contexte[4] = 0;

	p1_proc1.pile[SIZE_PILE_EXEC - 1] = (int32_t) &proc1;

    // Init tableDesProcs
    tableDesProcs[0] = &p0_idle;
    tableDesProcs[1] = &p1_proc1;
    // Init Elu
    processusElu = &p0_idle;

    // Démarrer le premier processus
    idle();*/

    init_ordonnanceur(); // Init l'ordonnanceur
    cree_processus(&idle, "idle");
    //p0_idle.etat = ELU;
    cree_processus(&proc1, "Proc1");

    idle();

    while(1)
        hlt();

    return;
}
