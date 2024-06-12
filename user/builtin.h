#ifndef BUILTIN_H
#define BUILTIN_H

extern int start(int (*pt_func)(void*), unsigned long ssize, int prio, const char *name, void *arg);
extern void exit(int retval);
extern int kill(int pid);

extern char *mon_nom(void);
extern int getpid(void);

extern int getprio(int pid);
extern int chprio(int pid, int newprio);

extern int pcount(int fid, int *count);
extern int pcreate(int fid);
extern int pdelete(int fid);
extern int preceive(int fid, int *msg);
extern int preset(int fid);
extern int psend(int fid, int msg);

extern void clock_settings(unsigned long *quartz, unsigned long *ticks);
extern unsigned long current_clock(void);

extern void wait_clock(unsigned long ticks);
extern int waitpid(int, int *);

extern void cons_echo(int);
extern void cons_read(char *, int);
extern void cons_write(char *, int);

#endif