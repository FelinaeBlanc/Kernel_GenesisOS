#include "theme.h"
#include "sound_enum.h"
#include "builtin.h"
#include "stdio.h"
#include "utils.h"
#include "theme_lib.h"

int play_melody_proc_loop(void* arg){
    // Theme
    Theme* theme = (Theme*)arg;
    while(1){
        play_melody(theme->melody, theme->nbNotes, theme->tempo);
    }
    return 0;
}
int play_melody_proc_once(void* arg){
    Theme* theme = (Theme*)arg;
    play_melody(theme->melody, theme->nbNotes, theme->tempo);
    return 0;
}


void play_melody(const int melody[], int nbNotes, int tempo) {

    int wholenote = (60000 * 4) / tempo;

    // Itérer sur les notes de la mélodie
    for (int thisNote = 0; thisNote < nbNotes * 2; thisNote = thisNote + 2) {
        
        // Calculer la durée de chaque note
        int divider = melody[thisNote + 1];
        int noteDuration = 0;

        if (divider > 0) {
        // Note régulière
            noteDuration = wholenote / divider;
        } else if (divider < 0) {
        // Note pointée (durée négative)
            noteDuration = wholenote / (-1*divider);
            noteDuration *= 1.5; // Augmenter la durée de moitié pour les notes pointées
        }

        // Jouer le son avec la fréquence et la durée spécifiées
        int frequency = melody[thisNote];
        play_sound(frequency, noteDuration/10);

        // Attendre la durée spécifiée avant de jouer la prochaine note
        wait_clock(current_clock() + noteDuration/10);
    }
}

