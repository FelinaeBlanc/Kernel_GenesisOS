
#include "kbd.h"
#include "string.h"
#include "stdio.h"
#include "console.h"
#include "processus.h"

char tampon[BUFFER_SIZE];
int ptampon = 0;
bool read=false;

void keyboard_data(char *str){
	// on remplie le tampon ici
	int i = 0;

	// recopie les nouveaux caractères disponibles dans le tampon associé au clavier.
	while(ptampon < BUFFER_SIZE && i < (int)strlen(str)) {
		// printf("Prout %d\n",(int)str[i]);
		// on gere ici le echo 
		// traite_car(str[i], VERT);

		char c = str[i];

		if(echo){
			traite_car(c, BLANC);
		}

		switch (c)
		{
		case 127:
			if (ptampon%LARGEUR != 0){
				ptampon--;
			}
			break;
		case '\t':
			tampon[ptampon] = str[i];
			ptampon++;
			verifie_es();
			break;
		
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
			if (c <= 126 && c >= 32){
				tampon[ptampon] = str[i];
				ptampon++;
				verifie_es();
			}
			break;
		}
		i++;
	}

	if(ptampon == BUFFER_SIZE) {
		verifie_es();
		traite_car('\a', BLANC);
		return;
	}

	return;
}
