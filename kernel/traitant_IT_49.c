#include "builtin_enum.h"
#include "processus.h"
#include "files.h"
#include "horloge.h"
#include "console.h"

int traitant_IT_49_switch() {
    int eax_val;
    int retval = 0;

    // Sauvegarder les valeurs des registres dans des variables locales
    __asm__ __volatile__ (
        "movl %%eax, %0\n"
        : "=r"(eax_val)
    );

    switch (eax_val) {
        case GETPID:
            retval = getpid();
            break;

        case CONS_PUTBYTES: {
            int len;
            const char *s;
            __asm__ __volatile__ (
                "movl %%ebx, %0\n"
                "movl %%ecx, %1\n"
                : "=r"(s), "=r"(len)
            );
            console_putbytes(s, len);
            break;
        }

        case START: {
            int (*pt_func)(void *);
            unsigned long ssize;
            int prio;
            const char *name;
            void *arg;
            __asm__ __volatile__ (
                "movl %%ebx, %0\n"
                "movl %%ecx, %1\n"
                "movl %%edx, %2\n"
                "movl %%esi, %3\n"
                "movl %%edi, %4\n"
                : "=r"(pt_func), "=r"(ssize), "=r"(prio), "=r"(name), "=r"(arg)
            );
            retval = start(pt_func, ssize, prio, name, arg);
            break;
        }

        case EXIT: {
            int ret;
            __asm__ __volatile__ (
                "movl %%ebx, %0\n"
                : "=r"(ret)
            );
            exit(ret);
            break;
        }

        case KILL: {
            int pid2;
            __asm__ __volatile__ (
                "movl %%ebx, %0\n"
                : "=r"(pid2)
            );
            retval = kill(pid2);
            break;
        }

        case CHPRIO: {
            int pid1;
            int newprio;
            __asm__ __volatile__ (
                "movl %%ebx, %0\n"
                "movl %%ecx, %1\n"
                : "=r"(pid1), "=r"(newprio)
            );
            retval = chprio(pid1, newprio);
            break;
        }

        // case MON_NOM:
        //     retval = (int)mon_nom();
        //     break;

        case GETPRIO: {
            int pid3;
            __asm__ __volatile__ (
                "movl %%ebx, %0\n"
                : "=r"(pid3)
            ); 
            retval = getprio(pid3);
            break;
        }

        case PCOUNT: {
            int fid2;
            int *count1;
            __asm__ __volatile__ (
                "movl %%ebx, %0\n"
                "movl %%ecx, %1\n"
                : "=r"(fid2), "=r"(count1)
            );
            retval = pcount(fid2, count1);
            break;
        }

        case PCREATE: {
            int count;
            __asm__ __volatile__ (
                "movl %%ebx, %0\n"
                : "=r"(count)
            );
            retval = pcreate(count);
            break;
        }

        case PDELETE: {
            int fid1;
            __asm__ __volatile__ (
                "movl %%ebx, %0\n"
                : "=r"(fid1)
            );
            retval = pdelete(fid1);
            break;
        }

        case PRECEIVE: {
            int fid3;
            int *message;
            __asm__ __volatile__ (
                "movl %%ebx, %0\n"
                "movl %%ecx, %1\n"
                : "=r"(fid3), "=r"(message)
            );
            retval = preceive(fid3, message);
            break;
        }

        case PRESET: {
            int fid4;
            __asm__ __volatile__ (
                "movl %%ebx, %0\n"
                : "=r"(fid4)
            );
            retval = preset(fid4);
            break;
        }

        case PSEND: {
            int fid5;
            int message2;
            __asm__ __volatile__ (
                "movl %%ebx, %0\n"
                "movl %%ecx, %1\n"
                : "=r"(fid5), "=r"(message2)
            );
            retval = psend(fid5, message2);
            break;
        }

        case CLOCK_SETTINGS: {
            unsigned long *q;
            unsigned long *t;
            __asm__ __volatile__ (
                "movl %%ebx, %0\n"
                "movl %%ecx, %1\n"
                : "=r"(q), "=r"(t)
            );
            clock_settings(q, t);
            break;
        }

        case CURRENT_CLOCK:
            retval = current_clock();
            break;

        case WAIT_CLOCK: {
            unsigned long t;
            __asm__ __volatile__ (
                "movl %%ebx, %0\n"
                : "=r"(t)
            );
            wait_clock(t);
            break;
        }

        case WAITPID: {
            int pid;
            int *ret;
            __asm__ __volatile__ (
                "movl %%ebx, %0\n"
                "movl %%ecx, %1\n"
                : "=r"(pid), "=r"(ret)
            );
            retval = waitpid(pid, ret);
            break;
        }

        case CONS_ECHO: {
            int on;
            __asm__ __volatile__ (
                "movl %%ebx, %0\n"
                : "=r"(on)
            );
            cons_echo(on);
            break;
        }

        case CONS_READ:
            // char *string;
            // unsigned long length;
            // __asm__ __volatile__ (
            //     "movl %%ebx, %0\n"
            //     "movl %%ecx, %1\n"
            //     : "=r"(string), "=r"(length)
            // );
            // retval = cons_read(string, length);
            break;

        case CONS_WRITE: {
            const char *str;
            long size;
            __asm__ __volatile__ (
                "movl %%ebx, %0\n"
                "movl %%ecx, %1\n"
                : "=r"(str), "=r"(size)
            );
            cons_write(str, size);
            break;
        }

        default:
            // ERREUR A FAIRE !!! PAS DE VALEUR CONNUE
            break;
    }

    return retval;
}
