#include "../headers/expresso.h"
#include "../headers/mapa.h"

int main(){
    int opcao = 0;
    char arquivo[100];
    Expresso expresso;
    
    printf("\nOs cientistas espaciais fizeram um programa para calcular um caminho possivel para a celebracao.\nVoce pode usar o programa, mas como dono do expresso tera que aumentar o salario de todos os \ncientistas que ajudaram no processo :) (nao negociavel) \n");

    while(opcao != 3){
        printf("\n ====================== MENU ====================== \n");
        printf("1) Digite o nome do mapa a ser usado na exploracao: \n");
        printf("2) Encontrar caminho viavel para a celebracao \n");
        printf("3) Imprimir mapa\n");
        printf("4) Sair do programa \n");

        scanf("%d", &opcao);
        
        switch (opcao){
            case 1:
                printf("Nome do arquivo: \n");
                scanf("%s", arquivo);
                lerArquivo(arquivo, &expresso);
                printf("Arquivo lido com sucesso! \n");
                break;
            case 2:
                if(!mapa){
                    printf("\nPor favor, escolha a opcao 1 primeiro \n(* o aumento de salario realmente valeu a pena? :/ *) \n");
                    break;
                }else{
                    imprimeMapa();
                    bool ok = encontrarCaminho(&expresso);
                }

                break;
            case 3:
                {
                    printf("Imprimindo o mapa");
                    imprimeMapa();
                }
            default:
                break;
        }
    }  
    return 0;
}