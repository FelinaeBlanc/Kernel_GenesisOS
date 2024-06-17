#include "semaphore.h"
#include "queue.h"
#include "mem.h"
#include "processus.h"
#include "stdbool.h"

Semaphore * tableauSem[NBSEM];

LIST_HEAD(sid_liste);

// Ajoute un sid libre
void add_sid(int sid){
    SidLibre * new_sid = mem_alloc(sizeof(SidLibre));
    new_sid->sid = sid;
    queue_add(new_sid, &sid_liste, SidLibre, chainage, sid);
}

// Retourne un sid libre
int new_sid(){
    if (queue_empty(&sid_liste)){
        return -1;
    }
    
    SidLibre * sid = queue_out(&sid_liste, SidLibre, chainage);
    int sid_val = sid->sid;
    mem_free( sid, sizeof(SidLibre));

    return sid_val;
}

// initialise la liste des sid libre
void init_semaphores(){
    INIT_LIST_HEAD(&sid_liste);

    for (int i = 0; i < NBSEM; i++){
        add_sid(i);
    }
}

bool isSidValide(int sid){
    return (sid >= 0 && sid < NBSEM);
}
bool isSidValideAndExist(int sid){
    return isSidValide(sid) && tableauSem[sid] != NULL;
}

/*
La valeur de retour de scount est :
    -1 si la valeur de sem est invalide,
    sinon, les 16 bits de poids fort sont à 0, et les 16 bits de poids faible, interprétés comme un 
    entier signé sur 16 bits, sont la valeur du sémaphore.
*/
int scount(int sid){
    if (!isSidValideAndExist(sid)){
        return -1;
    }
    return tableauSem[sid]->count;
}
/*
La primitive screate alloue un sémaphore libre et l'initialise à la valeur count.
S'il n'y a plus de sémaphore disponible, ou si count est négatif, l'appel échoue et la valeur de retour 
est -1, sinon elle est égale a l'identification du sémaphore qui a été alloué.
*/
int screate(short int count){
    if (count < 0){ return -1; }

    int sid = new_sid();
    if (sid == -1){
        return -1;
    }

    Semaphore * new_sem = mem_alloc(sizeof(Semaphore));
    if (new_sem == NULL){
        add_sid(sid);
        return -1;
    }

    new_sem->sid = sid;
    new_sem->count = count;
    INIT_LIST_HEAD(&new_sem->queueAttente);

    tableauSem[sid] = new_sem;

    return sid;
}
/*
La primitive sdelete fait passer tous les processus (s'il y en a) de l'état bloqué sur sémaphore 
identifié par sem, à l'état activable,  et rend invalide comme identification de sémaphore 
la valeur sem.
Si la valeur de sem est invalide, la valeur de retour de sdelete est -1, sinon elle est nulle.
*/
int sdelete(int sid){
    if (!isSidValideAndExist(sid)){
        return -1;
    }

    Semaphore * sem = tableauSem[sid];
    while (!queue_empty(&sem->queueAttente)){
        Processus * proc = queue_out(&sem->queueAttente, Processus, chainage);
        proc->etat = ACTIVABLE;
        proc->operationStatus = -3;
        queue_add(proc, &proc_activables, Processus, chainage, prio);
    }

    mem_free(sem, sizeof(Semaphore));
    tableauSem[sid] = NULL;
    add_sid(sid);

    return 0;
}
/*
La primitive "sreset" fait passer, s'il en existe, tous les processus se trouvant dans l'état bloqué sur 
le sémaphore identifié par "sem" à l'état activable et associe la valeur de "count" à ce sémaphore.
Si la valeur de "sem" est invalide ou si la valeur de count est négative, la fonction échoue et la valeur de 
retour est -1, sinon elle est nulle.
*/
int sreset(int sid, short int count){
    if (!isSidValideAndExist(sid) || count < 0){
        return -1;
    }

    Semaphore * sem = tableauSem[sid];
    while (!queue_empty(&sem->queueAttente)){
        Processus * proc = queue_out(&sem->queueAttente, Processus, chainage);
        proc->etat = ACTIVABLE;
        proc->operationStatus = -4;
        queue_add(proc, &proc_activables, Processus, chainage, prio);
    }

    sem->count = count;
    return 0;
}

