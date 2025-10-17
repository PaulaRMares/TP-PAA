#include "../headers/expresso.h"
#include "../headers/mapa.h"

int main(){
    int opcao = 0;
    char arquivo[100];
    Expresso expresso;
    bool modo_analise_ativo = false;

    printf("\nOs cientistas espaciais fizeram um programa para calcular um caminho possivel para a celebracao.\nVoce pode usar o programa, mas como dono do expresso tera que aumentar o salario de todos os \ncientistas que ajudaram no processo :) (nao negociavel) \n");

    while(opcao != 6){
        printf("\n ====================== MENU ====================== \n");
        printf("Status do modo de analise: %s\n", modo_analise_ativo ? "ATIVADO" : "DESATIVADO");
        printf("1) Digite o nome do mapa a ser usado na exploracao\n");
        printf("2) Encontrar caminho (imprimindo passos)\n");
        printf("3) Imprimir mapa\n");
        printf("4) Encontrar caminho (sem imprimir passos)\n");
        printf("5) Ativar/Desativar modo de analise\n");
        printf("6) Sair do programa\n");

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
                Analise *pa = modo_analise_ativo ? &a : NULL;
                bool ok = encontrarCaminho(&expresso, pa, true);
                if (modo_analise_ativo) {
                    printf("=== MODO DE ANALISE ===\n");
                    printf("Tempo (ms): %.3f\n", a.tempo_ms);
                    printf("Chamadas recursivas: %ld\n", a.chamadas_recursivas);
                    printf("Profundidade maxima: %d\n", a.profundidade_max);
                    printf("Resultado: %s\n", ok ? "Caminho encontrado" : "Sem caminho");
                }
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
                Analise *pa = modo_analise_ativo ? &a : NULL;
                bool ok = encontrarCaminho(&expresso, pa, false);
                if (modo_analise_ativo) {
                    printf("=== MODO DE ANALISE ===\n");
                    printf("Tempo (ms): %.3f\n", a.tempo_ms);
                    printf("Chamadas recursivas: %ld\n", a.chamadas_recursivas);
                    printf("Profundidade maxima: %d\n", a.profundidade_max);
                    printf("Resultado: %s\n", ok ? "Caminho encontrado" : "Sem caminho");
                } else {
                    printf("Executado sem imprimir passos e sem analise.\n");
                }
                break;
            }

            case 5:
                modo_analise_ativo = !modo_analise_ativo;
                printf("Modo de analise agora esta: %s\n", modo_analise_ativo ? "ATIVADO" : "DESATIVADO");
                break;

            case 6:
                break;

            default:
                printf("Opcao invalida.\n");
                break;
        }
    }
    return 0;
}