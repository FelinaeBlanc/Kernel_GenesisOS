#include "sound.h"
#include "horloge.h"
#include "stdint.h"
#include "cpu.h"
#include "processus.h"

// Fonction pour émettre un bip
void play_sound(unsigned int frequency) {
    unsigned int divisor = 1193180 / frequency;
    outb(0xB6, 0x43); // Commande pour configurer le PIT channel 2
    outb((uint8_t)(divisor & 0xFF), 0x42); // LSB
    outb((uint8_t)((divisor >> 8) & 0xFF), 0x42); // MSB

    // Activer le haut-parleur
    uint8_t tmp = inb(0x61);
    if (tmp != (tmp | 3)) {
        outb(tmp | 3, 0x61);
    }
}

// Arrête le BIP
void nosound() {
 	uint8_t tmp = inb(0x61) & 0xFC;
 	outb(tmp, 0x61);
}

void set_duration(int t){
    duration = current_clock() + t;
}

void beep() {
    play_sound(500); // Fréquence du son (en Hz)
    set_duration(10);
}

