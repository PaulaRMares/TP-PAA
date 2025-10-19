#include "../headers/mapa.h"
#include "../headers/expresso.h"

//  Variáveis Globais
char **mapa;                        // matriz 2D para salvar o mapa
bool **visitado_no_caminho;         // <<< MUDANÇA AQUI: Matriz de controle do backtracking
int dur_dim, dur_aum;               // D' A (constantes do jogo)
int altura, largura;                // altura e largura da matriz
bool encontrar_caminho = false;     // se encontrou um caminho válido
int total_pecas = 0;

//  Mudanças: 'dur_atual', 'chamadas_recursivas' e 'backtrack_cont' removidas
//  Mudanças: Retorno mudou para 'bool' para tratar erro

bool lerArquivo(char *nome_arquivo, Expresso *expresso_inicial) {
    char caminho_completo[256];
    snprintf(caminho_completo, sizeof(caminho_completo), "entrada/%s", nome_arquivo);

    FILE *arquivo = fopen(caminho_completo, "r");
    if(!arquivo){
        perror("Erro ao abrir o arquivo"); // 'perror' é mais descritivo
        return false;
    }

    //  Libera memória de um mapa anterior, se existir
    if (mapa) liberarMemoria();

    int durabilidade_inicial_lida; // Variável local
    fscanf(arquivo, "%d %d %d", &durabilidade_inicial_lida, &dur_dim, &dur_aum);
    
    //  Configura o estado inicial do expresso
    expresso_inicial->dur_atual = durabilidade_inicial_lida;
    //  Removida a atribuição de dur_dim e dur_aum da struct
    //  (pq eu acho que eles não pertencem ao estado)

    fscanf(arquivo, "%d %d", &altura, &largura);
    if (altura <= 0 || largura <= 0) {
        printf("Dimensões do mapa inválidas.\n");
        fclose(arquivo);
        return false;
    }

    //  Inicio da Alocação Dinâmica
    mapa = (char **)malloc(altura * sizeof(char *));
    visitado_no_caminho = (bool **)malloc(altura * sizeof(bool *)); // <<< MUDANÇA AQUI

    if (mapa == NULL || visitado_no_caminho == NULL) {
        printf("Erro de alocação de memória para as linhas.\n");
        fclose(arquivo);
        return false;
    }

    for (int i = 0; i < altura; i++) {
        mapa[i] = (char *)malloc(largura * sizeof(char));
        visitado_no_caminho[i] = (bool *)malloc(largura * sizeof(bool)); // <<< MUDANÇA AQUI

        if (mapa[i] == NULL || visitado_no_caminho[i] == NULL) {
             printf("Erro de alocação de memória para as colunas.\n");
             // (Nota: aqui deveria ter um loop de liberação do que já foi alocado)
             fclose(arquivo);
             return false;
        }
    }
    //  Fim da Alocação

    total_pecas = 0;

    for(int i = 0; i < altura; i++){
        for(int j = 0; j < largura; j++){
            char c;
            fscanf(arquivo, " %c", &c); // O espaço antes do %c é crucial! se tirar vai bugar!
            mapa[i][j] = c;
            if (c == 'P') total_pecas++;
            if (c == 'X'){
                expresso_inicial->pos_x = i;
                expresso_inicial->pos_y = j;
            }
        }
    }

    fclose(arquivo);
    printf("Mapa carregado: %dx%d, pecas: %d\n", altura, largura, total_pecas);
    
    return true;
}

//  Libera memoria do mapa
void liberarMemoria(){
    if (mapa) {
        for(int i=0; i<altura; i++) {
            free(mapa[i]);
        }
        free(mapa);
        mapa = NULL; // Só pra garantir
    }

    //  Libera a matriz 'visitado_no_caminho'
    if (visitado_no_caminho) {
         for(int i=0; i<altura; i++) {
            free(visitado_no_caminho[i]);
        }
        free(visitado_no_caminho);
        visitado_no_caminho = NULL;
    }
}

//  Adicionada função para resetar a matriz 'visitado'
//  Isso é necessário antes de cada chamada ao 'encontrarCaminho'
void resetarVisitados() {
    if (visitado_no_caminho) {
        for (int i = 0; i < altura; i++) {
            memset(visitado_no_caminho[i], 0, largura * sizeof(bool));
            // memset é mais rápido que um loop for
            // obs: se tá na internet é verdade (reddit)
        }
    }
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