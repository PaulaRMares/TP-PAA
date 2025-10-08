#include "../headers/mapa.h"
#include "../headers/expresso.h"

/*
int lerArquivo(char *nome_arquivo){
    int dur_diminuida, dur_aumentada, dur_atual, altura, largura;
    int linha, coluna;
    int i,j;

    char caminho_completo[256];
    snprintf(caminho_completo, sizeof(caminho_completo), "entrada/%s", nome_arquivo);

    FILE *arquivo = fopen(caminho_completo, "r");
    if(!arquivo){
        printf("\n Erro ao abrir %c", caminho_completo);
    }

    // le a primeira linha
    if(fscanf(arquivo, "%d %d %d", &dur_diminuida, &dur_aumentada, &dur_atual) != 3){
        fprintf(stderr, "Erro: nao foi possivel ler D, D' e A.\n");
        fclose(arquivo);

        return 0;
    }

    // le a primeira linha
    if(fscanf(arquivo, "%d %d", &altura, &largura) != 2){
        fprintf(stderr, "Erro: nao foi possivel ler H e W.\n");
        fclose(arquivo);

        return 0;
    }

    mapa = (char**)malloc(sizeof(char*) * altura);

    // remove \n
    int c;
    do{ 
        c = fgetc(arquivo); 
    }while (c == '\n' || c == '\r');

    if(c != EOF){
        ungetc(c, arquivo);
    } // devolve o 1º não-\n/\r

    char linha_lida[100];
    for(i=0; i<altura; i++){
        if(!fgets(linha_lida, sizeof(linha_lida), arquivo)){
            fprintf(stderr, "Erro lendo linha %d do mapa\n", i+1); return 0;
        }
        // Remove \n e \r
        size_t len = strlen(linha_lida);
        while(len>0 && (linha_lida[len-1] == '\n' || linha_lida[len-1] == '\r')){
            linha_lida[--len] = '\0';
        }
        // Copia caractere a caractere; se a linha for menor, completa com '.'
        for(j=0; j<largura; j++){
            mapa[i][j] = (j < (int)len) ? linha_lida[j] : '.';
        }
    }
    fclose(arquivo);

    // SOMENTE PARA TESTE
    // Exemplo: imprimir a matriz
    puts("Mapa:");
    for (i=0; i<altura; i++) {
        for (j=0; j<largura; j++) putchar(mapa[i][j]);
        putchar('\n');
    }

    return 0;
}
*/