#include "main.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int initEVerificaTipoFlag(MovimentoEntrePilhas * mov , char * line){
    char str1[8] = "*[]mxcd" , str2[7]= "<>MXCD" , str3[5] = "aAkK";
    if(pertenceString(*line,str1)){
        mov->numMovsP++;
        initArrP(mov);
        return 1;
    }
    else{
        if(pertenceString(*line,str2)){
            mov->numMovsC++;
            initArrC(mov);
            return 2;
        }
        else if(pertenceString(*line,str3)){
            mov->numMovsP++;
            initArrP(mov);
            return 3;
        }
        else return 4;
    }
}


void addMovInstruction(MovimentoEntrePilhas * mov,long tagDestino , char * line){
    while(*line != '#' && *line!='\n' && *line!=' '&& *line!='\0'){
        int u = initEVerificaTipoFlag(mov,line);
        int n1= mov->numMovsC , n2 = mov->numMovsP;
        if(u==1) (mov->arrP + n2 - 1)->flagsPegavel[mov->arrP->numFlagsPegavel ++] = flagPegavelCalc(line);
        else if(u==2)  (mov->arrC + n1 - 1)->flagsColocavel[mov->arrC->numFlagsColocavel ++] = flagColocavelCalc(line);
        else if(u==3) (mov->arrP + n2 - 1)->flagRestricoes[mov->arrP->numRestricoes ++] = flagRestricoesCalc(line);
        else if(u==4) mov->colocaEmPilhaVazia = &pilhaVazia;
        line++;
    }
}

void movInstruction(GameSettings * gs, char * line)
{
    long tagOrigem = 0 , tagDestino = 0;
    line = criarTag(&tagOrigem , line);
    line = criarTag(&tagDestino , line);
    int n = ++gs->jogo.numCondicoesMov;
    //Agora line aponta para as flags
    MovimentoEntrePilhas * existeregra = comparaTags(gs->jogo.movimentoPilhas, tagOrigem, tagDestino,n);
    if(existeregra == NULL){ //Verificar se ja existem movimentos entre estas duas pilhas guardadas
        gs->jogo.movimentoPilhas = realloc(gs->jogo.movimentoPilhas,sizeof(struct MovimentoEntrePilhas)*n);
        existeregra = gs->jogo.movimentoPilhas + n - 1;
        existeregra->numMovsC = 0;
        existeregra->numMovsP=0;
        existeregra->tagOrig = tagOrigem;
        existeregra->tagDest = tagDestino;
        existeregra->colocaEmPilhaVazia = NULL;
    }
    addMovInstruction(existeregra,tagDestino,line);
}

void autoInstruction(GameSettings * gs, char * line)
{
    long tagOrigem =0 , tagDestino = 0;
    line = criarTag(&tagOrigem , line);
    line = criarTag(&tagDestino , line);
    int n = ++gs->jogo.qntdAutoMoves;
    //Agora line aponta para as flags
    AutoMoves * existeregra = comparaTags(gs->jogo.movimentoPilhas, tagOrigem, tagDestino,n);
    if(existeregra == NULL){ //Verificar se ja existem movimentos entre estas duas pilhas guardadas
        gs->jogo.autoMoves = realloc(gs->jogo.movimentoPilhas,sizeof(struct MovimentoEntrePilhas)*n);
        existeregra = gs->jogo.autoMoves + n - 1;
        existeregra->numMovsC = 0;
        existeregra->numMovsP=0;
        existeregra->tagOrig = tagOrigem;
        existeregra->tagDest = tagDestino;
        existeregra->colocaEmPilhaVazia = NULL;
    }
    addMovInstruction(existeregra,tagDestino,line);
}

void tipoInstruction(GameSettings * gs , char * line){
    int n = ++gs->jogo.numPilhas;
    gs->jogo.pilhas = realloc(gs->jogo.pilhas,sizeof(struct RegrasPilha)*n);
    long tag=0;
    char * nomeDaPilha = procuraNomeString(line);
    line = criarTag(&tag,line);
    RegrasPilha * pilhaAtual = gs->jogo.pilhas + n - 1;
    pilhaAtual->tag = tag;
    pilhaAtual->nomePilha = nomeDaPilha;
    calculaRulesPilha(pilhaAtual,line);
}

void initInstruction(GameSettings * gs , char * line,MatrizJogo * mj){
    long insTag=0;
    int num = ++mj->numLinhasMatriz;
    line = criarTag(&insTag,line);
    mj->linhasMatriz = realloc(mj->linhasMatriz,sizeof(struct PilhaDeCartas)*num);
    if(mj->linhasMatriz == NULL) printf("Realloc failed in initInstruction\n");
    PilhaDeCartas * p = mj->linhasMatriz + num - 1;
    p->tagPilha = insTag;
    p->cartasPilha = NULL;
    p->numCartasPilha = strToNumber(line);
}

