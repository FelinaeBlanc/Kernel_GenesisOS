#ifndef THEME_H
#define THEME_H

#include "sound_enum.h"

typedef struct _MelodyNote {
    Note note;
    int duration;
} MelodyNote;

typedef struct _Melody {
    MelodyNote* notes;
    int length;
} Melody;

extern void play_melody(MelodyNote* melody, int length);
extern int music_handler_loop(void* arg);
extern int music_handler_once(void* arg);

extern void cmd_theme(int argc, char *argv[]);

#endif