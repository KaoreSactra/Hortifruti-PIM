#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

// Função para adicionar um produto
void adicionarProduto(char NomeProduto[][50], int *quantidade)
{
    if (*quantidade >= 10)
    {
        printf("Inventário cheio! Não é possível adicionar mais produtos.\n");
        return;
    }

    printf("Digite o nome do produto: ");
    fgets(NomeProduto[*quantidade], 50, stdin);

    // Remove o caractere de nova linha
    size_t len = strlen(NomeProduto[*quantidade]);
    if (len > 0 && NomeProduto[*quantidade][len - 1] == '\n')
    {
        NomeProduto[*quantidade][len - 1] = '\0';
    }

    (*quantidade)++;
    printf("Produto adicionado com sucesso!\n");
}

// Função para remover um produto
void removerProduto(char NomeProduto[][50], int *quantidade)
{
    if (*quantidade == 0)
    {
        printf("Nenhum produto para remover.\n");
        return;
    }

    int indice;
    printf("Digite o número do produto que deseja remover (1 a %d): ", *quantidade);
    scanf("%d", &indice);
    getchar(); // Limpa o buffer

    if (indice < 1 || indice > *quantidade)
    {
        printf("Número inválido.\n");
        return;
    }

    // Move os produtos seguintes para trás
    for (int i = indice - 1; i < *quantidade - 1; i++)
    {
        strcpy(NomeProduto[i], NomeProduto[i + 1]);
    }

    (*quantidade)--;
    printf("Produto removido com sucesso!\n");
}

// Função para editar um produto
void editarProduto(char NomeProduto[][50], int quantidade)
{
    if (quantidade == 0)
    {
        printf("Nenhum produto para editar.\n");
        return;
    }

    int indice;
    printf("Digite o número do produto que deseja editar (1 a %d): ", quantidade);
    scanf("%d", &indice);
    getchar(); // Limpa o buffer

    if (indice < 1 || indice > quantidade)
    {
        printf("Número inválido.\n");
        return;
    }

    printf("Digite o novo nome do produto: ");
    fgets(NomeProduto[indice - 1], 50, stdin);

    // Remove o caractere de nova linha
    size_t len = strlen(NomeProduto[indice - 1]);
    if (len > 0 && NomeProduto[indice - 1][len - 1] == '\n')
    {
        NomeProduto[indice - 1][len - 1] = '\0';
    }

    printf("Produto editado com sucesso!\n");
}

// Função para salvar os produtos em um arquivo de texto
void salvarProdutosEmArquivo(const char NomeProduto[][50], int quantidade, const char *nomeArquivo)
{
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
        return;
    }

    for (int i = 0; i < quantidade; i++)
    {
        fprintf(arquivo, "%s\n", NomeProduto[i]);
    }

    fclose(arquivo);
    printf("Produtos salvos no arquivo %s com sucesso!\n", nomeArquivo);
}

// Função para gerenciar inventário (Adicionar, Remover, Editar)
void inventariarProduto(char NomeProduto[][50], int *quantidade)
{
    int opcao;

    do
    {
        printf("-------------- Inventário --------------\n");
        printf("1 = Adicionar Produto\n2 = Remover Produto\n3 = Editar Produto\n4 = Voltar ao Menu Principal\n");
        printf("----------------------------------------\n");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer

        switch (opcao)
        {
        case 1:
            adicionarProduto(NomeProduto, quantidade);
            salvarProdutosEmArquivo(NomeProduto, *quantidade, "produtos.txt");
            break;
        case 2:
            removerProduto(NomeProduto, quantidade);
            salvarProdutosEmArquivo(NomeProduto, *quantidade, "produtos.txt");
            break;
        case 3:
            editarProduto(NomeProduto, *quantidade);
            salvarProdutosEmArquivo(NomeProduto, *quantidade, "produtos.txt");
            break;
        case 4:
            printf("Voltando ao menu principal...\n");
            break;
        default:
            printf("Opção inválida.\n");
        }
    } while (opcao != 4);
}

// Função para exibir o estoque
void estoque(const char NomeProduto[][50], int quantidade)
{
    if (quantidade == 0)
    {
        printf("Nenhum produto no inventário.\n");
        return;
    }

    printf("----------- Estoque -----------\n");
    for (int i = 0; i < quantidade; i++)
    {
        printf("Produto %d: %s\n", i + 1, NomeProduto[i]);
    }
    printf("--------------------------------\n");
}

int main()
{
    setlocale(LC_ALL, "portuguese");

    char NomeProduto[10][50];
    char contador[10];
    int quantidade = 0;
    int escolha;

    do
    {
        printf("-------------- Menu --------------\n");
        printf("1 = Inventariar\n2 = Estoque\n");
        printf("----------------------------------\n");
        scanf("%d", &escolha);
        getchar(); // Limpa o buffer

        switch (escolha)
        {
        case 1:
            inventariarProduto(NomeProduto, &quantidade);
            break;
        case 2:
            estoque(NomeProduto, quantidade);
            break;
        default:
            printf("Opção inválida.\n");
        }

        printf("Deseja continuar? ");
        scanf(" %9s", contador);

    } while (strcasecmp(contador, "s") == 0 || strcasecmp(contador, "sim") == 0 || strcasecmp(contador, "y") == 0 || strcasecmp(contador, "yes") == 0 || strcasecmp(contador, "1") == 0);

    return 0;
}
