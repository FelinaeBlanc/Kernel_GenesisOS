#include "builtin.h"
#include "stdio.h"
#include "string.h"
#include "random.h"
#include "pendu.h"
#include "utils.h"
#include "theme.h"

MelodyNote action_horror_music_notes[] = {
    {C4, 150}, {E4, 150}, {G4, 150}, {A4, 150}, {REST, 50}, 
    {A4, 150}, {G4, 150}, {E4, 150}, {C4, 150}, {REST, 50},
    {E4, 100}, {REST, 50}, {E4, 100}, {REST, 50}, {F4, 100}, {REST, 50}, {F4, 100}, {REST, 50}, 
    {G4, 100}, {REST, 50}, {G4, 100}, {REST, 50}, {A4, 100}, {REST, 50}, {A4, 100}, {REST, 50}, 
    {B4, 100}, {REST, 50}, {C5, 100}, {REST, 50}, {D5, 100}, {REST, 50}, {E5, 100}, {REST, 50},
    {C5, 150}, {G4, 150}, {E4, 150}, {C4, 150}, {REST, 50}, 
    {D5, 150}, {A4, 150}, {F4, 150}, {D4, 150}, {REST, 50},
    {E5, 200}, {REST, 100}, {E5, 200}, {REST, 100}, {D5, 200}, {REST, 100}, {D5, 200}, {REST, 100},
    {C5, 200}, {REST, 100}, {C5, 200}, {REST, 100}, {B4, 200}, {REST, 100}, {B4, 200}, {REST, 100},
    {A4, 200}, {REST, 100}, {A4, 200}, {REST, 100}, {G4, 200}, {REST, 100}, {G4, 200}, {REST, 100},
    {F4, 200}, {REST, 100}, {F4, 200}, {REST, 100}, {E4, 200}, {REST, 100}, {E4, 200}, {REST, 100},
    {D4, 200}, {REST, 100}, {D4, 200}, {REST, 100}, {C4, 200}, {REST, 100}, {C4, 200}, {REST, 100}
};
MelodyNote victory_horror_music_notes[] = {
    {E4, 200}, {G4, 200}, {C5, 400},
    {E5, 200}, {G5, 200}, {C6, 400},
    {REST, 200},
    {C5, 200}, {G4, 200}, {E4, 200}, {C4, 600}
};
MelodyNote defeat_horror_music_notes[] = {
    {A3, 400}, {REST, 200}, {G3, 400}, {REST, 200}, 
    {F3, 400}, {REST, 200}, {E3, 400}, {REST, 200},
    {D3, 400}, {REST, 200}, {C3, 400}, {REST, 200},
    {B2, 600}, {REST, 400}
};


Melody action_horror_music = { action_horror_music_notes, sizeof(action_horror_music_notes) / sizeof(MelodyNote) };
Melody victory_horror_music = { victory_horror_music_notes, sizeof(victory_horror_music_notes) / sizeof(MelodyNote) };
Melody defeat_horror_music = { defeat_horror_music_notes, sizeof(defeat_horror_music_notes) / sizeof(MelodyNote) };

// Dictionnaire de mots
const char* dictionary[] = {
    "programmation", "ordinateur", "informatique", "developpement", "logiciel",
    "materiel", "reseau", "internet", "base", "donnees", "memoire", "processeur"
};

void choose_random_word(char* word) {
    int word_count = sizeof(dictionary) / sizeof(dictionary[0]);
    srand((unsigned int)current_clock());
    int random_index = rand_int() % word_count;
    strcpy(word, dictionary[random_index]);
}

void display_word(const char* word, const int* found) {
    for (int i = 0; word[i] != '\0'; i++) {
        if (found[i]) {
            printf("%c", word[i]);
        } else {
            printf("_");
        }
    }
    printf("\n");
}

void cmd_pendu(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    char word[WORD_LENGTH];
    choose_random_word(word);

    int word_length = strlen(word);
    int found[word_length];
    memset(found, 0, sizeof(found));

    int tries_left = MAX_TRIES;
    char guessed_letters[26] = {0};
    int guessed_count = 0;
    int win = 0;

    // start music!
    int action_horror_music_pid = start(&music_handler_loop, 4000, getprio(getpid()), "music_handler", (void*)&action_horror_music);
    if (action_horror_music_pid < 0) {
        printf("Impossible de lancer la musique...\n");
        return;
    }

    printf("Bienvenue dans le jeu du pendu !\n");

    while (tries_left > 0) {
        printf("Mot a deviner : ");
        display_word(word, found);
        printf("Tentatives restantes : %d\n", tries_left);
        printf("Lettres deja proposees : %s\n", guessed_letters);
        printf("Proposez une lettre : \n");

        char buffer[2]; // Buffer to read one character plus the null terminator
        cons_read(buffer, 2);
        char guess = buffer[0];

        if (strchr(guessed_letters, guess)) {
            printf("Vous avez deja propose cette lettre.\n");
            continue;
        }

        guessed_letters[guessed_count++] = guess;
        guessed_letters[guessed_count] = '\0';

        int correct_guess = 0;
        for (int i = 0; i < word_length; i++) {
            if (word[i] == guess) {
                found[i] = 1;
                correct_guess = 1;
            }
        }

        if (!correct_guess) {
            tries_left--;
            printf("Mauvaise reponse !\n");
        } else {
            printf("Bonne reponse !\n");
        }

        int all_found = 1;
        for (int i = 0; i < word_length; i++) {
            if (!found[i]) {
                all_found = 0;
                break;
            }
        }
        
        if (all_found) {
            win = 1;
            break;
        }
    }

    // Arrêter la musique d'action
    if (action_horror_music_pid >= 0) {
        kill(action_horror_music_pid);
    }

    if (win) {
        printf("Felicitations ! Vous avez trouve le mot : %s\n", word);
        start(&music_handler_once, 4000, getprio(getpid()), "music_handler", (void*)&victory_horror_music);
    } else {
        printf("Vous avez perdu ! Le mot etait : %s\n", word);
        start(&music_handler_once, 4000, getprio(getpid()), "music_handler", (void*)&defeat_horror_music);
    }
}
