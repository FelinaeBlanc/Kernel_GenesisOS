#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "stdint.h"
/*
 * This is the function called by printf to send its output to the screen. You
 * have to implement it in the kernel and in the user program.
 */

#define LARGEUR 80
#define HAUTEUR 25

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




extern void console_putbytes(const char *s, int len);

#endif
