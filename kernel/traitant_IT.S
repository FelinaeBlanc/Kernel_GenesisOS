# cette directive sert a rendre l’etiquette publique
    .text
    .globl traitant_IT_32, traitant_IT_49, traitant_IT_33

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

traitant_IT_33:
    # sauvegarde des registres importants
    pushl %eax
    pushl %edx
    pushl %ecx
    # acquittement de l'interruption
    mov $0x20, %al
    outb %al,$0x20
    # scancode obtenu à passer à do_scancode
    inb $0x60,%al
    pushl %eax
    call do_scancode
    popl %eax

    # restauration des registres importants
    popl %ecx
    popl %edx
    popl %eax
    iret
    