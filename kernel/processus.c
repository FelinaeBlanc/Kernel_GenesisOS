#include "stdint.h"
#include "processus.h"
#include "stddef.h"
#include "mem.h"
#include "string.h"
#include "stdlib.h"

struct PidLibre * PidLibreTete;
struct Processus * ProcActivTete;
struct Processus * ProcActivQueue;


// retourne le pid en tete (PidLibreTete), déplace la tete ensuite
int32_t get_pid(){
    int32_t pid = -1;
    if (PidLibreTete != NULL){
        struct PidLibre * pidLibreTeteCopy = PidLibreTete;
        // On déplace la tête
        pid = PidLibreTete->pid;
        PidLibreTete = PidLibreTete->nextPid;
        // Libère la struct pid prise
        mem_free( pidLibreTeteCopy, sizeof(struct PidLibre));
    }
    return pid;
}
// ajoute un pid à la liste des pid libre en tete !
void add_pid(int32_t pid){
    struct PidLibre * pLibreNew = mem_alloc(sizeof(struct PidLibre));
    pLibreNew->pid = pid;
    pLibreNew->nextPid = PidLibreTete; // Le next devient la tête
    PidLibreTete = pLibreNew; // La tête est mise avec le nouvelle tête
}

int32_t cree_processus(void (*code)(void), char *nom){
    int32_t pid = get_pid();

    if(pid != -1){
        struct Processus * proc = mem_alloc(sizeof(struct Processus));
        strcpy(proc->nom, nom);
        proc->pid = pid;
        proc->etat = ACTIVABLE;
        proc->contexte[1] = (int32_t)&proc->pile[SIZE_PILE_EXEC-1];
        proc->pile[SIZE_PILE_EXEC - 1] = (int32_t)code;

        tableDesProcs[pid] = proc;

        insert_queue_activable(proc);

        if (processusElu == NULL){
            processusElu = proc;
            proc->etat = ELU;
        }
    }
    
    return pid;
}

int32_t mon_pid(void){
    if (processusElu != NULL){
        return processusElu->pid;
    } else {
        return -1;
    }
}

char *mon_nom(void){
    if (processusElu != NULL){
        return processusElu->nom;
    } else {
        return "No Proc";
    }
}

/********* Ordonnanceur *********/

void ordonnance(void){
    int32_t actualPid = mon_pid();
    int32_t nextPid = (actualPid+1)%MAX_PROCESS; // prendre le tableau[(i+1)%MAX_PROCESS]
    
    tableDesProcs[actualPid]->etat = ACTIVABLE;
    tableDesProcs[nextPid]->etat = ELU;
    processusElu = tableDesProcs[nextPid];
    
    ctx_sw(tableDesProcs[actualPid]->contexte,tableDesProcs[nextPid]->contexte);
}

void init_ordonnanceur(){

    struct PidLibre * pLibre = mem_alloc(sizeof(struct PidLibre));
    pLibre->pid = 0;
    
    PidLibreTete = pLibre;
    
    for (int i=1;i<MAX_PROCESS;i++){
        struct PidLibre * pLibreNext = mem_alloc(sizeof(struct PidLibre));
        pLibreNext->pid = i;
        pLibre->nextPid = pLibreNext;
        pLibre = pLibreNext;
    }
}

/********* Activable *********/

void insert_tete_activable(struct Processus *proc){
    if(ProcActivTete == NULL){ // Initialise la queue et la tete avec lui même (1 seul élément)
        ProcActivTete = proc;
        ProcActivQueue = proc;

        ProcActivTete->next = NULL;
        ProcActivQueue->next = NULL;
    }
    else {
        proc->next = ProcActivTete;
        ProcActivTete = proc;
    }
}

void insert_queue_activable(struct Processus *proc){
    if(ProcActivTete == NULL){ // Initialise la queue et la tete avec lui même (1 seul élément)
        ProcActivTete = proc;
        ProcActivQueue = proc;

        ProcActivTete->next = NULL;
        ProcActivQueue->next = NULL;
    }
    else {
        proc->next = ProcActivQueue;
        ProcActivQueue = proc;
    }
}
