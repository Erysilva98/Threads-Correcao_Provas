#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"
#include "utils.h"

#define BUFFER_SIZE 1024

// Lê o arquivo respostas.csv, filtrando candidatos com o código especificado
int lerRespostas(const char *nomeArquivo, Candidato candidatos[], int *quantidade, const char *codigoDisciplina)
{
    FILE *file = fopen(nomeArquivo, "r");
    if (!file)
    {
        perror("Erro ao abrir o arquivo de respostas");
        return -1;
    }
    char buffer[BUFFER_SIZE];
    int count = 0;

    // Lê cada linha do arquivo
    while (fgets(buffer, BUFFER_SIZE, file))
    {
        // Remove o caractere de nova linha, se existir
        buffer[strcspn(buffer, "\n")] = '\0';

        // Tokeniza a linha: espera-se id, código da disciplina e 30 respostas
        char *token = strtok(buffer, ",");
        if (token == NULL)
            continue;
        char id[20];
        strncpy(id, token, 19);
        id[19] = '\0';
        removeQuotes(id);

        token = strtok(NULL, ",");
        if (token == NULL)
            continue;
        char codigo[10];
        strncpy(codigo, token, 9);
        codigo[9] = '\0';
        removeQuotes(codigo);

        // Filtra apenas os candidatos com o código especificado
        if (strcmp(codigo, codigoDisciplina) != 0)
            continue;

        // Lê as 30 respostas
        char respostas[NUM_QUESTOES + 1];
        int i;
        for (i = 0; i < NUM_QUESTOES; i++)
        {
            token = strtok(NULL, ",");
            if (token == NULL)
                break;
            removeQuotes(token);
            // Cada resposta é considerada como um único caractere
            respostas[i] = token[0];
        }
        if (i != NUM_QUESTOES)
        {
            // Linha com respostas insuficientes; ignora a linha
            continue;
        }
        respostas[NUM_QUESTOES] = '\0'; // Finaliza a string

        // Preenche a estrutura do candidato
        strncpy(candidatos[count].id, id, 19);
        candidatos[count].id[19] = '\0';
        strncpy(candidatos[count].respostas, respostas, NUM_QUESTOES);
        candidatos[count].respostas[NUM_QUESTOES] = '\0';
        candidatos[count].notas[0] = 0;
        candidatos[count].notas[1] = 0;
        candidatos[count].notas[2] = 0;
        candidatos[count].mediaFinal = 0;
        count++;
    }
    *quantidade = count;
    fclose(file);
    return 0;
}

// Lê o arquivo gabarito.csv com as 30 respostas oficiais
int lerGabarito(const char *nomeArquivo, Gabarito *gabarito)
{
    FILE *file = fopen(nomeArquivo, "r");
    if (!file)
    {
        perror("Erro ao abrir o arquivo do gabarito");
        return -1;
    }
    char buffer[BUFFER_SIZE];
    if (fgets(buffer, BUFFER_SIZE, file) == NULL)
    {
        fclose(file);
        return -1;
    }
    // Remove o caractere de nova linha
    buffer[strcspn(buffer, "\n")] = '\0';
    // Tokeniza a linha: espera-se 30 respostas
    int i;
    for (i = 0; i < NUM_QUESTOES; i++)
    {
        char *token = strtok(i == 0 ? buffer : NULL, ",");
        if (token == NULL)
            break;
        removeQuotes(token);
        gabarito->gabarito[i] = token[0];
    }
    if (i != NUM_QUESTOES)
    {
        fclose(file);
        return -1;
    }
    gabarito->gabarito[NUM_QUESTOES] = '\0';
    // Inicializa os arrays de acertos e dos pesos das questões
    for (i = 0; i < NUM_QUESTOES; i++)
    {
        gabarito->acertos[i] = 0;
        gabarito->questoes[i] = 0;
    }
    fclose(file);
    return 0;
}

// Gera o relatório final em classificados.csv com os dados dos candidatos ordenados
int gerarRelatorio(Candidato candidatos[], int quantidade, const char *nomeArquivo)
{
    FILE *file = fopen(nomeArquivo, "w");
    if (!file)
    {
        perror("Erro ao abrir o arquivo para escrita");
        return -1;
    }
    // Cabeçalho do CSV
    fprintf(file, "ID,Nota_Lingua,Nota_Matematica,Nota_Conhecimento,Media_Final,Posicao\n");
    int i;
    for (i = 0; i < quantidade; i++)
    {
        // A posição no ranking é baseada na ordem do array (já ordenado)
        fprintf(file, "%s,%.2f,%.2f,%.2f,%.2f,%d\n",
                candidatos[i].id,
                candidatos[i].notas[0],
                candidatos[i].notas[1],
                candidatos[i].notas[2],
                candidatos[i].mediaFinal,
                i + 1);
    }
    fclose(file);
    return 0;
}

// Gera o arquivo filtro.csv contendo os candidatos filtrados (mesmo formato do CSV original)
int gerarFiltro(Candidato candidatos[], int quantidade, const char *nomeArquivo)
{
    FILE *file = fopen(nomeArquivo, "w");
    if (!file)
    {
        perror("Erro ao abrir o arquivo filtro para escrita");
        return -1;
    }
    // Cabeçalho do CSV (opcional)
    fprintf(file, "ID,Codigo,Respostas\n");
    int i, j;
    for (i = 0; i < quantidade; i++)
    {
        // Imprime o ID e o código fixo "0701", e em seguida as 30 respostas individuais
        fprintf(file, "\"%s\",\"0701\"", candidatos[i].id);
        for (j = 0; j < NUM_QUESTOES; j++)
        {
            fprintf(file, ",\"%c\"", candidatos[i].respostas[j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
    return 0;
}
