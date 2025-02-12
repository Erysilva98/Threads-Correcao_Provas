#include <stdio.h>
#include <stdlib.h>
#include "processamento.h"
#include "utils.h"
#include <omp.h>

void calcularNotas(Candidato candidatos[], int quantidade, Gabarito *gabarito)
{
    int i, j;
    // Inicializa o contador de acertos para cada questão
    for (i = 0; i < NUM_QUESTOES; i++)
    {
        gabarito->acertos[i] = 0;
    }

// *** Tarefa 1: Comparar as respostas dos candidatos com o gabarito ***
// (Esta etapa é paralelizável: cada candidato é processado de forma independente)
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

    // Calcula o percentual de acerto de cada questão: p(n) = (acertos/quantidade)*100.
    float porcentagens[NUM_QUESTOES];
    for (i = 0; i < NUM_QUESTOES; i++)
    {
        porcentagens[i] = ((float)gabarito->acertos[i] / quantidade) * 100.0;
    }

    // *** Tarefa 2: Cálculo do valor de cada questão para cada componente curricular ***
    // Para cada componente (0: Língua Portuguesa, 1: Matemática e Lógica, 2: Conhecimento Específico)
    for (int comp = 0; comp < 3; comp++)
    {
        int start = comp * 10;
        int end = start + 10;
        float maxPercent = 0.0;
        // 2.a – Determina p(M) para o grupo (maior percentual de acerto)
        for (i = start; i < end; i++)
        {
            if (porcentagens[i] > maxPercent)
                maxPercent = porcentagens[i];
        }
        // 2.b – Calcula o grau de dificuldade GD(n) para cada questão:
        // GD(n) = (p(n)/p(M)) * 4.
        float gd[10]; // Vetor temporário para o grupo
        for (i = 0; i < 10; i++)
        {
            int idx = start + i;
            if (maxPercent > 0)
                gd[i] = (porcentagens[idx] / maxPercent) * 4.0;
            else
                gd[i] = 0;
        }
        // 2.c – Soma dos GD(n) do grupo
        float somaGD = 0.0;
        for (i = 0; i < 10; i++)
        {
            somaGD += gd[i];
        }
        // 2.d – Calcula a pontuação P(n) de cada questão:
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

// *** Tarefa 3: Cálculo das notas dos candidatos ***
// Para cada candidato, se a resposta estiver correta, soma-se o valor P(n) da questão
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
                { // Língua Portuguesa
                    candidatos[i].notas[0] += gabarito->questoes[j];
                }
                else if (j < 20)
                { // Matemática e Lógica
                    candidatos[i].notas[1] += gabarito->questoes[j];
                }
                else
                { // Conhecimento Específico
                    candidatos[i].notas[2] += gabarito->questoes[j];
                }
            }
        }
        // A média final é a soma das notas dos três componentes
        candidatos[i].mediaFinal = candidatos[i].notas[0] + candidatos[i].notas[1] + candidatos[i].notas[2];
    }
}

void ordenarClassificados(Candidato candidatos[], int quantidade)
{
    quickSort(candidatos, 0, quantidade - 1);
}
