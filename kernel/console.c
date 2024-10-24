#include "cpu.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "console.h"
#include "segment.h"
#include "stdbool.h"
#include "kbd.h"
#include "processus.h"
#include "queue.h"
#include "sound.h"
#include "mem.h"

// variable pour l'affichage
uint16_t ligne, colonne;
bool echo = true;
int base_color = BLANC;
int id_col_start = 0;
// variable pour l'historique
char * history[HISTORY_SIZE];
int index_history = 0;
int current_idex_history = 0;

// variable pour le scroll
uint16_t screen_buffer[HAUTEUR*100][LARGEUR];
uint16_t buff_ligne = 0;
uint16_t buff_display_ligne = 0;

/************Fonction écriture console**************/
// revoie l'adresse mémoir de lig col
uint16_t *ptr_mem(uint32_t lig, uint32_t col)
{
    return (uint16_t*)(DEBUT_MEM + 2 * (lig * LARGEUR + col)) ;
}

void set_id_start(int i) {
    if (i<0 || i>LARGEUR) return;
    id_col_start = i;
}

uint16_t car_value(uint8_t ct, uint8_t cf, uint8_t cl, char c){
    uint8_t char_value = c;
    uint8_t attr_value = (cl << 7) | (cf << 4) | (ct);
    uint16_t value = (attr_value << 8) | char_value;
    return value;
}

// écrit le caratère c, a lig col, de couleur cl, fond cf
void ecrit_car(uint16_t lig, uint16_t col, uint8_t ct, uint8_t cf, uint8_t cl, char c) {
    uint16_t *p = ptr_mem(lig, col);
    *p = car_value(ct,cf,cl,c);
}

void ecrit_car_value(uint16_t lig, uint16_t col, uint16_t val){
    uint16_t *p = ptr_mem(lig, col);
    *p = val;
}

void set_color(int c){
    base_color = c;
}

void efface_ecran(void){
    for (uint32_t i=0; i<HAUTEUR; i++ ){
        for (uint32_t j=0; j<LARGEUR; j++){
            ecrit_car(i, j, NOIR, NOIR, FALSE, ' ');
        }
    }
    buff_display_ligne = buff_ligne;
}


void place_curseur(uint32_t lig, uint32_t col)
{
    outb(0x0F, SELECT_PORT);
    uint16_t pos = (uint16_t)(col + lig * LARGEUR);
    uint8_t pos_bas = (uint8_t)pos;
    outb(pos_bas, RW_PORT);

    outb(0x0E, SELECT_PORT);
    uint8_t pos_haut = (uint8_t)(pos >> 8);
    outb(pos_haut, RW_PORT);

    ligne = lig;
    colonne = col;
}

void efface_ligne(void){
    for (uint32_t j=id_col_start; j<LARGEUR; j++){
        ecrit_car(ligne, j, NOIR, NOIR, FALSE, ' ');
        screen_buffer[buff_ligne][j] = car_value(NOIR, NOIR, FALSE, ' ');
    }
    place_curseur(ligne, id_col_start);
}

void avance_curseur(void){
    int col = colonne +1;
    int lig = ligne;

    if (col% LARGEUR == 0) return;
    
    place_curseur(lig, col);
}

void defilement(void) {
    memmove(ptr_mem(0, 0), ptr_mem(1, 0), (HAUTEUR - 1) * LARGEUR * 2);

    for (uint32_t j = 0; j < LARGEUR; j++) {
        ecrit_car((HAUTEUR - 1), j, NOIR, NOIR, FALSE, ' ');
    }
    place_curseur((HAUTEUR - 1), 0);
}

