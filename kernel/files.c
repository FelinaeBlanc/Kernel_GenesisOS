#include "processus.h"
#include "files.h"
#include "stdbool.h"
#include "queue.h"
#include "horloge.h"
#include "mem.h"

File * tableauFile[NBQUEUE];

LIST_HEAD(fid_liste);

// Ajoute un fid libre
void add_fid(int fid){
    FidLibre * new_fid = mem_alloc(sizeof(FidLibre));
    new_fid->fid = fid;
    queue_add(new_fid, &fid_liste, FidLibre, chainage, fid);
}

// Retourne un fid libre
int new_fid(){
    if (queue_empty(&fid_liste)){
        return -1;
    }
    
    FidLibre * fid = queue_out(&fid_liste, FidLibre, chainage);
    int fid_val = fid->fid;
    mem_free( fid, sizeof(FidLibre));

    return fid_val;
}

// initialise la liste des fid libre
void init_files(){
    INIT_LIST_HEAD(&fid_liste);

    for (int i = 0; i < NBQUEUE; i++){
        add_fid(i);
    }
}

int getNbMsg(File * f){
    int cpt = 0;
    
    Message *current;
    queue_for_each(current, &f->queueMsg, Message, chainage) {
        cpt++;
    }
    return cpt;
}

int getNbProcs(File * f){
    int cpt = 0;

    Processus *current;
    queue_for_each(current, &f->queueAttente, Processus, chainage) {
        cpt++;
    }
    return cpt;
}

void add_msg(File * f, int message){
    assert(getNbMsg(f) < f->maxMsg);

    Message * new_msg = mem_alloc(sizeof(Message));
    new_msg->message = message;
    queue_add(new_msg, &f->queueMsg, Message, chainage, place); // FIFO

}

int retire_msg(File * f){

    Message * msg = queue_out(&f->queueMsg, Message, chainage);
    int message = msg->message;
    mem_free(msg, sizeof(Message));

    return message;
}

Processus * retire_proc(File * f){
    assert(getNbProcs(f) > 0);
    Processus * proc = queue_out(&f->queueAttente, Processus, chainage);

    return proc;
}

// Ajoute un processus à la file d'attente
void add_proc(File * f, Processus * proc){
    queue_add(proc, &f->queueAttente, Processus, chainage, prio);
    proc->fid = f->fid;
    proc->etat = ATTEND_FILE;
}

bool isFidValide(int fid){
    return (fid >= 0 && fid < NBQUEUE);
}
bool isFidValideAndExist(int fid){
    return isFidValide(fid) && tableauFile[fid] != NULL;
}


/*
-La primitive pcount lit la quantité de données et de processus en attente sur la file fid. 
Si count n'est pas nul, elle y place une valeur négative égale à l'opposé du nombre 
de processus bloqués sur file vide ou une valeur positive égale à la somme du nombre 
de messages dans la file et du nombre de processus bloqués sur file pleine.
-Si la valeur de fid est invalide, pcount signale l'erreur en retournant un code de 
retour négatif, sinon 0.*/
int pcount(int fid, int *count){
    if (!isFidValideAndExist(fid)){ return -1; }
    File * f = tableauFile[fid];
    
    if (count != NULL){
        int nbMsg = getNbMsg(f);
        int nbProc = getNbProcs(f);
        
        if (nbMsg == 0){
            *count = -(nbProc);
        } else {
            *count = nbMsg + nbProc;
        }
    } else {
        return -1;
    }
    
    return 0;
}


// La primitive pcreate alloue une file de capacité égale à la valeur de count
int pcreate(int maxMsg){

    //  si la valeur de count est négative ou nulle l
    if (maxMsg <= 0 || maxMsg > MAXMSG){ return -1; }

    int fid = new_fid();
    if (!isFidValide(fid)){ return -1;}

    File * f = mem_alloc(sizeof(File));
    f->fid = fid;
    f->maxMsg = maxMsg;

    INIT_LIST_HEAD(&f->queueAttente);
    INIT_LIST_HEAD(&f->queueMsg);
    tableauFile[fid] = f;
    
    return fid;
}


