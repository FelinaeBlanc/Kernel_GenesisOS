#ifndef FILES_H
#define FILES_H

#include "processus.h"
#include "queue.h"
#include "stdbool.h"

#define NBQUEUE 10

typedef struct _FidLibre {
    int fid;
    link chainage;
} FidLibre;

typedef struct _Message {
    int message;
    link chainage;
    int place; // place dans la file (ordre decroissant)
} Message;
typedef struct _File {
    int fid;
    int maxMsg;

    int nbMsg;
    int nbProc;

    struct list_link queueAttente;
    struct list_link queueMsg;
} File;


File * tableauFile[NBQUEUE];

/**

    pcreate : crée une file de messages
    pdelete : détruit une file de messages
    psend : dépose un message dans une file
    preceive : retire un message d'une file
    preset : réinitialise une file
    pcount : renvoie l'état courant d'une file

**/
void init_files(void);

// crée une file de messages
int pcreate(int count);

// détruit une file de messages
int pdelete(int fid);


// La primitive psend envoie le message dans la file identifiée par fid
int psend(int fid, int message);

// retire un message d'une file
int preceive(int fid,int *message);

// réinitialise une file
int preset(int fid);

// La primitive pcount lit la quantité de données et de processus en attente sur la file fid.
int pcount(int fid, int *count);

bool isFidValideAndExist(int fid);

#endif
