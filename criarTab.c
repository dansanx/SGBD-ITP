#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include "criarTab.h"

void controlaMenu() {
    char resposta;
    printf("Deseja voltar ao menu? (s/n): ");
    scanf(" %c", &resposta);

    if (resposta == 'n' || resposta == 'N') {
        printf("Saindo do programa...\n");
        sleep(2);
        exit(0);  // Ou retorne a função principal de acordo com suas necessidades
    }
}

void criarTabela() {
    char nome_tabela[55];
    printf("\e[1;1H\e[2J"); // Move o cursor para a linha 1, coluna 1 e limpa a interface

    printf("==== CRIAR TABELA ====\n");
    printf("Nome da tabela: ");
    scanf("%s", nome_tabela);

    // Criando arquivo da tabela
    FILE *arquivo_tabela = fopen(strcat(nome_tabela, ".txt"), "w");
    if (arquivo_tabela == NULL) {
        printf("\nErro ao abrir arquivo.\n");
        return;
    }

    // Obtendo o número de colunas
    int qtd_colunas;
    printf("Quantidade de colunas: ");
    scanf("%d", &qtd_colunas);

    // Escrevendo no arquivo da tabela
    fprintf(arquivo_tabela, "0 %d\n", qtd_colunas);

    // Obtendo nomes das colunas
    char nome_coluna[21];
    for (int i = 0; i < qtd_colunas; i++) {
        if (i == 0) {
            printf("Nome - Chave primaria (coluna 1): ");
        } else {
            printf("Nome - Coluna %d: ", i + 1);
        }
        scanf("%s", nome_coluna);
        fprintf(arquivo_tabela, "%s%s", i == 0 ? "" : "|", nome_coluna);
    }

    printf("\e[1;1H\e[2J");
    printf("Tabela criada.\n");

    fclose(arquivo_tabela);

    // Adicionando o nome da tabela à lista de tabelas
    FILE *lista_tabelas = fopen("listaTab.txt", "a");
    if (lista_tabelas == NULL) {
        printf("\nErro ao abrir arquivo.\n");
        return;
    }

    fprintf(lista_tabelas, "%s\n", nome_tabela);
    fclose(lista_tabelas);

    controlaMenu();
}

void listarTabelas() {
    printf("\e[1;1H\e[2J");
    FILE *lista_tabelas = fopen("listaTab.txt", "r");

    if (lista_tabelas == NULL) {
        printf("\nErro ao abrir arquivo.\n");
        return;
    }

    printf("\n==== LISTAR TABELAS ====\n");

    char nome_tabela[55];
    while (fscanf(lista_tabelas, "%s", nome_tabela) != EOF) {
        printf("%s\n", nome_tabela);
    }
    // EOF - END OF FILE

    fclose(lista_tabelas);

    controlaMenu();
}

void criarNovaLinha() {
    FILE *lista_tabelas = fopen("listaTab.txt", "r");

    if (lista_tabelas == NULL) {
        printf("\nErro ao abrir arquivo.\n");
        return;
    }

    printf("\n==== CRIAR NOVA LINHA ====\n");

    char nome_tabela[55];
    int indice = 1;
    while (fscanf(lista_tabelas, "%s", nome_tabela) != EOF) {
        printf("%d - %s\n", indice, nome_tabela);
        indice++;
    }

    fclose(lista_tabelas);

    int escolha;
    printf("Escolha o numero da tabela para criar nova linha (ou 0 para voltar): ");
    scanf("%d", &escolha);

    if (escolha == 0) {
        return; // Voltar ao menu principal
    }

    // Abrir o arquivo da tabela escolhida para leitura
    char nome_arquivo[55];
    snprintf(nome_arquivo, sizeof(nome_arquivo), "%s.txt", nome_tabela);
    FILE *arquivo_tabela = fopen(nome_arquivo, "r");

    if (arquivo_tabela == NULL) {
        printf("\nErro ao abrir arquivo da tabela.\n");
        return;
    }

    // Identificar a quantidade de colunas
    fseek(arquivo_tabela, 2, SEEK_SET); // Move para o terceiro caractere do arquivo
    int qtd_colunas;
    fscanf(arquivo_tabela, "%1d", &qtd_colunas);

    // Ler a segunda linha (cabeçalho) e extrair os nomes das colunas
    fseek(arquivo_tabela, 0, SEEK_SET); // Move para o início do arquivo
    char linha[255];
    fgets(linha, sizeof(linha), arquivo_tabela); // Lê a primeira linha (não utilizada)
    fgets(linha, sizeof(linha), arquivo_tabela); // Lê a segunda linha (cabeçalho)

    // Extrair nomes das colunas
    char *token = strtok(linha, "|");
    char nomes_colunas[qtd_colunas][255];
    for (int i = 0; i < qtd_colunas; i++) {
        strcpy(nomes_colunas[i], token);
        token = strtok(NULL, "|");
    }

    fclose(arquivo_tabela);

    // Abrir o arquivo da tabela escolhida para escrita (modo append)
    arquivo_tabela = fopen(nome_arquivo, "a");

    if (arquivo_tabela == NULL) {
        printf("\nErro ao abrir arquivo da tabela.\n");
        return;
    }

    // Incluir quebra de linha antes de adicionar nova linha
    if (ftell(arquivo_tabela) > 0) {
        fprintf(arquivo_tabela, "\n");
    }

    printf("Insira os dados para cada coluna:\n");

    // Gerar chave primária única (valor inteiro aleatório)
    srand(time(NULL));
    int chave_primaria = rand();

    // Escrever a chave primária
    fprintf(arquivo_tabela, "%d", chave_primaria);

    // Perguntar e escrever os dados para as colunas restantes
    char dado[255];
    for (int i = 1; i < qtd_colunas; i++) {
        printf("Insira o dado para a coluna %s: ", nomes_colunas[i]);
        scanf("%s", dado);
        fprintf(arquivo_tabela, "|%s", dado);
    }

    fprintf(arquivo_tabela, "\n");

    fclose(arquivo_tabela);

    printf("Nova linha criada com sucesso.\n");

    controlaMenu();
}

