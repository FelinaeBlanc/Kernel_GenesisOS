#include "builtin.h"
#include "piano.h"
#include "theme.h"
#include "sound_enum.h"
#include "stdio.h"
#include "stdbool.h"


void cmd_piano(int argc, char *argv[]) {
    (void) argc;
    (void) argv;
    
    printf(" ____________________________________\n");
    printf("|\\                                    \\\n");
    printf("| \\                                    \\\n");
    printf("|  \\____________________________________\\\n");
    printf("|  |       __---_ _---__                |\n");
    printf("|  |      |======|=====|                |\n");
    printf("|  |      |======|=====|                |\n");
    printf("|  |  ____|__---_|_---_|______________  |\n");
    printf("|  | |                                | |\n");
    printf("|   \\ \\                                \\ \\\n");
    printf("|  \\ ||\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\| |     \n");
    printf("|  |\\  ,--------------------------------  |\n");
    printf("|  ||| |                               || |\n");
    printf(" \\ ||| |           -  -                || |\n");
    printf("  \\'|| |-----------\\\\-\\\\---------------|| |\n");
    printf("    \\|_|            \"  \"               \\|_|\n");
    printf("PIANO ! Joue une musique en tappant une touche de A a P !\n");
    printf("Touche"); set_color(ROUGE); printf(" Q "); set_color(BLANC); printf("pour quitter!\n");

    char buffer[1] = {0};
    cons_echo(false);
    cons_read(buffer, 1);
    
    while (buffer[0] != 'q') {
        switch(buffer[0]){
            case 'a':
                play_sound(NOTE_C4, 10);
                printf("C4 ");
                break;
            case 'z':
                play_sound(NOTE_E4, 10);
                printf("E4 ");
                break;
            case 'e':
                play_sound(NOTE_G4, 10);
                printf("G4 ");
                break;
            case 'r':
                play_sound(NOTE_B4, 10);
                printf("B4 ");
                break;
            case 't':
                play_sound(NOTE_D5, 10);
                printf("D5 ");
                break;
            case 'y':
                play_sound(NOTE_F5, 10);
                printf("F5 ");
                break;
            case 'u':
                play_sound(NOTE_A5, 10);
                printf("A5 ");
                break;
            case 'i':
                play_sound(NOTE_C6, 10);
                printf("C6 ");
                break;
            case 'o':
                play_sound(NOTE_E6, 10);
                printf("E6 ");
                break;
            case 'p':
                play_sound(NOTE_G6, 10);
                printf("G6 ");
                break;
            default:
                printf("Unknown key: %c\n", buffer[0]);
                break;
        }

        //wait_clock(current_clock() + 20);
        cons_read(buffer, 1);
    }
    cons_echo(true);
    printf("\n");
    
}