#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "main.h"

// cores basicas so para este ficheiro
#define RED   "\033[31m"
#define RESET "\033[0m"
#define YELLOW "\033[33m"
    
void printJogadasUtilizador(void){
    printf(YELLOW "  [J]" RESET " Jogar  ");
    printf(YELLOW "  [D]" RESET " Desfazer  ");
    printf(YELLOW "  [S]" RESET " Salvar  ");
    printf(YELLOW "  [Q]" RESET " Sair\n");
    printf("  >> ");
    fflush(stdout);
}

PossiveisJogadas interfacePessoaJogo(GameSettings * gs , MatrizJogo * mj ,LastMoveLL * undoState){
    char jogada = 0;
    PossiveisJogadas estadoJogada = valid;
    printJogadasUtilizador();
    while(getchar() != '\n');
    scanf("%c",&jogada);
    estadoJogada = recebeInput(jogada);
    if(estadoJogada==undo) undoMove(mj,undoState);
    else if(estadoJogada==valid) estadoJogada = efetuaJogadaMovimentoCartas(gs,mj,undoState);
    else if(estadoJogada==invalid) avisaJogadaInvalida();
    return estadoJogada;
}

void gameLoop(GameSettings * gs , MatrizJogo * mj,LastMoveLL * undoState){
    PossiveisJogadas option = valid;
    while(option != quit && option != GameWon ){
        option = interfacePessoaJogo(gs,mj,undoState);
        desenhaInterfaceJogo(gs,*mj);
    }
    if(option==GameWon){
        animacaoVitoria();
    }
    else if(option==save){
        //TODO: salvar o jogo
        printf("(salvar ainda nao implementado...)\n");
    }
    else{
        mensagemSaida();
    }
}


int main(void){
    srand(time(NULL));
    splashScreen();
    GameSettings currentGame = initStructs();
    MatrizJogo mj = initMatrizJogo();
    LastMoveLL undoState = NULL;
    int notSuccess = readFiles(&currentGame,&mj);
    if(notSuccess){
        printf(RED "ERRO! Ficheiro nao encontrado ou invalido.\n" RESET);
        return 1;
    }
    gameLoop(&currentGame,&mj,&undoState);
    memoryFree(currentGame,mj,undoState);
    return 0;
}