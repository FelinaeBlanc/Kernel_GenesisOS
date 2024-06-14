#include "start.h"
#include "stdio.h"
#include "builtin.h"
#include "string.h"
#include "test.h"

#define CMD_SIZE 25

int superShell(void*){
  
  while (1){
    char str[CMD_SIZE];
    printf("$ ");
    cons_read(str, CMD_SIZE);
    if (strcmp(str, "exit") == 0)
      break;
    
    else if (strcmp(str, "help")==0){
      printf("exit:        sortie du noyau\n");
      printf("ps  :        informations sur les processus existants\n");
      
    }

  }

  return 0;
}

int proc_runner(){
  // printf("proc runner !\n");
  //test_until(20);
  //test_run(17);
  return 0;
}


void printWelcomeMessage() {
    printf("**********************************************\n");
    printf("*                                            *\n");
    printf("*            Welcome to GenesisOS            *\n");
    printf("*                                            *\n");
    printf("**********************************************\n");
    printf("*                                            *\n");
    printf("*      Your journey to the stars begins!     *\n");
    printf("*                                            *\n");
    printf("**********************************************\n");
    printf("\n");
    printf("Kernel Version: 1.0.0\n");
    printf("\n");
    printf("Type 'help' for a list of commands.\n");
    printf("\n");
}


void user_start(void) {
  printWelcomeMessage(NULL);
  //start(&proc_runner, 4000, 128, NULL, NULL);
  start(&superShell, 4000, 1, NULL, NULL);

  return;
}
