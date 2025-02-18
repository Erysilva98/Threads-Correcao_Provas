#include <stdio.h>
#include <stdlib.h>
#include "processamento.h"
#include "utils.h"
#include <omp.h>

void calcularNotas(Candidato candidatos[], int quantidade, Gabarito *gabarito)
{
    int i, j;
    // Inicializa o contador de acertos para cada questao
    for (i = 0; i < NUM_QUESTOES; i++)
    {
        gabarito->acertos[i] = 0;
    }

// Tarefa 1: Comparar as respostas dos candidatos com o gabarito (paralelizavel)
#pragma omp parallel for private(j)
    for (i = 0; i < quantidade; i++)
    {
        for (j = 0; j < NUM_QUESTOES; j++)
        {
            if (candidatos[i].respostas[j] == gabarito->gabarito[j])
            {
#pragma omp atomic
                gabarito->acertos[j]++;
            }
        }
    }

    // Calcula o percentual de acerto de cada questao: p(n) = (acertos/quantidade)*100.
    float porcentagens[NUM_QUESTOES];
    for (i = 0; i < NUM_QUESTOES; i++)
    {
        porcentagens[i] = ((float)gabarito->acertos[i] / quantidade) * 100.0;
    }

    // Tarefa 2: Calculo do valor de cada questao para cada componente curricular
    // Para cada componente (0: Lingua Portuguesa, 1: Matematica e Logica, 2: Conhecimento Especifico)
    for (int comp = 0; comp < 3; comp++)
    {
        int start = comp * 10;
        int end = start + 10;
        float maxPercent = 0.0;
        // 2.a - Determina p(M) para o grupo (maior percentual de acerto)
        for (i = start; i < end; i++)
        {
            if (porcentagens[i] > maxPercent)
                maxPercent = porcentagens[i];
        }
        // 2.b - Calcula o grau de dificuldade GD(n) para cada questao:
        // GD(n) = (p(n)/p(M)) * 4.
        float gd[10]; // Vetor temporario para o grupo
        for (i = 0; i < 10; i++)
        {
            int idx = start + i;
            if (maxPercent > 0)
                gd[i] = (porcentagens[idx] / maxPercent) * 4.0;
            else
                gd[i] = 0;
        }
        // 2.c - Soma dos GD(n) do grupo
        float somaGD = 0.0;
        for (i = 0; i < 10; i++)
        {
            somaGD += gd[i];
        }
        // 2.d - Calcula a pontuacao P(n) de cada questao:
        // P(n) = (GD(n)/somaGD) * 100.
        for (i = 0; i < 10; i++)
        {
            int idx = start + i;
            if (somaGD > 0)
                gabarito->questoes[idx] = (gd[i] / somaGD) * 100.0;
            else
                gabarito->questoes[idx] = 0;
        }
    }

// Tarefa 3: Calculo das notas dos candidatos
// Para cada candidato, se a resposta estiver correta, soma-se o valor P(n) da questao
#pragma omp parallel for private(j)
    for (i = 0; i < quantidade; i++)
    {
        // Zera as notas para cada componente
        candidatos[i].notas[0] = 0;
        candidatos[i].notas[1] = 0;
        candidatos[i].notas[2] = 0;
        for (j = 0; j < NUM_QUESTOES; j++)
        {
            if (candidatos[i].respostas[j] == gabarito->gabarito[j])
            {
                if (j < 10)
                { // Lingua Portuguesa
                    candidatos[i].notas[0] += gabarito->questoes[j];
                }
                else if (j < 20)
                { // Matematica e Logica
                    candidatos[i].notas[1] += gabarito->questoes[j];
                }
                else
                { // Conhecimento Especifico
                    candidatos[i].notas[2] += gabarito->questoes[j];
                }
            }
        }
        // A media final eh a media ponderada (media simples, pois os pesos sao iguais) das tres disciplinas
        candidatos[i].mediaFinal = (candidatos[i].notas[0] + candidatos[i].notas[1] + candidatos[i].notas[2]) / 3.0;
    }
} // Fim da funcao calcularNotas

void ordenarClassificados(Candidato candidatos[], int quantidade)
{
    quickSort(candidatos, 0, quantidade - 1);
}
