#ifndef PROCESSUS_H
#define PROCESSUS_H

#include "stdint.h"
#include "queue.h"
#include "stdbool.h"

#define MAX_PROCESS 50
#define SIZE_PILE_EXEC 80000
#define MAX_SIZE_PILE 80000
#define PRIO_MAX 255
#define PRIO_IDLE 0

enum Etat { ELU, ACTIVABLE, ENDORMI, MOURANT, ZOMBIE, ATTEND_FILS, ATTEND_FILE };
//enum Prio { IDLE, COMMUN, PRIORITAIRE };

typedef struct _PidLibre {
    int pid;
    struct _PidLibre * suivPid;
}PidLibre;

extern PidLibre * PidLibreTete;


typedef struct _Processus {
    int pid;
    char nom[25];
    enum Etat etat;
    int prio;

    uint32_t secReveille;
    
    // Pile
    int32_t contexte[5];  // Array to hold the saved registers (ebx, esp, ebp, esi, edi)
    int32_t pile[SIZE_PILE_EXEC];  // Stack for the process
    // chainage + filiation
    link chainage; // chainage pour les liaisons avec ordre de priorité
    struct Fils * FilsTete; // Liste des fils

    int retval;
    struct _Processus * pere;

    // Files de messages
    int fid;
    int fileValue;
    bool isOperationSuccess;
} Processus;


struct Fils {
    Processus * procFils;
    struct Fils * suiv;
};

// Déclaration des variables globales de processus
extern struct list_link proc_activables;

extern Processus * ProcIdle;
extern Processus * ProcElu;
extern Processus * tableDesProcs[MAX_PROCESS];

/* Fonction control switch */
extern void ctx_sw(int *, int *);
extern void ctx_ld(int *);
/* Fonction de retour qui appel exit avec %eax en argument*/
extern void exit_routine(void);

extern void init_ordonnanceur(void);
extern void ordonnanceur(void);

int getpid(void);
char *mon_nom(void);

// Fonctions de prio
extern int getprio(int pid);
extern int chprio(int pid, int newprio);

extern int start(int (*pt_func)(void*), unsigned long ssize, int prio, const char *name, void *arg);

/******Edormi******/

extern void wait_clock(unsigned long ticks);

extern void insert_endormi(Processus *proc);

extern void verifie_reveille(unsigned long ticks);

/*****Exit*****/

extern void exit(int retval);

extern int kill(int pid);

/**** Fonction Filiation ****/

extern int waitpid(int, int *);

#endif
