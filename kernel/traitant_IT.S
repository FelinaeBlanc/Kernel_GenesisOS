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
    call  traitant_IT_49_switch
    iret