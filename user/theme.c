#include "theme.h"
#include "sound_enum.h"
#include "builtin.h"
#include "stdio.h"
#include "utils.h"

MelodyNote happy_birthday[] = {
    {G4, 500}, {G4, 500}, {A4, 1000}, {G4, 1000}, {C5, 1000}, {B4, 2000},
    {G4, 500}, {G4, 500}, {A4, 1000}, {G4, 1000}, {D5, 1000}, {C5, 2000},
    {G4, 500}, {G4, 500}, {G5, 1000}, {E5, 1000}, {C5, 1000}, {B4, 1000}, {A4, 1000},
    {F5, 500}, {F5, 500}, {E5, 1000}, {C5, 1000}, {D5, 1000}, {C5, 1000}
};
// Définition de la mélodie "Tetris Theme (Korobeiniki)"
MelodyNote tetris_theme[] = {
    {E5, 300}, {B4, 150}, {C5, 150}, {D5, 300}, {C5, 150}, {B4, 150}, {A4, 300}, {A4, 150}, {C5, 150}, {E5, 300}, {D5, 150}, {C5, 150},
    {B4, 300}, {B4, 150}, {C5, 150}, {D5, 300}, {E5, 300}, {C5, 300}, {A4, 300}, {A4, 300}, {D5, 300}, {F5, 150}, {A5, 300}, {G5, 150},
    {F5, 150}, {E5, 300}, {C5, 150}, {E5, 300}, {D5, 150}, {C5, 150}, {B4, 300}, {B4, 150}, {C5, 150}, {D5, 300}, {E5, 300}, {C5, 300},
    {A4, 300}, {A4, 300}, {E4, 300}, {C5, 150}, {B4, 150}, {A4, 300}, {A4, 150}, {B4, 150}, {C5, 300}, {D5, 300}, {F5, 150}, {A5, 300},
    {G5, 150}, {F5, 150}, {E5, 300}, {C5, 150}, {E5, 300}, {D5, 150}, {C5, 150}, {B4, 300}, {B4, 150}, {C5, 150}, {D5, 300}, {E5, 300},
    {C5, 300}, {A4, 300}, {A4, 300}, {D5, 300}, {F5, 150}, {A5, 300}, {G5, 150}, {F5, 150}, {E5, 300}, {C5, 150}, {E5, 300}, {D5, 150},
    {C5, 150}, {B4, 300}, {B4, 150}, {C5, 150}, {D5, 300}, {E5, 300}, {C5, 300}, {A4, 300}, {A4, 300}
};
MelodyNote mario_theme[] = {
    {E7, 125}, {E7, 125}, {REST, 125}, {E7, 125},
    {REST, 125}, {C7, 125}, {E7, 125}, {REST, 125},
    {G7, 125}, {REST, 375}, {G6, 125}, {REST, 375},

    {C7, 125}, {REST, 250}, {G6, 125}, {REST, 250},
    {E6, 125}, {REST, 250}, {A6, 125}, {REST, 125},
    {B6, 125}, {REST, 125}, {Bb6, 125}, {A6, 125},
    {REST, 125}, {G6, 125}, {E7, 125}, {REST, 125},
    {G7, 125}, {A7, 125}, {REST, 125}, {F7, 125},
    {G7, 125}, {REST, 125}, {E7, 125}, {REST, 125},
    {C7, 125}, {D7, 125}, {B6, 125}, {REST, 250},

    {C7, 125}, {REST, 250}, {G6, 125}, {REST, 250},
    {E6, 125}, {REST, 250}, {A6, 125}, {REST, 125},
    {B6, 125}, {REST, 125}, {Bb6, 125}, {A6, 125},
    {REST, 125}, {G6, 125}, {E7, 125}, {REST, 125},
    {G7, 125}, {A7, 125}, {REST, 125}, {F7, 125},
    {G7, 125}, {REST, 125}, {E7, 125}, {REST, 125},
    {C7, 125}, {D7, 125}, {B6, 125}, {REST, 250},

    {REST, 375}, {G7, 125}, {Fs7, 125}, {F7, 125},
    {Ds7, 125}, {REST, 125}, {E7, 125}, {REST, 125},
    {Gs6, 125}, {A6, 125}, {C7, 125}, {REST, 125},
    {A6, 125}, {C7, 125}, {D7, 125},

    {REST, 250}, {REST, 250}, {Ds7, 125}, {REST, 125},
    {D7, 125}, {REST, 125}, {C7, 125}, {REST, 125},
    {REST, 125}, {C6, 125}, {REST, 125}, {REST, 125},
    {REST, 125}, {REST, 125}, {REST, 125},

    {E6, 125}, {REST, 125}, {C6, 125}, {REST, 125},
    {D6, 125}, {REST, 125}, {B5, 125}, {REST, 125},
    {REST, 125}, {E6, 125}, {REST, 125}, {C6, 125},
    {REST, 125}, {D6, 125}, {REST, 125}, {B5, 125},
    {REST, 125}, {REST, 125}, {REST, 125},

    {REST, 375}, {G7, 125}, {Fs7, 125}, {F7, 125},
    {Ds7, 125}, {REST, 125}, {E7, 125}, {REST, 125},
    {Gs6, 125}, {A6, 125}, {C7, 125}, {REST, 125},
    {A6, 125}, {C7, 125}, {D7, 125},

    {REST, 250}, {REST, 250}, {Ds7, 125}, {REST, 125},
    {D7, 125}, {REST, 125}, {C7, 125}, {REST, 125},
    {REST, 125}, {C6, 125}, {REST, 125}, {REST, 125},
    {REST, 125}, {REST, 125}, {REST, 125},

    {E6, 125}, {REST, 125}, {C6, 125}, {REST, 125},
    {D6, 125}, {REST, 125}, {B5, 125}, {REST, 125},
    {REST, 125}, {REST, 125}, {REST, 125}, {REST, 125},
    {REST, 125}, {REST, 125}, {REST, 125}
};
MelodyNote castlevania_theme[] = {
    {E4, 150}, {G4, 150}, {A4, 150}, {G4, 150}, {E4, 150}, {G4, 150}, {A4, 150}, {G4, 150},
    {F4, 200}, {A4, 200}, {B4, 200}, {A4, 200}, {F4, 200}, {A4, 200}, {B4, 200}, {A4, 200},
    {E4, 150}, {G4, 150}, {A4, 150}, {G4, 150}, {E4, 150}, {G4, 150}, {A4, 150}, {G4, 150},
    {D4, 200}, {F4, 200}, {G4, 200}, {F4, 200}, {D4, 200}, {F4, 200}, {G4, 200}, {F4, 200},
    {C4, 250}, {E4, 250}, {F4, 250}, {E4, 250}, {C4, 250}, {E4, 250}, {F4, 250}, {E4, 250},
    {B3, 300}, {D4, 300}, {E4, 300}, {D4, 300}, {B3, 300}, {D4, 300}, {E4, 300}, {D4, 300},
    // Partie suivante
    {E4, 150}, {G4, 150}, {A4, 150}, {G4, 150}, {E4, 150}, {G4, 150}, {A4, 150}, {G4, 150},
    {F4, 200}, {A4, 200}, {B4, 200}, {A4, 200}, {F4, 200}, {A4, 200}, {B4, 200}, {A4, 200},
    {E4, 150}, {G4, 150}, {A4, 150}, {G4, 150}, {E4, 150}, {G4, 150}, {A4, 150}, {G4, 150},
    {D4, 200}, {F4, 200}, {G4, 200}, {F4, 200}, {D4, 200}, {F4, 200}, {G4, 200}, {F4, 200},
    {C4, 250}, {E4, 250}, {F4, 250}, {E4, 250}, {C4, 250}, {E4, 250}, {F4, 250}, {E4, 250},
    {B3, 300}, {D4, 300}, {E4, 300}, {D4, 300}, {B3, 300}, {D4, 300}, {E4, 300}, {D4, 300}
};

