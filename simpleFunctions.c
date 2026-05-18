#include "main.h"
#include <stdio.h>
#include <stdlib.h>

int valorCarta(int c){
    for(;c>13;c-=13);
    return c;
}

int exponenciacao(int base,int expo){
    int ret=1;
    for(int i=0;i<expo;i++){
        ret*=base;
    }
    return ret;
}

char * criarTag(unsigned long * tag,char * line){
    int j=0;
    *tag = 0;
    while(*line != '#' && *line!='\n' && *line!=' ' && *line != '\0'){
        *tag+=(*line - 64)*exponenciacao(27,j); //-64 por serem letras maisculas ('A' = 65)
        // Multiplicamos por 27^j para cada tag ser unica (26 letras no alfabeto e apenas letras maiusculas)
        line++;
        j++;
    }
    return (line+1);
}

char * procuraNomeString(char * line){
    int i;
    char * temp = line , * nome = NULL , * head;
    //Para verificar o tamanho do nome do jogo
    for(i=0;*temp != '#' && *temp!='\n' && *temp!=' '&& *line!='\0';i++,temp++);
    nome = malloc(sizeof(char)*i + 1);
    head = nome;
    for(;*line != '#' && *line!='\n' && *line!=' ' && *line!='\0';nome++,line++){
        *nome=*line;
    }
    *nome = '\0';
    return head;
}


int numCaracteres(char * line){
    int i=(-1);
    while(*line != '#' && *line!='\n' && *line!=' ' && *line != '\0'){
        i++;
        line++;
    }
    return i;
}

int strToNumber(char * line){
    int i=numCaracteres(line) , num = 0;
    while(i > (-1) && *line != '#' && *line!='\n' && *line!=' ' && *line != '\0'){
        num += (*line-48)*exponenciacao(10,i);
        i--;
        line++;
    }
    return num;
}

void calculaRulesPilha(RegrasPilha * rp , char * line){
    rp->cartaTopoVisivel=0;
    rp->existeMaxCartas=0;
    rp->todaPilhaVisivel=0;
    while(*line != '#' && *line!='\n' && *line!=' ' && *line != '\0'){
        if(*line == '=') rp->todaPilhaVisivel = 1;
        else if(*line == '^') rp->cartaTopoVisivel = 1;
        else if(*line == '1') rp->existeMaxCartas = 1;
        line++;
    }
}


MovimentoEntrePilhas * comparaTags (MovimentoEntrePilhas * mp ,unsigned long tagOrig ,unsigned long tagDest , int n)
{
    if(mp == NULL) return mp;
    for (int i = 0; i < n; i++) {
        if ((mp+i)->tagOrig == tagOrig && (mp+i)->tagDest == tagDest) {
            return (mp+i);
        }
    }
    return NULL;
}


unsigned long procuraTag(MatrizJogo * mj , int pilha){
    if(pilha>mj->numLinhasMatriz) return -1;
    else return ((mj->linhasMatriz + pilha)->tagPilha);
}



char convertToNaipe(int div){
    switch(div){
        case 0 : 
            return 'C';
        break;
        case 1 :
            return 'D';
        break;
        case 2 :
            return 'H';
        break;
        default :
            return 'S';
        break;
    }
}

int pertenceString(char c , char str[]){
    for(int i=0;str[i] != '\0';i++){
        if(c==str[i]) return 1;
    }
    return 0;
}


int pilhaVaziaVerify(MatrizJogo * mj , int pilhaDest , MovimentoEntrePilhas * mov){
    int n = mj->numLinhasMatriz;
    int pilhaValida = n > pilhaDest && n > 0 ;
    int colocaVazia = mov!=NULL && mov->colocaEmPilhaVazia != NULL ;
    int pilhaVazia = (mj->linhasMatriz + pilhaDest)->numCartasPilha == 0 ;
    return(pilhaValida && pilhaVazia && colocaVazia);
}
