#include "stdint.h"
#include "processus.h"
#include "stddef.h"
#include "mem.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "horloge.h"
#include "console.h"
#include "stdbool.h"

PidLibre * PidLibreTete;
Processus * tableDesProcs[MAX_PROCESS];

Processus * ProcIdle;
Processus * ProcElu;

bool is_idle_started = false;

/********Affichage********/
void affiche_table_process(){
    for(int i=0; i<MAX_PROCESS; i++){
        if(tableDesProcs[i]!=NULL)
        printf("processus %s pid = %i etat = %i\n", tableDesProcs[i]->nom, tableDesProcs[i]->pid, tableDesProcs[i]->etat);
    }
}

/*********Gestion liste********/

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
int32_t new_pid(){
    int32_t pid = -1;
    if (PidLibreTete != NULL){
        PidLibre * pidLibreTeteCopy = PidLibreTete;
        // On déplace la tête
        pid = PidLibreTete->pid;
        PidLibreTete = PidLibreTete->suivPid;
        // Libère la struct pid prise
        mem_free( pidLibreTeteCopy, sizeof(PidLibre));
    }
    return pid;
}
// ajoute un pid à la liste des pid libre en tete !
void add_pid(int32_t pid){
    PidLibre * pLibreNew = mem_alloc(sizeof(PidLibre));
    pLibreNew->pid = pid;
    pLibreNew->suivPid = PidLibreTete; // Le suiv devient la tête
    PidLibreTete = pLibreNew; // La tête est mise avec le nouvelle tête
}

// ajout un fils à un processus
void add_fils(Processus * pere, Processus * fils){
    struct Fils * filsNew = mem_alloc(sizeof(struct Fils));
    filsNew->procFils = fils;
    filsNew->suiv = pere->FilsTete;
    pere->FilsTete = filsNew;
}

// int start(int (*pt_func)(void*), unsigned long ssize, int prio, const char *name, void *arg);
// Crée un processus et l'ajoute à la queue des processus activables
// pile du processus ssize octets utilisables sans compter l'espace nécessaire au stockage de l'adresse de retour
int start(int (*pt_func)(void*), unsigned long ssize, int prio, const char *name, void *arg){
    int pid = new_pid();

    ssize += 5*8; // ssize + la place des param
    if (prio<0 || prio > PRIO_MAX){
        return -1;
    }

    if(prio == PRIO_IDLE && pid != 0 ){
        printf("Cannot assigne the min priority to a process other thant IDLE\n");
        return -1;
    }

    if(pid != -1){
        Processus * proc = mem_alloc(sizeof(Processus));
        // on gere les attribus
        strcpy(proc->nom, name);
        proc->pid = pid;
        proc->etat = ACTIVABLE;
        proc->secReveille = 0;
        proc->prio = prio;
        // on gere les adresses de retour
        proc->contexte[1] = (int32_t)&proc->pile[SIZE_PILE_EXEC-2];
        proc->pile[SIZE_PILE_EXEC - 1] = (int32_t)fin_processus;
        proc->pile[SIZE_PILE_EXEC - 2] = (int32_t)pt_func;

        // gestion des arguments
        //proc->pile[SIZE_PILE_EXEC - 3 ] = (int32_t)arg;
        arg = arg;

        // on ajoute le processus à la table des processus
        tableDesProcs[pid] = proc;

        // gestion des filiation
        if (ProcElu != ProcIdle){
            proc->pere = ProcElu; // Le proc ELU est celui qui a creer le fils
        } else {
            proc->pere = NULL;
        }
        
        if (ProcElu != NULL){ // Ajoute le parent si c'est pas idle
            printf("Fils ajouté parent: %s enfant: %s\n", ProcElu->nom, proc->nom);
            add_fils(ProcElu, proc);
            
            queue_add(proc, &proc_activables, Processus, chainage, prio);
            if (proc->prio > ProcElu->prio){
                ordonnanceur();
            }

        } else { // Le 1er processus est idle !
            ProcElu = proc;
            ProcIdle = proc;
            proc->etat = ELU;
        }

        
    }
    return pid;
}

int getpid(void){
    // return pid de pro elu
    return ProcElu != NULL ? ProcElu->pid :  -1;
}
char *mon_nom(void){
    // return nom du pro elu
    return ProcElu != NULL ? ProcElu->nom : "No Proc";
}

int getprio(int pid){
    if (pid < 0 || pid >= MAX_PROCESS){ // On vérifie que le pid est valide
        return -1;
    }
    Processus * proc = tableDesProcs[pid];
    if (proc == NULL){
        return -1;
    }
    return proc->prio;
}

