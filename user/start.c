#include "start.h"
#include "stdio.h"
#include "builtin.h"
#include "string.h"

int proc_a(void * a){
  int d = * (int *) (a);
    printf("proc_a [temps = %lu] processus pid = %i\n", current_clock(),
        getpid());
    printf("MON ARG EST %d\n", d);
    // wait_clock(1000);
    return 20202;
}

int proc_b(){
    printf("proc_b [temps = %lu] processus pid = %i\n", current_clock(),
        getpid());
    //kill(2);
    //printf("Normalement je passe pas ICI!!\n");
    while(1);
  return 0;
}

void user_start(void) {

    int a = 154545;
    start(&proc_a, 512, 128, "proc_a", (void *)(&a));
    start(&proc_b, 512, 128, "proc_b", NULL);

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
    
    while (1);
    
    return;
}
