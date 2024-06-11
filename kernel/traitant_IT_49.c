#include "builtin_enum.h"
#include "processus.h"
#include "files.h"
#include "horloge.h"
#include "console.h"

void traitant_IT_49() {
    int eax_val, ebx_val, ecx_val, edx_val, esi_val, edi_val;
    int retval;

    // Sauvegarder les valeurs des registres dans des variables locales
    __asm__ __volatile__ (
        "movl %%eax, %0\n"
        "movl %%ebx, %1\n"
        "movl %%ecx, %2\n"
        "movl %%edx, %3\n"
        "movl %%esi, %4\n"
        "movl %%edi, %5\n"
        : "=r"(eax_val), "=r"(ebx_val), "=r"(ecx_val), "=r"(edx_val), "=r"(esi_val), "=r"(edi_val)
    );


    switch (eax_val)
    {
    case START:
        // start(int (*pt_func)(void*), unsigned long ssize, int prio, const char *name, void *arg)
        retval = start( (int (*)(void*))ebx_val, (unsigned long)ecx_val, edx_val, (const char *)esi_val, (void *)edi_val);
        break;
    case EXIT:
        exit(ebx_val);
        break;
    case KILL:
        retval = kill(ebx_val);
        break;
    case MON_NOM:
        retval = (int)mon_nom();
        break;
    case GETPID:
        retval = getpid();
        break;
    case GETPRIO:
        retval = getprio(ebx_val);
        break;
    case CHPRIO:
        retval = chprio(ebx_val, ecx_val);
        break;
    case PCOUNT:
        retval = pcount(ebx_val, (int *)ecx_val );
        break;
    case PCREATE:
        retval = pcreate(ebx_val);
        break;
    case PDELETE:
        retval = pdelete(ebx_val);
        break;
    case PRECEIVE:
        retval = preceive(ebx_val, (int *)ecx_val);
        break;
    case PRESET:
        retval = preset(ebx_val);
        break;
    case PSEND:
        retval = psend(ebx_val, ecx_val);
        break;
    case CLOCK_SETTINGS:
        clock_settings((unsigned long *)ebx_val, (unsigned long *)ecx_val);
        break;
    case CURRENT_CLOCK:
        retval = (int)current_clock();
        break;
    case WAIT_CLOCK:
        wait_clock(ebx_val);
        break;
    case WAITPID:
        retval = waitpid(ebx_val, (int *)ecx_val);
        break;
    case CONS_ECHO:
        cons_echo(ebx_val);
        break;
    case CONS_READ:
        // retval = cons_read((char *)ebx_val, ecx_val);
        break;
    case CONS_WRITE:
        cons_write((char *)ebx_val, ecx_val);
        break;

    default:
        // ERREUR A FAIRE !!! PAS DE VALEUR CONNUE
        break;
    }

    __asm__ __volatile__ ("movl %0, %%eax\n" : : "r"(retval));

    __asm__ __volatile__ ("iret;");
}
