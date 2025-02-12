#ifndef PROCESSAMENTO_H
#define PROCESSAMENTO_H

#define MAX_CANDIDATOS 1000
#define NUM_QUESTOES 30

typedef struct
{
    char id[20];
    char respostas[NUM_QUESTOES + 1];
    float notas[3]; // [0]: Língua Portuguesa, [1]: Matemática e Lógica, [2]: Conhecimento Específico
    float mediaFinal;
} Candidato;

typedef struct
{
    float questoes[NUM_QUESTOES]; // Armazena os pesos P(n) de cada questão
    char gabarito[NUM_QUESTOES + 1];
    int acertos[NUM_QUESTOES]; // Número de acertos por questão
} Gabarito;

// Protótipos das funções de processamento
void calcularNotas(Candidato candidatos[], int quantidade, Gabarito *gabarito);
void ordenarClassificados(Candidato candidatos[], int quantidade);

#endif
