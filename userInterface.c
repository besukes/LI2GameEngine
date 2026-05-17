#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // para o usleep

// cores ANSI - aprendi isto numa aula de SO lol
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define DIM     "\033[2m"

// cores de texto
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define MAGENTA "\033[35m"

// cores de fundo
#define BG_GREEN  "\033[42m"
#define BG_BLACK  "\033[40m"
#define BG_WHITE  "\033[47m"

// largura do terminal (mais ou menos)
#define TERMINAL_WIDTH 80

// -------------------------------------------------------
// funcoes auxiliares de desenho
// -------------------------------------------------------

// limpa o ecra
void clearScreen(void) {
    printf("\033[2J\033[H");
}

// imprime uma linha horizontal
void printHLine(int size, char c) {
    for (int i = 0; i < size; i++) putchar(c);
    putchar('\n');
}

// centraliza texto numa linha de tamanho fixo
void printCentered(const char * text, int width) {
    int len = strlen(text);
    int spaces = (width - len) / 2;
    for (int i = 0; i < spaces; i++) putchar(' ');
    printf("%s\n", text);
}

// animacao de carregamento estilo retro
void loadingAnim(int steps) {
    printf(CYAN "  Carregando" RESET);
    fflush(stdout);
    for (int i = 0; i < steps; i++) {
        usleep(120000);
        printf(YELLOW "." RESET);
        fflush(stdout);
    }
    printf("\n");
    usleep(200000);
}

// -------------------------------------------------------
// ecrã de boas vindas / splash screen
// -------------------------------------------------------

void splashScreen() {
    clearScreen();
    printf("\n\n");
    printf(GREEN BOLD);
    printHLine(TERMINAL_WIDTH, '=');

    printf("\n");
    printCentered("", TERMINAL_WIDTH);
    printCentered("  ___           _ _        _    ", TERMINAL_WIDTH);
    printCentered(" / __| ___ | (_) |_ __ _(_)_ _ ___ ", TERMINAL_WIDTH);
    printCentered(" \\__ \\/ _ \\| | |  _/ _` | | '_/ -_)", TERMINAL_WIDTH);
    printCentered(" |___/\\___/|_|_|\\__\\__,_|_|_| \\___|", TERMINAL_WIDTH);
    printf("\n");
    printCentered("[ LI2 Game Engine v0.1 ]", TERMINAL_WIDTH);
    printf("\n");

    printHLine(TERMINAL_WIDTH, '=');
    printf(RESET "\n");

    loadingAnim(5);
}

// -------------------------------------------------------
// desenha uma carta individual
// -------------------------------------------------------

// Unicode playing card codepoints (U+1F0A0 block)
// espadas: 1F0A1-1F0AE  copas: 1F0B1-1F0BE
// ouros:   1F0C1-1F0CE  paus:  1F0D1-1F0DE
// nota: J=AB/BB/CB/DB, Q=AD/BD/CD/DD, K=AE/BE/CE/DE (skip AC/BC/CC/DC = cavaleiro)
const char * getUnicodeCard(Carta c) {
    static const char * espadas[] = {
        "", "\U0001F0A1", "\U0001F0A2", "\U0001F0A3", "\U0001F0A4",
        "\U0001F0A5", "\U0001F0A6", "\U0001F0A7", "\U0001F0A8",
        "\U0001F0A9", "\U0001F0AA", "\U0001F0AB", "\U0001F0AD", "\U0001F0AE"
    };
    static const char * copas[] = {
        "", "\U0001F0B1", "\U0001F0B2", "\U0001F0B3", "\U0001F0B4",
        "\U0001F0B5", "\U0001F0B6", "\U0001F0B7", "\U0001F0B8",
        "\U0001F0B9", "\U0001F0BA", "\U0001F0BB", "\U0001F0BD", "\U0001F0BE"
    };
    static const char * ouros[] = {
        "", "\U0001F0C1", "\U0001F0C2", "\U0001F0C3", "\U0001F0C4",
        "\U0001F0C5", "\U0001F0C6", "\U0001F0C7", "\U0001F0C8",
        "\U0001F0C9", "\U0001F0CA", "\U0001F0CB", "\U0001F0CD", "\U0001F0CE"
    };
    static const char * paus[] = {
        "", "\U0001F0D1", "\U0001F0D2", "\U0001F0D3", "\U0001F0D4",
        "\U0001F0D5", "\U0001F0D6", "\U0001F0D7", "\U0001F0D8",
        "\U0001F0D9", "\U0001F0DA", "\U0001F0DB", "\U0001F0DD", "\U0001F0DE"
    };

    int v = c.valor;
    if (v < 1 || v > 13) return "?";
    switch(c.naipe) {
        case 'S': return espadas[v];
        case 'H': return copas[v];
        case 'D': return ouros[v];
        case 'C': return paus[v];
        default:  return "?";
    }
}

