#include "builtin.h"
#include "stdio.h"
#include "string.h"
#include "random.h"
#include "more_less_game.h"
#include "utils.h"
#include "theme.h"
#include "theme_lib.h"

// Jeu du plus ou moins

unsigned int try_left;
unsigned int answer;

// CMD Fonction principale
void cmd_more_less_game(int argc, char *argv[]) {
    (void)argc;
    (void)argv;


    srand(78 * current_clock() + 4562133);
    unsigned int answer = rand_int() % 1001;
    unsigned int try_left = 10;

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
            play_melody(victory_melody, victory_nbNotes, victory_tempo);
            break;
        } else if (guess < answer) {
            printf("C'est plus !\n");
        } else {
            printf("C'est moins !\n");
        }

        try_left--;
        if (try_left == 0) {
            printf("Vous avez perdu ! Le nombre mystere etait %d\n", answer);
            play_melody(defeat_melody, defeat_nbNotes, defeat_tempo);
            break;
        }
    }

}
