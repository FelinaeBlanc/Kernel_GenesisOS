#include "builtin.h"
#include "stdio.h"
#include "string.h"
#include "random.h"
#include "pendu.h"
#include "utils.h"
#include "theme.h"
#include "theme_lib.h"

// Dictionnaire de mots
const char* dictionary[] = {
    "programmation", "ordinateur", "informatique", "developpement", "logiciel",
    "materiel", "reseau", "internet", "base", "donnees", "memoire", "processeur",
    "lion", "elephant", "tigre", "girafe", "dauphin", "requin", "aigle", "serpent",
    "pomme", "banane", "orange", "fraise", "chocolat", "fromage", "pain", "poulet",
    "table", "chaise", "lampe", "tasse", "couteau", "ordinateur", "porte", "fenetre",
    "physique", "chimie", "biologie", "mathematiques", "astronomie", "geologie",
    "football", "basketball", "tennis", "natation", "escrime", "rugby", "boxe", "cyclisme",
    "peinture", "sculpture", "musique", "danse", "theatre", "photographie", "cinema",
    "plante", "jardin", "fleur", "arbre", "fruit", "legume", "herbe", "terre", "eau",
    "soleil", "lune", "etoile", "planete", "galaxie", "univers", "comete", "meteore",
    "chaussure", "pantalon", "chemise", "robe", "chapeau", "manteau", "echarpe",
    "histoire", "geographie", "philosophie", "psychologie", "sociologie", "anthropologie",
    "anglais", "francais", "espagnol", "allemand", "italien", "portugais", "chinois",
    "chat", "chien", "oiseau", "poisson", "lapin", "hamster", "cheval", "mouton"
};

void choose_random_word(char* word) {
    int word_count = sizeof(dictionary) / sizeof(dictionary[0]);
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

    Theme theme_music;
    int rdm = rand_int() % 4;
    switch (rdm){
        case 0:
            theme_music = get_harrypotter();
            break;
        case 1:
            theme_music = get_pinkpanther();
            break;
        case 2:
            theme_music = get_songofstorms();
            break;
        case 3:
            theme_music = get_doom();
            break;
    }
    int pid_player = start(play_melody_proc_loop, 4000 ,getprio(getpid()) , "Music Player Pendu", &theme_music);
    if (pid_player < 0) {
        printf("Impossible de lancer la music...\n");
    }

    char word[WORD_LENGTH];
    choose_random_word(word);

    int word_length = strlen(word);
    int found[word_length];
    memset(found, 0, sizeof(found));

    int tries_left = MAX_TRIES;
    char guessed_letters[26] = {0};
    int guessed_count = 0;
    int win = 0;

    printf("Bienvenue dans le jeu du pendu !\n");

    while (tries_left > 0) {
        printf("Mot a deviner : ");
        display_word(word, found);
        printf("Tentatives restantes : %d\n", tries_left);
        printf("Lettres deja proposees : %s\n", guessed_letters);
        printf("Proposez une lettre : ");

        char buffer[2]; // Buffer to read one character plus the null terminator
        cons_read(buffer, 2);
        char guess = buffer[0];
        printf("\n");
    
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

    if (pid_player >= 0){
        kill(pid_player);
        waitpid(pid_player, NULL);
    }

     Theme end_theme;
    if (win) {
        printf("Felicitations ! Vous avez trouve le mot : %s\n", word);
        end_theme = get_victory();
    } else {
        printf("Vous avez perdu ! Le mot etait : %s\n", word);
        end_theme = get_defeat();
    }
    pid_player = start(play_melody_proc_once, 4000, getprio(getpid()), "Music Player Pendu", &end_theme);
    if (pid_player >= 0) {
        waitpid(pid_player, NULL);
    }
}
