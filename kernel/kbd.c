
#include "kbd.h"
#include "string.h"
#include "stdio.h"
#include "console.h"
#include "processus.h"
#include "cpu.h"
#include "sound.h"

char tampon[BUFFER_SIZE];
int ptampon = 0;
bool read=false;

void keyboard_data(char *str) {
    int i = 0;
    while (i < (int)strlen(str) && (ptampon < BUFFER_SIZE || str[i] == 127)) {
        char c = str[i];

        if (echo) {
            traite_car(c);
        }

        switch (c) {
            case '\x1b':
                if(i >= (int)strlen(str) - 2) break;
                if(str[i+1]!='[') break;

                switch (str[i+2])
                {
                case 'A':
                    if(current_idex_history == 0) break; // si on a pas d'historique on skip

                    strcpy(tampon, history[current_idex_history-1]);
                    ptampon = (int)strlen(history[current_idex_history-1]);
                    current_idex_history--;
                    efface_ligne();
                    printf("%s", tampon);
                    break;
                
                case 'B':
                    if (index_history > current_idex_history) { // si on est pas au bout de la liste
                        current_idex_history++;
                        strcpy(tampon, history[current_idex_history-1]);
                        ptampon = (int)strlen(history[current_idex_history-1]);
                        efface_ligne();
                        printf("%s", tampon);
                    }
                    else { // si on arrive au bout de l'historique
                        ptampon = 0;
                        efface_ligne();
                    }
                    break;

                case 'C':
                    avance_curseur();
                    break;
                
                case 'D':
                    printf("\b");
                    break;

                case '5':
                    defillement_haut();
                    i++;
                    break;

                case '6':
                    defillement_bas();
                    i++;
                    break;

                case '[':
                    // pour les caractère F1 F2...
                    if(i >= (int)strlen(str) - 3) break;
                    switch (str[i+3])
                    {
                    case 'A':
                        // on check si l'élu n'est pas shell 0
                        if(ProcElu == tableProcShell[0]) break;
                        // si le shell existe alors aller dessus
                        if(tableDesProcs[0]!=NULL){
                            
                            ProcElu->etat = ACTIVABLE;
                            ordonnanceur();
                            switch_console(0);
                        }
                        break;
                    case 'B':
                        if(ProcElu == tableProcShell[1]) break;
                        if(tableDesProcs[1]!=NULL){
                            ProcElu->etat = ACTIVABLE;
                            ordonnanceur();
                        }
                        break;
                    case 'C':
                        if(ProcElu == tableProcShell[2]) break;
                        if(tableDesProcs[2]!=NULL){
                            //ProcElu = tableDesProcs[2];
                        }
                        break;
                    case 'D':
                        if(ProcElu == tableProcShell[3]) break;
                        if(tableDesProcs[3]!=NULL){
                            //ProcElu = tableDesProcs[3];
                        }
                        break;
                    case 'E':
                        if(ProcElu == tableProcShell[4]) break;
                        if(tableDesProcs[4]!=NULL){
                            //ProcElu = tableDesProcs[4];
                        }
                        break;
                    default:
                        break;
                    }
                    break;
                    i+=2; //pour skip 3 caractère au lieu de 2
                default:
                    break;
                }
                // on skip les 2 prochains caractères
                i+=2;
            break;
            case 127: // Backspace
                if (ptampon > 0) {
                    ptampon--;
                }
                else {
                    traite_car('\a');
                }
                break;
            case '\t':
            case '\n':
                tampon[ptampon] = str[i];
				ptampon++;
				verifie_es();
				break;
			case '\r':
                read = true;
                verifie_es();
				break;
            default:
                if(c<32){
                    if(64 + c=='C'){ 
                        nosound();
                        kill(getpid());
                    }
                }
                else if (c >= 32 && c <= 126) {
                    tampon[ptampon++] = c;
                    verifie_es();
                }
                break;
        }
        i++;
    }

    if (ptampon == BUFFER_SIZE) {
        verifie_es();
        traite_car('\a'); // Sonnerie
    }
}

void kbd_leds(unsigned char leds){
    (void)leds ;
    outb(0x60, 0xED);
    //udelay(100);
    outb(0x60, 0x06);
}

