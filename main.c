#include <stdio.h>  // Essencial para entrada e saída padrão e manipulação de arquivos.
#include <string.h> // Usada para realizar operações em strings e blocos de memória.
#include <ctype.h>  // Trabalha com caracteres individuais de forma eficiente.
#include <locale.h> // Adapta o programa para diferentes regiões ou idiomas.
#include <stdlib.h> // Funções utilitárias para memória, conversões e controle de execução.

// Função para importar estoque
void carregarProdutosDeArquivo(char NomeProduto[][30], float Quantidade[], float Preco[], int *quantidade, const char *nomeArquivo)
{
    // Abrir txt onde importa o estoque
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL)
    {
        printf("Arquivo '%s' não encontrado. Inventário vazio.\n", nomeArquivo);
        *quantidade = 0;
        return;
    }

    // Leitura de linhas
    char linha[200]; // Máximo 200 caracteres por linha.
    *quantidade = 0; // Porteiro para saber a quantidade de produtos
    while (fgets(linha, sizeof(linha), arquivo))
    {
        linha[strcspn(linha, "\n")] = '\0'; // Remove o caractere de nova linha (\n) que pode ser incluído pelo fgets.
        char nome[30];
        float preco, quantidadeProduto;

        // Scannear linhas
        if (sscanf(linha, "%[^|]|%f|%f", nome, &preco, &quantidadeProduto) == 3) // "%[^|]" separa o nome do produto até a |.
        {
            // Armazenamento nos Arrays
            if (*quantidade < 50)
            {
                snprintf(NomeProduto[*quantidade], 100, "%s", nome);
                Preco[*quantidade] = preco;
                Quantidade[*quantidade] = quantidadeProduto;
                (*quantidade)++;
            }
        }
        else // Tratamento de Erros
        {
            printf("Erro ao interpretar a linha: '%s'\n", linha);
        }
    }

    fclose(arquivo);
    printf("%d produtos carregados com sucesso do arquivo '%s'.\n", *quantidade, nomeArquivo);
}

// Função para exportar estoque para txt
void salvarProdutosEmArquivo(const char NomeProduto[][30], float Quantidade[], float Preco[], int quantidade, const char *nomeArquivo)
{
    // Abrir txt onde exporta o estoque
    FILE *arquivo = fopen(nomeArquivo, "w"); // Abrir o arquivo no modo de escrita: "w"
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo '%s'.\n", nomeArquivo);
        return;
    }

    // Salvar o arquivo
    for (int i = 0; i < quantidade; i++)
    {
        fprintf(arquivo, "%s|%.2f|%.2f\n", NomeProduto[i], Preco[i], Quantidade[i]);
    }

    fclose(arquivo);
    printf("Produtos salvos no arquivo '%s'.\n", nomeArquivo);
}

// Função do caixa
void registrarCompra(char NomeProduto[][30], float Quantidade[], float Preco[], int *quantidade)
{
    // Cria um arquivo de texto para fazer a nota fiscal.
    FILE *notaFiscal = fopen("nota_fiscal.txt", "w");
    if (notaFiscal == NULL)
    {
        printf("Erro ao criar a nota fiscal.\n");
        return;
    }

    // Variáveis
    int produtoIndex; // Recebe o ID do produto selecionado para compra.
    float quantidadeCompra; // Quantidade do produto que será comprada.
    float totalCompra = 0.0; // Acumula o valor total da compra (soma de todos os produtos comprados).
    char continuarCompra[10]; // Armazena a resposta do usuário para continuar ou encerrar a compra.

    system("cls"); // Limpar tela para exibir novo menu
    printf("----------------------- Início da Compra -----------------------\n");

    // Loop de Compra
    do
    {
        // Entrada de ID
        printf("Digite o ID do produto para compra (1 a %d): ", *quantidade);
        scanf("%d", &produtoIndex);
        getchar();

        // Validação de ID
        if (produtoIndex < 1 || produtoIndex > *quantidade)
        {
            printf("Produto inválido.\n");
            continue;
        }

        // Entrada de quantidade
        printf("Digite a quantidade do produto a ser comprada (kg): ");
        scanf("%f", &quantidadeCompra);
        getchar();

        if (quantidadeCompra > Quantidade[produtoIndex - 1])
        {
            printf("Quantidade insuficiente em estoque.\n");
            continue;
        }

        Quantidade[produtoIndex - 1] -= quantidadeCompra; // reduzir a quantidade

        // calculo de preço a ser pago
        float preco = Preco[produtoIndex - 1];
        float totalProduto = preco * quantidadeCompra;
        totalCompra += totalProduto;

        //Adiciona os detalhes da compra ao arquivo nota_fiscal.txt
        fprintf(notaFiscal, "----------------------------------------------------------\n");
        fprintf(notaFiscal, "Produto: %s\n", NomeProduto[produtoIndex - 1]);
        fprintf(notaFiscal, "Quantidade comprada (kg): %.2f\n", quantidadeCompra);
        fprintf(notaFiscal, "R$/kg: %.2f\n", preco);
        fprintf(notaFiscal, "Total: %.2f\n", totalProduto);

        printf("Produto adicionado à nota fiscal.\n");

        // repetir loop e adicionar mais produtos
        printf("Deseja adicionar mais produtos à compra? (s/n): ");
        scanf(" %9s", continuarCompra);
    } while (strcasecmp(continuarCompra, "s") == 0 || strcasecmp(continuarCompra, "sim") == 0 || strcasecmp(continuarCompra, "y") == 0 || strcasecmp(continuarCompra, "yes") == 0);

    // exibir valor na tela
    fprintf(notaFiscal, "----------------------- Nota Fiscal ----------------------\n");
    fprintf(notaFiscal, "Total geral (R$): %.2f\n", totalCompra);
    fclose(notaFiscal); // fechar nota_fiscal.txt

    system("cls"); // Limpar tela para exibir novo menu
    printf("---------------------- Nota Fiscal ----------------------\n");
    printf("Total geral: %.2f\n", totalCompra);
    printf("----------------------------------------------------------\n");

    printf("Compra registrada com sucesso! Nota fiscal gerada.\n");
    salvarProdutosEmArquivo(NomeProduto, Quantidade, Preco, *quantidade, "estoque.txt"); // Atualizar estoque, tirando oq foi comprado
}

