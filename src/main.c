#include "../headers/expresso.h"
#include "../headers/mapa.h"

void resetarVisitados(); 

int main(){
    int opcao = 0;
    char arquivo[100];
    Expresso expresso;
    bool modo_analise_ativo = false;

    // Inicializa a variável global 'mapa' (de mapa.c) como nula
    // para que a verificação 'if(!mapa)' funcione antes de carregar
    mapa = NULL; 

    printf("\nOs cientistas espaciais fizeram um programa para calcular um caminho possivel para a celebracao.\nVoce pode usar o programa, mas o dono do expresso tera que aumentar o salario de todos os \ncientistas que ajudaram no processo :) (nao negociavel) \n");

    while(opcao != 6){
        printf("\n ====================== MENU ====================== \n");
        printf("Status do modo de analise: %s\n", modo_analise_ativo ? "ATIVADO" : "DESATIVADO");
        printf("1) Carregar o mapa a ser usado na exploracao\n");
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
                
                //  Checar o retorno de lerArquivo
                if (lerArquivo(arquivo, &expresso)) {
                    printf("Arquivo lido com sucesso!\n");
                } else {
                    printf("Falha ao ler o arquivo '%s'. Verifique o nome ou o caminho.\n", arquivo);
                }
                break;

            case 2: {
                if(!mapa){
                    printf("\nPor favor, escolha a opcao 1 primeiro.\n");
                    break;
                }
                
                //  Resetar a matriz de visitados
                resetarVisitados(); 

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
                
                //  Resetar a matriz de visitados aqui tbm
                resetarVisitados(); 

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
                    //  Impressão de feedback para o usuário
                    if (ok) {
                         printf("Caminho encontrado (sem impressao de passos).\n");
                    } else {
                         printf("Nao foi possivel encontrar um caminho (sem impressao de passos).\n");
                    }
                }
                break;
            }

            case 5:
                modo_analise_ativo = !modo_analise_ativo;
                printf("Modo de analise agora esta: %s\n", modo_analise_ativo ? "ATIVADO" : "DESATIVADO");
                break;

            case 6:
            printf("Encerrando o programa e liberando memorias.\n");
            
            //  liberarVisitados() foi removida
            liberarMemoria(); 
            
            printf("Memorias liberadas e encerando o programa.\n");
                break;

            default:
                printf("Opcao invalida.\n");
                break;
        }
    }
    
    return 0;
}