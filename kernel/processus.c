#include "stdint.h"
#include "processus.h"
#include "stddef.h"
#include "mem.h"
#include "user_stack_mem.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "horloge.h"
#include "console.h"
#include "stdbool.h"
#include "files.h" // Pour les files (chprio)
#include "semaphore.h" // Pour les sémaphores
#include "traitant_IT_49.h"
#include "traitant_IT_33.h"
#include "segment.h"
#include "processor_structs.h"
#include "cpu.h"

PidLibre * PidLibreTete;
Processus * tableDesProcs[MAX_PROCESS];

Processus * ProcIdle;
Processus * ProcElu;

/********Affichage********/
void affiche_table_process(){
    for(int i=0; i<MAX_PROCESS; i++){
        if(tableDesProcs[i]!=NULL)
        printf("[PID %i] processus %s etat = %i\n", tableDesProcs[i]->pid, tableDesProcs[i]->nom, tableDesProcs[i]->etat);
    }
}

/*********Gestion liste********/

LIST_HEAD(proc_activables);
LIST_HEAD(proc_endormis);
LIST_HEAD(proc_mourants);
LIST_HEAD(proc_bloque_es);

void init_listes(void) {
    INIT_LIST_HEAD(&proc_activables);
    INIT_LIST_HEAD(&proc_endormis);
    INIT_LIST_HEAD(&proc_mourants);
    INIT_LIST_HEAD(&proc_bloque_es);
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
    if(pid <= -1){ return -1; } 

    // Vérifie Prio
    if (prio<=0 || prio > PRIO_MAX){ add_pid(pid); return -1;}

    // Vérifie dépassement avant addition
    unsigned long octets_reserver = USER_STACK_SIZE * sizeof(int32_t); // Nos 3 mots (arg, exit_routine, pt_func) + operation free_mourant_queue etc..
    if (ssize > ULONG_MAX - octets_reserver){ add_pid(pid); return -1; }

    // taille des pile coté user/kernel
    unsigned long User_taillePileOctets = ssize + octets_reserver; // En octets
    unsigned long Kernel_taillePileOctets = KERNEL_STACK_SIZE * sizeof(int32_t);


    Processus * proc = mem_alloc(sizeof(Processus));
    if (proc == NULL){
        add_pid(pid);
        return -1;
    }
    // Alloue pile user
    int32_t * pileUser = (int32_t *) user_stack_alloc(User_taillePileOctets);
    if (pileUser == NULL){
        add_pid(pid);
        mem_free(proc, sizeof(Processus));
        return -1;
    }
    // Alloue pile kernel
    int32_t * pileKernel = (int32_t *) mem_alloc(Kernel_taillePileOctets);
    if (pileKernel == NULL){
        add_pid(pid);
        user_stack_free(pileUser, User_taillePileOctets);
        mem_free(proc, sizeof(Processus));
        return -1;
    }
    
    if (name == NULL){
        name = "NoName";
    }
    strncpy(proc->nom, name, sizeof(proc->nom) - 1);

    proc->pid = pid;
    proc->etat = ACTIVABLE;
    proc->secReveille = 0;
    proc->prio = prio;
    proc->place = 0; // Variable dummy pour queue FIFO uniquement sans priorité

    proc->pileUser = pileUser;
    proc->pileKernel = pileKernel;

    int32_t nbMotUser = User_taillePileOctets / sizeof(int32_t); // On assume que la taille de la pile est un multiple de 4
    proc->pileSizeUser = User_taillePileOctets;
    int32_t nbMotKernel = Kernel_taillePileOctets / sizeof(int32_t); // On assume que la taille de la pile est un multiple de 4
    proc->pileSizeKernel = Kernel_taillePileOctets;

    // gestion de la pile

    // CS (code segment) désigne le segment contenant le programme en cours d'exécution ;
    // DS (data segment) désigne le segment des données courantes ;
    // SS (stack segment) désigne un segment de pile ;
    // ES, FS et GS permettent d'adresser trois segments supplémentaires.

    proc->pileUser[nbMotUser-1] = (int32_t)arg;  // gestion des arguments
    proc->pileUser[nbMotUser-2] = EXIT_ROUTINE; // Ret adresse
    //proc->pileUser[nbMotUser - 3] = (int32_t)pt_func;

    proc->pileKernel[nbMotKernel-1] = USER_DS; // SS
    proc->pileKernel[nbMotKernel-2] = (int32_t)(&proc->pileUser[nbMotUser - 2]); // ESP User
    proc->pileKernel[nbMotKernel-3] = USER_EFLAGS; // EFLAGS
    proc->pileKernel[nbMotKernel-4] = USER_CS; // CS
    proc->pileKernel[nbMotKernel-5] = (int32_t)pt_func; // EIP
    proc->pileKernel[nbMotKernel-6] = (int32_t)iret_func; // iret!

    // Ajoute les canaries au début et fin de la pile
    //proc->pileKernel[0] = CANARY_VALUE_A;
    //proc->pileKernel[1] = CANARY_VALUE_B;
    //proc->pileKernel[2] = CANARY_VALUE_C;

    //proc->pileKernel[nbMot - 1] = CANARY_VALUE_A;
    //proc->pileKernel[nbMot - 2] = CANARY_VALUE_B;
    //proc->pileKernel[nbMot - 3] = CANARY_VALUE_C;

    // (ebx, esp, ebp, esi, edi)
    proc->contexte[1] = (int32_t)&proc->pileKernel[nbMotKernel-6];
    tableDesProcs[pid] = proc; // on ajoute le processus à la table des processus

    // gestion des filiation
    if (ProcElu != ProcIdle){
        proc->pere = ProcElu; // Le proc ELU est celui qui a creer le fils
        add_fils(ProcElu, proc);
    } else {
        proc->pere = NULL;
    }

    queue_add(proc, &proc_activables, Processus, chainage, prio);

    // affiche_table_process();
    if (prio > ProcElu->prio){ // On switch si on a une meilleur prio !
        ordonnanceur();
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
    if (pid <= 0 || pid >= MAX_PROCESS){ // On vérifie que le pid est valide
        return -1;
    }
    if (newPrio <= 0 || newPrio > PRIO_MAX){
        return -1;
    }

    Processus * proc = tableDesProcs[pid];
    if (proc == NULL || proc->etat == ZOMBIE){ // Pas de kill de zombie
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
    } else if (proc->etat == ACTIVABLE) { // Si c'est pas l'élu, on le replace dans la liste mais qu'il est activable !!!!
        queue_del(proc, chainage);
        queue_add(proc, &proc_activables, Processus, chainage, prio);

        if (ProcElu->prio < newPrio){
            ordonnanceur();
        }
    } else if (proc->etat == ATTEND_FILE){
        int fid = proc->fid;
        assert(isFidValideAndExist(fid));
        File * file = tableauFile[fid];
        /*
         Un processus bloqué sur file vide et dont la priorité est changée par chprio, 
        est considéré comme le dernier processus (le plus jeune) de sa nouvelle priorité.
        */
        queue_del(proc, chainage);
        queue_add(proc, &file->queueAttente, Processus, chainage, prio);
    }
    // ENDORMI, MOURANT, ZOMBIE, ATTEND_FILS
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
        //printf("Je suis mort %s\n", procMort->nom);

        free_childs(procMort); // On libère les fils

        tableDesProcs[procMort->pid] = NULL;
        add_pid(procMort->pid); // On libère le pid
        mem_free(procMort->pileKernel, procMort->pileSizeKernel ); // On libère la pile kernel
        user_stack_free(procMort->pileUser, procMort->pileSizeUser); // On libère la pile user
        mem_free(procMort, sizeof(Processus));
    }
}

