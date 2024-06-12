
.text
	.global	getpid, console_putbytes
getpid:
    # sauvegarde des registres importants

    pushl %ebx
    pushl %edx
    pushl %ecx
    pushl %esi
    pushl %edi
    pushl %ebp

    movl $0, %eax
    # appel a la fonction C realisant le traitant
    int $49
    # restauration des registres importants
    popl %ebp
    popl %edi
    popl %esi
    popl %ecx
    popl %edx
    popl %ebx

    ret

console_putbytes:
    # sauvegarde des registres importants
    pushl %ebx
    pushl %edx
    pushl %ecx
    pushl %esi
    pushl %edi
    pushl %ebp

    movl $1, %eax
    # appel a la fonction C realisant le traitant
    int $49
    # restauration des registres importants
    popl %ebp
    popl %edi
    popl %esi
    popl %ecx
    popl %edx
    popl %ebx

    ret