// La primitive pdelete détruit la file identifiée par fid
int pdelete(int fid){
    if (!isFidValideAndExist(fid)){ return -1; }
    File * f = tableauFile[fid];

    bool callOrdonnanceur = false;
    int procEluPrio = ProcElu->prio;
    
    // Remet les processus en attente dans l'état activable
    Processus * proc;
    while ((proc = queue_out(&f->queueAttente, Processus, chainage)) != NULL){
        proc->etat = ACTIVABLE;
        proc->fid = -1;
        proc->operationStatus = false; // N'a pas réussis l'opération
        queue_add(proc, &proc_activables, Processus, chainage, prio);

        if (proc->prio > procEluPrio){
            callOrdonnanceur = true;
        }
    }
    // Libère les messages de la file
    Message * msg;
    while ((msg = queue_out(&f->queueMsg, Message, chainage)) != NULL){
        mem_free(msg, sizeof(Message));
    }

    // Supprime la file de la mémoire
    tableauFile[fid] = NULL;
    add_fid(fid);
    mem_free(f, sizeof(File));

    if (callOrdonnanceur){ // Si un processus de plus haute priorité a été ajouté
        ordonnanceur();
    }

    return 0;
}


/*
La primitive preset vide la file identifiée par la valeur de fid et fait passer dans 
l'état activable ou actif (selon les priorités) tous les processus, s'il en existe, 
se trouvant dans l'état bloqué sur file pleine ou dans l'état bloqué 
sur file vide (ces processus auront une valeur strictement négative comme 
valeur de retour de psend ou preceive). Les messages se trouvant dans la file sont abandonnés.
Si la valeur de fid est invalide, la valeur de retour de preset est strictement 
négative sinon elle est nulle.
*/
int preset(int fid){
    if (!isFidValideAndExist(fid)){ return -1; }
    File * f = tableauFile[fid];

    bool callOrdonnanceur = false;
    int procEluPrio = ProcElu->prio;
    
    // Remet les processus en attente dans l'état activable
    Processus * proc;
    while ((proc = queue_out(&f->queueAttente, Processus, chainage)) != NULL){
        proc->etat = ACTIVABLE;
        proc->fid = -1;
        proc->operationStatus = false; // N'a pas réussis l'opération
        queue_add(proc, &proc_activables, Processus, chainage, prio);

        if (proc->prio > procEluPrio){
            callOrdonnanceur = true;
        }
    }
    // Libère les messages de la file
    Message * msg;
    while ((msg = queue_out(&f->queueMsg, Message, chainage)) != NULL){
        mem_free(msg, sizeof(Message));
    }

    if (callOrdonnanceur){ // Si un processus de plus haute priorité a été ajouté
        ordonnanceur();
    }

    return 0;
}

/*
La primitive psend envoie le message dans la file identifiée par fid. 
On distingue trois cas :

Il est possible également, qu'après avoir été mis dans l'état bloqué sur file pleine, 
le processus soit remis dans l'état activable par un autre processus ayant exécuté 
preset ou pdelete. Dans ce cas, la valeur de retour de psend est strictement négative.
*/
// La primitive psend envoie le message dans la file identifiée par fid
int psend(int fid, int message){
    if (!isFidValideAndExist(fid)){ return -1; }
    File * f = tableauFile[fid];

    bool callOrdonnanceur = false;
    int procEluPrio = ProcElu->prio;
    // Sauvegarde des valeurs
    ProcElu->fileValue = message; 
    ProcElu->operationStatus = true; // pour savoir si il a été débloqué par un pdelete ou preset

    /*-Si la file est vide et que des processus sont bloqués en attente de message, 
    alors le processus le plus ancien dans la file parmi les plus prioritaires est 
    débloqué et reçoit ce message.*/
    if (getNbMsg(f) == 0 && !queue_empty(&f->queueAttente)){
        Processus * proc = retire_proc(f);
        proc->fid = -1;
        proc->fileValue = message;
        proc->operationStatus = true;

        proc->etat = ACTIVABLE;
        queue_add(proc, &proc_activables, Processus, chainage, prio);

        if (proc->prio > procEluPrio){
            callOrdonnanceur = true;
        }
    } else if (f->maxMsg == getNbMsg(f)){ // file pleine
    /*--Si la file est pleine, le processus appelant passe dans l'état bloqué sur 
    file pleine jusqu'à ce qu'une place soit disponible dans la file pour y mettre 
    le message.*/
        // On attend que la file se libère
        add_proc(f, ProcElu); // Ajoute le processus à la file d'attente
        ordonnanceur();
        // la file s'est libérée... On vérifie si l'opération a réussi
        if (!ProcElu->operationStatus){ // L'opération a échouer (pdelete ou preset a été appelé)
            return -1;
        }
    } else {
        /*--Sinon, la file n'est pas pleine et aucun processus n'est bloqué en attente 
        de message. Le message est alors déposé directement dans la file.*/
        add_msg(f, message);
    }
    if (callOrdonnanceur){ // Si un processus de plus haute priorité a été ajouté
        ordonnanceur();
    }

    return 0;
}

