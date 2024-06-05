#include "stdint.h"
#include "processus.h"
#include "stddef.h"
#include "mem.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "horloge.h"
#include "queue.h"

struct PidLibre * PidLibreTete;
struct Processus * tableDesProcs[MAX_PROCESS];

struct Processus * ProcElu;

LIST_HEAD(proc_activables);
LIST_HEAD(proc_endormis);
LIST_HEAD(proc_mourants);
void init_listes(void) {
    INIT_LIST_HEAD(&proc_activables);
    INIT_LIST_HEAD(&proc_endormis);
    INIT_LIST_HEAD(&proc_mourants);

    printf("Listes initialisées\n");
}


// retourne le pid en tete (PidLibreTete), déplace la tete ensuite
int32_t get_pid(){
    int32_t pid = -1;
    if (PidLibreTete != NULL){
        struct PidLibre * pidLibreTeteCopy = PidLibreTete;
        // On déplace la tête
        pid = PidLibreTete->pid;
        PidLibreTete = PidLibreTete->suivPid;
        // Libère la struct pid prise
        mem_free( pidLibreTeteCopy, sizeof(struct PidLibre));
    }
    return pid;
}
// ajoute un pid à la liste des pid libre en tete !
void add_pid(int32_t pid){
    struct PidLibre * pLibreNew = mem_alloc(sizeof(struct PidLibre));
    pLibreNew->pid = pid;
    pLibreNew->suivPid = PidLibreTete; // Le suiv devient la tête
    PidLibreTete = pLibreNew; // La tête est mise avec le nouvelle tête
}

// ajout un fils à un processus
void add_fils(struct Processus * pere, struct Processus * fils){
    struct Fils * filsNew = mem_alloc(sizeof(struct Fils));
    filsNew->procFils = fils;
    filsNew->suiv = pere->FilsTete;
    pere->FilsTete = filsNew;
}

// int start(int (*pt_func)(void*), unsigned long ssize, int prio, const char *name, void *arg);
// Crée un processus et l'ajoute à la queue des processus activables
int32_t cree_processus(void (*code)(void), int prio, char *nom){
    int32_t pid = get_pid();

    if(pid != -1){
        struct Processus * proc = mem_alloc(sizeof(struct Processus));
        // on gere les attribus
        strcpy(proc->nom, nom);
        proc->pid = pid;
        proc->etat = ACTIVABLE;
        proc->secReveille = 0;
        proc->prio = prio;
        // on gere les adresses de retour
        proc->contexte[1] = (int32_t)&proc->pile[SIZE_PILE_EXEC-2];
        proc->pile[SIZE_PILE_EXEC - 1] = (int32_t)fin_processus;
        proc->pile[SIZE_PILE_EXEC - 2] = (int32_t)code;

        // gestion des filiation
        proc->pere = ProcElu; // Le proc ELU est celui qui a creer le fils
        if (ProcElu != NULL){
            printf("Fils ajouté\n");
            add_fils(ProcElu, proc);
        } else {
            printf("Idle!!!!\n");
        }

        tableDesProcs[pid] = proc;
        queue_add(proc, &proc_activables, struct Processus, chainage, prio);

    }
    return pid;
}

int32_t mon_pid(void){
    // return pid de pro elu
    return ProcElu != NULL ? (int32_t) ProcElu->pid :  -1;
}

char *mon_nom(void){
    // return nom du pro elu
    return ProcElu != NULL ? ProcElu->nom : "No Proc";
}

/********* Ordonnanceur *********/

// Libère les processus mourant
void free_mourant_queue(){
    struct Processus * procMort;
    while ((procMort = queue_out(&proc_mourants, struct Processus, chainage)) != NULL){
        tableDesProcs[procMort->pid] = NULL;
        add_pid(procMort->pid); // On libère le pid
        mem_free(procMort, sizeof(struct Processus));
    }
}


/*********TEST FNC*********/
// Fonction pour afficher l'état de chaque processus
void proc_test(){
    printf("proc_test [temps = %u] processus %s pid = %i\n", nbr_secondes(), mon_nom(),
          mon_pid());
    dors(3);
}
/***********END TEST FNC*********/

