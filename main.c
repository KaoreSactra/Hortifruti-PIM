#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <stdlib.h>

void carregarProdutosDeArquivo(char NomeProduto[][100], int Quantidade[], float Preco[], int *quantidade, const char *nomeArquivo)
{
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL)
    {
        printf("Arquivo '%s' não encontrado. Inventário vazio.\n", nomeArquivo);
        *quantidade = 0;
        return;
    }

    char linha[200];
    *quantidade = 0;
    while (fgets(linha, sizeof(linha), arquivo))
    {
        linha[strcspn(linha, "\n")] = '\0';
        char nome[100];
        float preco;
        int quantidadeProduto;

        if (sscanf(linha, "%[^|]|%f|%d", nome, &preco, &quantidadeProduto) == 3)
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

void salvarProdutosEmArquivo(const char NomeProduto[][100], int Quantidade[], float Preco[], int quantidade, const char *nomeArquivo)
{
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo '%s'.\n", nomeArquivo);
        return;
    }

    for (int i = 0; i < quantidade; i++)
    {
        fprintf(arquivo, "%s|%.2f|%d\n", NomeProduto[i], Preco[i], Quantidade[i]);
    }

    fclose(arquivo);
    printf("Produtos salvos no arquivo '%s'.\n", nomeArquivo);
}

void registrarCompra(char NomeProduto[][100], int Quantidade[], float Preco[], int *quantidade)
{
    FILE *notaFiscal = fopen("nota_fiscal.txt", "w");
    if (notaFiscal == NULL)
    {
        printf("Erro ao criar a nota fiscal.\n");
        return;
    }

    int produtoIndex, quantidadeCompra;
    float totalCompra = 0.0;
    char continuarCompra[10];

    system("cls");
    printf("----------------------- Início da Compra -----------------------\n");

    do
    {
        printf("Digite o ID do produto para compra (1 a %d): ", *quantidade);
        scanf("%d", &produtoIndex);
        getchar();

        if (produtoIndex < 1 || produtoIndex > *quantidade)
        {
            printf("Produto inválido.\n");
            continue;
        }

        printf("Digite a quantidade do produto a ser comprada: ");
        scanf("%d", &quantidadeCompra);
        getchar();

        if (quantidadeCompra > Quantidade[produtoIndex - 1])
        {
            printf("Quantidade insuficiente em estoque.\n");
            continue;
        }

        Quantidade[produtoIndex - 1] -= quantidadeCompra;

        float preco = Preco[produtoIndex - 1];
        float totalProduto = preco * quantidadeCompra; // Total para este produto
        totalCompra += totalProduto;
        fprintf(notaFiscal, "----------------------------------------------------------\n");
        fprintf(notaFiscal, "Produto: %s\n", NomeProduto[produtoIndex - 1]);
        fprintf(notaFiscal, "Quantidade comprada (kg): %d\n", quantidadeCompra);
        fprintf(notaFiscal, "R$/kg: %.2f\n", preco);
        fprintf(notaFiscal, "Total: %.2f\n", totalProduto);

        printf("Produto adicionado à nota fiscal.\n");

        printf("Deseja adicionar mais produtos à compra? (s/n): ");
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

void adicionarProduto(char NomeProduto[][100], int Quantidade[], float Preco[], int *quantidade)
{
    if (*quantidade >= 50)
    {
        printf("Inventário cheio! Não é possível adicionar mais produtos.\n");
        return;
    }

    char nome[100];
    float preco;
    int quantidadeProduto;

    printf("Digite o nome do produto: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    printf("Digite o preço do produto por quilograma (R$/kg): ");
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

void removerProduto(char NomeProduto[][100], int Quantidade[], float Preco[], int *quantidade)
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
        printf("ID inválido.\n");
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

void editarProduto(char NomeProduto[][100], int Quantidade[], float Preco[], int quantidade)
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
        printf("ID inválido.\n");
        return;
    }

    char nome[100];
    float preco;
    int quantidadeProduto;

    printf("Digite o novo nome do produto: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    printf("Digite o novo preço do produto por quilograma (R$/kg): ");
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

void inventariarProduto(char NomeProduto[][100], int Quantidade[], float Preco[], int *quantidade)
{
    int opcao;
    do
    {
        system("cls");
        printf("------------ Inventário ------------\n");
        printf("1. Adicionar Produto\n");
        printf("2. Remover Produto\n");
        printf("3. Editar Produto\n");
        printf("4. Voltar\n");
        printf("------------------------------------\n");
        printf("Escolha uma opção: ");
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
            printf("Opção inválida.\n");
        }
    } while (opcao != 4);
}

void estoque(const char NomeProduto[][100], int Quantidade[], float Preco[], int quantidade)
{
    if (quantidade == 0)
    {
        printf("Nenhum produto no inventário.\n");
        return;
    }

    system("cls");
    printf("------------ Estoque ------------\n");
    for (int i = 0; i < quantidade; i++)
    {
        printf("%d: %s | R$/kg: %.2f | Quantidade (kg): %d\n", i + 1, NomeProduto[i], Preco[i], Quantidade[i]);
    }
    printf("---------------------------------\n");
}

int main()
{
    setlocale(LC_ALL, "Portuguese_Brazil.1252");

    char NomeProduto[50][100];
    int Quantidade[50];
    float Preco[50];
    int quantidade = 0;
    int escolha;
    char continuar[10];

    carregarProdutosDeArquivo(NomeProduto, Quantidade, Preco, &quantidade, "estoque.txt");

    do
    {
        system("cls");
        printf("------------ Menu ------------\n");
        printf("1. Inventário\n");
        printf("2. Estoque\n");
        printf("3. Caixa\n");
        printf("4. Sair\n");
        printf("------------------------------\n");
        printf("Escolha uma opção: ");
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
            printf("Opção inválida.\n");
        }

        if (escolha != 4)
        {
            printf("Deseja continuar no programa? (s/n): ");
            scanf("%s", continuar);
        }

    } while (strcasecmp(continuar, "s") == 0 && escolha != 4);

    return 0;
}
