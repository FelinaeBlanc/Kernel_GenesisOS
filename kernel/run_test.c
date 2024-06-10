#include "test.h"
#include "console.h"
#include "string.h"
#include "stdint.h"
#include "stdio.h"

void print_succeed(int n){
    int i=0;
    int j=0;
    char str[LARGEUR];
    sprintf(str, "Test succeed %d\n", n);

    while(i*HAUTEUR + j < (int)strlen(str)){
        traite_car(str[i*HAUTEUR + j], VERT);
        j++;
        if (j>=HAUTEUR){
            j = j%HAUTEUR;
            i++;
        }
        if(ligne == HAUTEUR-1){
            defilement();
            ligne --;
        }
    }
}

void print_failed(int n){
    int i=0;
    int j=0;
    char str[LARGEUR];
    sprintf(str, "Test failed %d\n", n);

    while(i*HAUTEUR + j < (int)strlen(str)){
        traite_car(str[i*HAUTEUR + j], ROUGE);
        j++;
        if (j>=HAUTEUR){
            j = j%HAUTEUR;
            i++;
        }
        if(ligne == HAUTEUR-1){
            defilement();
            ligne --;
        }
    }
}


void test_until(int n){
    int t;
    for(int i=1; i<=n; i++){
        t = test_run(i);
        if(t == 0){
        print_succeed(i);
        } else {
        print_failed(i);
        }
    }
}