MelodyNote nyan_cat_theme[] = {
    {E6, 200}, {B5, 100}, {C6, 100}, {D6, 100}, {C6, 100}, {B5, 100}, {C6, 100}, {E6, 100}, {D6, 100},
    {C6, 100}, {B5, 200}, {E6, 200}, {C6, 100}, {D6, 100}, {B5, 100}, {C6, 100}, {E6, 100}, {D6, 100},
    {C6, 100}, {B5, 200}, {C6, 200}, {G5, 100}, {B5, 100}, {G5, 100}, {E5, 100}, {G5, 100}, {C6, 100},
    {B5, 100}, {G5, 100}, {E5, 200}, {G5, 200}, {B5, 100}, {G5, 100}, {E5, 100}, {G5, 100}, {C6, 100},
    {B5, 100}, {G5, 100}, {E5, 200}, {B5, 200}, {G5, 100}, {B5, 100}, {G5, 100}, {E5, 100}, {G5, 100},
    {C6, 100}, {B5, 100}, {G5, 100}, {E5, 200}, {B5, 200}, {G5, 100}, {B5, 100}, {G5, 100}, {E5, 100},
    {G5, 100}, {E5, 100}, {C6, 100}, {A5, 100}, {B5, 100}, {A5, 100}, {G5, 100}, {E5, 100}, {D5, 100},
    {E5, 100}, {G5, 100}, {B5, 100}, {A5, 100}, {G5, 100}, {E5, 100}, {C5, 100}, {D5, 100}, {B4, 100},
    {C5, 100}, {E5, 100}, {G5, 100}, {B5, 100}, {A5, 100}, {G5, 100}, {E5, 100}, {C5, 100}, {D5, 100},
    {B4, 100}, {E5, 100}, {G5, 100}, {F5, 100}, {E5, 100}, {D5, 100}, {E5, 100}, {G5, 100}, {B5, 100},
    {A5, 100}, {G5, 100}, {E5, 100}, {C5, 100}, {D5, 100}, {B4, 100}, {E5, 100}, {G5, 100}, {B5, 100},
    {A5, 100}, {G5, 100}, {E5, 100}, {C5, 100}, {D5, 100}, {B4, 100}, {E5, 100}, {G5, 100}, {F5, 100},
    {E5, 100}, {D5, 100}, {E5, 100}, {G5, 100}, {B5, 100}, {A5, 100}, {G5, 100}, {E5, 100}, {C5, 100},
    {D5, 100}, {B4, 100}, {E5, 100}, {G5, 100}, {B5, 100}, {A5, 100}, {G5, 100}, {E5, 100}, {C5, 100},
    {D5, 100}, {B4, 100}, {E5, 100}, {G5, 100}, {F5, 100}, {E5, 100}, {D5, 100}, {E5, 100}, {G5, 100},
    {B5, 100}, {A5, 100}, {G5, 100}, {E5, 100}, {C5, 100}, {D5, 100}, {B4, 100}
};

