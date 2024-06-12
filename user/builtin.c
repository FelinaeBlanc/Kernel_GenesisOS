#include "builtin.h"

void get_pid_asm(void);

int getpid(void) {
    get_pid_asm();
    return 0;
}
// int start(int (*pt_func)(void*), unsigned long ssize, int prio, const char *name, void *arg) {
//     // __asm__ _volatile_();
//     return 0;
// }

// void exit(int retval) {
//     return;
// }

// int kill(int pid) {
//     return 0;
// }
