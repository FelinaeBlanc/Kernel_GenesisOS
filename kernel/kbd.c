
#include "kbd.h"
#include "string.h"
#include "stdio.h"
#include "console.h"

char tampon[BUFFER_SIZE];
int ptampon = 0;

void keyboard_data(char *str){
	// on remplie le tampon ici
	int i = 0;

	// recopie les nouveaux caractères disponibles dans le tampon associé au clavier.
	while(ptampon < BUFFER_SIZE && i < (int)strlen(str)) {
		// printf("Prout %d\n",(int)str[i]);
		traite_car(str[i], VERT);
		tampon[ptampon] = str[i];
		i++;
		ptampon ++;

	}
	if(ptampon == BUFFER_SIZE) {
		printf("-BIP-");
		return;
	}
	return;
}