MelodyNote jingle_bells[] = {
    {E4, 400}, {E4, 400}, {E4, 800}, {E4, 400}, {E4, 400}, {E4, 800}, {E4, 400}, {G4, 400},
    {C4, 400}, {D4, 400}, {E4, 800}, {F4, 400}, {F4, 400}, {F4, 800}, {F4, 400}, {E4, 400}, 
    {E4, 400}, {E4, 800}, {E4, 400}, {D4, 400}, {D4, 800}, {E4, 400}, {D4, 400}, {G4, 800},
    {E4, 400}, {E4, 400}, {E4, 800}, {E4, 400}, {E4, 400}, {E4, 800}, {E4, 400}, {G4, 400},
    {C4, 400}, {D4, 400}, {E4, 800}
};
MelodyNote twinkle_twinkle[] = {
    {C4, 500}, {C4, 500}, {G4, 500}, {G4, 500}, {A4, 500}, {A4, 500}, {G4, 1000},
    {F4, 500}, {F4, 500}, {E4, 500}, {E4, 500}, {D4, 500}, {D4, 500}, {C4, 1000},
    {G4, 500}, {G4, 500}, {F4, 500}, {F4, 500}, {E4, 500}, {E4, 500}, {D4, 1000},
    {G4, 500}, {G4, 500}, {F4, 500}, {F4, 500}, {E4, 500}, {E4, 500}, {D4, 1000}
};
MelodyNote ode_to_joy[] = {
    {E4, 400}, {E4, 400}, {F4, 400}, {G4, 400}, {G4, 400}, {F4, 400}, {E4, 400}, {D4, 400},
    {C4, 400}, {C4, 400}, {D4, 400}, {E4, 400}, {E4, 400}, {D4, 400}, {D4, 400}, {E4, 400},
    {E4, 400}, {F4, 400}, {G4, 400}, {G4, 400}, {F4, 400}, {E4, 400}, {D4, 400}, {C4, 400},
    {C4, 400}, {D4, 400}, {E4, 400}, {D4, 400}, {C4, 400}, {C4, 400},
    {E4, 400}, {E4, 400}, {F4, 400}, {G4, 400}, {G4, 400}, {F4, 400}, {E4, 400}, {D4, 400},
    {C4, 400}, {C4, 400}, {D4, 400}, {E4, 400}, {D4, 400}, {C4, 400}, {C4, 400}
};
MelodyNote au_clair_de_la_lune[] = {
    {C4, 500}, {D4, 500}, {E4, 500}, {C4, 500}, {D4, 500}, {E4, 500}, {E4, 1000},
    {D4, 500}, {E4, 500}, {F4, 500}, {D4, 500}, {E4, 500}, {F4, 500}, {F4, 1000},
    {E4, 500}, {D4, 500}, {C4, 1000},
    {C4, 500}, {D4, 500}, {E4, 500}, {C4, 500}, {D4, 500}, {E4, 500}, {E4, 1000},
    {D4, 500}, {E4, 500}, {F4, 500}, {D4, 500}, {E4, 500}, {F4, 500}, {F4, 1000},
    {E4, 500}, {D4, 500}, {C4, 1000}
};
MelodyNote frere_jacques[] = {
    {C4, 500}, {D4, 500}, {E4, 500}, {C4, 500},
    {C4, 500}, {D4, 500}, {E4, 500}, {C4, 500},
    {E4, 500}, {F4, 500}, {G4, 1000},
    {E4, 500}, {F4, 500}, {G4, 1000},
    {G4, 500}, {A4, 500}, {G4, 500}, {F4, 500},
    {G4, 500}, {A4, 500}, {G4, 500}, {F4, 500},
    {E4, 500}, {C4, 500}, {E4, 500}, {C4, 500},
    {C4, 500}, {D4, 500}, {E4, 500}, {C4, 500},
    {E4, 500}, {F4, 500}, {G4, 1000},
    {E4, 500}, {F4, 500}, {G4, 1000},
    {G4, 500}, {A4, 500}, {G4, 500}, {F4, 500},
    {G4, 500}, {A4, 500}, {G4, 500}, {F4, 500},
    {E4, 500}, {C4, 500}, {E4, 500}, {C4, 500}
};
MelodyNote jai_du_bon_tabac[] = {
    {C4, 500}, {D4, 500}, {E4, 500}, {E4, 500}, {D4, 500}, {C4, 500},
    {D4, 500}, {E4, 500}, {F4, 500}, {F4, 500}, {E4, 500}, {D4, 500},
    {E4, 500}, {F4, 500}, {G4, 500}, {G4, 500}, {F4, 500}, {E4, 500},
    {F4, 500}, {G4, 500}, {A4, 500}, {A4, 500}, {G4, 500}, {F4, 500},
    {G4, 1000},
    {C4, 500}, {D4, 500}, {E4, 500}, {E4, 500}, {D4, 500}, {C4, 500},
    {D4, 500}, {E4, 500}, {F4, 500}, {F4, 500}, {E4, 500}, {D4, 500},
    {E4, 500}, {F4, 500}, {G4, 500}, {G4, 500}, {F4, 500}, {E4, 500},
    {F4, 500}, {G4, 500}, {A4, 500}, {A4, 500}, {G4, 500}, {F4, 500},
    {G4, 1000}
};