void listarDadosTabela() {
    FILE *lista_tabelas = fopen("listaTab.txt", "r");

    if (lista_tabelas == NULL) {
        printf("\nErro ao abrir arquivo.\n");
        return;
    }

    printf("\n==== LISTAR DADOS DE TABELA ====\n");

    char nome_tabela[55];
    int indice = 1;
    while (fscanf(lista_tabelas, "%s", nome_tabela) != EOF) {
        printf("%d - %s\n", indice, nome_tabela);
        indice++;
    }

    fclose(lista_tabelas);

    int escolha;
    printf("Escolha o numero da tabela para listar os dados (ou 0 para voltar): ");
    scanf("%d", &escolha);

    if (escolha == 0) {
        return; // Voltar ao menu principal
    }

    // Abrir o arquivo da tabela escolhida
    char nome_arquivo[55];
    snprintf(nome_arquivo, sizeof(nome_arquivo), "%s", nome_tabela);
    FILE *arquivo_tabela = fopen(nome_arquivo, "r");

    if (arquivo_tabela == NULL) {
        printf("\nErro ao abrir arquivo da tabela.\n");
        return;
    }

    // Ler e mostrar os dados da tabela
    char linha[255];
    while (fgets(linha, sizeof(linha), arquivo_tabela) != NULL) {
        printf("%s", linha);
    }

    fclose(arquivo_tabela);

    controlaMenu();
}

void pesquisaTab() {
    FILE *lista_tabelas = fopen("listaTab.txt", "r");

    if (lista_tabelas == NULL) {
        printf("\nErro ao abrir arquivo.\n");
        return;
    }

    printf("\n==== PESQUISAR VALOR EM UMA TABELA ====\n");

    char nome_tabela[55];
    int indice = 1;
    while (fscanf(lista_tabelas, "%s", nome_tabela) != EOF) {
        printf("%d - %s\n", indice, nome_tabela);
        indice++;
    }

    fclose(lista_tabelas);

    int escolha;
    printf("Escolha o numero da tabela para pesquisa (ou 0 para voltar): ");
    scanf("%d", &escolha);

    if (escolha == 0) {
        return; // Voltar ao menu principal
    }

    // Abrir o arquivo da tabela escolhida para leitura
    snprintf(nome_tabela, sizeof(nome_tabela), "%s.txt", nome_tabela);
    FILE *arquivo_tabela = fopen(nome_tabela, "r");

    if (arquivo_tabela == NULL) {
        printf("\nErro ao abrir arquivo da tabela.\n");
        return;
    }

    // Pedir ao usuário para inserir o valor a ser pesquisado
    char valor_pesquisar[255];
    printf("Digite o valor a ser pesquisado (case-insensitive): ");
    scanf("%s", valor_pesquisar);

    // Convertendo o valor a ser pesquisado para minúsculas (case-insensitive)
    for (int i = 0; valor_pesquisar[i]; i++) {
        valor_pesquisar[i] = tolower(valor_pesquisar[i]);
    }

    // Implementação da lógica de pesquisa
    char linha[255];
    int linha_atual = 1;
    int encontrado = 0;

    while (fgets(linha, sizeof(linha), arquivo_tabela) != NULL) {
        // Manter uma cópia da linha original para impressão
        char linha_original[255];
        strcpy(linha_original, linha);

        // Convertendo a linha apenas para minúsculas (case-insensitive)
        for (int i = 0; linha[i]; i++) {
            linha[i] = tolower(linha[i]);
        }

        // Comparando palavras completas usando "|" como delimitador
        char *palavra = strtok(linha, "| \t\n");

        while (palavra != NULL) {
            if (strcmp(palavra, valor_pesquisar) == 0) {
                printf("Valor encontrado na linha %d: %s", linha_atual, linha_original);
                encontrado = 1;
                break; // Encerrar a verificação se a palavra for encontrada
            }
            palavra = strtok(NULL, "| \t\n");
        }

        linha_atual++;
    }

    if (!encontrado) {
        printf("Valor nao encontrado.\n");
    }

    fclose(arquivo_tabela);

    controlaMenu();
}