int chprio(int pid, int newPrio) {
    if (pid < 0 || pid >= MAX_PROCESS){ // On vérifie que le pid est valide
        return -1;
    }
    if (newPrio<=0 || newPrio > PRIO_MAX){
        //printf("valeur de priorité non conforme");
        return -1;
    }

    Processus * proc = tableDesProcs[pid];
    if (proc == NULL){
        return -1;
    }

    int anciennePrio = proc->prio;
    proc->prio = newPrio;
    
    Processus *procActiablePrio = queue_top(&proc_activables, Processus, prio);

    // Si procElu modifie sa prio, on vérifie si on doit le remettre dans la queue
    if (ProcElu == proc){
        if (newPrio < procActiablePrio->prio){
            ordonnanceur();
        }
    } else { // Si c'est pas l'élu, on le replace dans la liste
        queue_del(proc, chainage);
        queue_add(proc, &proc_activables, Processus, chainage, prio);

        if (ProcElu->prio < newPrio){
            ordonnanceur();
        }
    }
    return anciennePrio;
}

/********* Ordonnanceur *********/
void free_childs(Processus * proc){
    struct Fils * fils = proc->FilsTete;
    struct Fils * filsSuiv;
    while (fils != NULL){
        filsSuiv = fils->suiv;
        
        if (fils->procFils->etat == ZOMBIE){
            fils->procFils->etat = MOURANT;
            queue_add(fils->procFils, &proc_mourants, Processus, chainage, prio);
        } else {
            fils->procFils->pere = NULL;
        }

        mem_free(fils, sizeof(struct Fils));
        fils = filsSuiv;
    }
}
// Libère les processus mourant
void free_mourant_queue(){
    Processus * procMort;
    while ((procMort = queue_out(&proc_mourants, Processus, chainage)) != NULL){
        printf("Je suis mort %s\n", procMort->nom);

        free_childs(procMort); // On libère les fils

        tableDesProcs[procMort->pid] = NULL;
        add_pid(procMort->pid); // On libère le pid
        mem_free(procMort, sizeof(Processus));
    }
}


/***********END TEST FNC*********/
void ordonnanceur(void){
   
    if (!is_idle_started){ // On attend d'être dans un contexte idle au début
        return;
    }
    free_mourant_queue(); // On libère les processus mourant
    Processus * procEluActuel = ProcElu;
     
    if (!queue_empty(&proc_activables)) {
        switch (procEluActuel->etat) {
            case ENDORMI:
                queue_add(procEluActuel, &proc_endormis, Processus, chainage, secReveille);
                break;
            case MOURANT:
                queue_add(procEluActuel, &proc_mourants, Processus, chainage, prio);
                break;
            case ZOMBIE: // On ne doit pas rajouter dans la liste si Zombie ou Attend
            case ATTEND_FILS:
                break;
            default: // Est activable sinon !
                procEluActuel->etat = ACTIVABLE;
                queue_add(procEluActuel, &proc_activables, Processus, chainage, prio);
                break;
        }

        Processus *procEluSuiv = queue_out(&proc_activables, Processus, chainage);
        procEluSuiv->etat = ELU;
        ProcElu = procEluSuiv; 
        
        ctx_sw(procEluActuel->contexte, ProcElu->contexte);
    }
}

void init_ordonnanceur(){

    PidLibre * pLibre = mem_alloc(sizeof(PidLibre));
    pLibre->pid = 0;
    
    PidLibreTete = pLibre;
    
    for (int i=1;i<MAX_PROCESS;i++){
        PidLibre * pLibresuiv = mem_alloc(sizeof(PidLibre));
        pLibresuiv->pid = i;
        pLibre->suivPid = pLibresuiv;
        pLibre = pLibresuiv;
    }

    init_listes();
}

/*******Endormi******/
void wait_clock(unsigned long ticks){
    // On assume que IDLE n'appelle jamais wait_clock 

    Processus * procEndormir = ProcElu;
    procEndormir->etat = ENDORMI;
    procEndormir->secReveille = current_clock()+ticks; // On ajoute le temps de réveil
    
    ordonnanceur();
}


