#include "stdint.h"
#include "processus.h"
#include "stddef.h"
#include "mem.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "horloge.h"

struct PidLibre *PidLibreTete;
struct Processus *ProcActivTete;
struct Processus *ProcActivQueue;
struct Processus *ProcElu;
struct Processus *ProcMourantTete;

struct Processus *ProcDortTete;

struct Processus *tableDesProcs[MAX_PROCESS];

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

// void AddFils(struct Processus * pere, struct Processus * fils){
//     struct Fils * filsNew = mem_alloc(sizeof(struct Fils));
//     filsNew->procFils = fils;
//     filsNew->suiv = pere->FilsTete;
//     pere->FilsTete = filsNew;
// }

// int start(int (*pt_func)(void*), unsigned long ssize, int prio, const char *name, void *arg);
// Crée un processus et l'ajoute à la queue des processus activables
int32_t cree_processus(void (*code)(void), int prio, char *nom){
    int32_t pid = get_pid();

    if(pid != -1){
        struct Processus * proc = mem_alloc(sizeof(struct Processus));
        // on gere les attributs
        strcpy(proc->nom, nom);
        proc->pid = pid;
        proc->etat = ACTIVABLE;
        proc->secReveille = 0;
        proc->prio = prio;
        // on gere les adresses de retour
        proc->contexte[1] = (int32_t)&proc->pile[SIZE_PILE_EXEC-2];
        proc->pile[SIZE_PILE_EXEC - 1] = (int32_t)fin_processus;
        proc->pile[SIZE_PILE_EXEC - 2] = (int32_t)code;

        proc->prec = NULL;
        proc->suiv = NULL;

        tableDesProcs[pid] = proc;
        insert_queue_activable(proc);
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
    // on libère les pid mourant 
    struct Processus *current = ProcMourantTete;
    struct Processus *suiv;
    while (current != NULL) {
        suiv = current->suiv;
        tableDesProcs[current->pid] = NULL;

        add_pid(current->pid); // On libère le pid
        mem_free(current, sizeof(struct Processus));
        current = suiv;
    }
    ProcMourantTete = NULL;
}


/*********TEST FNC*********/
// Fonction pour afficher l'état de chaque processus
// void proc_test(){
//     printf("proc_test [temps = %u] processus %s pid = %i\n", nbr_secondes(), mon_nom(),
//           mon_pid());
//     dors(3);
// }
/***********END TEST FNC*********/
void afficher_etats_processus() {
    for (int i = 0; i < MAX_PROCESS; ++i) {
        struct Processus *proc = tableDesProcs[i];
        if (proc != NULL) {
            printf("Processus PID: %d, Nom: %s, Etat: %d\n", proc->pid, proc->nom, proc->etat);
        }
    }
}

void ordonnanceur(void){
    free_mourant_queue(); // On libère les processus mourant
    //afficher_etats_processus();
    if (ProcElu == NULL){
        // si on a pas d'elu on prend le premier activable
        ProcElu = ProcActivTete; // on assume le fait qu'au moins un processus existe
        ProcElu->etat = ELU;
        ProcActivTete = ProcActivTete->suiv;
        if (ProcActivTete != NULL) {
            ProcActivTete->prec = NULL;
        } else {
            ProcActivQueue = NULL;
        }
        return;
    }
    /*****TEST******/
    // cree_processus(&proc_test, COMMUN , "proc_test");
    /*****END TEST***/
    
    struct Processus * procActuel = ProcElu;
    struct Processus * procsuiv = ProcActivTete;
    if(procsuiv != NULL){
        // Switch les états
        procsuiv->etat = ELU;
        ProcElu = procsuiv;
        ProcActivTete = procsuiv->suiv;
        if (ProcActivTete != NULL) {
            ProcActivTete->prec = NULL;
        } else {
            ProcActivQueue = NULL;
        }

        // on mets l'acctuelle dans la liste des activable (en dernier!) 
        insert_queue_activable(procActuel);
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
}

/********* Activable *********/


void insert_queue_activable(struct Processus *proc) {
    proc->etat = ACTIVABLE;

    if (ProcActivTete == NULL) {
        // Si la liste est vide, on insère le processus en tête
        ProcActivTete = proc;
        ProcActivQueue = proc;
        proc->suiv = NULL;
        proc->prec = NULL;
    } else {
        struct Processus *current = ProcActivTete;

        // Trouver la position d'insertion en fonction de la priorité
        while (current != NULL && current->prio >= proc->prio) {
            current = current->suiv;
        }

        if (current == ProcActivTete) {
            // Insérer en tête
            proc->suiv = ProcActivTete;
            proc->prec = NULL;
            ProcActivTete->prec = proc;
            ProcActivTete = proc;
        } else if (current == NULL) {
            // Insérer en queue
            proc->suiv = NULL;
            proc->prec = ProcActivQueue;
            ProcActivQueue->suiv = proc;
            ProcActivQueue = proc;
        } else {
            // Insérer au milieu
            proc->suiv = current;
            proc->prec = current->prec;
            current->prec->suiv = proc;
            current->prec = proc;
        }
    }
}


/*******Endormi******/
void dors(uint32_t nbr_secs){

    struct Processus * procEndormir = ProcElu;
    procEndormir->etat = ENDORMI;
    procEndormir->secReveille = nbr_secondes()+nbr_secs; // On ajoute le temps de réveil
    // On change d'élu
    ProcElu = ProcActivTete;
    if (ProcElu != NULL) {
        ProcElu->etat = ELU;
        ProcActivTete = ProcActivTete->suiv;
        if (ProcActivTete != NULL) {
            ProcActivTete->prec = NULL;
        } else {
            ProcActivQueue = NULL;
        }
    }

    insert_endormi(procEndormir);
    if (ProcElu != NULL) {
        ctx_sw(procEndormir->contexte,ProcElu->contexte);
    }
}

void insert_endormi(struct Processus *proc) {
    // Si la liste est vide ou si le proc doit être placé au début
    if (ProcDortTete == NULL || proc->secReveille < ProcDortTete->secReveille) {
        proc->suiv = ProcDortTete;
        if (ProcDortTete != NULL) {
            ProcDortTete->prec = proc;
        }
        ProcDortTete = proc;
        proc->prec = NULL;
    } else {
        // Parcourir la liste pour trouver l'endroit d'insertion
        struct Processus *current = ProcDortTete;
        while(current->suiv != NULL && current->suiv->secReveille <= proc->secReveille){
            current = current->suiv;
        }

        // Insérer le proc juste avant l'élément courant
        proc->suiv = current->suiv;
        if (current->suiv != NULL) {
            current->suiv->prec = proc;
        }
        current->suiv = proc;
        proc->prec = current;
    }
}


void verifie_reveille(uint32_t secSystem){

    struct Processus *current = ProcDortTete;
    while (current != NULL) {
        if (current->secReveille > secSystem){
            break; // Sert à rien de continuer de vérifier le reste...
        }

        struct Processus *suiv = current->suiv;
        insert_queue_activable(current);
        current = suiv;

    }
    ProcDortTete = current;
    if (ProcDortTete != NULL) {
        ProcDortTete->prec = NULL;
    }
}

/*****Exit*****/

void fin_processus(void) {
    struct Processus *procMort = ProcElu;

    // On tue procMort et on l'insère en tête des morts
    procMort->etat = MOURANT;
    procMort->suiv = ProcMourantTete;
    if (ProcMourantTete != NULL) {
        ProcMourantTete->prec = procMort;
    }
    ProcMourantTete = procMort;

    // On change d'élu
    ProcElu = ProcActivTete;
    if (ProcElu != NULL) {
        ProcElu->etat = ELU;
        ProcActivTete = ProcActivTete->suiv;
        if (ProcActivTete != NULL) {
            ProcActivTete->prec = NULL;
        } else {
            ProcActivQueue = NULL;
        }
        ctx_sw(procMort->contexte, ProcElu->contexte);
    }
}

/*************Affichage pour debug *****************/


void print_activable_queue() {
    struct Processus *current = ProcActivTete;
    printf("Activable: ELU = %s |",ProcElu->nom);
    while (current != NULL) {
        printf("-> %s",current->nom);
        current = current->suiv;
    }
    printf("\n");
}

void print_endormis_queue() {
    struct Processus *current = ProcDortTete;
    printf("Endormis: ");
    while (current != NULL) {
        printf("-> %s\n", current->nom);
        current = current->suiv;
    }
    printf("\n");
}

/*************Filiation**************/

// int waitpid(int pid, int *retvalp);
// int waitpid(int pid, int *retvalp){
//     if (pid >= MAX_PROCESS){ // On vérifie que le pid est valide
//         return -1;
//     }
//     if (pid >= 0){ // On attend un processus spécifique
//         struct Processus * proc = tableDesProcs[pid];
//         if (proc == NULL || proc->pere != ProcElu){
//             return -1;
//         }
//         while(proc->etat != ZOMBIE){
//             ordonnanceur();
//         }
//         *retvalp = proc->retvalp;
//         return pid;
//     }

//     // On attend le premier fils zombie qui se termine
//     struct Processus *procFils = ProcElu->procFilsTete;
//     if (procFils == NULL){ // Pas de fils
//         return -1;
//     }

//     while(1){ // On attend un fils zombie
//         while(procFils != NULL && procFils->etat != ZOMBIE){
//             procFils = procFils->suiv;
//         }
//         if (procFils != NULL){
//             break;
//         }
//         ordonnanceur();
//     }
    
    
//     *retvalp = procFils->retvalp;
//     return pid;
// }

// TODO : int kill(int pid);

