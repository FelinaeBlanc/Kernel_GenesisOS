# cette directive sert a rendre l’etiquette publique
    .text
    .globl traitant_IT_32, traitant_IT_49

# debut du traitant
traitant_IT_32:
    # sauvegarde des registres importants
    pushl %eax
    pushl %edx
    pushl %ecx
    # appel a la fonction C realisant le traitant
    call tic_PIT
    # restauration des registres importants
    popl %ecx
    popl %edx
    popl %eax
    # fin du traitant
    iret
    
traitant_IT_49:
    pushl %ebx
    pushl %ecx
    pushl %edx
    pushl %esi
    pushl %edi

    # Passer les registres en tant qu'arguments à la fonction
    pushl %edi
    pushl %esi
    pushl %edx
    pushl %ecx
    pushl %ebx
    pushl %eax  # eax est encore utilisé comme argument mais non sauvegardé/restauré

    call traitant_IT_49_switch
    addl $24, %esp  # Nettoyer les arguments de la pile (6 * 4 octets)

    popl %edi
    popl %esi
    popl %edx
    popl %ecx
    popl %ebx

    iret