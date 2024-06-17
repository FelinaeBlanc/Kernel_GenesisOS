#ifndef SEMPAPHORE_H_
#define SEMPAPHORE_H_

#include "queue.h"

#define SHORTINT_MAX ((short int)0b0111111111111111)
#define SHORTINT_MIN ((short int)0b1000000000000000)
#define NBSEM ((short int) 10)

typedef struct _SidLibre {
    short int sid;
    link chainage;
} SidLibre;

typedef struct  _Semaphore {
    short int sid;
    short int count;
    struct list_link queueAttente;
} Semaphore;

extern Semaphore * tableauSem[NBSEM];

extern int scount(int sid);
extern int screate(short int count);
extern int sdelete(int sid);
extern int sreset(int sid, short int count);
extern int signal(int sid);
extern int signaln(int sid, short int count);
extern int try_wait(int sid);
extern int wait(int sid);

extern void init_semaphores(void);

#endif