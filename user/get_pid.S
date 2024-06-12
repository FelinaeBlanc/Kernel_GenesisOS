
.text
	.global	get_pid_asm
get_pid_asm:
    # sauvegarde des registres importants
    pushl %eax
    pushl %ebx
    pushl %edx
    pushl %ecx
    pushl %esi
    pushl %edi
    pushl %ebp

    // movl $4, %eax
    # appel a la fonction C realisant le traitant
    int $49
    # restauration des registres importants
    popl %ebp
    popl %edi
    popl %esi
    popl %ecx
    popl %edx
    popl %ebx
    popl %eax
    ret