// Função para adicionar um novo produto ao estoque
void adicionarProduto(char NomeProduto[][30], float Quantidade[], float Preco[], int *quantidade)
{
    // Verificação de Limite do Estoque
    if (*quantidade >= 50)
    {
        printf("Inventário cheio! Não é possível adicionar mais produtos.\n");
        return;
    }

    // Variáveis
    char nome[30];
    float preco;
    int quantidadeProduto;

    // Entrada dos Dados do Produto
    printf("Digite o nome do produto: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    printf("Digite o preço do produto por quilograma (R$/kg): ");
    scanf("%f", &preco);
    getchar();

    printf("Digite a quantidade do produto: ");
    scanf("%d", &quantidadeProduto);
    getchar();

    // Adicionando o Produto ao Estoque
    snprintf(NomeProduto[*quantidade], 100, "%s", nome);
    Preco[*quantidade] = preco;
    Quantidade[*quantidade] = quantidadeProduto;
    (*quantidade)++;

    printf("Produto adicionado com sucesso!\n");
}

// Função para remover um produto existente do estoque
void removerProduto(char NomeProduto[][30], float Quantidade[], float Preco[], int *quantidade)
{
    // Verificação do Estoque
    if (*quantidade == 0)
    {
        printf("Nenhum produto para remover.\n");
        return;
    }

    // Entrada do ID do Produto
    int indice;
    printf("Digite o ID do produto que deseja remover (1 a %d): ", *quantidade);
    scanf("%d", &indice);
    getchar();

    // Validação do ID
    if (indice < 1 || indice > *quantidade)
    {
        printf("ID inválido.\n");
        return;
    }

    // Remoção do Produto
    for (int i = indice - 1; i < *quantidade - 1; i++)
    {
        strcpy(NomeProduto[i], NomeProduto[i + 1]);
        Quantidade[i] = Quantidade[i + 1];
        Preco[i] = Preco[i + 1];
    }

    // Atualização do Total de Produtos
    (*quantidade)--;
    printf("Produto removido com sucesso!\n");
}

// Função para editar um produto existente do estoque
void editarProduto(char NomeProduto[][30], float Quantidade[], float Preco[], int quantidade)
{
    // Verificação do Estoque
    if (quantidade == 0)
    {
        printf("Nenhum produto para editar.\n");
        return;
    }

    // Entrada do ID do Produto
    int indice;
    printf("Digite o ID do produto que deseja editar (1 a %d): ", quantidade);
    scanf("%d", &indice);
    getchar();

    // Validação do ID
    if (indice < 1 || indice > quantidade)
    {
        printf("ID inválido.\n");
        return;
    }

    // Variáveis
    char nome[30];
    float preco;
    int quantidadeProduto;

    // Entrada dos Dados do Produto
    printf("Digite o novo nome do produto: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    printf("Digite o novo preço do produto por quilograma (R$/kg): ");
    scanf("%f", &preco);
    getchar();

    printf("Digite a nova quantidade do produto: ");
    scanf("%d", &quantidadeProduto);
    getchar();

    // Adicionando o Produto ao Estoque
    snprintf(NomeProduto[indice - 1], 100, "%s", nome);
    Preco[indice - 1] = preco;
    Quantidade[indice - 1] = quantidadeProduto;

    printf("Produto editado com sucesso!\n");
}

// Função para gerenciar o inventário do programa
void inventariarProduto(char NomeProduto[][30], float Quantidade[], float Preco[], int *quantidade)
{
    int opcao;

    // Estrutura de Repetição
    do
    {
        // Exibição do Menu
        system("cls"); // Limpar tela para exibir novo menu
        printf("------------ Inventário ------------\n");
        printf("1. Adicionar Produto\n");
        printf("2. Remover Produto\n");
        printf("3. Editar Produto\n");
        printf("4. Voltar\n");
        printf("------------------------------------\n");

        // Leitura da Opção
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao)
        {
        case 1:
            adicionarProduto(NomeProduto, Quantidade, Preco, quantidade); // Função de adicionar produto
            salvarProdutosEmArquivo(NomeProduto, Quantidade, Preco, *quantidade, "estoque.txt"); // Salvamento do Estoque
            break;
        case 2:
            removerProduto(NomeProduto, Quantidade, Preco, quantidade); // Função de remover produto
            salvarProdutosEmArquivo(NomeProduto, Quantidade, Preco, *quantidade, "estoque.txt"); // Salvamento do Estoque
            break;
        case 3:
            editarProduto(NomeProduto, Quantidade, Preco, *quantidade); // Função de edit produto
            salvarProdutosEmArquivo(NomeProduto, Quantidade, Preco, *quantidade, "estoque.txt"); // Salvamento do Estoque
            break;
        case 4:
            printf("Voltando ao menu principal...\n");
            break;
        default:
            printf("Opção inválida.\n");
        }
    } while (opcao != 4);
}

// Função para exibir estoque do programa
void estoque(const char NomeProduto[][30], float Quantidade[], float Preco[], int quantidade)
{
    // Verificar estoque
    if (quantidade == 0)
    {
        printf("Nenhum produto no inventário.\n");
        return;
    }

    // Exibir Estoque
    system("cls"); // Limpar tela para exibir novo menu
    printf("---------------------------------- Estoque ----------------------------------\n");
    for (int i = 0; i < quantidade; i++)
    {
        printf("%03d: %-29s | R$/kg: %-6.2f | Quantidade (kg): %6.2f\n", i + 1, NomeProduto[i], Preco[i], Quantidade[i]);
    }
    printf("-----------------------------------------------------------------------------\n");
}

// Iniciar o programa
int main()
{
    // idioma definido para português com a codificação (ISO 8859-1) | idioma default: "Portuguese_Brazil.1252"
    setlocale(LC_ALL, "Portuguese_Brazil.1252");

    // Variáveis
    char NomeProduto[50][30]; // Nome do produto, primeira casa [50] espaços disponiveis no estoque, segunda casa [30] para 29 caractéres possiveis pro nome
    float Quantidade[50]; // Quantidade de espaço no estoque
    float Preco[50]; // Preço dos produtos, casa [50] espaços disponiveis no estoque
    int quantidade = 0; // Quantidade para ID
    int escolha; // Escolha para entrar em menu
    char continuar[10]; // Continuar para escolha de manter o programa aberto, [10] caracteres limite

    // Chamar função de importar estoque usando "estoque.txt"
    carregarProdutosDeArquivo(NomeProduto, Quantidade, Preco, &quantidade, "estoque.txt");

    do
    {
        // Menu principal
        system("cls"); // Limpar tela para exibir novo menu
        printf("------------ Menu ------------\n");
        printf("1. Inventário\n");
        printf("2. Estoque\n");
        printf("3. Caixa\n");
        printf("4. Sair\n");
        printf("------------------------------\n");
        printf("Escolha uma opção: ");
        scanf("%d", &escolha); // Entrar na opção definida pelo usuário
        getchar();

        // Switch para escolha
        switch (escolha)
        {
        case 1:
            // Chamar função de inventário
            inventariarProduto(NomeProduto, Quantidade, Preco, &quantidade);
            break;
        case 2:
            // Chamar função de estoque
            estoque(NomeProduto, Quantidade, Preco, quantidade);
            break;
        case 3:
            // Chamar função de caixa
            registrarCompra(NomeProduto, Quantidade, Preco, &quantidade);
            break;
        case 4:
            // Fechar programa
            printf("Saindo do programa...\n");
            break;
        default:
            printf("Opção inválida.\n");
        }

        // função para continuar o programa
        if (escolha != 4)
        {
            printf("Deseja continuar no programa? (s/n): ");
            scanf("%s", continuar);
        }

    } while (strcasecmp(continuar, "s") == 0 || // Caso "s", continuar o programa
            strcasecmp(continuar, "sim") == 0 || // Caso "sim", continuar o programa
            strcasecmp(continuar, "y") == 0 || // Caso "y", continuar o programa
            strcasecmp(continuar, "yes") == 0 && escolha != 4); // Caso "yes", continuar o programa
    return 0;
}