void ordonnanceur(void){
    free_mourant_queue(); // On libère les processus mourant
    
    if (ProcElu == NULL){
        // si on a pas d'elu on prend le premier activable
        ProcElu = queue_out(&proc_activables, struct Processus, chainage); // on assume le fait qu'au moins un processus existe
        ProcElu->etat = ELU;
        return;
    }
    /*****TEST******/
    cree_processus(&proc_test, COMMUN , "proc_test");
    /*****END TEST***/
    
    struct Processus * procActuel = ProcElu;
    struct Processus * procsuiv = queue_out(&proc_activables, struct Processus, chainage);
    if(procsuiv != NULL){
        // Switch les états
        procsuiv->etat = ELU;
        ProcElu = procsuiv;
        queue_add(procActuel, &proc_activables, struct Processus, chainage, prio);

        ctx_sw(procActuel->contexte,procsuiv->contexte);
    }
}

void init_ordonnanceur(){

    struct PidLibre * pLibre = mem_alloc(sizeof(struct PidLibre));
    pLibre->pid = 0;
    
    PidLibreTete = pLibre;
    
    for (int i=1;i<MAX_PROCESS;i++){
        struct PidLibre * pLibresuiv = mem_alloc(sizeof(struct PidLibre));
        pLibresuiv->pid = i;
        pLibre->suivPid = pLibresuiv;
        pLibre = pLibresuiv;
    }

    init_listes();
}

/*******Endormi******/
void dors(uint32_t nbr_secs){

    struct Processus * procEndormir = ProcElu;
    procEndormir->etat = ENDORMI;
    procEndormir->secReveille = nbr_secondes()+nbr_secs; // On ajoute le temps de réveil
    // On change d'élu
    ProcElu = queue_out(&proc_activables, struct Processus, chainage); // on assume le fait qu'au moins un processus existe
    ProcElu->etat = ELU;

    // On ajoute le processus endormi
    queue_add(procEndormir, &proc_endormis, struct Processus, chainage, secReveille);
    ctx_sw(procEndormir->contexte,ProcElu->contexte);
}


void verifie_reveille(uint32_t secSystem) {
    struct Processus *current;
    // use queue_for_each_prev
    queue_for_each_prev(current, &proc_endormis, struct Processus, chainage) {
        if (current->secReveille <= secSystem){
            current->etat = ACTIVABLE;
            queue_add(current, &proc_activables, struct Processus, chainage, prio);
            queue_del(current, chainage);
        } else {
            break;
        }
    }
}


/*****Exit*****/
// void exit(int retval);
// etval est passée à son père quand il appelle waitpid
void fin_processus(void){
    struct Processus * procMort = ProcElu;
    procMort->etat = MOURANT;
    queue_add(procMort, &proc_mourants, struct Processus, chainage, prio);

    // On change d'élu
    ProcElu = queue_out(&proc_activables, struct Processus, chainage);
    ProcElu->etat = ELU;

    ctx_sw(procMort->contexte, ProcElu->contexte);
}

/*************Filiation**************/

// int waitpid(int pid, int *retvalp);
int waitpid(int pid, int *retvalp){
    if (pid >= MAX_PROCESS){ // On vérifie que le pid est valide
        return -1;
    }
    if (pid >= 0){ // On attend un processus spécifique
        struct Processus * proc = tableDesProcs[pid];
        if (proc == NULL || proc->pere != ProcElu){
            return -1;
        }
        while(proc->etat != ZOMBIE){
            ordonnanceur();
        }
        *retvalp = proc->retvalp;
        return pid;
    }

    if (queue_empty(&ProcElu->chainage_fils)){
        return -1;
    }
    
    struct Processus * procFils;
    // for queue_for_each
    while (1){
        queue_for_each(procFils, &ProcElu->chainage_fils, struct Processus, chainage){
            if (procFils->etat == ZOMBIE){
                break;
            }
        }
        if (procFils != NULL){
            break;
        }
        ordonnanceur();
    }
    
    *retvalp = procFils->retvalp;
    return pid;
}

// TODO : int kill(int pid);

