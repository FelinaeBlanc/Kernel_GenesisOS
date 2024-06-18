#ifndef THEME_H
#define THEME_H

#include "sound_enum.h"


extern void play_melody(const int melody[], int nbNotes, int tempo);
extern int music_handler_loop(void* arg);
extern int music_handler_once(void* arg);

extern void cmd_theme(int argc, char *argv[]);

#endif