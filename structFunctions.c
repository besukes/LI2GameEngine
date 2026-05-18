#include "main.h"
#include <stdlib.h>

void initializeBothArrays(MovimentoEntrePilhas * mov){
    int n = ++mov->numMovs;
    mov->arrC = realloc(mov->arrC,sizeof(struct FlagFuncArrayC)*n);
    mov->arrP = realloc(mov->arrP , sizeof(struct FlagFuncArrayP)*n);
    ArrayFlagsColocar * arrC = mov->arrC + n - 1;
    ArrayFlagsPegar * arrP = mov->arrP + n - 1;
    arrC->numFlagsColocavel=0;
    arrP->numFlagsPegavel = 0;
    arrP->numRestricoes = 0;
    arrP->variasCartasMoviveis = 0;
}


void initJogoStruct(GameSettings * gs){
    (gs->jogo).nomeJogo=NULL;
    (gs->jogo).numPilhas=0;
    (gs->jogo).numCondicoesMov=0;
    (gs->jogo).movimentoPilhas=NULL;
    (gs->jogo).pilhas=NULL;
}

void initFileStruct(GameSettings * gs){
    (gs->file).tamanhoMemoria=0;
    (gs->file).tamanhoReal=0;
    (gs->file).instructions=NULL;
}

GameSettings initStructs(void){
    GameSettings gs ;
    initFileStruct(&gs);
    gs.numBaralhos=0;
    initJogoStruct(&gs);
    gs.numCondicoesVitoria=0;
    gs.winCon = NULL;
    gs.jogo.qntdAutoMoves = 0;
    gs.jogo.autoMoves = NULL;
    return gs;
}

MatrizJogo initMatrizJogo(void){
    MatrizJogo mj;
    mj.numLinhasMatriz=0;
    mj.linhasMatriz = NULL;
    return mj;
}

void addCartasLinkedList(LastMoveLL novo , int indx , int num , MatrizJogo m){
    int i = (m.linhasMatriz + indx)->numCartasPilha;
    Carta * pilha = (m.linhasMatriz + indx)->cartasPilha;
    for(int j=i-num , counter = 0 ; j<i ; j++ , counter++){
        (novo->cartasMovidas + counter)->naipe = (pilha + j)->naipe;
        (novo->cartasMovidas + counter)->valor = (pilha + j)->valor;
    }
}


void headLinkedList(LastMoveLL * l , int indexAnterior, int indexNovo, int numCartasMovidas , MatrizJogo matriz){
    LastMoveLL novo = malloc(sizeof(struct LastMoveLLNode));
    novo->indexPilhaAnterior = indexAnterior;
    novo->indexNovaPilha = indexNovo;
    novo->numCartasMovidas = numCartasMovidas;
    novo->prox = *l;
    novo->cartasMovidas = malloc(sizeof(struct Carta)*numCartasMovidas);
    addCartasLinkedList(novo,indexAnterior,numCartasMovidas,matriz);
    *l=novo;
}


void initColunasMatriz(MatrizJogo * mj){
    int lim = mj->numLinhasMatriz;
    PilhaDeCartas * p = mj->linhasMatriz;
    if(p!=NULL){
        for(int i=0;i<lim;i++){
            (p+i)->cartasPilha = malloc(sizeof(struct Carta)*(p+i)->numCartasPilha);
        }
    }
}