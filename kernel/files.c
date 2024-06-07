#include "files.h"
#include "stdbool.h"
#include "queue.h"
#include "mem.h"

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
        new_fid(i);
    }
}



// La primitive pcreate alloue une file de capacité égale à la valeur de count
int pcreate(int count){
    File * f = mem_alloc(sizeof(File));
    int fid = new_fid();

    if(fid < 0){
        return -1;
    }

    f->fid = fid;
    f->vide = true;
    f->count = count;
    
    return 0;
}

// La primitive pdelete détruit la file identifiée par fid
int pdelete(int fid){
    File * f = tableauFile[fid];
    if(f == NULL){
        return -1;
    }

    if(!queue_empty(&f->queueAttente)){
        return -1;
    }

    tableauFile[fid] = NULL;
    add_fid(fid);
    mem_free(f, sizeof(File));
    return 0;
}