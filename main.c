#include <stdio.h>  // Essencial para entrada e sa�da padr�o e manipula��o de arquivos.
#include <string.h> // Usada para realizar opera��es em strings e blocos de mem�ria.
#include <ctype.h>  // Trabalha com caracteres individuais de forma eficiente.
#include <locale.h> // Adapta o programa para diferentes regi�es ou idiomas.
#include <stdlib.h> // Fun��es utilit�rias para mem�ria, convers�es e controle de execu��o.

// Fun��o para importar estoque
void carregarProdutosDeArquivo(char NomeProduto[][30], float Quantidade[], float Preco[], int *quantidade, const char *nomeArquivo)
{
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL)
    {
        printf("Arquivo '%s' n�o encontrado. Invent�rio vazio.\n", nomeArquivo);
        *quantidade = 0;
        return;
    }

    char linha[200];
    *quantidade = 0;
    while (fgets(linha, sizeof(linha), arquivo))
    {
        linha[strcspn(linha, "\n")] = '\0';
        char nome[30];
        float preco, quantidadeProduto;

        if (sscanf(linha, "%[^|]|%f|%f", nome, &preco, &quantidadeProduto) == 3)
        {
            if (*quantidade < 50)
            {
                snprintf(NomeProduto[*quantidade], 100, "%s", nome);
                Preco[*quantidade] = preco;
                Quantidade[*quantidade] = quantidadeProduto;
                (*quantidade)++;
            }
        }
        else
        {
            printf("Erro ao interpretar a linha: '%s'\n", linha);
        }
    }

    fclose(arquivo);
    printf("%d produtos carregados com sucesso do arquivo '%s'.\n", *quantidade, nomeArquivo);
}

// Fun��o para exportar estoque para txt
void salvarProdutosEmArquivo(const char NomeProduto[][30], float Quantidade[], float Preco[], int quantidade, const char *nomeArquivo)
{
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo '%s'.\n", nomeArquivo);
        return;
    }

    for (int i = 0; i < quantidade; i++)
    {
        fprintf(arquivo, "%s|%.2f|%.2f\n", NomeProduto[i], Preco[i], Quantidade[i]);
    }

    fclose(arquivo);
    printf("Produtos salvos no arquivo '%s'.\n", nomeArquivo);
}

// Fun��o do caixa
void registrarCompra(char NomeProduto[][30], float Quantidade[], float Preco[], int *quantidade)
{
    FILE *notaFiscal = fopen("nota_fiscal.txt", "w");
    if (notaFiscal == NULL)
    {
        printf("Erro ao criar a nota fiscal.\n");
        return;
    }

    int produtoIndex;
    float quantidadeCompra;
    float totalCompra = 0.0;
    char continuarCompra[10];

    system("cls");
    printf("----------------------- In�cio da Compra -----------------------\n");

    do
    {
        printf("Digite o ID do produto para compra (1 a %d): ", *quantidade);
        scanf("%d", &produtoIndex);
        getchar();

        if (produtoIndex < 1 || produtoIndex > *quantidade)
        {
            printf("Produto inv�lido.\n");
            continue;
        }

        printf("Digite a quantidade do produto a ser comprada (kg): ");
        scanf("%f", &quantidadeCompra);
        getchar();

        if (quantidadeCompra > Quantidade[produtoIndex - 1])
        {
            printf("Quantidade insuficiente em estoque.\n");
            continue;
        }

        Quantidade[produtoIndex - 1] -= quantidadeCompra;

        float preco = Preco[produtoIndex - 1];
        float totalProduto = preco * quantidadeCompra;
        totalCompra += totalProduto;
        fprintf(notaFiscal, "----------------------------------------------------------\n");
        fprintf(notaFiscal, "Produto: %s\n", NomeProduto[produtoIndex - 1]);
        fprintf(notaFiscal, "Quantidade comprada (kg): %.2f\n", quantidadeCompra);
        fprintf(notaFiscal, "R$/kg: %.2f\n", preco);
        fprintf(notaFiscal, "Total: %.2f\n", totalProduto);

        printf("Produto adicionado � nota fiscal.\n");

        printf("Deseja adicionar mais produtos � compra? (s/n): ");
        scanf(" %9s", continuarCompra);
    } while (strcasecmp(continuarCompra, "s") == 0 || strcasecmp(continuarCompra, "sim") == 0 || strcasecmp(continuarCompra, "y") == 0 || strcasecmp(continuarCompra, "yes") == 0);

    fprintf(notaFiscal, "----------------------- Nota Fiscal ----------------------\n");
    fprintf(notaFiscal, "Total geral (R$): %.2f\n", totalCompra);
    fclose(notaFiscal);

    system("cls");
    printf("---------------------- Nota Fiscal ----------------------\n");
    printf("Total geral: %.2f\n", totalCompra);
    printf("----------------------------------------------------------\n");

    printf("Compra registrada com sucesso! Nota fiscal gerada.\n");
    salvarProdutosEmArquivo(NomeProduto, Quantidade, Preco, *quantidade, "estoque.txt");
}

