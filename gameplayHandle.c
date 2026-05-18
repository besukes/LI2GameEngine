#include "main.h"
#include <stdio.h>
#include <stdlib.h>

void efetuaAutoJogada(GameSettings * gs,MatrizJogo * mj,LastMoveLL * lm){ //NECESSITA SER FEITA

}

int verifyColocarCartas(ArrayFlagsColocar * arr , Carta * ultimaCartaOrig , Carta * cartaDest){
    int valido = 1 , k = arr->numFlagsColocavel , j = 0;
    while(j<k && valido == 1){
        int (*func)(Carta,Carta) = arr->flagsColocavel[j++];
        if (func != NULL) valido = valido && func(*cartaDest,*ultimaCartaOrig);
    }
    return valido;
}


int validaCondicoes(ArrayFlagsPegar * arr , PilhaDeCartas * p , int num){
    int i = p->numCartasPilha - 1, lim = i-num + 1 , max = arr->numFlagsPegavel ,
        verify = 0;
    for(;i>lim && verify == 0;i--){
        verify = 1;
        for(int j=0;j<max && verify == 1;j++){
            int (*func)(Carta,Carta) = arr->flagsPegavel[j];
            if(num > 1 && !arr->variasCartasMoviveis) verify = 0;
            verify = verify && func(*(p->cartasPilha + i - 1),*(p->cartasPilha + i));
        }
    }
    return verify;
}

int validaRestricoes(ArrayFlagsPegar * arr , MatrizJogo * mj , int pilha1){
    int lim = arr->numRestricoes , valido = 1;
    for(int i=0;i<lim && valid == 1;i++){
        int (*func)(int,MatrizJogo *) = arr->flagRestricoes[i];
        valido = valido && func(pilha1,mj);
    }
    return valido;
}

int verificaPegarCartasValido(ArrayFlagsPegar * arr , PilhaDeCartas * p, int num , MatrizJogo * mj , int pilha1){
    if(num==1) return 1;
    else return (validaCondicoes(arr,p,num) && validaRestricoes(arr,mj,pilha1));
}



PossiveisJogadas jogadaValida(MovimentoEntrePilhas * mov , MatrizJogo * mj , int pilha1 , int pilha2 , int num){
    if(mov==NULL) return invalid;
    PilhaDeCartas * pilhaOrig = (mj->linhasMatriz + pilha1) , * pilhaDest = (mj->linhasMatriz + pilha2);
    int novoIndx = pilhaOrig->numCartasPilha - num
        ,ndest = pilhaDest->numCartasPilha - 1,
        max = mov->numMovs;
    ArrayFlagsColocar * arrC = mov->arrC;
    ArrayFlagsPegar * arrP = mov->arrP;
    Boolean validaJogada = 0;
    for(int i=0;i<max && arrC !=NULL && arrP !=NULL && validaJogada==0;i++,arrC++,arrP++){
        validaJogada = verificaPegarCartasValido(arrP,pilhaOrig,num,mj,pilha1) 
                                        && 
            ( pilhaVaziaVerify(mj,pilha2,mov) 
                || verifyColocarCartas(arrC,pilhaOrig->cartasPilha + novoIndx,pilhaDest->cartasPilha + ndest)
            )
        ;
    }
    if(validaJogada) return valid;
    else return invalid;
}


void handleEfetuaJogada(GameSettings * gs , MatrizJogo * mj ,LastMoveLL * lm , int pilha1,int pilha2,int numCartas){
    PilhaDeCartas * pilhaDest = mj->linhasMatriz + pilha2,
                  * pilhaOrig = mj->linhasMatriz + pilha1;
    int numAnt = pilhaDest->numCartasPilha , numNovaPilha = numAnt + numCartas , 
        numOrig = pilhaOrig->numCartasPilha - numCartas;
    pilhaDest->numCartasPilha +=numCartas;
    pilhaDest->cartasPilha = realloc(pilhaDest->cartasPilha,sizeof(struct PilhaDeCartas)*numNovaPilha);
    for(int i=numAnt,j = numOrig ; i<numNovaPilha ;i++,j++){
        *(pilhaDest->cartasPilha + i) = *(pilhaOrig->cartasPilha + j);
    }
    headLinkedList(lm,pilha1,pilha2,numCartas,*mj);
    pilhaOrig->numCartasPilha-=numCartas;
    pilhaOrig->cartasPilha = realloc(pilhaOrig->cartasPilha,sizeof(struct PilhaDeCartas)*numOrig);
}

PossiveisJogadas verificaValidadeDados(int pilha1,int pilha2,int num,MatrizJogo * mj){
    int max = mj->numLinhasMatriz;
    if(pilha1>=max || pilha2>=max || (mj->linhasMatriz + pilha1)->numCartasPilha < num) return invalid;
    else return valid;
}

PossiveisJogadas efetuaJogadaMovimentoCartas(GameSettings * gs , MatrizJogo * mj , LastMoveLL * undoState){
    int pilha1 = 0, pilha2 = 0, numCartas = 0;
    pedeJogadaUtilizador(&pilha1,&pilha2,&numCartas);
    PossiveisJogadas estadoJogada = verificaValidadeDados(pilha1,pilha2,numCartas,mj);
    long tag1 = procuraTag(mj,pilha1) , tag2 = procuraTag(mj,pilha2);
    if(tag1!=(-1) && tag2!=(-1) && estadoJogada == valid){
        MovimentoEntrePilhas * mov = comparaTags(gs->jogo.movimentoPilhas,tag1,tag2,gs->jogo.numCondicoesMov);
        estadoJogada = jogadaValida(mov,mj,pilha1,pilha2,numCartas);
        if(estadoJogada == valid){
            handleEfetuaJogada(gs,mj,undoState,pilha1,pilha2,numCartas);
            efetuaAutoJogada(gs,mj,undoState);
        }
    }
    else estadoJogada = invalid;
    return estadoJogada;
}




void pedeJogadaUtilizador(int * p1 , int * p2 ,int * num){
    printf("Insira o número da pilha à qual quer ir buscar cartas\n");
    scanf("%d",p1);
    printf("Insira o número de cartas que quer pegar\n");
    scanf("%d",num);
    printf("Insira a pilha onde quer colocar essas cartas\n");
    scanf("%d",p2);
}



PossiveisJogadas recebeInput(char jogada){
    switch(jogada){
        case 'Q' :
            return quit;
        break;
        case 'D' : 
            return undo;
        break;
        case 'S' :
            return save;
        break;
        case 'J' :
            return valid;
        break;
        default :
            printf("Inseriste uma jogada inválida , portanto o jogo fechou.");
            return quit;
        break;
    }
}