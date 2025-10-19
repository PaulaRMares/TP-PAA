#ifndef EXPRESSO_H
#define EXPRESSO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <time.h> // Para clock()

//  Define o estado ATUAL do expresso durante a recursão.
//  Contém apenas as variáveis que mudam de um passo para o outro.

typedef struct {
    int dur_atual;  // Durabilidade atual (D)
    int pos_x;      // Posição atual na LINHA
    int pos_y;      // Posição atual na COLUNA
    int pecas;      // Total de peças COLETADAS até o momento
} Expresso;

//  Armazena as métricas para o "Modo de Análise" do trabalho.

typedef struct {
    long chamadas_recursivas; // Total de chamadas recursivas
    int  profundidade_max;    // Nível máximo de recursão alcançado
    double tempo_ms;          // Tempo total em milissegundos (extra)
} Analise;

//  Função pública que inicia o processo de backtracking.

bool encontrarCaminho(Expresso *e, Analise *analise, bool imprimir_passos);

//  Esta função configura o ambiente de análise, reseta o controle de
//  visitados e chama a função 'dfs' interna para encontrar o caminho.

//  "Expresso *e" é um Ponteiro para a struct Expresso contendo o ESTADO INICIAL
//  (posição 'X' e durabilidade inicial 'D').

//  "Analise *analise" é um Ponteiro para a struct Analise onde as métricas serão
//  salvas. Se for NULL, o modo de análise é desativado.

//  A função 'liberarVisitados()' foi removida daqui,
//  pq a matriz 'visitado_no_caminho' agora é gerenciada
//  pelas funções 'liberarMemoria()' e 'resetarVisitados()' em mapa.c

#endif // EXPRESSO_H