void cmd_theme(int argc, char *argv[]){
    (void)argc;(void)argv;
    printf("Bienvenue dans le menu des themes!\n");
    printf("Voici la liste des themes disponibles:\n");
    printf("01: Bloody tears\n");
    printf("02: Silentnight\n");
    printf("03: Canon In D\n");
    printf("04: Greenhill\n");
    printf("05: Harry potter\n");
    printf("06: Imperial march\n");
    printf("07: Keyboard cat\n");
    printf("08: Miichannel\n");
    printf("09: Never gonna give you up\n");
    printf("10: Autres\n");

    char input[3];
    cons_read(input, 3);
    printf("\n");
    if (atoi(input) == 10){
        printf("11: Pink panther\n");
        printf("12: Prince Elgor\n");
        printf("13: Professor Layton\n");
        printf("14: Song of storms\n");
        printf("15: Startrek intro\n");
        printf("16: Super Mario\n");
        printf("17: Take On Me\n");
        printf("18: Tetris\n");
        printf("19: The godfather\n");
        printf("20: Vampire Killer\n");
        printf("21: Zelda Lullaby\n");
        printf("22: Zelda Theme\n");
        printf("23: Doom\n");
        printf("24: Game Of Throne\n");
        printf("25: Evangelion\n");
        printf("26: Retour\n");

        printf("Entrez le numero de la musique que vous voulez jouer:\n");
        cons_read(input, 2);
        printf("\n");
    }
    int choice = atoi(input);
    printf("\n");
    if (choice == 26){
        return;
    }


    set_color(VERT_CLAIR);
    printf("Playing... ");

    Theme theme_music;
    int ok = 0;
    switch (choice)
    {
        case 1: // Bloody tears
            set_color(BLEU_CLAIR); printf("Bloody tears\n");
            theme_music = get_bloodytears();
            ok = 1;
            break;
        case 2: // Silentnight
            set_color(BLEU_CLAIR); printf("Silentnight\n");
            theme_music = get_silentnight();
            ok = 1;
            break;
        case 3: // Canon In D
            set_color(BLEU_CLAIR); printf("Canon In D\n");
            theme_music = get_cannonind();
            ok = 1;
            break;
        case 4: // Greenhill
            set_color(VERT); printf("Greenhill\n");
            theme_music = get_greenhill();
            ok = 1;
            break;
        case 5: // Harry potter
            set_color(MAGENTA); printf("Harry Potter\n");
            theme_music = get_harrypotter();
            ok = 1;
            break;
        case 6: // Imperial march
            set_color(ROUGE); printf("Imperial march\n");
            theme_music = get_imperialmarch();
            ok = 1;
            break;
        case 7: // Keyboard cat
            set_color(BLEU); printf("Keyboard cat\n");
            theme_music = get_keyboardcat();
            ok = 1;
            break;
        case 8: // miichannel
            set_color(MAGENTA_CLAIR); printf("Miichannel\n");
            theme_music = get_miichannel();
            ok = 1;
            break;
        case 9: // Never gonna give you up
            set_color(BLEU_CLAIR); printf("Never gonna give you up\n");
            theme_music = get_nvrgonnagiveuup();
            ok = 1;
            ok = 1;
            break;
        case 11: // Pink panther
            set_color(ROUGE_CLAIR); printf("Pink panther\n");
            theme_music = get_pinkpanther();
            ok = 1;
            break;
        case 12: // Prince Elgor
            set_color( ROUGE); printf("Prince Elgor\n");
            theme_music = get_princelgor();
            ok = 1;
            break;
        case 13: // Professorlayton
            set_color(BLANC); printf("Professor Layton\n");
            theme_music = get_professorlayton();
            ok = 1;
            break;
        case 14: // Song of storms
            set_color(GRIS_CLAIR); printf("Song of storms\n");
            theme_music = get_songofstorms();
            ok = 1;
            break;
        case 15: // Startrek intro
            set_color(BLEU_CLAIR); printf("Startrek intro\n");
            theme_music = get_startrek_intro();
            ok = 1;
            break;
        case 16: // Super Mario
            set_color(ROUGE); printf("Super Mario\n");
            theme_music = get_supermario();
            ok = 1;
            break;
        case 17: // Take On Me
            set_color(VERT_CLAIR); printf("Take On Me\n");
            theme_music = get_takeonme();
            ok = 1;
            break;
        case 18: // Tetris
            set_color(JAUNE); printf("Tetris\n");
            theme_music = get_tetris();
            ok = 1;
            break;
        case 19: // The godfather
            set_color(GRIS_FONCE); printf("The godfather\n");
            theme_music = get_thegodfather();
            ok = 1;
            break;
        case 20: // Vampire Killer
            set_color(ROUGE); printf("Vampire Killer\n");
            theme_music = get_vampirekiller();
            ok = 1;
            break;
        case 21: // Zelda Lullaby
            set_color(BLEU_CLAIR); printf("Zelda Lullaby\n");
            theme_music = get_zeldaslullaby();
            ok = 1;
            break;
        case 22: // Zelda Theme
            set_color(VERT); printf("Zelda Theme\n");
            theme_music = get_zeldatheme();
            ok = 1;
            break;
        case 23: // Doom
            set_color(ROUGE); printf("Doom\n");
            theme_music = get_doom();
            ok = 1;
            break;
        case 24: // Game Of Throne
            set_color(GRIS_CLAIR); printf("Game Of Throne\n");
            theme_music = get_gameofthrone();
            ok = 1;
            break;
        case 25:
            set_color(BLANC); printf("Evangelion\n");
            theme_music = get_evangelion();
            ok = 1;
            break;
        default:
            set_color(ROUGE);
            printf("Melodie non reconnue\n");
            ok = 0;
            break;
    }
    if (ok){
        int pid_player = start(play_melody_proc_once, 4000 ,getprio(getpid()) , "Music Player Theme", &theme_music);
        if (pid_player < 0) {
            printf("Impossible de lancer la music...\n");
        } else {
            waitpid(pid_player, NULL);
            printf("Music terminée\n");
            kill(pid_player);
        }
    }
    printf("\n");
    set_color(BLANC);

    return;
}
