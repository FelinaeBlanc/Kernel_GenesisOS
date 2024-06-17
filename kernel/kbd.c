
#include "kbd.h"
#include "string.h"
#include "stdio.h"
#include "console.h"
#include "processus.h"

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
    (void)leds;
}

