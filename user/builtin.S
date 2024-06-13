#include "builtin_enum.h"
.text
    .global getpid, console_putbytes, start, exit, kill,\
        getprio, chprio, pcount, pcreate, pdelete, preceive, preset,\
        psend, clock_settings, current_clock, wait_clock, waitpid,\
        cons_echo, cons_read, cons_write

# Définition des macros
.macro save_registers
    pushl %ebx
    pushl %esi
    pushl %edi
    pushl %ebp

    # save all params, max 5
    movl 20(%esp), %ebx
    movl 24(%esp), %ecx
    movl 28(%esp), %edx
    movl 32(%esp), %esi
    movl 36(%esp), %edi

.endm

.macro restore_registers
    popl %ebp
    popl %edi
    popl %esi
    popl %ebx
.endm


getpid:
    save_registers
    movl $GETPID, %eax
    int $49
    restore_registers
    ret

console_putbytes:
    save_registers
    movl $CONS_PUTBYTES, %eax
    int $49
    restore_registers
    ret

start:
    save_registers
    movl $START, %eax
    int $49
    restore_registers
    ret

exit:
    save_registers
    movl $EXIT, %eax
    int $49
    restore_registers
    ret

kill:
    save_registers
    movl $KILL, %eax
    int $49
    restore_registers
    ret

getprio:
    save_registers
    movl $GETPRIO, %eax
    int $49
    restore_registers
    ret


chprio:
    save_registers
    movl $CHPRIO, %eax
    int $49
    restore_registers
    ret

pcount:
    save_registers
    movl $PCOUNT, %eax
    int $49
    restore_registers
    ret

pcreate:
    save_registers
    movl $PCREATE, %eax
    int $49
    restore_registers
    ret

pdelete:
    save_registers
    movl $PDELETE, %eax
    int $49
    restore_registers
    ret

preceive:
    save_registers
    movl $PRECEIVE, %eax
    int $49
    restore_registers
    ret

preset:
    save_registers
    movl $PRESET, %eax
    int $49
    restore_registers
    ret

psend:
    save_registers
    movl $PSEND, %eax
    int $49
    restore_registers
    ret

clock_settings:
    save_registers
    movl $CLOCK_SETTINGS, %eax
    int $49
    restore_registers
    ret

current_clock:
    save_registers
    movl $CURRENT_CLOCK, %eax
    int $49
    restore_registers
    ret

wait_clock:
    save_registers
    movl $WAIT_CLOCK, %eax
    int $49
    restore_registers
    ret

waitpid:
    save_registers
    movl $WAITPID, %eax
    int $49
    restore_registers
    ret
cons_echo:
    save_registers
    movl $CONS_ECHO, %eax
    int $49
    restore_registers
    ret
cons_read:
    save_registers
    movl $CONS_READ, %eax
    int $49
    restore_registers
    ret

cons_write:
    save_registers
    movl $CONS_WRITE, %eax
    int $49
    restore_registers
    ret

