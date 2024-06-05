#ifndef PROCESSUS_H
#define PROCESSUS_H

#include "stdint.h"
#include "queue.h"

#define MAX_PROCESS 4
#define SIZE_PILE_EXEC 512

enum Etat { ELU, ACTIVABLE, ENDORMI, MOURANT, ZOMBIE };
enum Prio { IDLE, COMMUN, PRIORITAIRE};

struct PidLibre {
    uint32_t pid;
    struct PidLibre * suivPid;
};
extern struct PidLibre * PidLibreTete;

struct Fils {
    struct Processus * procFils;
    struct Fils * suiv;
};
struct Processus {
    uint32_t pid;
    char nom[25];
    enum Etat etat;
    enum Prio prio;

    uint32_t secReveille;
    
    // Pile
    int32_t contexte[5];  // Array to hold the saved registers (ebx, esp, ebp, esi, edi)
    int32_t pile[SIZE_PILE_EXEC];  // Stack for the process
    // chainage + filiation
    link chainage; // chianage pour les liaisons avec ordre de priorité
    struct Fils * FilsTete; // Liste des fils

    int retvalp;
    struct Processus * pere;
    
    
};
// Déclaration des variables globales de processus
extern struct Processus * ProcElu;
extern struct Processus * tableDesProcs[MAX_PROCESS];

/* Fonction control switch */
void ctx_sw(int *, int *);

void init_ordonnanceur(void);
void ordonnanceur(void);

int32_t mon_pid(void);
char *mon_nom(void);

int32_t cree_processus(void (*code)(void), int prio, char *nom);

/******Edormi******/

// tête des processus endormis !
extern struct Processus * ProcDortTete;

void dors(uint32_t nbr_secs);

void insert_endormi(struct Processus *proc);

void verifie_reveille(uint32_t);

/*****Exit*****/

void fin_processus(void);

#endif
