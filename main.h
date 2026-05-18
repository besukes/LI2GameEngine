//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////Typedefs e structs utilizados///////////////////////////////////////////////////
typedef int Boolean ;

typedef enum {quit,undo,save,invalid,valid,GameWon} PossiveisJogadas;

typedef char * String;

typedef char Naipe;

typedef struct Carta{
    int valor; // 1-13
    Naipe naipe; // C-Paus , D-Ouros , H-Copas , S-Espadas
}Carta;

typedef struct PilhaDeCartas{
    long tagPilha;
    int numCartasPilha;
    struct Carta * cartasPilha;
}PilhaDeCartas;


typedef struct MatrizJogo{
    int numLinhasMatriz;
    struct PilhaDeCartas * linhasMatriz;
} MatrizJogo;


typedef int (*FlagFunctionsP)(Carta,Carta);

typedef int (*FlagFunctionsC) (Carta,Carta);

typedef int (*FlagFunctionsR) (int,MatrizJogo *);

typedef int (*FlagFunctionsV) (int,MatrizJogo *);

typedef struct LastMoveLLNode{
    int indexPilhaAnterior;
    int indexNovaPilha;
    int numCartasMovidas;
    Carta * cartasMovidas;
    struct LastMoveLLNode * prox; // Aponta para o proximo elemento da lista
} * LastMoveLL;


typedef struct FlagFuncArrayP{
    int numFlagsPegavel; //Num de Flags que indicam cartas que se podem pegar de uma dada pilha
    FlagFunctionsP flagsPegavel[7];
    int numRestricoes;
    FlagFunctionsR flagRestricoes[4];
    Boolean variasCartasMoviveis;
}ArrayFlagsPegar;

typedef struct FlagFuncArrayC{
    int numFlagsColocavel; //Num de Flags que indicam cartas que se podem colocar numa dada pilha
    FlagFunctionsC flagsColocavel[7];
}ArrayFlagsColocar;


typedef struct MovimentoEntrePilhas{
    long tagOrig; //Pilha de onde vem cas cartas
    long tagDest; //Pilha onde vao parar as cartas
    int numMovs; //Num de movs que ja foram feitos
    //Array de um struct que guarda as condicoes de pegar Cartas de uma dada pilha
    ArrayFlagsPegar * arrP;
    //Array de um struct que guarda o numero de funcoes de um dado movimento de pilhas
    ArrayFlagsColocar * arrC;
    FlagFunctionsV colocaEmPilhaVazia;
}MovimentoEntrePilhas;


typedef struct RegrasPilha{
    long tag; //tag da pilha dada pela soma dos caracteres ASCII
    char * nomePilha;
    Boolean todaPilhaVisivel;
    Boolean cartaTopoVisivel;
    Boolean existeMaxCartas; // MAX cartas 1
}RegrasPilha;

typedef MovimentoEntrePilhas AutoMoves;

typedef struct JogoStruct{
    String nomeJogo;
    int numPilhas;
    RegrasPilha * pilhas;
    int numCondicoesMov;
    MovimentoEntrePilhas * movimentoPilhas;
    int qntdAutoMoves;
    AutoMoves * autoMoves;
}JogoStruct;


typedef struct WinCondition{
    long tagPilha;
    int numeroVitoriaPilha;
}WinCondition;

typedef struct OrdemFicheiro{
    int tamanhoMemoria;
    int tamanhoReal;
    String instructions;
}OrdemFicheiro;

typedef struct game{
    OrdemFicheiro file;
    int numBaralhos;
    JogoStruct jogo;
    int numCondicoesVitoria;
    WinCondition * winCon;
}GameSettings;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////Definições de algumas funções de vários módulos/////////////////////////////////////////

//Modulo structFunctions.c
GameSettings initStructs(void);
MatrizJogo initMatrizJogo(void);
void inicializaAutoMoves(AutoMoves * am , int tagOrig , int tagDest);
void initArrC(MovimentoEntrePilhas * mov);
void initArrP(MovimentoEntrePilhas * mov);
void headLinkedList(LastMoveLL * l , int indexAnterior, int indexNovo, int numCartasMovidas , MatrizJogo matriz);
void initColunasMatriz(MatrizJogo * mj);

//Modulo readFiles.c
int readFiles(GameSettings * gs,MatrizJogo * mj);


//Modulo simpleFunctions.c
int valorCarta(int c);
int exponenciacao(int base,int expo);
char * criarTag(long * tag,char * line);
int numCaracteres(char * line);
int strToNumber(char * line);
void calculaRulesPilha(RegrasPilha * rp , char * line);
MovimentoEntrePilhas * comparaTags (MovimentoEntrePilhas * mp ,long tagOrig , long tagDest , int n);
long procuraTag(MatrizJogo * mj , int pilha);
char convertToNaipe(int div);
int pertenceString(char c , char str[]);
int pilhaVaziaVerify(MatrizJogo * mj , int pilhaDest , MovimentoEntrePilhas * mov);
char * procuraNomeString(char * line);


//Modulo flagFunctions.c
int pilhaVazia(int linha,MatrizJogo * m);
FlagFunctionsC flagColocavelCalc(char * line);
FlagFunctionsP flagColocavelCalcAux(char * line);
FlagFunctionsP flagPegavelCalc(char * line);
FlagFunctionsP flagPegavelCalcAux(char * line);
FlagFunctionsR flagRestricoesCalc(char * line);


//Modulo randomizeCards.c
void randomizaJogo(GameSettings * gs , MatrizJogo * mj);


//Modulo gameplayHandle.c
PossiveisJogadas efetuaJogadaMovimentoCartas(GameSettings * gs , MatrizJogo * mj , LastMoveLL * undoState);
PossiveisJogadas recebeInput(char jogada);
void pedeJogadaUtilizador(int * p1 , int * p2 ,int * num);


//Modulo undoMove.c
void undoMove(MatrizJogo * mj , LastMoveLL * lm);


//Modulo userInterface.c
void desenhaInterfaceJogo(GameSettings * gs,MatrizJogo mj);
void splashScreen(void);
void animacaoVitoria(void);
void mensagemSaida(void);
void avisaJogadaInvalida(void);
void desenhaMenuPrincipal(void);


//Modulo freeMemory.c
void memoryFree(GameSettings gs , MatrizJogo mj , LastMoveLL lm);