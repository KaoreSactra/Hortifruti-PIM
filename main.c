#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <stdlib.h>

void adicionarProduto(char NomeProduto[][100], int Quantidade[], float Preco[], int *quantidade)
{
    // Limite de quantidade definido em 30
    if (*quantidade >= 30)
    {
        printf("inventário cheio! Não é possível adicionar mais produtos.\n");
        return;
    }

    char nome[50];
    float preco;

    printf("Digite o nome do produto: ");
    fgets(nome, 50, stdin);

    size_t len = strlen(nome);
    if (len > 0 && nome[len - 1] == '\n')
    {
        nome[len - 1] = '\0';
    }

    printf("Digite o preço do produto: ");
    scanf("%f", &preco);
    getchar();

    printf("Digite a quantidade do produto: ");
    scanf("%d", &Quantidade[*quantidade]);
    getchar();

    snprintf(NomeProduto[*quantidade], 100, "%s", nome);
    Preco[*quantidade] = preco;
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
    printf("Digite o número do produto que deseja remover (1 a %d): ", *quantidade);
    scanf("%d", &indice);
    getchar();

    if (indice < 1 || indice > *quantidade)
    {
        printf("Número inválido.\n");
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
    char nome[50];
    float preco;

    printf("Digite o número do produto que deseja editar (1 a %d): ", quantidade);
    scanf("%d", &indice);
    getchar();

    if (indice < 1 || indice > quantidade)
    {
        printf("Número inválido.\n");
        return;
    }

    printf("Digite o novo nome do produto: ");
    fgets(nome, 50, stdin);

    size_t len = strlen(nome);
    if (len > 0 && nome[len - 1] == '\n')
    {
        nome[len - 1] = '\0';
    }

    printf("Digite o novo preço do produto: ");
    scanf("%f", &preco);
    getchar();

    printf("Digite a nova quantidade do produto: ");
    scanf("%d", &Quantidade[indice - 1]);
    getchar();

    snprintf(NomeProduto[indice - 1], 100, "%s", nome);
    Preco[indice - 1] = preco;
    printf("Produto editado com sucesso!\n");
}

void salvarProdutosEmArquivo(const char NomeProduto[][100], int Quantidade[], float Preco[], int quantidade, const char *nomeArquivo)
{
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
        return;
    }

    for (int i = 0; i < quantidade; i++)
    {
        fprintf(arquivo, "%s | %.2f | %d\n", NomeProduto[i], Preco[i], Quantidade[i]);
    }

    fclose(arquivo);
    printf("Produtos salvos no arquivo %s com sucesso!\n", nomeArquivo);
}

void inventariarProduto(char NomeProduto[][100], int Quantidade[], float Preco[], int *quantidade)
{
    int opcao;

    do
    {
        system("cls");
        printf("-------------- inventário --------------\n");
        printf("1 = Adicionar Produto\n2 = Remover Produto\n3 = Editar Produto\n4 = Voltar ao Menu Principal\n");
        printf("----------------------------------------\n");
        scanf("%d", &opcao);
        getchar();

        switch (opcao)
        {
        case 1:
            adicionarProduto(NomeProduto, Quantidade, Preco, quantidade);
            salvarProdutosEmArquivo(NomeProduto, Quantidade, Preco, *quantidade, "produtos.txt");
            break;
        case 2:
            removerProduto(NomeProduto, Quantidade, Preco, quantidade);
            salvarProdutosEmArquivo(NomeProduto, Quantidade, Preco, *quantidade, "produtos.txt");
            break;
        case 3:
            editarProduto(NomeProduto, Quantidade, Preco, *quantidade);
            salvarProdutosEmArquivo(NomeProduto, Quantidade, Preco, *quantidade, "produtos.txt");
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
    printf("----------- Estoque -----------\n");
    for (int i = 0; i < quantidade; i++)
    {
        printf("%d: %s | Preço: %.2f | Quantidade: %d\n", i + 1, NomeProduto[i], Preco[i], Quantidade[i]);
    }
    printf("--------------------------------\n");
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
    printf("-------------- Início da Compra --------------\n");

    do
    {
        printf("Digite o número do produto para compra (1 a %d): ", *quantidade);
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
        fprintf(notaFiscal, "-----------------------------------------\n");
        fprintf(notaFiscal, "Produto: %s\n", NomeProduto[produtoIndex - 1]);
        fprintf(notaFiscal, "Quantidade comprada: %d\n", quantidadeCompra);
        fprintf(notaFiscal, "Preço unitário: %.2f\n", preco);
        fprintf(notaFiscal, "Total: %.2f\n", preco * quantidadeCompra);
        totalCompra += preco * quantidadeCompra;

        printf("Produto adicionado à nota fiscal.\n");

        printf("Deseja adicionar mais produtos à compra? (s/n): ");
        scanf(" %9s", continuarCompra);
        
    } while (strcasecmp(continuarCompra, "s") == 0 || strcasecmp(continuarCompra, "sim") == 0 || strcasecmp(continuarCompra, "y") == 0 || strcasecmp(continuarCompra, "yes") == 0 || strcasecmp(continuarCompra, "1") == 0);

    fprintf(notaFiscal, "-------------- Nota Fiscal --------------\n");
    fprintf(notaFiscal, "Total geral: %.2f\n", totalCompra);
    fclose(notaFiscal);

    system("cls");
    printf("-------------- Nota Fiscal --------------\n");
    printf("Total geral: %.2f\n", totalCompra);
    printf("-----------------------------------------\n");

    printf("Compra registrada com sucesso! Nota fiscal gerada.\n");
    salvarProdutosEmArquivo(NomeProduto, Quantidade, Preco, *quantidade, "produtos.txt");
}

int main()
{
    setlocale(LC_ALL, "Portuguese_Brazil.1252");

    // Limite de quantidade definido em 30
    char NomeProduto[30][100];
    int Quantidade[30];
    float Preco[30];
    int quantidade = 0;
    int escolha;
    char contador[10];

    do
    {
        system("cls");
        printf("-------------- Menu --------------\n");
        printf("1 = Inventariar\n2 = Estoque\n3 = Registrar Compra\n");
        printf("----------------------------------\n");
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
        default:
            printf("Opçãoo inválida.\n");
        }

        printf("Deseja continuar? ");
        scanf(" %9s", contador);

    } while (strcasecmp(contador, "s") == 0 || strcasecmp(contador, "sim") == 0 || strcasecmp(contador, "y") == 0 || strcasecmp(contador, "yes") == 0 || strcasecmp(contador, "1") == 0);

    return 0;
}
