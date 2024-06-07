    .globl exit_routine
# Réalise le return, et appel exit
exit_routine:
    # Push en argument %eax
    push %eax
    call exit