#include "stdint.h"
#include "processus.h"
#include "stddef.h"
#include "mem.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "horloge.h"


struct PidLibre * PidLibreTete;
struct Processus * ProcActivTete;
struct Processus * ProcActivQueue;
struct Processus * ProcElu;

struct Processus * ProcDortTete;

struct Processus * tableDesProcs[MAX_PROCESS];

int stop = 0;
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
    return ProcElu != NULL ? (int32_t) ProcElu->pid :  -1;
}

char *mon_nom(void){
    return ProcElu != NULL ? ProcElu->nom : "No Proc";
}

/********* Ordonnanceur *********/

void print_activable_queue() {
    struct Processus *current = ProcActivTete;
    printf("Activable: ELU = %s |",ProcElu->nom);
    while (current != NULL) {
        printf("-> %s",current->nom);
        current = current->next;
    }
    printf("\n");
}
void print_endormis_queue() {
    struct Processus *current = ProcDortTete;
    printf("Endormis: ");
    while (current != NULL) {
        printf("-> %s n",current->nom);
        if (current == current->next){
            printf("Boucle infinie... \n");
            printf("Activable: \n");
            print_activable_queue();
            stop = 1;
            break;
        }
        current = current->next;
    }
    printf("\n");
}


void ordonnanceur(void){
    if (stop){ return; }
    
    if (ProcElu == NULL){
        printf("Aucun processus Elu... On donne le 1er par defaut\n");
        ProcElu = ProcActivTete;
        ProcElu->etat = ELU;
        ProcActivTete = ProcActivTete->next;
        return;
    }

    struct Processus * procActuel = ProcElu;
    struct Processus * procNext = ProcActivTete;

    if(procNext != NULL){
        // Switch les états
        procNext->etat = ELU;
        ProcElu = procNext;
        ProcActivTete = procNext->next; // On déplace la tête de la queue

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


void insert_queue_activable(struct Processus *proc){
    if (ProcActivTete == NULL){
        ProcActivTete = proc;
        ProcActivQueue = proc;
    } else {
        ProcActivQueue->next = proc;
        ProcActivQueue = proc;
    }
    proc->etat = ACTIVABLE;
    ProcActivQueue->next = NULL; // On met le next à NULL
}

/*******Endormi******/
void dors(uint32_t nbr_secs){

    struct Processus * procEndormir = ProcElu;
    procEndormir->etat = ENDORMI;
    procEndormir->secReveille = nbr_secondes()+nbr_secs;

    ProcElu = ProcActivTete;
    ProcElu->etat = ELU;
    ProcActivTete = ProcActivTete->next;

    insert_endormi(procEndormir);
    ctx_sw(procEndormir->contexte,ProcElu->contexte);
}

void insert_endormi(struct Processus *proc) {
    // Si la liste est vide ou si le proc doit être placé au début
    if (ProcDortTete == NULL || proc->secReveille < ProcDortTete->secReveille) {
        proc->next = ProcDortTete;
        ProcDortTete = proc;
    } else {
        // Parcourir la liste pour trouver l'endroit d'insertion
        struct Processus *current = ProcDortTete;
        while(current->next != NULL && current->next->secReveille <= proc->secReveille){
            current = current->next;
        }

        // Insérer le proc juste avant l'élément courant
        proc->next = current->next;
        current->next = proc;
    }
}



void verifie_reveille(uint32_t secSystem){
    if (stop){ return; }
    /*printf("===== Verifie reveille %d\n",secSystem);
    printf("Avant reveille: ");
    print_activable_queue();
    print_endormis_queue();*/
    struct Processus *current = ProcDortTete;
    struct Processus *next;

    while (current != NULL) {
        if (current->secReveille > secSystem){
            break; // Sert à rien de continuer de vérifier le reste...
        }
        
        next = current->next;
        insert_queue_activable(current);
        current = next;
        
    }
    ProcDortTete = current; // On vire les procs réveiller !
}

