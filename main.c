#include <stdio.h>  // Essencial para entrada e sa�da padr�o e manipula��o de arquivos.
#include <string.h> // Usada para realizar opera��es em strings e blocos de mem�ria.
#include <ctype.h>  // Trabalha com caracteres individuais de forma eficiente.
#include <locale.h> // Adapta o programa para diferentes regi�es ou idiomas.
#include <stdlib.h> // Fun��es utilit�rias para mem�ria, convers�es e controle de execu��o.

// Fun��o para importar estoque
void carregarProdutosDeArquivo(char NomeProduto[][30], float Quantidade[], float Preco[], int *quantidade, const char *nomeArquivo)
{
    // Abrir txt onde importa estoque
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL)
    {
        printf("Arquivo '%s' n�o encontrado. Invent�rio vazio.\n", nomeArquivo);
        *quantidade = 0;
        return;
    }

    // Leitura de linhas
    char linha[200]; // M�ximo 200 caracteres por linha.
    *quantidade = 0; // Porteiro para saber a quantidade de produtos
    while (fgets(linha, sizeof(linha), arquivo))
    {
        linha[strcspn(linha, "\n")] = '\0'; // Remove o caractere de nova linha (\n) que pode ser inclu�do pelo fgets.
        char nome[30];
        float preco, quantidadeProduto;

        // Scannear linhas
        if (sscanf(linha, "%[^|]|%f|%f", nome, &preco, &quantidadeProduto) == 3) // "%[^|]" separa o nome do produto at� a |.
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

    system("cls"); // Limpar tela para exibir novo menu
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

    system("cls"); // Limpar tela para exibir novo menu
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
        system("cls"); // Limpar tela para exibir novo menu
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
    // idioma definido para portugu�s com a codifica��o (ISO 8859-1) | idioma default: "Portuguese_Brazil.1252"
    setlocale(LC_ALL, "Portuguese_Brazil.1252");

    // Setando vari�veis
    char NomeProduto[50][30]; // Nome do produto, primeira casa [50] espa�os disponiveis no estoque, segunda casa [30] para 29 caract�res possiveis pro nome
    float Quantidade[50]; // Quantidade de espa�o no estoque
    float Preco[50]; // Pre�o dos produtos, casa [50] espa�os disponiveis no estoque
    int quantidade = 0; // Quantidade para ID
    int escolha; // Escolha para entrar em menu
    char continuar[10]; // Continuar para escolha de manter o programa aberto, [10] caracteres limite

    // Chamar fun��o de importar estoque usando "estoque.txt"
    carregarProdutosDeArquivo(NomeProduto, Quantidade, Preco, &quantidade, "estoque.txt");

    do
    {
        // Menu principal
        system("cls"); // Limpar tela para exibir novo menu
        printf("------------ Menu ------------\n");
        printf("1. Invent�rio\n");
        printf("2. Estoque\n");
        printf("3. Caixa\n");
        printf("4. Sair\n");
        printf("------------------------------\n");
        printf("Escolha uma op��o: ");
        scanf("%d", &escolha); // Entrar na op��o definida pelo usu�rio
        getchar();

        // Switch para escolha
        switch (escolha)
        {
        case 1:
            // Chamar fun��o de invent�rio
            inventariarProduto(NomeProduto, Quantidade, Preco, &quantidade);
            break;
        case 2:
            // Chamar fun��o de estoque
            estoque(NomeProduto, Quantidade, Preco, quantidade);
            break;
        case 3:
            // Chamar fun��o de caixa
            registrarCompra(NomeProduto, Quantidade, Preco, &quantidade);
            break;
        case 4:
            // Fechar programa
            printf("Saindo do programa...\n");
            break;
        default:
            printf("Op��o inv�lida.\n");
        }

        // fun��o para continuar o programa
        if (escolha != 4)
        {
            printf("Deseja continuar no programa? (s/n): ");
            scanf("%s", continuar);
        }

    } while (strcasecmp(continuar, "s") == 0 && escolha != 4);

    return 0;
}