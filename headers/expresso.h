#ifndef EXPRESSO_H
#define EXPRESSO_H
#include <stdbool.h>

typedef struct{
    int dur_atual;
    int dur_dim;
    int dur_aum;
    int pos_x;      // posicao do expresso na linha
    int pos_y;
    int pecas;
    bool congelado;      // posicao do expresso na coluna
}Expresso;


bool encontrarCaminho(Expresso *e); 
void lerArquivo(char *nome_arquivo, Expresso *expresso);

#endif // EXPRESSO_H
