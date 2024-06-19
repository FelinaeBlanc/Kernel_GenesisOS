#ifndef THEME_H
#define THEME_H

#include "sound_enum.h"

typedef struct _Theme {
    const int* melody;
    int nbNotes;
    int tempo;
} Theme;

extern int play_melody_proc_loop(void* arg);
extern int play_melody_proc_once(void* arg);

extern void play_melody(const int melody[], int nbNotes, int tempo);
extern void cmd_theme(int argc, char *argv[]);

#endif