/*
La primitive preceive lit et enlève le premier (plus ancien) message de la file fid. 
Le message lu est placé dans *message si message n'est pas nul, sinon il est oublié.

Lorsque la file était pleine, on la complète immédiatement avec le message du processus 
bloqué en émission le plus ancien parmi les plus prioritaires, s'il en existe 
un (processus bloqué). Ce processus devient alors activable ou actif selon sa priorité.

L'obtention du premier message de la file peut nécessiter le passage dans l'état bloqué 
sur file vide jusqu'à ce qu'un autre processus exécute une primitive psend.

Il est possible également, qu'après avoir été mis dans l'état bloqué sur file vide, 
le processus soit remis dans l'état activable par un autre processus ayant exécuté 
preset ou pdelete. Dans ce cas, la valeur de retour de preceive est strictement négative.

Si la valeur de fid est invalide, la valeur de retour de preceive est strictement négative, 
sinon elle est nulle.

Un processus bloqué sur file vide et dont la priorité est changée par chprio, 
est considéré comme le dernier processus (le plus jeune) de sa nouvelle priorité.
*/

int preceive(int fid,int *message){
    /*Si la valeur de fid est invalide, la valeur de retour de preceive est strictement négative, 
        sinon elle est nulle. */
    if (!isFidValideAndExist(fid)){ return -1; }
    File * f = tableauFile[fid];

    bool callOrdonnanceur = false;
    int procEluPrio = ProcElu->prio;
    // Sauvegarde des valeurs
    ProcElu->fileValue = -1; // Sera set par la suite 
    ProcElu->operationStatus = true; // pour savoir si il a été débloqué par un pdelete ou preset

    if (getNbMsg(f) == 0){ // file vide
    /*
    L'obtention du premier message de la file peut nécessiter le passage dans l'état 
    bloqué sur file vide jusqu'à ce qu'un autre processus exécute une primitive psend.
    */
        // On attend que la file se remplisse
        add_proc(f, ProcElu); // Ajoute le processus à la file d'attente
        ordonnanceur();
        /*
        Il est possible également, qu'après avoir été mis dans l'état bloqué sur file vide, 
        le processus soit remis dans l'état activable par un autre processus ayant exécuté 
        preset ou pdelete. 
        Dans ce cas, la valeur de retour de preceive est strictement négative.
        */
        // L'opération a terminé... On vérifie si l'opération a réussi
        if (!ProcElu->operationStatus){ // L'opération a échouer (pdelete ou preset a été appelé)
            return -1;
        }

        if (message != NULL){
            *message = ProcElu->fileValue;
        }
    } else {
        /*
        La primitive preceive lit et enlève le premier (plus ancien) message de la file fid. 
        Le message lu est placé dans *message si message n'est pas nul, sinon il est oublié.
        */

        int val = retire_msg(f);
        if (message != NULL){
            *message = val;
        }

        if (!queue_empty(&f->queueAttente)){
            /*
            Lorsque la file était pleine, on la complète immédiatement avec le message du processus 
            bloqué en émission le plus ancien parmi les plus prioritaires, s'il en existe 
            un (processus bloqué). Ce processus devient alors activable ou actif selon sa priorité.
            */
            Processus * proc = retire_proc(f);
            proc->fid = -1;
            add_msg(f, proc->fileValue);
            proc->operationStatus = true;

            proc->etat = ACTIVABLE;
            queue_add(proc, &proc_activables, Processus, chainage, prio);

            if (proc->prio > procEluPrio){
                callOrdonnanceur = true;
            }
        }
    }

    if (callOrdonnanceur){ // Si un processus de plus haute priorité a été ajouté
        ordonnanceur();
    }

    return 0;
}









