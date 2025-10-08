#include "../headers/mapa.h"
#include "../headers/expresso.h"

// Global variables
char **mapa;                        // matriz 2D para salvar o mapa
int dur_atual, dur_dim, dur_aum;    // D D' A
int altura, largura;                // altura e largura da matriz
int chamadas_recursivas = 0;        // numero de chamadas recursivas (quanto o expresso andou)
bool encontrar_caminho = false;     // se encontrou um caminho válido
int backtrack_cont = 0;

// le arquivo e salva o mapa e posicao inicial do expresso
void lerArquivo(char *nome_arquivo, Expresso *expresso){
    char caminho_completo[256];
    snprintf(caminho_completo, sizeof(caminho_completo), "entrada/%s", nome_arquivo);

    FILE *arquivo = fopen(caminho_completo, "r");
    if(!arquivo){
        printf("Error opening the file!\n");
        exit(1);
    }

    // le a primeira linha
    fscanf(arquivo, "%d %d %d", &dur_atual, &dur_dim, &dur_aum);
    expresso->dur_atual = dur_atual;
    expresso->dur_dim = dur_dim;
    expresso->dur_aum = dur_aum;

    // le a segunda linha
    fscanf(arquivo, "%d %d", &altura, &largura);

    // aloca memoria pro mapa
    mapa = (char **)malloc(altura * sizeof(int *));
    for(int i=0; i<altura; i++){
        mapa[i] = (char *)malloc(largura * sizeof(int));
    }

    // le o mapa do arquivo
    for(int i=0; i<altura; i++){
        for(int j=0; j<largura; j++){
            char caracter;
            fscanf(arquivo, " %c", &caracter);
            mapa[i][j] = caracter;
            if(mapa[i][j] == 'X'){
                expresso->pos_x = i;
                expresso->pos_y = j;
            }
        }
    }

    fclose(arquivo);
}

// libera memoria do mapa
void liberarMemoria(){
    for(int i=0; i<altura; i++) {
        free(mapa[i]);
    }
    free(mapa);
}

void imprimeMapa(){
    printf("Mapa (%dx%d): \n", altura, largura);
    for(int i=0; i<altura; i++){
        for(int j=0; j<largura; j++){
            printf("%c", mapa[i][j]);
        }
        printf("\n");
    }
}