#include "cpu.h"
#include "stdio.h"

#include "stdint.h"
#include "string.h"
#include "console.h"
#include "segment.h"
#include "console.h"
#include "horloge.h"
#include "stdbool.h"

void ecrit_temps(char *s, int len){

    for(int i=0; i<len; i++){
        ecrit_car(0, LARGEUR-len +i, BLANC, NOIR, FALSE, s[i]);
    }
}

uint8_t lit_CMOS(uint8_t reg){
    outb(0x80 | reg, 0x70);
    uint8_t hex = inb(0x71);
    uint8_t dec = ((hex & 0xF0) >> 4) * 10 + (hex & 0x0F);
    return dec;
}

uint16_t nb_tic = 0;
uint16_t nb_sec = 0;

uint32_t secondes = 0;
uint32_t minutes = 0;
uint32_t heures = 0;
void tic_PIT(void) {
    outb(0x20, 0x20);  // Accusé de réception de l'interruption
    nb_tic++;

    if (nb_tic == 50) {
        nb_tic = 0;
        secondes++;

        if (secondes == 60) {
            secondes = 0;
            minutes++;
        }

        if (minutes == 60) {
            minutes = 0;
            heures++;
        }

        char timeString[LARGEUR];
        sprintf(timeString, "%02d:%02d:%02d", heures, minutes, secondes);
        ecrit_temps(timeString, 8);
    }
}


void traitant_IT_32();

// Ajoute le traitant et l'ajoute au numéro spécifique dans
// la table des vecteurs d'interrutpion
void init_traitant_IT(uint32_t num_IT, void (*traitant)(void)){
    uint32_t* tab_addr = (uint32_t*)0x1000;
    uint32_t res = (uint32_t)traitant;

    tab_addr[num_IT*2] = (res & 0x0000FFFF) | (KERNEL_CS << 16);
    tab_addr[num_IT*2+1] = (res & 0xFFFF0000) | 0x8E00;
}

// Règle la fréquence des signaux
void reg_frequence(void)
{
    uint16_t frequence = QUARTZ / CLOCKFREQ;

    outb(0x34,0x43);
    outb((uint8_t)(frequence & 0xFF), 0x40);
    outb((uint8_t)((frequence & 0xFF00) >> 8), 0x40);
}


// Prend  en paramètre le numéro de l'IRQ (entre 0 et 7) à gérer 
// ainsi qu'un boolean pour masquer / démasquer l'IRQ en question
void masque_IRQ(uint32_t num_IRQ, bool masque){
    outb((inb(0x21) & ~(1 << num_IRQ)) | (masque << num_IRQ), 0x21);
}

// Active l'écoute de l'IRQ 0 A.K.A l'interruption de l'horloge programmable
void sign_clock(void)
{
    masque_IRQ(0 , false);
}

void ecrit_date(char *chaine){
    for(size_t i=0; i<strlen(chaine); i++){
        ecrit_car(0, i,  BLANC, NOIR, FALSE, chaine[i]);
    }
}

void affiche_date(){
    char timeString[80];
    uint8_t s = lit_CMOS(0);
    uint8_t m = lit_CMOS(2);
    uint8_t h = lit_CMOS(4);
    sprintf(timeString,"%02d:%02d:%02d", h,m,s);
    ecrit_temps(timeString, 8);

    char dateString[80];
    uint8_t j = lit_CMOS(7);
    uint8_t mo = lit_CMOS(8);
    uint8_t a = lit_CMOS(9);
    sprintf(dateString,"%02d/%02d/%2d", j,mo,a);
    ecrit_date(dateString);

}