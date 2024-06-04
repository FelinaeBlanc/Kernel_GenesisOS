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

uint32_t nbr_secondes();

#endif