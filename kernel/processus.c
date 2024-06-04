#include "stdint.h"
#include "processus.h"
#include "stddef.h"
#include "mem.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"


struct PidLibre * PidLibreTete;
struct Processus * ProcActivTete;
struct Processus * ProcActivQueue;
struct Processus * ProcDortTete;

struct Processus * tableDesProcs[MAX_PROCESS];


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
        proc->secReveille = 0;

        tableDesProcs[pid] = proc;

        insert_queue_activable(proc);
    }
    
    return pid;
}

int32_t mon_pid(void){
    if (ProcActivTete != NULL){
        return ProcActivTete->pid;
    } else {
        return -1;
    }
}

char *mon_nom(void){
    if (ProcActivTete != NULL){
        return ProcActivTete->nom;
    } else {
        return "No Proc";
    }
}

/********* Ordonnanceur *********/

void print_activable_queue() {
    struct Processus *current = ProcActivTete;
    while (current != NULL) {
        printf("Processus PID: %d, Nom: %s, Etat: %d\n", current->pid, current->nom, current->etat);
        current = current->next;
    }
}

void ordonnanceur(void){
    print_activable_queue();
    struct Processus * procActuel = ProcActivTete;
    struct Processus * procNext;
    if(procActuel != NULL && procActuel->next != NULL){
        procNext = procActuel->next;

        //printf("Moving current process (%s, pid: %d) to end of queue.\n", procActuel->nom, procActuel->pid);
        //printf("Setting next process (%s, pid: %d) as active.\n", procNext->nom, procNext->pid);
        
        procActuel->etat = ACTIVABLE;
        procNext->etat = ELU;
        ProcActivTete = procNext;

        insert_queue_activable(procActuel);
        ctx_sw(procActuel->contexte,procNext->contexte);
    }
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
        proc->next = NULL;
        ProcActivTete = proc;
        ProcActivQueue = proc;
        ProcActivQueue->etat = ELU;
    }
    else {
        proc->next = ProcActivTete;
        ProcActivTete = proc;

        ProcActivQueue->etat = ELU;
        proc->next->etat = ACTIVABLE;
    }
    
}


void insert_queue_activable(struct Processus *proc){
    proc->next = NULL;

    if(ProcActivTete == NULL){ // Initialise la queue et la tete avec lui même (1 seul élément)
        ProcActivTete = proc;
        ProcActivQueue = proc;
        ProcActivQueue->etat = ELU;
    }
    else {
        ProcActivQueue->next = proc;
        ProcActivQueue = proc;
        ProcActivQueue->etat = ACTIVABLE;
    }
}

/*******Endormi******/
void dors(uint32_t nbr_secs){
    struct Processus * procEndormir = ProcActivTete;

    procEndormir->etat = ENDORMI;
    procEndormir->secReveille = nbr_secs;
    ProcActivTete = procEndormir->next;
    ProcActivTete->etat = ELU;

    insert_endormi(procEndormir);
    
    ctx_sw(procEndormir->contexte,ProcActivTete->contexte);
}

void insert_endormi(struct Processus *proc) {
    // Si la liste est vide ou si le proc doit être placé au début
    if (ProcDortTete == NULL || proc->secReveille < ProcDortTete->secReveille) {
        proc->next = ProcDortTete;
        ProcDortTete = proc;
    } else {
        // Parcourir la liste pour trouver l'endroit d'insertion
        struct Processus *current = ProcDortTete;
        while (current!= NULL && current->secReveille <= proc->secReveille) {
            if (current->next == NULL || current->next->secReveille > proc->secReveille) {
                break; // On a trouvé l'emplacement ou on atteint la fin de la liste
            }
            current = current->next;
        }

        // Insérer le proc juste avant l'élément courant
        proc->next = current->next;
        current->next = proc;
    }
}



void verifie_reveille(uint32_t secSystem){

    struct Processus *current = ProcDortTete;
    struct Processus *next;

    while (current != NULL) {
        if (current->secReveille > secSystem){
            ProcDortTete = current; // On vire les procs réveiller !
            break; // Sert à rien de continuer de vérifier le reste...
        }
        
        next = current->next;
        insert_queue_activable(current);
        current = next;
        
    }
}

