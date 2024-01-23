#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "criarTab.h"

int main() {
    int opcao;

    while (1) {  // Loop infinito para controlar o fluxo manualmente
        printf("\e[1;1H\e[2J");
        printf("\n========== MENU ==========\n");
        printf("1 - Criar tabela\n");
        printf("2 - Listar tabelas\n");
        printf("3 - Criar nova linha\n");
        printf("4 - Listar dados de uma tabela\n");
        printf("5 - Pesquisar valor em uma tabela\n");
        printf("6 - Apagar uma linha de uma tabela\n");
        printf("7 - Apagar uma tabela\n");
        printf("0 - Sair\n");
        printf("===========================\n");

        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpar o buffer de entrada

        switch (opcao) {
            case 1:
                criarTabela();
                break;
            case 2:
                listarTabelas();
                break;
            case 3:
                criarNovaLinha();
                break;
            case 4:
                listarDadosTabela();
                break;
            case 5:
                pesquisaTab();
                break;
            case 6:
                apagarLinhaTab();
                break;
            case 7:
                apagarTabela();
                break;
            case 0:
                printf("Saindo do programa...\n");
                sleep(2);
                return 0;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    }

    return 0;
}