MelodyNote megalovania[] = {
    {D5, 150}, {D5, 150}, {REST, 150}, {A4, 150}, {D5, 150}, {F5, 150}, {F5, 150}, {F5, 150}, 
    {REST, 150}, {E5, 150}, {E5, 150}, {REST, 150}, {D5, 150}, {E5, 150}, {G5, 150}, {G5, 150}, 
    {G5, 150}, {REST, 150}, {F5, 150}, {F5, 150}, {REST, 150}, {E5, 150}, {F5, 150}, {A5, 150}, 
    {A5, 150}, {A5, 150}, {REST, 150}, {G5, 150}, {G5, 150}, {REST, 150}, {F5, 150}, {G5, 150}, 
    {B5, 150}, {B5, 150}, {B5, 150}, {REST, 150}, {A5, 150}, {A5, 150}, {REST, 150}, {G5, 150}, 
    {A5, 150}, {D6, 150}, {D6, 150}, {D6, 150}, {REST, 150}, {E6, 150}, {E6, 150}, {REST, 150}, 
    {D6, 150}, {E6, 150}, {G6, 150}, {G6, 150}, {G6, 150}, {REST, 150}, {F6, 150}, {F6, 150}, 
    {REST, 150}, {E6, 150}, {F6, 150}, {A6, 150}, {A6, 150}, {A6, 150}, {REST, 150}, {G6, 150}, 
    {G6, 150}, {REST, 150}, {F6, 150}, {G6, 150}, {B6, 150}, {B6, 150}, {B6, 150}, {REST, 150}, 
    {A6, 150}, {A6, 150}, {REST, 150}, {G6, 150}, {A6, 150}, {D7, 150}, {D7, 150}, {D7, 150}
};

