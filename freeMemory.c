#include "main.h"
#include <stdlib.h>


void freeMatrizJogo(MatrizJogo mj){
    int p = mj.numLinhasMatriz;
    for(int i=0;i<p;i++){
        free((mj.linhasMatriz + i)->cartasPilha);
    }
    free(mj.linhasMatriz);
}

void freeAutoMoves(JogoStruct jogo,int n){
    AutoMoves * tempo = jogo.autoMoves;
    for(int i=0;i<n;i++,tempo++){
        free(tempo->arrC);
        free(tempo->arrP);
    }
    free(jogo.autoMoves);
}

void freeGameSettings(GameSettings gs){
    free(gs.winCon);
    JogoStruct jogo = gs.jogo;
    free(jogo.pilhas);
    int n = jogo.numCondicoesMov , n1 = jogo.qntdAutoMoves;
    MovimentoEntrePilhas * temp = jogo.movimentoPilhas;
    for(int i=0;i<n;i++,temp++){
        free(temp->arrP);
        free(temp->arrC);
    }
    free(jogo.movimentoPilhas);
    freeAutoMoves(jogo,n1);
}



void freeLinkedList(LastMoveLL lm){
    while(lm!=NULL){
        LastMoveLL temp = lm;
        lm=lm->prox;
        Carta * c = temp->cartasMovidas;
        if(c!=NULL) free(c);
        free(temp);
    }
}

void memoryFree(GameSettings gs , MatrizJogo mj , LastMoveLL lm){
    freeLinkedList(lm);
    freeGameSettings(gs);
    freeMatrizJogo(mj);
    free(gs.file.instructions);
}