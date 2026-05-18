#include "main.h"
#include <stdlib.h>

/////////////////////////////////////////////////Funcoes de ver se as cartas sao pegaveis////////////////////////////////////

int pilhaVazia(int linha,MatrizJogo * m){
    int n = m->numLinhasMatriz;
    if(n == 0 || linha >= n) return 0;
    else return ((m->linhasMatriz + linha)->numCartasPilha == 0);
}

int cartaTopoAs(int linha,MatrizJogo * m){
    int n = m->numLinhasMatriz;
    if(n == 0 || linha >= n) return 0;
    else{
        int num = (m->linhasMatriz + linha)->numCartasPilha;
        return (((m->linhasMatriz + linha)->cartasPilha + num)->valor == 1 );
    }
}

int cartaTopoRei(int linha,MatrizJogo * m){
    int n = m->numLinhasMatriz;
    if(n == 0 || linha >= n) return 0;
    else{
        int num = (m->linhasMatriz + linha)->numCartasPilha;
        return (((m->linhasMatriz + linha)->cartasPilha + num)->valor == 13 );
    }
}

int cartaFundoAs(int linha,MatrizJogo * m){
    int n = m->numLinhasMatriz;
    if(n == 0 || linha >= n) return 0;
    else{
        PilhaDeCartas * pilha = (m->linhasMatriz + linha);
        int num = pilha->numCartasPilha;
        return ( num > 0 && (pilha->cartasPilha)->valor == 1 );
    }
}

int cartaFundoRei(int linha,MatrizJogo * m){
    int n = m->numLinhasMatriz;
    if(n == 0 || linha >= n) return 0;
    else{
        PilhaDeCartas * pilha = (m->linhasMatriz + linha);
        int num = pilha->numCartasPilha;
        return ( num > 0 && (pilha->cartasPilha)->valor == 13 );
    }
}

int diferentesCores(Carta carta1,Carta carta2){
    if(carta1.naipe == 'D' || carta1.naipe == 'H'){
        return (carta2.naipe == 'C' || carta2.naipe == 'S');
    }
    else return (carta2.naipe == 'D' || carta2.naipe == 'H');
}

int mesmaCor(Carta carta1,Carta carta2){
    if(carta1.naipe == 'D' || carta1.naipe == 'H'){
        return (carta2.naipe == 'D' || carta2.naipe == 'H');
    }
    else return (carta2.naipe == 'C' || carta2.naipe == 'S');
}

int alternadosNaipes(Carta carta1,Carta carta2){
    return(carta1.naipe != carta2.naipe); // !mesmoNaipe
}


int mesmoNaipe(Carta carta1,Carta carta2){
    return (carta1.naipe == carta2.naipe);
}

int flagCrescente(Carta carta1,Carta carta2){
    return (carta1.valor - carta2.valor == -1);
}

int flagDecrescente(Carta carta1,Carta carta2){
    return (carta1.valor - carta2.valor == 1);
}
int flagDifpor1(Carta carta1 , Carta carta2){
    return(carta1.valor-carta2.valor == 1 || carta1.valor-carta2.valor == (-1));
}

int sempreMovivel(Carta c1,Carta c2){
    return 1;
}


///////////////////////////////////////////////Funcoes que calculam a funcao correspondente a uma flag/////////////////////
FlagFunctionsR flagRestricoesCalc(char * line){
    switch(*line){
        case 'a' :
            return &cartaTopoAs;
        break;
        case 'A' :
            return &cartaFundoAs;
        break;
        case 'k' : 
            return &cartaTopoRei;
        break;
        case 'K' :
            return &cartaFundoRei; 
        break;
        default:
            return NULL;
        break;
    }
}


FlagFunctionsP flagPegavelCalcAux(char * line){
    switch(*line){
        case 'm' :
            return &mesmoNaipe;
        break;
        case 'x' :
            return &alternadosNaipes;
        break;
        case 'c' :
            return &mesmaCor;
        break;
        case 'd' : 
            return &diferentesCores;
        break;
        default:
            return NULL;
        break;
    }
}

FlagFunctionsP flagPegavelCalc(MovimentoEntrePilhas * mov ,char * line){
    switch(*line){
        case '*' :
            return &sempreMovivel;
        break;
        case '+' :
            (mov->arrP + mov->numMovs - 1)-> variasCartasMoviveis = 1;
            return &sempreMovivel;
        break;
        case '[' :
            return &flagDecrescente;
        break;
        case ']' :
            return &flagCrescente;
        break;
        default:
            return flagPegavelCalcAux(line);
        break;
    }
}


FlagFunctionsP flagColocavelCalcAux(char * line){
    switch(*line){
        case 'C' :
            return &mesmaCor ;
        break;
        case 'D' :
            return &diferentesCores;
        break;
        default:
            return NULL;
        break;
    }
}

FlagFunctionsC flagColocavelCalc(char * line){
    switch(*line){
        case '>' :
            return &flagCrescente;
        break;
        case '<' :
            return &flagDecrescente;
        break;
        case '~' :
            return &flagDifpor1;
        break;
        case 'M' :
            return &mesmoNaipe;
        break;
        case 'X' :
            return &alternadosNaipes;
        break;
        default:
            return flagColocavelCalcAux(line);
        break;
    }
}