#define CARD_BACK "\U0001F0A0"

int naipeVermelho(char naipe) {
    return (naipe == 'H' || naipe == 'D');
}

// inline: so o simbolo unicode da carta com cor
void desenhaCartaInline(Carta c, int visivel) {
    if (!visivel) {
        printf(CYAN "%s " RESET, CARD_BACK);
        return;
    }
    const char * card = getUnicodeCard(c);
    if (naipeVermelho(c.naipe))
        printf(RED BOLD "%s " RESET, card);
    else
        printf(WHITE BOLD "%s " RESET, card);
}

// versao alta: carta destacada com moldura simples
void desenhaCartaAlta(Carta c, int visivel) {
    const char * cor = naipeVermelho(c.naipe) ? RED : WHITE;
    if (!visivel) {
        printf(CYAN BOLD "[ %s ]\n" RESET, CARD_BACK);
        return;
    }
    printf("%s%s[ %s ]\n" RESET, cor, BOLD, getUnicodeCard(c));
}


// -------------------------------------------------------
// desenha uma pilha de cartas
// -------------------------------------------------------

// mostra o numero e quantas cartas tem
void desenhaCabecalhoPilha(int indx, int numCartas, RegrasPilha * regra) {
    printf(CYAN BOLD " [%d]" RESET, indx);
    printf(DIM "(%d cartas)" RESET, numCartas);
    printf("  ");
}

// imprime as cartas de uma pilha em formato horizontal compacto
// so mostra as ultimas MAX_VISIVEL cartas
void desenhaPilha(PilhaDeCartas * p, int indx, RegrasPilha * regra) {
    int n = p->numCartasPilha;
    int MAX_VISIVEL = 6; // nao mostrar tudo senao fica enorme

    printf(GREEN "  Pilha %d" RESET " ", indx);

    if (n == 0) {
        printf(DIM "[ vazia ]" RESET "\n");
        return;
    }

    // mostra so as ultimas cartas, ou todas se couberem
    int inicio = (n > MAX_VISIVEL) ? (n - MAX_VISIVEL) : 0;

    if (inicio > 0) {
        printf(DIM "... " RESET); // indica que ha mais cartas em baixo
    }

    for (int i = inicio; i < n; i++) {
        Carta c = *(p->cartasPilha + i);
        int visivel = 1;

        // se a regra disser que so o topo e visivel
        if (regra != NULL && regra->cartaTopoVisivel && !regra->todaPilhaVisivel) {
            visivel = (i == n - 1); // so a ultima e visivel
        }

        desenhaCartaInline(c, visivel);
        if (i < n - 1) printf(DIM "-" RESET);
    }

    printf("\n");
}

// -------------------------------------------------------
// encontra regra para uma pilha (por tag)
// -------------------------------------------------------
// nao e a mais eficiente mas funciona lol
RegrasPilha * encontraRegra(GameSettings * gs, long tag) {
    int n = gs->jogo.numPilhas;
    for (int i = 0; i < n; i++) {
        if ((gs->jogo.pilhas + i)->tag == tag) {
            return gs->jogo.pilhas + i;
        }
    }
    return NULL; // sem regra especifica
}

// -------------------------------------------------------
// FUNCAO PRINCIPAL: desenha o estado do jogo
// -------------------------------------------------------

