#include "builtin_enum.h"
#include "processus.h"
#include "files.h"
#include "horloge.h"
#include "console.h"

/******** Protections ********/
bool isUserSpace(int addr){
    return (addr == 0) || (addr >= USER_START && addr < USER_END);
}

int traitant_IT_49_switch(int eax_val, int ebx_val, int ecx_val, int edx_val, int esi_val, int edi_val) {
    int retval = 0;

    switch (eax_val) {
        case GETPID:
            retval = getpid();
            break;

        case CONS_PUTBYTES:
            if (!isUserSpace(ebx_val))
                return -1;
            console_putbytes((const char *)ebx_val, ecx_val);
            break;

        case START:
            // Vérification adresse fonction et chaine de caractères
            if (!isUserSpace(ebx_val) || !isUserSpace(esi_val))
                return -1;

            retval = start((int (*)(void *))ebx_val, ecx_val, edx_val, (const char *)esi_val, (void *)edi_val);
            break;

        case EXIT:
            exit(ebx_val);
            break;

        case KILL:
            retval = kill(ebx_val);
            break;

        case CHPRIO:
            retval = chprio(ebx_val, ecx_val);
            break;

        case GETPRIO:
            retval = getprio(ebx_val);
            break;

        case PCOUNT:
            if (!isUserSpace(ecx_val))
                return -1;
            retval = pcount(ebx_val, (int *)ecx_val);
            break;

        case PCREATE:
            retval = pcreate(ebx_val);
            break;

        case PDELETE:
            retval = pdelete(ebx_val);
            break;

        case PRECEIVE:
            if (!isUserSpace(ecx_val))
                return -1;
            retval = preceive(ebx_val, (int *)ecx_val);
            break;

        case PRESET:
            retval = preset(ebx_val);
            break;

        case PSEND:
            retval = psend(ebx_val, ecx_val);
            break;

        case CLOCK_SETTINGS:
            if (!isUserSpace(ebx_val) || !isUserSpace(ecx_val))
                return -1;
            clock_settings((unsigned long *)ebx_val, (unsigned long *)ecx_val);
            break;

        case CURRENT_CLOCK:
            retval = current_clock();
            break;

        case WAIT_CLOCK:
            wait_clock(ebx_val);
            break;

        case WAITPID:
            if (!isUserSpace(ecx_val))
                return -1;
            retval = waitpid(ebx_val, (int *)ecx_val);
            break;

        case CONS_ECHO:
            cons_echo(ebx_val);
            break;

        case CONS_READ:
            if (!isUserSpace(ebx_val))
                return -1;
            retval = cons_read((char *)ebx_val, ecx_val);
            break;

        case CONS_WRITE:
            if (!isUserSpace(ebx_val))
                return -1;
            cons_write((const char *)ebx_val, ecx_val);
            break;
        case EXIT_KERNEL:
            exit_kernel();
            break;
        case PS:
            affiche_table_process();
            break;
        default:
            // ERREUR A FAIRE !!! PAS DE VALEUR CONNUE
            break;
    }

    return retval;
}