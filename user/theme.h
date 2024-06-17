#ifndef THEME_H
#define THEME_H

#include "sound_enum.h"

typedef struct {
    Note note;
    int duration;
} MelodyNote;

extern void play_melody(MelodyNote* melody, int length);

extern void cmd_theme(int argc, char *argv[]);

#endif