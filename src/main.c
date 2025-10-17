#include "../headers/expresso.h"
#include "../headers/mapa.h"

int main(){
    int opcao = 0;
    char arquivo[100];
    Expresso expresso;
    
    printf("\nOs cientistas espaciais fizeram um programa para calcular um caminho possivel para a celebracao.\nVoce pode usar o programa, mas como dono do expresso tera que aumentar o salario de todos os \ncientistas que ajudaram no processo :) (nao negociavel) \n");

     while(opcao != 5){
        printf("\n ====================== MENU ====================== \n");
        printf("1) Digite o nome do mapa a ser usado na exploracao\n");
        printf("2) Encontrar caminho (imprimindo passos)\n");
        printf("3) Imprimir mapa\n");
        printf("4) Modo de analise (tempo, chamadas, profundidade)\n");
        printf("5) Sair do programa\n");

        scanf("%d", &opcao);

        switch (opcao){
            case 1:
                printf("Nome do arquivo: ");
                scanf("%99s", arquivo);
                lerArquivo(arquivo, &expresso);
                printf("Arquivo lido com sucesso!\n");
                break;

            case 2: {
                if(!mapa){
                    printf("\nPor favor, escolha a opcao 1 primeiro.\n");
                    break;
                }
                Analise a;
                bool ok = encontrarCaminho(&expresso, &a, /*imprimir_passos=*/true);
                // Após executar, também mostramos as métricas (útil para depurar):
                printf("=== ANALISE (passos impressos) ===\n");
                printf("Tempo (ms): %.3f\n", a.tempo_ms);
                printf("Chamadas recursivas: %ld\n", a.chamadas_recursivas);
                printf("Profundidade maxima: %d\n", a.profundidade_max);
                break;
            }

            case 3:
                if(!mapa){
                    printf("\nPor favor, escolha a opcao 1 primeiro.\n");
                    break;
                }
                imprimeMapa();
                break;

            case 4: {
                if(!mapa){
                    printf("\nPor favor, escolha a opcao 1 primeiro.\n");
                    break;
                }
                Analise a;
                bool ok = encontrarCaminho(&expresso, &a, /*imprimir_passos=*/false);
                printf("=== MODO DE ANALISE ===\n");
                printf("Tempo (ms): %.3f\n", a.tempo_ms);
                printf("Chamadas recursivas: %ld\n", a.chamadas_recursivas);
                printf("Profundidade maxima: %d\n", a.profundidade_max);
                // Se quiser, indique sucesso/fracasso sem poluir com os passos:
                printf("Resultado: %s\n", ok ? "Caminho encontrado" : "Sem caminho");
                break;
            }

            case 5:
                // encerrar
                break;

            default:
                printf("Opcao invalida.\n");
                break;
        }
    }
    return 0;
}