void desenhaInterfaceJogo(GameSettings * gs, MatrizJogo mj) {
    clearScreen();

    // header do jogo
    printf(GREEN BOLD);
    printHLine(TERMINAL_WIDTH, '-');
    char titulo[100];
    // nome do jogo ou generico se nao tiver
    const char * nome = gs->jogo.nomeJogo ? gs->jogo.nomeJogo : "Solitario";
    snprintf(titulo, sizeof(titulo), "  %s  ", nome);
    printCentered(titulo, TERMINAL_WIDTH);
    printHLine(TERMINAL_WIDTH, '-');
    printf(RESET "\n");

    // desenha cada pilha
    int n = mj.numLinhasMatriz;
    for (int i = 0; i < n; i++) {
        PilhaDeCartas * p = mj.linhasMatriz + i;
        RegrasPilha * regra = encontraRegra(gs, p->tagPilha);
        desenhaPilha(p, i, regra);
    }

    printf("\n");
    printHLine(TERMINAL_WIDTH, '-');

    

    printHLine(TERMINAL_WIDTH, '-');
    printf("\n");
}

// -------------------------------------------------------
// menu principal (antes do jogo comecar)
// -------------------------------------------------------

void desenhaMenuPrincipal(void) {
    clearScreen();

    printf("\n");
    printf(GREEN BOLD);
    printHLine(TERMINAL_WIDTH, '#');
    printf(RESET);

    printf("\n");
    printf(CYAN BOLD);
    printCentered("BEM VINDO AO LI2 SOLITAIRE ENGINE", TERMINAL_WIDTH);
    printf(RESET "\n");

    printf(DIM);
    printCentered("escolhe uma opcao abaixo:", TERMINAL_WIDTH);
    printf(RESET "\n\n");

    // as opcoes do menu com um borzinho simples
    printf("    " YELLOW BOLD "1." RESET "  Novo Jogo\n");
    printf("    " YELLOW BOLD "2." RESET "  Carregar Jogo Guardado\n");
    printf("    " YELLOW BOLD "3." RESET "  Sair\n");

    printf("\n");
    printf(GREEN BOLD);
    printHLine(TERMINAL_WIDTH, '#');
    printf(RESET "\n");

    printf(WHITE "  >> " RESET);
    fflush(stdout);
}

// -------------------------------------------------------
// mensagem de vitoria (animada de forma simples)
// -------------------------------------------------------

void animacaoVitoria(void) {
    clearScreen();
    printf("\n\n\n");

    // pisca umas vezes
    for (int i = 0; i < 3; i++) {
        printf(YELLOW BOLD);
        printCentered("*** PARABENS!! GANHASTE!! ***", TERMINAL_WIDTH);
        printf(RESET);
        fflush(stdout);
        usleep(400000);
        clearScreen();
        printf("\n\n\n");
        usleep(200000);
    }

    printf(YELLOW BOLD);
    printCentered("*** PARABENS!! GANHASTE!! ***", TERMINAL_WIDTH);
    printf(RESET "\n\n");

    printf(GREEN);
    printCentered("  \\o/  ", TERMINAL_WIDTH);
    printCentered("  | |  ", TERMINAL_WIDTH);
    printCentered("  / \\ ", TERMINAL_WIDTH);
    printf(RESET "\n\n");

    printf(DIM);
    printCentered("prime qualquer tecla para sair...", TERMINAL_WIDTH);
    printf(RESET "\n");
}

// mensagem de derrota / desistencia
void mensagemSaida(void) {
    printf("\n");
    printf(MAGENTA BOLD);
    printCentered("Maior sorte para a proxima!!", TERMINAL_WIDTH);
    printf(RESET);
    printf(DIM);
    printCentered("Obrigado por jogares :)", TERMINAL_WIDTH);
    printf(RESET "\n");
}

// aviso de jogada invalida - simples
void avisaJogadaInvalida(void) {
    printf(RED "  >> Jogada invalida! Tenta outra vez.\n" RESET);
    usleep(600000); // deixa ver a mensagem um bocado
}