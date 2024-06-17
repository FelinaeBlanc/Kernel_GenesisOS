#include "theme.h"
#include "sound_enum.h"
#include "builtin.h"
#include "stdio.h"


MelodyNote happy_birthday[] = {
    {G4, 500}, {G4, 500}, {A4, 1000}, {G4, 1000}, {C5, 1000}, {B4, 2000},
    {G4, 500}, {G4, 500}, {A4, 1000}, {G4, 1000}, {D5, 1000}, {C5, 2000},
    {G4, 500}, {G4, 500}, {G5, 1000}, {E5, 1000}, {C5, 1000}, {B4, 1000}, {A4, 1000},
    {F5, 500}, {F5, 500}, {E5, 1000}, {C5, 1000}, {D5, 1000}, {C5, 1000}
};
MelodyNote hedwigs_theme[] = {
    {E4, 500}, {A4, 500}, {C5, 250}, {B4, 250}, {A4, 250}, {G4, 250}, {Fs4, 250}, {G4, 250}, {B4, 500}, {D5, 500}, {C5, 1000}, // Première phrase
    {A4, 500}, {C5, 500}, {E5, 250}, {D5, 250}, {C5, 250}, {B4, 250}, {A4, 250}, {B4, 250}, {G4, 500}, {A4, 500}, {Fs4, 1000}, // Deuxième phrase
    {E4, 500}, {A4, 500}, {C5, 250}, {B4, 250}, {A4, 250}, {G4, 250}, {Fs4, 250}, {G4, 250}, {B4, 500}, {D5, 500}, {C5, 1000}, // Troisième phrase
    {A4, 500}, {C5, 500}, {E5, 250}, {D5, 250}, {C5, 250}, {B4, 250}, {A4, 250}, {B4, 250}, {G4, 500}, {A4, 500}, {Fs4, 1000}  // Quatrième phrase
};


void play_melody(MelodyNote* melody, int length) {
    for (int i = 0; i < length; i++) {
        play_sound(melody[i].note, melody[i].duration/10);
        wait_clock(current_clock() + melody[i].duration/10);
      
    }
    printf("Finit!\n");
}

void cmd_theme(int argc, char *argv[]){
    argc = argc;
    argv = argv;
    //play_melody(happy_birthday, sizeof(happy_birthday) / sizeof(MelodyNote));
    play_melody(hedwigs_theme, sizeof(hedwigs_theme) / sizeof(MelodyNote));
    return;
}
