#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>

#define ENTRADA_PATH "../../entrada/"

// Função para verificar qual será o próximo número de arquivo (entradaN.txt)
int proximo_numero_arquivo() {
    DIR *dir;
    struct dirent *ent;
    int max_n = 0;

    dir = opendir(ENTRADA_PATH);
    if (dir == NULL) {
        printf("Erro: não foi possível abrir a pasta %s\n", ENTRADA_PATH);
        exit(1);
    }

    while ((ent = readdir(dir)) != NULL) {
        int n;
        if (sscanf(ent->d_name, "entrada%d.txt", &n) == 1) {
            if (n > max_n) max_n = n;
        }
    }
    closedir(dir);
    return max_n + 1;
}

// Função para criar o caminho garantido entre X e F
void gerar_caminho(char **mapa, int altura, int largura, int x1, int y1, int x2, int y2) {
    int x = x1, y = y1;
    mapa[x][y] = 'X';

    // Caminha até F em um estilo "zig-zag" simples
    while (x != x2 || y != y2) {
        if (x != x2 && rand() % 2) {
            int passo = (x < x2) ? 1 : -1;
            x += passo;
            mapa[x][y] = '|';
        } else if (y != y2) {
            int passo = (y < y2) ? 1 : -1;
            y += passo;
            mapa[x][y] = '-';
        }
        if (mapa[x][y] == '-' || mapa[x][y] == '|')
            mapa[x][y] = '+';
    }

    mapa[x2][y2] = 'F';
}

// Função para gerar o mapa aleatório com caminho garantido
void gerar_mapa(int altura, int largura, char **mapa) {
    for (int i = 0; i < altura; i++)
        for (int j = 0; j < largura; j++)
            mapa[i][j] = '.'; // tudo vazio inicialmente

    // Escolher posições iniciais e finais diferentes
    int x1 = rand() % altura, y1 = rand() % largura;
    int x2, y2;
    do {
        x2 = rand() % altura;
        y2 = rand() % largura;
    } while (x1 == x2 && y1 == y2);

    gerar_caminho(mapa, altura, largura, x1, y1, x2, y2);

    // Adicionar peças 'P' ao longo do mapa
    int num_pecas = (altura * largura) / 10;
    for (int i = 0; i < num_pecas; i++) {
        int px = rand() % altura;
        int py = rand() % largura;
        if (mapa[px][py] == '.' || mapa[px][py] == '+' || mapa[px][py] == '-' || mapa[px][py] == '|')
            mapa[px][py] = 'P';
    }

    // Adicionar alguns caminhos extras aleatórios
    for (int i = 0; i < (altura * largura) / 5; i++) {
        int cx = rand() % altura;
        int cy = rand() % largura;
        if (mapa[cx][cy] == '.') {
            mapa[cx][cy] = (rand() % 2) ? '-' : '|';
        }
    }
}

int main() {
    srand(time(NULL));

    int D, Dp, A;
    int altura, largura;

    printf("=== GERADOR DE MAPAS TP-PAA ===\n\n");
    printf("Informe o valor de D (durabilidade inicial): ");
    scanf("%d", &D);
    printf("Informe o valor de D' (desgaste por movimento): ");
    scanf("%d", &Dp);
    printf("Informe o valor de A (ganho por peca): ");
    scanf("%d", &A);
    printf("Informe a altura do mapa: ");
    scanf("%d", &altura);
    printf("Informe a largura do mapa: ");
    scanf("%d", &largura);

    char **mapa = malloc(altura * sizeof(char *));
    for (int i = 0; i < altura; i++)
        mapa[i] = malloc(largura * sizeof(char));

    gerar_mapa(altura, largura, mapa);

    int n = proximo_numero_arquivo();
    char nome_arquivo[100];
    sprintf(nome_arquivo, "%sentrada%d.txt", ENTRADA_PATH, n);

    FILE *fp = fopen(nome_arquivo, "w");
    if (!fp) {
        printf("Erro ao criar o arquivo %s\n", nome_arquivo);
        return 1;
    }

    fprintf(fp, "%d %d %d\n", D, Dp, A);
    fprintf(fp, "%d %d\n", altura, largura);
    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            fputc(mapa[i][j], fp);
        }
        fputc('\n', fp);
    }

    fclose(fp);

    printf("\nMapa gerado com sucesso em: %s\n", nome_arquivo);

    for (int i = 0; i < altura; i++) free(mapa[i]);
    free(mapa);

    return 0;
}
