
// son numéro, qui servira d'identifiant unique dans le système, et que l'on appelle typiquement pid (pour Process IDentifier) ;
// son nom, une chaine de caractères qui servira à produire des traces lisibles ;
// son état : un processus peut être « élu » (c'est à dire que c'est lui que le processeur 
// est en train d'exécuter), « activable » (c'est à dire qu'il est prêt à être élu et qu'il attend que le
//  processeur se libère), « endormi » pour une durée donnée, etc. ;
// son contexte d'exécution sur le processeur : il s'agit en pratique d'une zone mémoire servant à sauvegarder le contenu des registres important du processeur lorsqu'on arrête le processus pour passer la main à un autre, et à restaurer ce contenu ensuite lorsque le processus reprend la main ;
// sa pile d'exécution : qui est l'espace mémoire dans lequel sont stockés notamment les variables locales, les paramètres passés aux fonctions, etc..
#include "stdint.h"

#define MAX_PROCESS 2
#define SIZE_PILE_EXEC 512


enum Etat { ELU, ACTIVABLE, ENDORMI };


struct Processus {
    uint32_t pid;
    char nom[25];
    enum Etat etat;
    int32_t contexte[5];
    int32_t pile[SIZE_PILE_EXEC];
};

extern struct Processus * tableDesProcs[MAX_PROCESS];
extern struct Processus * processusElu;


void ctx_sw(void* old_ctx, void* new_ctx);