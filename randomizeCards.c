#include "main.h"
#include <stdlib.h>
#include <time.h>

void initMatrizCartasJogo(int arrayCartas[],MatrizJogo * mj){
    int i,j,counter=0;
    int n = mj->numLinhasMatriz;
    for(i=0;i<n;i++){
        PilhaDeCartas * p = mj->linhasMatriz + i;
        int maxPilha = p->numCartasPilha;
        for(j=0 ;j  < maxPilha ; j++ , counter++){
            Carta * c = p->cartasPilha + j ;
            c->naipe = convertToNaipe((arrayCartas[counter] -1)/13);
            c->valor = valorCarta(arrayCartas[counter]);
        }
    }
}


void shuffleArray(int arrayCartas[] , int n){
    for(int i = 0 ; i < n*52 ; i++){
        int j = rand() % (n*52);
        int temp = arrayCartas[i];
        arrayCartas[i] = arrayCartas[j];
        arrayCartas[j] = temp;
    }
}

void initArrCartas(int arrCartas[] , int n){
    for(int i=0;i<n*52;i++){
        arrCartas[i] = i + 1;
    }
}

void randomizaJogo(GameSettings * gs , MatrizJogo * mj){
    int n = gs->numBaralhos , arrCartas[n*52];
    initArrCartas(arrCartas,n);
    shuffleArray(arrCartas,n);
    initMatrizCartasJogo(arrCartas,mj);
}



