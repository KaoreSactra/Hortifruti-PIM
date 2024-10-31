#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Função para inventariar produtos, recebendo os nomes e armazenando-os em um array
void inventariarProduto(char NomeProduto[][50])
{
    for (int c = 0; c < 10; c++)
    {
        printf("Digite o nome do produto %d: ", c + 1);
        fgets(NomeProduto[c], 50, stdin); // Lê o nome do produto e armazena em NomeProduto[c]

        // Remove o caractere de nova linha do final da string, se presente
        size_t len = strlen(NomeProduto[c]);
        if (len > 0 && NomeProduto[c][len - 1] == '\n')
        {
            NomeProduto[c][len - 1] = '\0';
        }
    }
}

// Função para exibir os nomes dos produtos armazenados no array NomeProduto
void exibirNome(char NomeProduto[][50])
{
    int t = 0;
    while (t < 10)
    {
        printf("Produto %d: %s\n", t + 1, NomeProduto[t]); // Exibe o nome do produto com índice t
        t++;
    }
}

// Função para salvar os nomes dos produtos em um arquivo de texto
void salvarProdutosEmArquivo(const char NomeProduto[][50], const char *nomeArquivo)
{
    FILE *arquivo = fopen(nomeArquivo, "w"); // Abre o arquivo em modo de escrita
    if (arquivo == NULL)
    { // Verifica se houve erro na abertura do arquivo
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
        return;
    }

    // Grava cada nome de produto no arquivo, um por linha
    for (int i = 0; i < 10; i++)
    {
        fprintf(arquivo, "%s\n", NomeProduto[i]);
    }

    fclose(arquivo); // Fecha o arquivo após a gravação
    printf("Produtos salvos no arquivo %s com sucesso!\n", nomeArquivo);
}

int main()
{
    char NomeProduto[10][50]; // Array para armazenar até 10 nomes de produtos, com até 50 caracteres cada
    char contador[10];        // String para armazenar a resposta do usuário sobre continuar ou não
    int escolha;

    do
    {
        // Exibe o menu de opções para o usuário
        printf("-------------------------------------------------------------\n");
        printf("1 = Inventariar\n2 = Estoque\n");
        printf("-------------------------------------------------------------\n");
        scanf("%d", &escolha);
        getchar(); // Limpa o buffer para evitar problemas com fgets na leitura de strings

        // Seleciona a ação com base na escolha do usuário
        switch (escolha)
        {
        case 1:
            inventariarProduto(NomeProduto);                      // Chama função para inventariar produtos
            salvarProdutosEmArquivo(NomeProduto, "produtos.txt"); // Salva os produtos no arquivo
            break;
        case 2:
            exibirNome(NomeProduto); // Chama função para exibir os produtos
            break;
        default:
            printf("Opção inválida.\n"); // Mensagem para entrada inválida
        }

        // Pergunta ao usuário se deseja continuar
        printf("Deseja continuar? ");
        scanf(" %9s", contador); // Lê a resposta do usuário (até 9 caracteres)

    } while (strcasecmp(contador, "s") == 0 ||
            strcasecmp(contador, "sim") == 0 ||
            strcasecmp(contador, "y") == 0 ||
            strcasecmp(contador, "yes") == 0); // Continua o loop caso a resposta seja afirmativa

    return 0;
}
