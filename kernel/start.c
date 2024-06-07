#include "debugger.h"
#include "cpu.h"
#include "stdio.h"
#include "processus.h"
#include "stdlib.h"
#include "string.h"
#include "horloge.h"
#include "files.h"
#include "test.h"

int idle() {
  // printf("JE SUIS ENTREE DANS IDLE\n");
  for (;;) {
    sti(); // usage de sti et cli avant la phase 5 (possible après)
    hlt(); // autre exception possible: le code de vos tests
    cli(); // JAMAIS de sti ou de cli dans le reste de votre kernel
  }
}

int proc_runner(){
  printf("proc runner !\n");
  test_run(4);
  printf("Proc runner fini!!!!!!!!!!!!!!!!!!!!!\n");
  return 0;
}
/*
int proc_files_a(void * arg){
  printf("Ok prend la main\n");
  int fid = pcreate(2);
  psend(fid, 1);
  psend(fid, 2);
  psend(fid, 3);
  psend(fid, 4);
  printf("REPREND LA MAIN ca marche ? fid:%d \n",fid);
  arg = arg;
  return 0;
}

int proc_files_b(void * arg){
  printf("Proc fils b");
  arg = arg;
  int msg;
  preceive(9,&msg);
  printf("1- preceive:%d\n",msg);

  preceive(9,&msg);
  printf("2- preceive:%d\n",msg);

    preceive(9,&msg);
  printf("3- preceive:%d\n",msg);

    preceive(9,&msg);
  printf("4- preceive:%d\n",msg);
  return 0;
}*/


void kernel_start(void)
{
  //void * arg =  (int *) 1;
  printf("\f");

  init_ordonnanceur(); // Init l'ordonnanceur


  start(&proc_runner, SIZE_PILE_EXEC, 128, "proc_runner", NULL);

  idle();
  
  return;
}
