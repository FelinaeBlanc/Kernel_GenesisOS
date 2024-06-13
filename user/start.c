#include "start.h"
#include "stdio.h"
#include "builtin.h"
#include "string.h"

int proc1 (){
  // le processus P1 imprime le caractère . à raison de un par seconde ; 
  while(1){
    printf("a\n");
    wait_clock(1*50);
  }
  return 0;
}

int proc2 (){
  // le processus P2 imprime le caractère - toutes les 2 secondes ;  
  while(1){
    printf("b\n");
    wait_clock(2*50);
  }
  return 0;
}

int proc3 (){
  // le processus P3 imprime le caractère + toutes les 5 secondes ; 
  while(1){
    printf("c\n");
    wait_clock(5*50);
  }
  return 0;
}
int proc4 (){
  // le processus P4 imprime le caractère * toutes les 10 secondes ;
  while(1){
    printf("d\n");
    wait_clock(5*50);
  }
  return 0;
}


int proc_principale(){
  
  //start(&proc1, SIZE_PILE_EXEC, 128, "proc_1", NULL);
  // le programme principal lance 4 processus P1 à P4 et exécute un wait_clock d'une minute ;

  int pid1 = start(&proc1, 512, 128, "proc_1", NULL);
  int pid2 = start(&proc2, 512, 128, "proc_2", NULL);
  int pid3 = start(&proc3, 512, 128, "proc_3", NULL);
  int pid4 = start(&proc4, 512, 128, "proc_4", NULL);
  // la périodicité est gérée par un appel à wait_clock dans tous les cas ;
  wait_clock(3000); // 60s *50

  // à son réveil, le processus principal tue ses fils et se termine.
  kill(pid1);
  kill(pid2);
  kill(pid3);
  kill(pid4);

  return 0;
}



void user_start(void) {

  start(&proc_principale, 512, 128,"proc_principale", NULL);
  // int a = 154545;
  // int b = start(&proc_a, 512, 128, "proc_a", (void *)(&a));
  // start(&proc_b, 512, 127, "proc_b", NULL);

  // printf("J'ai mon ret : %d\n", b);


  /*int monpid = getpid();
  printf("Mon pid est: %d ", monpid);
  int prio = getprio(monpid);
  printf(": prio => %d\n", prio);

  int ancienne_prio = chprio(monpid,10);
  printf("Ancienne prio: %d et nouvelle prio: %d\n", ancienne_prio, getprio(monpid) );
  */
  
  // printf("Attend 2 secondes...\n");
  // wait_clock(100*2);
  // printf("Reveille! \n");

  // unsigned long quartz;
  // unsigned long ticks;
  // clock_settings(&quartz, &ticks);
  // printf("CLOCK SETTINGS: %lu et %lu",quartz,ticks);
  
  // while (1);
  
  return;
}