// bool checkCanary(Processus * proc){
//     if (proc == NULL){ return false; }

//     int32_t nbMot = proc->pileSize / sizeof(int32_t);
//     return (proc->pileKernel[0] == CANARY_VALUE_A && proc->pileKernel[1] == CANARY_VALUE_B && proc->pileKernel[2] == CANARY_VALUE_C) 
//     && (proc->pileKernel[nbMot - 1] == CANARY_VALUE_A && proc->pileKernel[nbMot - 2] == CANARY_VALUE_B && proc->pileKernel[nbMot - 3] == CANARY_VALUE_C);
// }

/***********Ordonnanceur*********/
void ordonnanceur(void){

    free_mourant_queue();
    //free_mourant_queue(); // On libère les processus mourant
    Processus * procEluActuel = ProcElu;
    if (!queue_empty(&proc_activables)) {

        /*if (procEluActuel != ProcIdle && !checkCanary(procEluActuel)){
            printf("Canary corrompu ! EXIT /!\\\n");
            *(char *)0 = 0;
        }*/

        switch (procEluActuel->etat) {
            case MOURANT:
                queue_add(procEluActuel, &proc_mourants, Processus, chainage, prio);
                break;

            case ELU:
            case ACTIVABLE: // Est activable sinon !
                procEluActuel->etat = ACTIVABLE;
                queue_add(procEluActuel, &proc_activables, Processus, chainage, prio);
                break;
                
            default:
                break;
        }

        Processus *procEluSuiv = queue_out(&proc_activables, Processus, chainage);
        procEluSuiv->etat = ELU;
        ProcElu = procEluSuiv; 

        int32_t nbMotKernel = ProcElu->pileSizeKernel / sizeof(int32_t);
        tss.esp0 =  (int) &ProcElu->pileKernel[nbMotKernel-1];//ProcElu->contexte[1];
        ctx_sw(procEluActuel->contexte,ProcElu->contexte);
    }
}

