#ifndef PROCESSUS_H
#define PROCESSUS_H

#include "stdint.h"

#define MAX_PROCESS 10
#define SIZE_PILE_EXEC 512

enum Etat { ELU, ACTIVABLE, ENDORMI, MOURANT, ZOMBIE };
enum Prio { IDLE, COMMUN, PRIORITAIRE };

struct Processus {
    uint32_t pid;
    char nom[25];
    enum Etat etat;
    uint32_t secReveille;
    enum Prio prio;
    // Pile
    int32_t contexte[5];  // Array to hold the saved registers (ebx, esp, ebp, esi, edi)
    int32_t pile[SIZE_PILE_EXEC];  // Stack for the process
    // chainage
    struct Processus *prec;
    struct Processus *suiv;
};

// tête et queue des processus activables
extern struct Processus * ProcActivTete;
extern struct Processus * ProcActivQueue;
extern struct Processus * ProcElu;
extern struct Processus * ProcMourantTete;

// tête des processus endormis
extern struct Processus *ProcDortTete;

struct PidLibre {
    uint32_t pid;
    struct PidLibre *suivPid;
};

extern struct PidLibre *PidLibreTete;

extern struct Processus *tableDesProcs[MAX_PROCESS];

/* Fonction control switch */
void ctx_sw(int *, int *);

void init_ordonnanceur(void);
void ordonnanceur(void);

int32_t mon_pid(void);
char *mon_nom(void);

void insert_tete_activable(struct Processus *proc);

void insert_queue_activable(struct Processus *proc);

int32_t cree_processus(void (*code)(void), int prio, char *nom);

/******Endormi******/

void dors(uint32_t nbr_secs);

void insert_endormi(struct Processus *proc);

void verifie_reveille(uint32_t);

/*****Exit*****/

void fin_processus(void);

#endif
