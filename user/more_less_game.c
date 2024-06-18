#include "builtin.h"
#include "stdio.h"
#include "string.h"
#include "random.h"
#include "more_less_game.h"
#include "utils.h"
#include "theme.h"

// Jeu du plus ou moins

unsigned int try_left;
unsigned int answer;
unsigned int win = 0;

MelodyNote action_music_notes[] = {
    {C5, 200}, {E5, 200}, {G5, 400}, {REST, 100}, 
    {C5, 200}, {E5, 200}, {G5, 200}, {B5, 200}, {A5, 200}, {G5, 200}, {F5, 200}, {E5, 400}, 
    {REST, 100}, 
    {E5, 150}, {G5, 150}, {B5, 150}, {G5, 150}, {D5, 150}, {F5, 150}, {A5, 150}, {G5, 400}, 
    {REST, 100}, 
    {C6, 200}, {B5, 200}, {A5, 200}, {G5, 200}, {F5, 200}, {E5, 200}, {D5, 200}, {C5, 400}, 
    {REST, 100}, 
    {C5, 150}, {D5, 150}, {E5, 150}, {F5, 150}, {G5, 150}, {A5, 150}, {B5, 150}, {C6, 400}, 
    {REST, 100}, 
    {D6, 200}, {C6, 200}, {B5, 200}, {A5, 200}, {G5, 200}, {F5, 200}, {E5, 200}, {D5, 400}, 
    {REST, 100}, 
    {E5, 150}, {F5, 150}, {G5, 150}, {A5, 150}, {B5, 150}, {C6, 150}, {D6, 150}, {E6, 400}, 
    {REST, 100}, 
    {F6, 200}, {E6, 200}, {D6, 200}, {C6, 200}, {B5, 200}, {A5, 200}, {G5, 200}, {F5, 400}, 
    {REST, 100}, 
    {E5, 150}, {D5, 150}, {C5, 150}, {B4, 150}, {A4, 150}, {G4, 150}, {F4, 150}, {E4, 400}, 
    {REST, 100}, 
    {G5, 200}, {F5, 200}, {E5, 200}, {D5, 200}, {C5, 200}, {B4, 200}, {A4, 200}, {G4, 400}, 
    {REST, 100}, 
    {C5, 200}, {E5, 200}, {G5, 400}, {REST, 100}, 
    {C5, 200}, {E5, 200}, {G5, 200}, {B5, 200}, {A5, 200}, {G5, 200}, {F5, 200}, {E5, 400}, 
    {REST, 100}, 
    {E5, 150}, {G5, 150}, {B5, 150}, {G5, 150}, {D5, 150}, {F5, 150}, {A5, 150}, {G5, 400}
};
MelodyNote victory_music_notes[] = {
    {C5, 200}, {E5, 200}, {G5, 200}, {C6, 400},
    {E6, 200}, {G6, 200}, {C7, 400},
    {REST, 200},
    {G6, 200}, {E6, 200}, {C6, 400},
    {G5, 200}, {E5, 200}, {C5, 600}
};
MelodyNote defeat_music_notes[] = {
    {C5, 200}, {A4, 200}, {F4, 200}, {D4, 400},
    {REST, 200},
    {D4, 200}, {F4, 200}, {A4, 400},
    {REST, 200},
    {C5, 200}, {A4, 200}, {F4, 600}
};

Melody action_music = { action_music_notes, sizeof(action_music_notes) / sizeof(MelodyNote) };
Melody victory_music = { victory_music_notes, sizeof(victory_music_notes) / sizeof(MelodyNote) };
Melody defeat_music = { defeat_music_notes, sizeof(defeat_music_notes) / sizeof(MelodyNote) };

// CMD Fonction principale
void cmd_more_less_game(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    // start music!
    int action_music_pid = start(&music_handler_loop, 4000, getprio(getpid()), "music_handler", (void*)&action_music);
    if (action_music_pid < 0) {
        printf("Impossible de lancer la musique...\n");
        return;
    }

    srand(78 * current_clock() + 4562133);
    unsigned int answer = rand_int() % 1001;
    unsigned int try_left = 10;
    unsigned int win = 0;

    printf("Bienvenue dans le jeu du plus ou moins !\n");
    printf("Devinez le nombre entre 0 et 1000\n");

    while (try_left > 0) {
        printf("Il vous reste %d essais\n", try_left);
        printf("Entrez un nombre: ");
        char buffer[5];
        cons_read(buffer, 5);
        unsigned int guess = atoi(buffer);

        printf("\n");
        if (guess == answer) {
            printf("Bravo ! Vous avez trouve le nombre mystere !\n");
            win = 1;
            break;
        } else if (guess < answer) {
            printf("C'est plus !\n");
        } else {
            printf("C'est moins !\n");
        }

        try_left--;
        if (try_left == 0) {
            printf("Vous avez perdu ! Le nombre mystere etait %d\n", answer);
            win = 0;
            break;
        }
    }

    // Arrêter la musique d'action
    if (action_music_pid >= 0) {
        kill(action_music_pid);
    }

    // Jouer la musique de victoire ou de défaite
    if (win) {
        start(&music_handler_once, 4000, getprio(getpid()), "music_handler", (void*)&victory_music);
    } else {
        start(&music_handler_once, 4000, getprio(getpid()), "music_handler", (void*)&defeat_music);
    }
}
