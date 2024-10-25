#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "stdint.h"
#include "stdbool.h"
/*
 * This is the function called by printf to send its output to the screen. You
 * have to implement it in the kernel and in the user program.
 */

#define LARGEUR 80
#define HAUTEUR 25
#define DEBUT_MEM 0xB8000
#define SELECT_PORT 0x3D4
#define RW_PORT 0x3D5

// Les couleurs de texte et de fond
#define NOIR 0x00
#define BLEU 0x01
#define VERT 0x02
#define CYAN 0x03
#define ROUGE 0x04
#define MAGENTA 0x05
#define MARRON 0x06
#define GRIS_CLAIR 0x07
#define GRIS_FONCE 0x08
#define BLEU_CLAIR 0x09
#define VERT_CLAIR 0x0A
#define CYAN_CLAIR 0x0B
#define ROUGE_CLAIR 0x0C
#define MAGENTA_CLAIR 0x0D
#define JAUNE 0x0E
#define BLANC 0x0F

#define TRUE 0x01
#define FALSE 0x00

extern int id_col_start;
extern uint16_t ligne, colonne;
extern bool echo;

extern uint16_t screen_buffer[HAUTEUR*100][LARGEUR];
extern uint16_t buff_ligne;
extern uint16_t buff_display_ligne;

#define HISTORY_SIZE 25
extern char * history[HISTORY_SIZE];
extern int index_history;
extern int current_idex_history;

extern void console_putbytes(const char *s, int len);

void set_id_start(int i);
void efface_ligne(void);
void avance_curseur(void);
extern void set_color(int c);
extern void traite_car(char c);
/* Si on est nul, désactive l'écho sur la console, sinon le réactive. */
extern void cons_echo(int on);
extern void cons_switch_echo();
extern void afficher_historique();

/* Envoie sur le terminal la suite de caractères de longueur size à l'adresse str. */
extern void cons_write(const char *str, long size);

/* Si length est nul, cette fonction retourne 0. Sinon, elle attend que l'utilisateur ait tapé une ligne complète */
extern int cons_read(char *string, unsigned long length);

extern void defillement_haut();
extern void defillement_bas();

#endif
