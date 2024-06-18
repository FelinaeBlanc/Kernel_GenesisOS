#include "theme.h"
#include "sound_enum.h"
#include "builtin.h"
#include "stdio.h"
#include "utils.h"
#include "theme_lib.h"




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
    if (argc != 2) {
        printf("Utilisation: theme <num>\n");
        return;
    }
    
    set_color(VERT_CLAIR);
    printf("Playing... ");

    switch (atoi(argv[1]))
    {
        case 1: // Bloody tears
            set_color(BLEU_CLAIR); printf("Bloody tears\n");
            play_melody(bloodytears_melody, bloodytears_nbNotes, bloodytears_tempo);
            break;
        case 2: // Silentnight
            set_color(BLEU_CLAIR); printf("Silentnight\n");
            play_melody(silentnight_melody, silentnight_nbNotes, silentnight_tempo);
            break;
        case 3: // Canon In D
            set_color(BLEU_CLAIR); printf("Canon In D\n");
            play_melody(cannonind_melody, cannonind_nbNotes, cannonind_tempo);
            break;
        case 4: // Greenhill
            set_color(VERT); printf("Greenhill\n");
            play_melody(greenhill_melody, greenhill_nbNotes, greenhill_tempo);
            break;
        case 5: // Harry potter
            set_color(MAGENTA); printf("Harry Potter\n");
            play_melody(harrypotter_melody,harrypotter_nbNotes,harrypotter_tempo);
            break;
        case 6: // Imperial march
            set_color(ROUGE); printf("Imperial march\n");
            play_melody(imperialmarch_melody,imperialmarch_nbNotes,imperialmarch_tempo);
            break;
        case 7: // Keyboard cat
            set_color(BLEU); printf("Keyboard cat\n");
            play_melody(keyboardcat_melody,keyboardcat_nbNotes,keyboardcat_tempo);
            break;
        case 8: // miichannel
            set_color(MAGENTA_CLAIR); printf("Miichannel\n");
            play_melody(miichannel_melody,miichannel_nbNotes,miichannel_tempo);
            break;
        case 9: // Never gonna give you up
            set_color(BLEU_CLAIR); printf("Never gonna give you up\n");
            play_melody(nvrgonnagiveuup_melody,nvrgonnagiveuup_nbNotes,nvrgonnagiveuup_tempo);
            break;
        case 10: // Pink panther
            set_color(ROUGE_CLAIR); printf("Pink panther\n");
            play_melody(pinkpanther_melody,pinkpanther_nbNotes,pinkpanther_tempo);
            break;
        case 11: // Prince Elgor
            set_color( ROUGE); printf("Prince Elgor\n");
            play_melody(princelgor_melody,princelgor_nbNotes,princelgor_tempo);
            break;
        case 12: // Professorlayton
            set_color(BLANC); printf("Professor Layton\n");
            play_melody(professorlayton_melody,professorlayton_nbNotes,professorlayton_tempo);
            break;
        case 13: // Song of storms
            set_color(GRIS_CLAIR); printf("Song of storms\n");
            play_melody(songofstorms_melody,songofstorms_nbNotes,songofstorms_tempo);
            break;
        case 14: // Startrek intro
            set_color(BLEU_CLAIR); printf("Startrek intro\n");
            play_melody(startrek_intro_melody,startrek_intro_nbNotes,startrek_intro_tempo);
            break;
        case 15: // Super Mario
            set_color(ROUGE); printf("Super Mario\n");
            play_melody(supermario_melody,supermario_nbNotes,supermario_tempo);
            break;
        case 16: // Take On Me
            set_color(VERT_CLAIR); printf("Take On Me\n");
            play_melody(takeonme_melody,takeonme_nbNotes,takeonme_tempo);
            break;
        case 17: // Tetris
            set_color(JAUNE); printf("Tetris\n");
            play_melody(tetris_melody,tetris_nbNotes,tetris_tempo);
            break;
        case 18: // The godfather
            set_color(GRIS_FONCE); printf("The godfather\n");
            play_melody(thegodfather_melody,thegodfather_nbNotes,thegodfather_tempo);
            break;
        case 19: // Vampire Killer
            set_color(ROUGE); printf("Vampire Killer\n");
            play_melody(vampirekiller_melody,vampirekiller_nbNotes,vampirekiller_tempo);
            break;
        case 20: // Zelda Lullaby
            set_color(BLEU_CLAIR); printf("Zelda Lullaby\n");
            play_melody(zeldaslullaby_melody,zeldaslullaby_nbNotes,zeldaslullaby_tempo);
            break;
        case 21: // Zelda Theme
            set_color(VERT); printf("Zelda Theme\n");
            play_melody(zeldatheme_melody,zeldatheme_nbNotes,zeldatheme_tempo); 
            break;
        case 22: // Doom
            set_color(ROUGE); printf("Doom\n");
            play_melody(doom_melody,doom_nbNotes,doom_tempo);
            break;
        case 23: // Game Of Throne
            set_color(GRIS_CLAIR); printf("Game Of Throne\n");
            play_melody(gameofthrone_melody,gameofthrone_nbNotes,gameofthrone_tempo);
            break;
        default:
            printf("Melodie non reconnue\n");
            break;
    }

    printf("\n");
    set_color(BLANC);

    return;
}
