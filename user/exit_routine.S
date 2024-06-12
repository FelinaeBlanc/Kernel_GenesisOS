    .section .exit_routine, "ax"
    .globl exit_routine

exit_routine:
    push %eax
    call exit