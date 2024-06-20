#ifndef BUILTIN_H
#define BUILTIN_H

#define NOIR 0x00
#define BLEU 0x01
#define VERT 0x02
#define CYAN 0x03
#define ROUGE 0x04
#define MAGENTA 0x05
#define MARRON 0x06
#define GRIS_CLAIR 0x07
#define GRIS_FONCE 0x08
#define BLEU_CLAIR 0x09
#define VERT_CLAIR 0x0A
#define CYAN_CLAIR 0x0B
#define ROUGE_CLAIR 0x0C
#define MAGENTA_CLAIR 0x0D
#define JAUNE 0x0E
#define BLANC 0x0F

extern int start(int (*pt_func)(void*), unsigned long ssize, int prio, const char *name, void *arg);
extern void exit_kernel(void);
extern void exit(int retval);
extern int kill(int pid);

extern void affiche_table_process();
extern void set_color(int c);
extern void set_id_start(int i);

//extern char *mon_nom(void);
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
extern int cons_read(char *, int);
extern void cons_write(char *, int);

extern void console_putbytes(const char *s, int len);

/**semaphore**/
extern int scount(int sid);
extern int screate(short int count);
extern int sdelete(int sid);
extern int sreset(int sid, short int count);
extern int signal(int sid);
extern int signaln(int sid, short int count);
extern int try_wait(int sid);
extern int wait(int sid);

extern void play_sound(unsigned int frequency, int t);
extern void cons_switch_echo();
extern void afficher_historique();
extern int add_shell(int pid, int num);
extern void set_console(int num);

#endif