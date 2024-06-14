#include "start.h"
#include "stdio.h"
#include "builtin.h"
#include "string.h"
#include "test.h"

int proc_runner(){
  // printf("proc runner !\n");
  //test_until(20);
  test_run(19);
  printf("Proc runner fini!!!!!!!!!!!!!!!!!!!!!\n");
  return 0;
}

void user_start(void) {
  //char str[5];
  start(&proc_runner, 4000, 128, "proc_runner", NULL);

  //cons_read(str, 5);

  // printf("%s\n",str);
  return;
}
