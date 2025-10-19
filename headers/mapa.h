#ifndef MAPA_H
#define MAPA_H

#include "expresso.h"

/*
    ==========================================================
    Variáveis Globais (Definidas em mapa.c)
    ==========================================================
    Estas variáveis definem o "estado do mundo" que é constante
    durante uma única execução do backtracking.
*/

// --- Regras do Jogo ---
extern int dur_dim;     // D' (custo por movimento)
extern int dur_aum;     // A (ganho por peça)
extern int total_pecas; // Total de peças 'P' no mapa

// --- Estruturas de Dados do Mapa ---
extern char **mapa;
extern bool **visitado_no_caminho; // Matriz para marcar células visitadas NO CAMINHO ATUAL
extern int altura, largura;

// --- Flag de Controle ---
// Usada para parar a recursão assim que a saída 'F' for encontrada.
extern bool caminho_encontrado;

/*
    ==========================================================
    Protótipos de Funções (Implementadas em mapa.c)
    ==========================================================
    
    Lê o arquivo de entrada, aloca dinamicamente 'mapa' e 'visitado_no_caminho',
    e configura o estado inicial do expresso.
    
    'nome_arquivo' é o nome do arquivo a ser lido (ex: "mapa1.txt").
    'expresso_inicial' é um Ponteiro para a struct Expresso que será preenchida com
    os valores iniciais (posição X, durabilidade D).
    
    Por fim, retorna 'true' se a leitura e alocação foram bem-sucedidas, 'false' caso contrário.
*/

bool lerArquivo(char *nome_arquivo, Expresso *expresso_inicial);

//  Libera toda a memória alocada dinamicamente para 'mapa' e 'visitado_no_caminho'.
void liberarMemoria();

//  Limpa a matriz 'visitado_no_caminho' (define todos os valores como false).
//  Deve ser chamada antes de cada nova busca (encontrarCaminho).
void resetarVisitados();

//  Função auxiliar para imprimir o estado atual do mapa no console.
void imprimeMapa();

#endif // MAPA_H