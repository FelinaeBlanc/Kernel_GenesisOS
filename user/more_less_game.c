#include "builtin.h"
#include "stdio.h"
#include "string.h"
#include "random.h"
#include "more_less_game.h"
#include "utils.h"
#include "theme.h"
#include "theme_lib.h"

// Jeu du plus ou moins
// theme vampire killer

unsigned int try_left;
unsigned int answer;

// CMD Fonction principale
void cmd_more_less_game(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    Theme theme_music;
    int rdm = rand_int() % 4;
    switch (rdm){
        case 0:
            theme_music = get_keyboardcat();
            break;
        case 1:
            theme_music = get_tetris();
            break;
        case 2:
            theme_music = get_gameofthrone();
            break;
        case 3:
            theme_music = get_takeonme();
            break;
    }

    int pid_player = start(play_melody_proc_loop, 4000 ,getprio(getpid()) , "Music Player", &theme_music);
    if (pid_player < 0) {
        printf("Impossible de lancer la music...\n");
    }

    unsigned int answer = rand_int() % 1001;
    unsigned int try_left = 10;
    int win = 0;

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
            win = 1;
            break;
        } else if (guess < answer) {
            printf("C'est plus !\n");
        } else {
            printf("C'est moins !\n");
        }

        try_left--;
        if (try_left == 0) {
            
            win = 0;
            break;
        }
    }

    if (pid_player >= 0){
        kill(pid_player);
        waitpid(pid_player, NULL);
    }

    Theme end_theme;
    if (win){
        printf("Bravo ! Vous avez trouve le nombre mystere !\n");
        end_theme = get_victory();
    } else {
        printf("Vous avez perdu ! Le nombre mystere etait %d\n", answer);
        end_theme = get_defeat();
    }
    pid_player = start(play_melody_proc_once, 4000, getprio(getpid()), "Music Player MoreLess", &end_theme);
    if (pid_player >= 0) {
        waitpid(pid_player, NULL);
    }
    
}
