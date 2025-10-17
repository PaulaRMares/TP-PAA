#include "../headers/mapa.h"
#include "../headers/expresso.h"

// Global variables
char **mapa;                        // matriz 2D para salvar o mapa
int dur_atual, dur_dim, dur_aum;    // D D' A
int altura, largura;                // altura e largura da matriz
int chamadas_recursivas = 0;        // numero de chamadas recursivas (quanto o expresso andou)
bool encontrar_caminho = false;     // se encontrou um caminho válido
int backtrack_cont = 0;
int total_pecas = 0;


// le arquivo e salva o mapa e posicao inicial do expresso
void lerArquivo(char *nome_arquivo, Expresso *expresso){
    char caminho_completo[256];
    snprintf(caminho_completo, sizeof(caminho_completo), "entrada/%s", nome_arquivo);

    FILE *arquivo = fopen(caminho_completo, "r");
    if(!arquivo){
        printf("Error opening the file!\n");
        exit(1);
    }

    if (mapa) liberarMemoria();

    fscanf(arquivo, "%d %d %d", &dur_atual, &dur_dim, &dur_aum);
    expresso->dur_atual = dur_atual;
    expresso->dur_dim   = dur_dim;
    expresso->dur_aum   = dur_aum;

    fscanf(arquivo, "%d %d", &altura, &largura);

    mapa = (char **)malloc(altura * sizeof(char *));
    for (int i = 0; i < altura; i++) {
        mapa[i] = (char *)malloc(largura * sizeof(char));
    }

    total_pecas = 0;

    for(int i = 0; i < altura; i++){
        for(int j = 0; j < largura; j++){
            char c;
            fscanf(arquivo, " %c", &c);
            mapa[i][j] = c;
            if (c == 'P') total_pecas++;
            if (c == 'X'){
                expresso->pos_x = i;
                expresso->pos_y = j;
            }
        }
    }

    fclose(arquivo);

    printf("Mapa carregado: %dx%d, pecas: %d\n", altura, largura, total_pecas);
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