// Fun��o para adicionar um novo produto ao estoque
void adicionarProduto(char NomeProduto[][30], float Quantidade[], float Preco[], int *quantidade)
{
    if (*quantidade >= 50)
    {
        printf("Invent�rio cheio! N�o � poss�vel adicionar mais produtos.\n");
        return;
    }

    char nome[30];
    float preco;
    int quantidadeProduto;

    printf("Digite o nome do produto: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    printf("Digite o pre�o do produto por quilograma (R$/kg): ");
    scanf("%f", &preco);
    getchar();

    printf("Digite a quantidade do produto: ");
    scanf("%d", &quantidadeProduto);
    getchar();

    snprintf(NomeProduto[*quantidade], 100, "%s", nome);
    Preco[*quantidade] = preco;
    Quantidade[*quantidade] = quantidadeProduto;
    (*quantidade)++;

    printf("Produto adicionado com sucesso!\n");
}

// Fun��o para remover um produto existente do estoque
void removerProduto(char NomeProduto[][30], float Quantidade[], float Preco[], int *quantidade)
{
    if (*quantidade == 0)
    {
        printf("Nenhum produto para remover.\n");
        return;
    }

    int indice;
    printf("Digite o ID do produto que deseja remover (1 a %d): ", *quantidade);
    scanf("%d", &indice);
    getchar();

    if (indice < 1 || indice > *quantidade)
    {
        printf("ID inv�lido.\n");
        return;
    }

    for (int i = indice - 1; i < *quantidade - 1; i++)
    {
        strcpy(NomeProduto[i], NomeProduto[i + 1]);
        Quantidade[i] = Quantidade[i + 1];
        Preco[i] = Preco[i + 1];
    }

    (*quantidade)--;
    printf("Produto removido com sucesso!\n");
}

// Fun��o para editar um produto existente do estoque
void editarProduto(char NomeProduto[][30], float Quantidade[], float Preco[], int quantidade)
{
    if (quantidade == 0)
    {
        printf("Nenhum produto para editar.\n");
        return;
    }

    int indice;
    printf("Digite o ID do produto que deseja editar (1 a %d): ", quantidade);
    scanf("%d", &indice);
    getchar();

    if (indice < 1 || indice > quantidade)
    {
        printf("ID inv�lido.\n");
        return;
    }

    char nome[30];
    float preco;
    int quantidadeProduto;

    printf("Digite o novo nome do produto: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    printf("Digite o novo pre�o do produto por quilograma (R$/kg): ");
    scanf("%f", &preco);
    getchar();

    printf("Digite a nova quantidade do produto: ");
    scanf("%d", &quantidadeProduto);
    getchar();

    snprintf(NomeProduto[indice - 1], 100, "%s", nome);
    Preco[indice - 1] = preco;
    Quantidade[indice - 1] = quantidadeProduto;

    printf("Produto editado com sucesso!\n");
}

// Fun��o para gerenciar o invent�rio do programa
void inventariarProduto(char NomeProduto[][30], float Quantidade[], float Preco[], int *quantidade)
{
    int opcao;
    do
    {
        system("cls");
        printf("------------ Invent�rio ------------\n");
        printf("1. Adicionar Produto\n");
        printf("2. Remover Produto\n");
        printf("3. Editar Produto\n");
        printf("4. Voltar\n");
        printf("------------------------------------\n");
        printf("Escolha uma op��o: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao)
        {
        case 1:
            adicionarProduto(NomeProduto, Quantidade, Preco, quantidade);
            salvarProdutosEmArquivo(NomeProduto, Quantidade, Preco, *quantidade, "estoque.txt");
            break;
        case 2:
            removerProduto(NomeProduto, Quantidade, Preco, quantidade);
            salvarProdutosEmArquivo(NomeProduto, Quantidade, Preco, *quantidade, "estoque.txt");
            break;
        case 3:
            editarProduto(NomeProduto, Quantidade, Preco, *quantidade);
            salvarProdutosEmArquivo(NomeProduto, Quantidade, Preco, *quantidade, "estoque.txt");
            break;
        case 4:
            printf("Voltando ao menu principal...\n");
            break;
        default:
            printf("Op��o inv�lida.\n");
        }
    } while (opcao != 4);
}

// Fun��o para exibir estoque do programa
void estoque(const char NomeProduto[][30], float Quantidade[], float Preco[], int quantidade)
{
    if (quantidade == 0)
    {
        printf("Nenhum produto no invent�rio.\n");
        return;
    }

    system("cls");
    printf("------------------------- Estoque -------------------------\n");
    for (int i = 0; i < quantidade; i++)
    {
        printf("%03d: %-15s | R$/kg: %-6.2f | Quantidade (kg): %6.2f\n", i + 1, NomeProduto[i], Preco[i], Quantidade[i]);
    }
    printf("-----------------------------------------------------------\n");
}

// Iniciar o programa
int main()
{
    setlocale(LC_ALL, "Portuguese_Brazil.1252");

    char NomeProduto[50][30];
    float Quantidade[50];
    float Preco[50];
    int quantidade = 0;
    int escolha;
    char continuar[10];

    carregarProdutosDeArquivo(NomeProduto, Quantidade, Preco, &quantidade, "estoque.txt");

    do
    {
        system("cls");
        printf("------------ Menu ------------\n");
        printf("1. Invent�rio\n");
        printf("2. Estoque\n");
        printf("3. Caixa\n");
        printf("4. Sair\n");
        printf("------------------------------\n");
        printf("Escolha uma op��o: ");
        scanf("%d", &escolha);
        getchar();

        switch (escolha)
        {
        case 1:
            inventariarProduto(NomeProduto, Quantidade, Preco, &quantidade);
            break;
        case 2:
            estoque(NomeProduto, Quantidade, Preco, quantidade);
            break;
        case 3:
            registrarCompra(NomeProduto, Quantidade, Preco, &quantidade);
            break;
        case 4:
            printf("Saindo do programa...\n");
            break;
        default:
            printf("Op��o inv�lida.\n");
        }

        if (escolha != 4)
        {
            printf("Deseja continuar no programa? (s/n): ");
            scanf("%s", continuar);
        }

    } while (strcasecmp(continuar, "s") == 0 && escolha != 4);

    return 0;
}