void verifie_reveille(unsigned long ticks) {
    Processus *current;
    Processus *next;

    // Itérer en sens inverse sur la liste des endormis
    queue_for_each(current, &proc_endormis, Processus, chainage) {
        // Sauvegarder le pointeur vers l'élément suivant car 'current' peut être modifié dans le corps de la boucle
        next = queue_entry(current->chainage.prev, Processus, chainage);
        if (current->secReveille <= ticks){
            // On le réveille !
            current->etat = ACTIVABLE;
            queue_del(current, chainage);
            queue_add(current, &proc_activables, Processus, chainage, prio);
        } else {
            break;
        }
        current = next;
    }
}

/*****Exit*****/
// void exit(int retval);
// etval est passée à son père quand il appelle waitpid
void fin_processus(void){
    Processus * procMort = ProcElu;
    printf("fin_processus Je suis mort %s\n", procMort->nom);
    if (procMort->pere != NULL){
        procMort->etat = ZOMBIE;

        if (procMort->pere->etat == ATTEND_FILS){
            printf("Enfant fin! Réveil du père pid:%d\n", procMort->pere->pid);
            procMort->pere->etat = ACTIVABLE;
            queue_add(procMort->pere, &proc_activables, Processus, chainage, prio);
        }
        
    } else {
        procMort->etat = MOURANT;
        // queue_add(procMort, &proc_mourants, Processus, chainage, prio);
    }
    ordonnanceur();
}

// La primitive kill termine le processus identifié par la valeur pid. Si ce processus était bloqué dans une file 
// d'attente pour un quelconque élément système, alors il en est retiré.
int kill(int pid) {
    // Si ce processus était bloqué dans une file d'attente pour un quelconque élément système
    if(pid <= 0 || pid >= MAX_PROCESS || tableDesProcs[pid] == NULL ){
        return -1;
    }

    Processus * proc = tableDesProcs[pid];
    // Gestion de l'elu avec une comparaison de pointeur (on sais jamais)
    if(proc == ProcElu){
        proc->etat = MOURANT;
        if (proc->pere != NULL){
            proc->etat = ZOMBIE;
            
        } else {
            proc->etat = MOURANT;
        }
        ordonnanceur();
    }
    else if (proc->etat == ACTIVABLE || proc->etat == ENDORMI || proc->etat == ATTEND_FILS) {
        // si il est dans la liste des activable on le tue
        queue_del(proc, chainage);
        if (proc->pere != NULL){
            proc->etat = ZOMBIE;
            if(proc->pere->etat == ATTEND_FILS){
                proc->pere->etat = ACTIVABLE;
                queue_add(proc->pere, &proc_activables, Processus, chainage, prio);
            }
            
        } else {
            proc->etat = MOURANT;
            queue_add(proc, &proc_mourants, Processus, chainage, prio);
        }
    }

    return 0;
}

/*************Filiation**************/

// int waitpid(int pid, int *retvalp);
int waitpid(int pid, int *retvalp){
    if (pid >= MAX_PROCESS){ // On vérifie que le pid est valide
        return -1;
    }

    Processus * proc;
    struct Fils * procFils;

    if (pid >= 0){ // On attend un processus spécifique
        proc = tableDesProcs[pid];
        if (proc == NULL || proc->pere != ProcElu){
            return -1;
        }
        while(proc->etat != ZOMBIE){
            ProcElu->etat = ATTEND_FILS;
            ordonnanceur();
        }

    } else { // Si pid < 0, On attend le premier fils zombie
        
        procFils = ProcElu->FilsTete;
        if (procFils == NULL){ // On vérifie que le processus a des fils
            return -1;
        }
        while(1){
            
            procFils = ProcElu->FilsTete; // On réinitialise
            while (procFils != NULL){
                if (procFils->procFils->etat == ZOMBIE){
                    break;
                }
                procFils = procFils->suiv;
            }
            if (procFils != NULL){
                break;
            } else {
                ProcElu->etat = ATTEND_FILS;
                ordonnanceur(); // On attend
            }
        }
        proc = procFils->procFils;
    }
    // On retire l'enfant de la liste de ses enfants (on réintère toute la liste)
    procFils = ProcElu->FilsTete;
    if (procFils->procFils == proc){
        ProcElu->FilsTete = procFils->suiv;
    } else {
        while(procFils->suiv->procFils != proc){
            procFils = procFils->suiv;
        }
        procFils->suiv = procFils->suiv->suiv;
    }
    // On copy les valeurs du processus enfant
    int procPid = proc->pid;
    int retval = proc->retvalp;

    // On libère le processus enfant
    queue_add(proc, &proc_mourants, Processus, chainage, prio);

    // On retourne les valeur du processus enfant trouvé !
    *retvalp = retval;
    return procPid;
}