MelodyNote marseillaise[] = {
    {G4, 400}, {G4, 400}, {A4, 400}, {B4, 400}, {C5, 800},
    {B4, 400}, {A4, 400}, {G4, 400}, {A4, 400}, {B4, 800},
    {A4, 400}, {G4, 400}, {F4, 400}, {G4, 400}, {A4, 800},
    {G4, 400}, {F4, 400}, {E4, 400}, {F4, 400}, {G4, 800},
    {F4, 400}, {E4, 400}, {D4, 400}, {E4, 400}, {F4, 800},
    {E4, 400}, {D4, 400}, {C4, 400}, {D4, 400}, {E4, 800},
    {D4, 400}, {C4, 400}, {B3, 400}, {C4, 400}, {D4, 800},
    {C4, 400}, {B3, 400}, {A3, 400}, {B3, 400}, {C4, 800}
};

void play_melody(MelodyNote* melody, int length) {
    for (int i = 0; i < length; i++) {
        play_sound(melody[i].note, melody[i].duration/10);
        wait_clock(current_clock() + melody[i].duration/10);
    }
}

int music_handler_loop(void* arg) {
    Melody* melody = (Melody*)arg;  // Convertir le pointeur void* en Melody*
    while (1) {
        play_melody(melody->notes, melody->length);
    }
    return 0;
}
int music_handler_once(void* arg) {
    Melody* melody = (Melody*)arg;  // Convertir le pointeur void* en Melody*
    play_melody(melody->notes, melody->length);
    return 0;
}

