#include "cpu.h"
#include "stdio.h"

#include "stdint.h"
#include "string.h"
#include "console.h"
#include "segment.h"
#include "stdbool.h"

uint16_t ligne, colonne;
bool echo = true; 

// revoie l'adresse mémoir de lig col
uint16_t *ptr_mem(uint32_t lig, uint32_t col)
{
    return (uint16_t*)(0xB8000 + 2 * (lig * LARGEUR + col)) ;
}


// écrit le caratère c, a lig col, de couleur cl, fond cf
void ecrit_car(uint16_t lig, uint16_t col, uint8_t ct, uint8_t cf, uint8_t cl, char c) {
    uint16_t *p = ptr_mem(lig, col);
    *p = 0;
    uint8_t char_value = c;
    uint8_t attr_value = (cl << 7) | (cf << 4) | (ct);
    uint16_t value = (attr_value << 8) | char_value;
    *p = value;
}

void efface_ecran(void){

    for (uint32_t i=0; i<15; i++ ){
        for (uint32_t j=0; j<LARGEUR; j++){
            ecrit_car(i, j, NOIR, NOIR, FALSE, ' ');
        }
    }
}

void place_curseur(uint32_t lig, uint32_t col)
{
    outb(0x0F, 0x3D4);
    uint16_t pos = (uint16_t)(col + lig * LARGEUR);
    uint8_t pos_bas = (uint8_t)pos;
    outb(pos_bas, 0x3D5);

    outb(0x0E, 0x3D4);
    uint8_t pos_haut = (uint8_t)(pos >> 8);
    outb(pos_haut, 0x3D5);

    ligne = lig;
    colonne = col;
}

void traite_car(char c, uint8_t ct ){
    uint32_t lig, col;

    if(!echo) return;

    switch (c)
    {
    case '\b':
        if (colonne >0) 
        place_curseur(ligne, colonne-1);
        else {
            place_curseur(ligne, 0);
        }
        break;

    case '\t':
        if (colonne%8 == 0) colonne++;
        col = colonne + (8 - colonne)%8;
        lig = ligne;
        if (col < colonne) {
            lig = ligne +1;
        } 
        place_curseur(lig, col);
        break;
    case '\n':
        place_curseur((ligne+1)%HAUTEUR, 0);
        break;
    case '\f':
        efface_ecran();
        place_curseur(0,0);
        break;
    case '\r':
        place_curseur(ligne, 0);
        break;
    
    default:
        if (c <= 126 && c >= 32){
            ecrit_car(ligne, colonne, ct, NOIR, FALSE, c);
            col = colonne +1;
            lig = ligne;

            if (col% LARGEUR == 0) {
                lig = (lig +1)%HAUTEUR ;
                col = 0;
            }
            place_curseur(lig, col);
        }
        break;
    }
}


void defilement(void){

    memmove(ptr_mem(0,0), ptr_mem(1, 0), (HAUTEUR-1)*LARGEUR*2);


    for(uint32_t j=0; j<LARGEUR; j++){
        ecrit_car((HAUTEUR-1), j, NOIR, NOIR, FALSE, ' ');
    }
    place_curseur((HAUTEUR-1), 0);
}

/*
 * This is the function called by printf to send its output to the screen. You
 * have to implement it in the kernel and in the user program.
 */
void console_putbytes(const char *s, int len){
    int i=0;
    int j=0;

    while(i*HAUTEUR + j < len){
        traite_car(s[i*HAUTEUR + j], BLANC);
        j++;
        if (j>=HAUTEUR){
            j = j%HAUTEUR;
            i++;
        }
        if(ligne == HAUTEUR-1){
            defilement();
            ligne --;
        }
    }
   
}

/* Si on est nul, désactive l'écho sur la console, sinon le réactive.*/
void cons_echo(int on){
    if(on == 0){
        echo = false;
    }
    else {
        echo = true;
    }
}

/* Envoie sur le terminal la suite de caractères de longueur size à l'adresse str. */
void cons_write(const char *str, long size){
    int i=0;
    int j=0;

    while(i*HAUTEUR + j < size){
        traite_car(str[i*HAUTEUR + j], BLANC);
        j++;
        if (j>=HAUTEUR){
            j = j%HAUTEUR;
            i++;
        }
        if(ligne == HAUTEUR-1){
            defilement();
            ligne --;
        }
    }
}

