
#include "kbd.h"
#include "string.h"
#include "stdio.h"
#include "console.h"
#include "processus.h"
#include "cpu.h"

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
                if (str[i + 1] == '[' && str[i + 2] == 'A' && current_idex_history != 0){
                    strcpy(tampon, history[current_idex_history-1]);
                    ptampon = (int)strlen(history[current_idex_history-1]);
                    current_idex_history--;
                    efface_ligne();
                    printf("%s", tampon);
                }
                else if(str[i + 1] == '[' && str[i + 2] == 'B'){
                    if (index_history > current_idex_history) {
                        current_idex_history++;
                        strcpy(tampon, history[current_idex_history-1]);
                        ptampon = (int)strlen(history[current_idex_history-1]);
                        efface_ligne();
                        printf("%s", tampon);
                    }
                    else {
                        ptampon = 0;
                        efface_ligne();
                    }
                }
                else if(str[i + 1] == '[' && str[i + 2] == 'D'){
                    printf("\b");
                }
                else if(str[i + 1] == '[' && str[i + 2] == 'C'){
                    avance_curseur();
                }
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
                if (c >= 32 && c <= 126) {
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