void winInstruction(GameSettings * gs , char * line){
    gs->numCondicoesVitoria++;
    int n = gs->numCondicoesVitoria , num = 0;
    gs->winCon = realloc(gs->winCon,sizeof(struct WinCondition)*gs->numCondicoesVitoria);
    long tag = 0;
    line = criarTag(&tag,line);
    num = strToNumber(line);
    WinCondition * wn = gs->winCon ;
    (wn + n - 1)->tagPilha = tag; 
    (wn + n - 1)->numeroVitoriaPilha = num;
}


void baralhoInstruction(GameSettings * gs , char * line){
    int baralhos=strToNumber(line);
    gs->numBaralhos=baralhos;
}

void jogoInstruction(GameSettings * gs,char * line){
    gs->jogo.nomeJogo = procuraNomeString(line);
}


void readInstructionLineAux(GameSettings * gs, char * line){
    switch(*line){
        case 'W' : 
            winInstruction(gs,line+4);
        break;
        case 'B' :
            baralhoInstruction(gs,line+9);
        break;
        case 'J' :
            jogoInstruction(gs,line+5);
        break;
        default: // Se a linha for vazia nao faz nada (i.e, comeca com espaco)
        break;
    }
}

void readInstructionLine(GameSettings * gs , char * line , MatrizJogo * mj){
    switch(*line){
        case 'M' :
            movInstruction(gs,line+4);
        break;
        case 'A' :
            autoInstruction(gs,line+5);
        break;
        case 'T' :
            tipoInstruction(gs,line+5);
        break;
        case 'I' :
            initInstruction(gs,line+5,mj);
        default: // Apenas para reduzir instrucoes
            readInstructionLineAux(gs,line);
    }
}

void readInstructions(GameSettings * gs , struct dirent * entry,MatrizJogo * mj){
    char path[267];
    snprintf(path,sizeof(path),"paciencias/%s",entry->d_name);
    FILE * file = fopen(path,"r");
    char line[100];
    while(fgets(line,sizeof(line),file)){
        readInstructionLine(gs,line,mj);
    }
    fclose(file);
}

int readGameInstructions(GameSettings * gs , char str[51] , MatrizJogo * mj){
    struct dirent * entry;
    DIR * dir = opendir("paciencias");
    int found=0;
    while((entry = readdir(dir)) != NULL && !found){
        if(strcmp(entry->d_name,str) == 0){
            found=1;
            readInstructions(gs,entry,mj);
        }
    }
    closedir(dir);
    return (!found);
}

void loadJogoAntigo(MatrizJogo * mj , int index , char * line){
    
}

int readGameFiles(GameSettings * gs , MatrizJogo * mj , struct dirent * entry){
    int i=0 , found=0;
    char path[267] , line[100];
    snprintf(path,sizeof(path),"paciencias/%s",entry->d_name);
    FILE * file = fopen(path,"r");
    while(fgets(line,sizeof(line),file)){
        loadJogoAntigo(mj,i++,line);
    }
    fclose(file);
    return (!found);
}


int readExistingGame(GameSettings * gs , char regrasJogo[51] , char loadJogo[51] , MatrizJogo * mj){
    int found = readGameInstructions(gs,regrasJogo,mj);
    initColunasMatriz(mj);
    struct dirent * entry;
    DIR * dir = opendir("paciencias");
    while((entry = readdir(dir)) != NULL && !found){
        if(strcmp(entry->d_name,loadJogo) == 0){
            found = found & 1;
            readGameFiles(gs,mj,entry);
        }
    }
    closedir(dir);
    return (!found);
}


int existingGame(GameSettings * gs , char str1[51] , char str2[51] , MatrizJogo * mj){
    printf("Insira o nome do jogo guardado que quer utilizar : \n");
    scanf(" %50s",str2);
    return readExistingGame(gs,str1,str2,mj);
}

int readFiles(GameSettings * gs,MatrizJogo * mj){
    int op , ret = 0 ;
    char str[51] ,str2[51];
    printf("Olá jogador.Para continuar insira o número da opção que melhor descreve oque quer fazer :\n"
            "1- LOAD NEW GAME\n"
            "2- START NEW GAME\n"
            "DEFAULT - START NEW GAME\n");
    scanf("%d",&op);
    printf("Insira o nome do ficheiro do qual pretende fazer uso para jogar (máximo 50 caracteres): \n");
    scanf(" %50s",str);
    switch(op){
        case 1 :
            ret = existingGame(gs,str,str2,mj);
        break;
        case 2 : 
            ret = readGameInstructions(gs,str,mj);
            initColunasMatriz(mj);
            randomizaJogo(gs,mj);
        break;
    }
    return ret;
}
