#ifndef MAPA_H
#define MAPA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>
#include "expresso.h"
/*
// variaveis globais
extern int **maze;
extern int num_rows, num_cols, num_keys;
extern int total_moves;
extern int recursive_calls;
extern int max_recursion_level;
extern bool found_exit;
extern int backtrack_count;
*/

// variaveis globais
extern char **mapa;                         // matriz 2D para salvar o mapa
extern int dur_atual, dur_dim, dur_aum;     // D, D', A
extern int altura, largura;                 // dimensões do mapa
extern int chamadas_recursivas;             // numero de chamadas recursivas (quanto o expresso andou)
extern bool encontrar_caminho;              // se encontrou um caminho válido
extern int backtrack_cont;

void lerArquivo(char *nome_arquivo, Expresso *expresso);
void liberarMemoria();
void imprimeMapa();

#endif // MAPA_H