/*
La primitive signal incrémente de 1 la valeur du sémaphore identifié par sem et si la valeur après incrémentation 
est négative ou nulle, le premier processus se trouvant dans l'état bloqué dans la file d'attente du sémaphore 
passe dans l'état activable ou actif.
Dans les deux cas, si l'opération doit provoquer un dépassement de capacité du compteur, alors elle n'est pas effectuée.
La valeur de retour de signal/signaln est -1 si la valeur de sem est invalide, -2 en cas de dépassement de capacité 
et nulle sinon.
*/
int signal(int sid){
    if (!isSidValideAndExist(sid)){
        return -1;
    }

    Semaphore * sem = tableauSem[sid];
    // Vérifie dépassement de capacité

    if (sem->count > SHORTINT_MAX - 1){ return -2; }
    sem->count++;

    if (sem->count <= 0){
        Processus * proc = queue_out(&sem->queueAttente, Processus, chainage);
        proc->etat = ACTIVABLE;
        proc->operationStatus = true;
        queue_add(proc, &proc_activables, Processus, chainage, prio);
    }
    

    return 0;

}
/*
La primitive signaln est équivalente à count primitives signal exécutées de manière atomique, i.e. sans qu'un 
autre processus puisse s'exécuter entre deux appel à signal.
Dans les deux cas, si l'opération doit provoquer un dépassement de capacité du compteur, alors elle n'est pas effectuée.
La valeur de retour de signal/signaln est -1 si la valeur de sem est invalide, -2 en cas de dépassement de capacité 
et nulle sinon.
*/
int signaln(int sid, short int count){ // ATTENTION : atomicité, car pas multi-thread et on fait tout ou rien en même temps !
    if (!isSidValideAndExist(sid)){
        return -1;
    }

    Semaphore * sem = tableauSem[sid];
    // Vérifie dépassement de capacité
    if (sem->count > SHORTINT_MAX - count){ return -2; }

    for (int i = 0; i < count; i++){ // On a vérifier le dépassement avant, donc tout devrait réussir ! (+ Sid valide) => atomiciter
        signal(sid);
    }

    return 0;
}
/*
La primitive try_wait teste la valeur du sémaphore. Si la valeur est supérieure strictement à 0, 
alors la primitive décrémente de 1 la valeur du sémaphore identifié par sem. Si la valeur avant 
décrémentation est négative ou nulle, une erreur est retournée pour indiquer que l'opération P ne 
peut être appelée sans bloquer le processus appelant. Enfin, si l'opération doit provoquer un 
dépassement de capacité du compteur, alors elle n'est pas effectuée.
La valeur de retour de try_wait est -1 si la valeur de sem est invalide, -3 si le compteur est négatif 
ou nul, -2 en cas de dépassement de capacité, et nulle sinon.
*/
int try_wait(int sid){
    if (!isSidValideAndExist(sid)){
        return -1;
    }

    Semaphore * sem = tableauSem[sid];
    // Vérifie dépassement de capacité
    if (sem->count < SHORTINT_MIN + 1){ return -2; }

    if (sem->count <= 0){ // Vérifie si le sémaphore est bloqué
        return -3;
    }
    // sem->count > 0 !
    sem->count--;

    return 0;
}
/*
La primitive wait décrémente de 1 la valeur du sémaphore identifié par sem. 
Si l'opération doit provoquer un dépassement de capacité du compteur, alors elle n'est pas effectuée. 
Sinon, si la valeur après décrémentation est strictement négative, le processus passe de l'état actif 
à l'état bloqué sur le sémaphore identifié par la valeur sem. Ce processus pourra repasser dans l'état 
activable ou actif lorsqu'un autre processus exécutera une primitive signal/signaln qui le libèrera, 
ou les primitives sreset ou sdelete.
La valeur de retour de wait est -1 si la valeur de sem est invalide, -2 en cas de dépassement de capacité, 
-3 si le réveil est consécutif à sdelete, -4 s'il est consécutif à sreset, nulle sinon.
*/
int wait(int sid){
    if (!isSidValideAndExist(sid)){
        return -1;
    }

    Semaphore * sem = tableauSem[sid];
    // Vérifie dépassement de capacité
    if (sem->count < SHORTINT_MIN + 1){ return -2; }
    sem->count--;

    if (sem->count < 0){
        ProcElu->etat = ATTEND_SEMAPHORE;
        queue_add(ProcElu, &sem->queueAttente, Processus, chainage, place);
        ordonnanceur();
        
        int operationStatus = ProcElu->operationStatus;
        if (operationStatus < 0){ // -3 ou -4 si le réveil est consécutif à sdelete ou sreset
            return operationStatus;
        }
    }
    
    
    return 0;

}
