#ifndef __HORLOGE_H__
#define __HORLOGE_H__

#include "stdint.h"

#define QUARTZ 0x1234DD
#define CLOCKFREQ 100
#define SCHEDFREQ 50
 
void ecrit_car(uint16_t lig, uint16_t col, uint8_t ct, uint8_t cf, uint8_t cl, char c);

void reg_frequence();

void traitant_IT_32();

void sign_clock(void);

void init_traitant_IT(uint32_t num_IT, void (*traitant)(void));

void affiche_date();

//Retourne le nombre d'interruptions d'horloge depuis le démarrage du noyau.
unsigned long current_clock();

//Retourne dans *quartz la fréquence du quartz du système et dans *ticks 
//le nombre d'oscillations du quartz entre chaque interruption.
void clock_settings(unsigned long *quartz, unsigned long *ticks);


#endif