#ifndef EXPRESSO_H
#define EXPRESSO_H
#include <stdbool.h>
#include <time.h>

typedef struct{
    int dur_atual;
    int dur_dim;
    int dur_aum;
    int pos_x;      // posicao do expresso na linha
    int pos_y;
    int pecas;
    bool congelado;      // posicao do expresso na coluna
}Expresso;

typedef struct {
    long chamadas_recursivas; // total de chamadas DFS
    int  profundidade_max;    // nível máximo de recursão
    double tempo_ms;          // tempo total em milissegundos
} Analise;



bool encontrarCaminho(Expresso *e, Analise *analise, bool imprimir_passos);
void liberarVisitados(void);

#endif // EXPRESSO_H