void cmd_theme(int argc, char *argv[]){
    if (argc != 2) {
        printf("Utilisation: theme <num>\n");
        return;
    }
    set_color(VERT_CLAIR);
    printf("Playing... ");
    switch (atoi(argv[1]))
    {
    case 1:
        set_color(BLEU_CLAIR);printf("H");
        set_color(VERT_CLAIR);printf("a");
        set_color(CYAN_CLAIR);printf("p");
        set_color(ROUGE_CLAIR);printf("p");
        set_color(MAGENTA_CLAIR);printf("y ");
        set_color(JAUNE);printf("B");
        set_color(BLEU);printf("i");
        set_color(VERT);printf("r");
        set_color(CYAN);printf("t");
        set_color(ROUGE);printf("h");
        set_color(MAGENTA);printf("d");
        set_color(MARRON);printf("a");
        set_color(GRIS_CLAIR);printf("y\n");
        
        play_melody(happy_birthday, sizeof(happy_birthday) / sizeof(MelodyNote));
        break;
    case 2:
        set_color(ROUGE_CLAIR);printf("T");
        set_color(MAGENTA_CLAIR);printf("E");
        set_color(JAUNE);printf("T");
        set_color(BLEU);printf("R");
        set_color(VERT);printf("I");
        set_color(CYAN);printf("S ");

        play_melody(tetris_theme, sizeof(tetris_theme) / sizeof(MelodyNote));
        break;
    case 3:
        set_color(BLEU_CLAIR);printf("S");
        set_color(JAUNE);printf("U");
        set_color(ROUGE_CLAIR);printf("P");
        set_color(VERT);printf("E");
        set_color(JAUNE);printf("R ");
        set_color(ROUGE_CLAIR);printf("M");
        set_color(VERT);printf("A");
        set_color(JAUNE);printf("R");
        set_color(BLEU_CLAIR);printf("I");
        set_color(VERT_CLAIR);printf("O ");

        play_melody(mario_theme, sizeof(mario_theme) / sizeof(MelodyNote));
        break;
    case 4:
        set_color(ROUGE_CLAIR);printf("CASTLEVANIA !");
        play_melody(castlevania_theme, sizeof(castlevania_theme) / sizeof(MelodyNote));
        break;
    case 5:
        set_color(MAGENTA);printf("DESTRUCTEUR D'OREILLE");
        play_melody(nyan_cat_theme, sizeof(nyan_cat_theme) / sizeof(MelodyNote));
        break;
    case 6:
        set_color(JAUNE); printf("Jingle Bells\n");
        play_melody(jingle_bells, sizeof(jingle_bells) / sizeof(MelodyNote));
        break;
    case 7:
        set_color(VERT); printf("Twinkle Twinkle\n");
        play_melody(twinkle_twinkle, sizeof(twinkle_twinkle) / sizeof(MelodyNote));
        break;
    case 8:
        set_color(CYAN); printf("Ode to Joy\n");
        play_melody(ode_to_joy, sizeof(ode_to_joy) / sizeof(MelodyNote));
        break;
    case 9:
        set_color(BLEU); printf("Au Clair de la Lune\n");
        play_melody(au_clair_de_la_lune, sizeof(au_clair_de_la_lune) / sizeof(MelodyNote));
        break;
    case 10:
        set_color(MARRON); printf("Frere Jacques\n");
        play_melody(frere_jacques, sizeof(frere_jacques) / sizeof(MelodyNote));
        break;
    case 11:
        set_color(ROUGE); printf("J'ai du bon tabac\n");
        play_melody(jai_du_bon_tabac, sizeof(jai_du_bon_tabac) / sizeof(MelodyNote));
        break;
    case 12:
        set_color(BLEU_CLAIR); printf("MEGALOVANIA\n");
        play_melody(megalovania, sizeof(megalovania) / sizeof(MelodyNote));
        break;
    case 13:
        printf("La.... ");
        set_color(BLEU); printf("Mars");
        set_color(BLANC); printf("eill");
        set_color(ROUGE); printf("aise  ");
        
        set_color(BLEU); printf("FRA");
        set_color(BLANC); printf("NCA");
        set_color(ROUGE); printf("ISE");
        set_color(BLANC); printf("!!!!\n");

        play_melody(marseillaise, sizeof(marseillaise) / sizeof(MelodyNote));
        break;
    default:
        printf("Melodie non reconnue\n");
        break;
    }

    printf("\n");
    set_color(BLANC);

    return;
}