void init_ordonnanceur(){

    // On initialise la liste des pid libre
    PidLibre * pLibre = mem_alloc(sizeof(PidLibre));
    pLibre->pid = 1;

    PidLibreTete = pLibre;
    for (int i=2;i<MAX_PROCESS;i++){
        PidLibre * pLibresuiv = mem_alloc(sizeof(PidLibre));
        pLibresuiv->pid = i;
        pLibre->suivPid = pLibresuiv;
        pLibre = pLibresuiv;
    }
    
    init_listes();
    init_semaphores();


    // On initialise le processus Idle
    // start implicite de idle 
    Processus * proc = mem_alloc(sizeof(Processus));

    // on gere les attribus
    strcpy(proc->nom, "IDLE");
    proc->pid = 0;
    proc->etat = ELU;
    proc->secReveille = 0;
    proc->prio = PRIO_IDLE;

    tableDesProcs[0] = proc;

    ProcIdle = proc;
    ProcElu = proc;

    init_files(); // Init les files !
    init_horloge(); // Init l'horloge après...
    init_traitant_IT(49, traitant_IT_49, IT49);

    init_traitant_IT(33, traitant_IT_33, IT33);
    masque_IRQ(1 , false);

    int (*user_start)(void*) = (int (*)(void*))USER_START; // 16M
    start(user_start, 0, 1, "USER", NULL);
}


/*******Endormi******/
void wait_clock(unsigned long ticks){
    // On assume que IDLE n'appelle jamais wait_clock 
    //affiche_table_process();
    Processus * procEndormi = ProcElu;
    procEndormi->etat = ENDORMI;
    procEndormi->secReveille = ticks; // On ajoute le temps de réveil
    queue_add(procEndormi, &proc_endormis, Processus, chainage, secReveille);
    // printf("Je m'endors %s %d %d\n", procEndormir->nom,procEndormir->etat,procEndormir->secReveille );
    ordonnanceur();
}


void verifie_reveille(unsigned long ticks) {
    Processus *current;
    Processus *next;

    bool callOrdonnanceur = false;
    int procEluPrio = ProcElu->prio;

    // Itérer en sens inverse sur la liste des endormis
    queue_for_each(current, &proc_endormis, Processus, chainage) {
        // Sauvegarder le pointeur vers l'élément suivant car 'current' peut être modifié dans le corps de la boucle
        next = queue_entry(current->chainage.prev, Processus, chainage);
        if (current->secReveille <= ticks){
            // On le réveille !
            current->etat = ACTIVABLE; 
            queue_del(current, chainage);
            queue_add(current, &proc_activables, Processus, chainage, prio);

            if (current->prio > procEluPrio){ // Vérifie si on doit appeler l'ordonnanceur
                callOrdonnanceur = true;
            }
        } else {
            break;
        }
        current = next;
    }

    if (callOrdonnanceur){ // Si un processus de plus haute priorité a été ajouté
        ordonnanceur();
    }
}

void verifie_es(void){

    Processus *current = queue_out(&proc_bloque_es, Processus, chainage);

    // on a pas de proc bloqué
    if(current==NULL) return;

    // on reveille l'élement 
    current->etat = ACTIVABLE; 
    queue_add(current, &proc_activables, Processus, chainage, prio);

    if(ProcElu->prio > current->prio){
        ordonnanceur();
    }
}

/*****Exit*****/
// Sort du noyau
void exit_kernel(void){
    outw(0x2000, 0x604);
}

// etval est passée à son père quand il appelle waitpid
void exit(int retval){
    Processus * procMort = ProcElu;
    procMort->retval = retval;

    // printf("fin_processus Je suis mort %s\n", procMort->nom);
    if (procMort->pere != NULL){
        procMort->etat = ZOMBIE;

        if (procMort->pere->etat == ATTEND_FILS){
            // retourner la retval au pere
            procMort->pere->etat = ACTIVABLE;
            queue_add(procMort->pere, &proc_activables, Processus, chainage, prio);
        }
        
    } else {
        procMort->etat = MOURANT;
    }
    ordonnanceur();

    while(1);
}

// La primitive kill termine le processus identifié par la valeur pid. Si ce processus était bloqué dans une file 
// d'attente pour un quelconque élément système, alors il en est retiré.
int kill(int pid) {
    // Si ce processus était bloqué dans une file d'attente pour un quelconque élément système
    if(pid <= 0 || pid >= MAX_PROCESS || tableDesProcs[pid] == NULL ){
        return -1;
    }

    Processus * proc = tableDesProcs[pid];
    if (proc->etat == ZOMBIE || proc->etat == MOURANT){ // Pas de kill de zombie
        return -1;
    }

    // Gestion de l'elu avec une comparaison de pointeur (on sais jamais)
    if(proc == ProcElu){
        if (proc->pere != NULL){
            proc->etat = ZOMBIE;
        } else {
            proc->etat = MOURANT;
        }
        ordonnanceur();
    }
    else {
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
        free_mourant_queue();
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
    int retval = proc->retval;

    // On libère le processus enfant
    proc->etat = MOURANT;
    proc->pere = NULL;
    queue_add(proc, &proc_mourants, Processus, chainage, prio);
    free_mourant_queue();
    
    // On retourne les valeur du processus enfant trouvé !
    if (retvalp != NULL){
        *retvalp = retval;
    }
    return procPid;
}

