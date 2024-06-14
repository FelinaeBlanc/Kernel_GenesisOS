
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

    while (ptampon < BUFFER_SIZE && i < (int)strlen(str)) {
        char c = str[i];

        if (echo) {
            traite_car(c, BLANC);
        }

        switch (c) {
            case 127: // Backspace
                if (ptampon > 0) {
                    ptampon--;
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
        traite_car('\a', BLANC); // Sonnerie
    }
}