void apagarLinhaTab() {
    FILE *lista_tabelas = fopen("listaTab.txt", "r");

    if (lista_tabelas == NULL) {
        printf("\nErro ao abrir arquivo.\n");
        return;
    }

    printf("\n==== APAGAR LINHA DE TABELA ====\n");

    char nome_tabela[55];
    int indice = 1;
    while (fscanf(lista_tabelas, "%s", nome_tabela) != EOF) {
        printf("%d - %s\n", indice, nome_tabela);
        indice++;
    }

    fclose(lista_tabelas);

    int escolha;
    printf("Escolha o numero da tabela para apagar linha (ou 0 para voltar): ");
    scanf("%d", &escolha);

    if (escolha == 0) {
        return; // Voltar ao menu principal
    }

    // Abrir o arquivo da tabela escolhida para leitura
    snprintf(nome_tabela, sizeof(nome_tabela), "%s.txt", nome_tabela);
    FILE *arquivo_tabela = fopen(nome_tabela, "r");

    if (arquivo_tabela == NULL) {
        printf("\nErro ao abrir arquivo da tabela.\n");
        return;
    }

    // Ler e mostrar as linhas do arquivo (ignorando as duas primeiras linhas)
    int linha_atual = 1;
    char linha[255];
    while (fgets(linha, sizeof(linha), arquivo_tabela) != NULL) {
        if (linha_atual > 2) {
            printf("%d - %s", linha_atual - 2, linha);
        }
        linha_atual++;
    }

    fclose(arquivo_tabela);

    // Implemente a lógica para apagar uma linha da tabela aqui
    printf("Escolha o numero da linha para apagar (ou 0 para voltar): ");
    scanf("%d", &escolha);

    if (escolha == 0) {
        return; // Voltar ao menu principal
    }

    // Reabrir o arquivo original para leitura
    arquivo_tabela = fopen(nome_tabela, "r");
    if (arquivo_tabela == NULL) {
        printf("\nErro ao abrir arquivo da tabela.\n");
        return;
    }

    // Criar um arquivo temporário para escrita
    FILE *temporario_tabela = fopen("temporario.txt", "w");
    if (temporario_tabela == NULL) {
        printf("\nErro ao criar arquivo temporario.\n");
        fclose(arquivo_tabela);
        return;
    }

    // Copiar todas as linhas, exceto a escolhida, para o arquivo temporário
    linha_atual = 1;
    while (fgets(linha, sizeof(linha), arquivo_tabela) != NULL) {
        if (linha_atual != escolha + 2) {
            fputs(linha, temporario_tabela);
        }
        linha_atual++;
    }

    fclose(arquivo_tabela);
    fclose(temporario_tabela);

    // Substituir o arquivo original pelo arquivo temporário
    remove(nome_tabela);
    rename("temporario.txt", nome_tabela);

    printf("Linha apagada com sucesso.\n");

    controlaMenu();
}

void apagarTabela() {
    FILE *lista_tabelas = fopen("listaTab.txt", "r");
    FILE *temp_lista_tabelas = fopen("temp_listaTab.txt", "w");

    if (lista_tabelas == NULL || temp_lista_tabelas == NULL) {
        printf("\nErro ao abrir arquivo.\n");
        return;
    }

    printf("\n==== APAGAR TABELA ====\n");

    char nome_tabela[55];
    int indice = 1;
    while (fscanf(lista_tabelas, "%s", nome_tabela) != EOF) {
        printf("%d - %s\n", indice, nome_tabela);
        indice++;
    }

    fclose(lista_tabelas);

    int escolha;
    printf("Escolha o numero da tabela que deseja apagar (ou 0 para cancelar): ");
    scanf("%d", &escolha);

    if (escolha == 0) {
        remove("temp_listaTab.txt");
        return; // Cancelando a operação
    }

    lista_tabelas = fopen("listaTab.txt", "r");

    indice = 1;
    while (fscanf(lista_tabelas, "%s", nome_tabela) != EOF) {
        if (indice != escolha) {
            fprintf(temp_lista_tabelas, "%s\n", nome_tabela);
        } else {
            // Removendo o arquivo da tabela
            char nome_arquivo[55];
            strcpy(nome_arquivo, nome_tabela);
            //strcat(nome_arquivo, ".txt");
            remove(nome_arquivo);
        }
        indice++;
    }

    fclose(lista_tabelas);
    fclose(temp_lista_tabelas);

    // Substituindo o arquivo original pelo temporário
    remove("listaTab.txt");
    rename("temp_listaTab.txt", "listaTab.txt");

    printf("Tabela apagada com sucesso.\n");

    controlaMenu();
}