void traite_car(char c) {
    uint32_t lig, col;

    switch (c)
    {
    case '\b': // BS 8
        if (colonne > id_col_start) { 
            place_curseur(ligne, colonne-1);
        } else {
            place_curseur(ligne, id_col_start);
        }
        break;
    case '\t': // HT 9
        if (colonne%8 == 0) colonne++;
        col = colonne + (8 - colonne)%8;
        lig = ligne;
        if (col < colonne) {
            lig = ligne +1;
            buff_ligne++;
        } 
        place_curseur(lig, col);
        break;
    case '\n': // LF 10
        place_curseur((ligne+1)%HAUTEUR, id_col_start);
        buff_ligne++;
        break;
    case '\f':
        efface_ecran();
        place_curseur(0,0);
        break;
    case '\r': // CR 13
        place_curseur(ligne, id_col_start);
        break;
    case '\x1b': // si c'est une flèche on écrit rien
        break;
    case 127: // DEL
        if (colonne > id_col_start) {
            ecrit_car(ligne, colonne - 1, NOIR, NOIR, FALSE, ' ');
            screen_buffer[buff_ligne][colonne-1] = car_value(NOIR, NOIR, FALSE, ' ');
            place_curseur(ligne, colonne - 1);
        }
        break;
    case '\a': // BEL 7
        beep();
        break;
    default:
        if(c<32){
            traite_car(94);
            c = 64 + c;
        }
        if (c <= 126 && c >= 32){
            ecrit_car(ligne, colonne, base_color, NOIR, FALSE, c);
            screen_buffer[buff_ligne][colonne] = car_value(base_color, NOIR, FALSE, c);
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

    if (ligne == HAUTEUR - 1) {
        defilement();
        buff_display_ligne++;
        ligne--;
    }
}



/*
 * This is the function called by printf to send its output to the screen. You
 * have to implement it in the kernel and in the user program.
 */
void console_putbytes(const char *s, int len) {
    for (int i = 0; i < len; i++) {
        traite_car(s[i]);
    }
}

/* Si on est nul, désactive l'écho sur la console, sinon le réactive.*/
void cons_echo(int on) {
    echo = on ? true : false;
}

void cons_switch_echo(){
    echo = (!echo);
}

void afficher_historique(){
    for(int i = 0; i<index_history; i++){
        printf("%s\n", history[i]);
    }
}

/* Envoie sur le terminal la suite de caractères de longueur size à l'adresse str. */
void cons_write(const char *str, long size) {
    for (long i = 0; i < size; i++) {
        traite_car(str[i]);
    }
}

/* cons_read prélève une ligne contenue dans le tampon associé au clavier pour la transférer à l'appelant.
 Si aucune ligne n'est disponible, l'appelant est bloqué jusqu'à la frappe du prochain caractère de fin de ligne. */
int cons_read(char *string, unsigned long length) {
    if (length == 0) return 0;

    while (!read && ptampon < (int)length) { // On attend de pouvoir lire
        ProcElu->etat = ATTEND_ES;
        queue_add(ProcElu, &proc_bloque_es, Processus, chainage, prio);
        ordonnanceur();
    }

    // Lecture des données depuis le tampon
    memset(string, 0, length); // On vide string avant de commencer
    read = false;
    int i = 0;
    while (i < (int)length && i < ptampon) {
        string[i] = tampon[i];
        i++;
    }

    // Gestion de l'historique des commandes
    if (length > 1) { // Ajouté pour éviter d'ajouter des commandes de longueur 1 à l'historique
        if (index_history == HISTORY_SIZE) {
            // Libérer la mémoire de la commande la plus ancienne
            mem_free(history[0], strlen(history[0])+1);
            // Décaler les entrées restantes vers le début
            for (int j = 1; j < HISTORY_SIZE; j++) {
                history[j - 1] = history[j];
            }
            index_history--;
        }

        // Allocation de mémoire pour la nouvelle commande
        history[index_history] = (char *)mem_alloc(strlen(string) + 1);
        strcpy(history[index_history], string);
        index_history++;
        current_idex_history = index_history;
    }

    int retval = i;

    // Décalage des données restantes dans le tampon
    if (i < ptampon) {
        memmove(tampon, tampon + i, ptampon - i);
        ptampon -= i;
    } else {
        ptampon = 0;
    }

    // Remplir le reste du tampon avec des zéros pour le vider
    memset(tampon + ptampon, 0, BUFFER_SIZE - ptampon);

    return retval;
}


void display_buff(){
    for(uint16_t i = buff_display_ligne; i <= buff_ligne; i++){
        for (uint16_t j = 0; j < LARGEUR; j++)
        {
            ecrit_car_value(i-buff_display_ligne, j, screen_buffer[i][j]);
        }
    }
}

void defillement_haut(){
    if(buff_display_ligne > 0) {
        buff_display_ligne--;
        display_buff();
    }
}

void defillement_bas() {
    if (buff_display_ligne + HAUTEUR <= buff_ligne +1 ) {
        buff_display_ligne++;
        display_buff();
        place_curseur(HAUTEUR-1, id_col_start);
    }
    efface_ligne();
}
