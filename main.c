#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "io.h"
#include "processamento.h"
#include "utils.h"

#define N_Interacoes 50000 // Para medicao de tempo de processamento paralelizado (numero de iteracoes)
#define MAX_CANDIDATOS 1000
#define NUM_QUESTOES 30
#define codigoDisciplina "0701"

int main(int argc, char *argv[])
{
    int num_threads = 4; // valor padrao
    if (argc > 1)
    {
        num_threads = atoi(argv[1]);
        if (num_threads <= 0)
        {
            fprintf(stderr, "Numero de threads invalido. Utilizando o valor padrao: 4\n");
            num_threads = 4;
        }
    }
    // Exibe o numero de processadores disponiveis (opcional)
    int nprocs = omp_get_num_procs();
    printf("Processadores disponiveis: %d\n", nprocs);

    // Configura o OpenMP: desabilita ajuste dinamico e define o numero de threads
    omp_set_dynamic(0);
    omp_set_num_threads(num_threads);
    printf("Numero de threads utilizados: %d\n", num_threads);

    Candidato candidatos[MAX_CANDIDATOS];
    Gabarito gabarito;
    int quantidade = 0;

    printf("Codigo de disciplina: %s\n", codigoDisciplina);

    // Leitura dos candidatos (filtra pelo codigo especificado)
    if (lerRespostas("respostas.csv", candidatos, &quantidade, codigoDisciplina) != 0)
    {
        fprintf(stderr, "Erro ao ler respostas.csv\n");
        return 1;
    }
    printf("Quantidade de candidatos filtrados: %d\n", quantidade);

    if (gerarFiltro(candidatos, quantidade, "filtro.csv") != 0)
    {
        fprintf(stderr, "Erro ao gerar filtro.csv\n");
        return 1;
    }
    if (lerGabarito("gabarito.csv", &gabarito) != 0)
    {
        fprintf(stderr, "Erro ao ler gabarito.csv\n");
        return 1;
    }

    // Medicao do tempo de processamento paralelizado (repetido N_Interacoes vezes para simular um processamento mais longo)
    int iteracoes = N_Interacoes;
    double tempo_total = 0.0;
    for (int i = 0; i < iteracoes; i++)
    {
        for (int j = 0; j < NUM_QUESTOES; j++)
        {
            gabarito.acertos[j] = 0;
        }
        for (int i = 0; i < quantidade; i++)
        {
            candidatos[i].notas[0] = 0;
            candidatos[i].notas[1] = 0;
            candidatos[i].notas[2] = 0;
            candidatos[i].mediaFinal = 0;
        }
        double t_inicial = omp_get_wtime();
        calcularNotas(candidatos, quantidade, &gabarito);
        double t_final = omp_get_wtime();
        tempo_total += (t_final - t_inicial);
    }
    double tempo_medio = tempo_total / iteracoes;
    printf("Tempo de processamento paralelizado (media de %d iteracoes): %f segundos\n", N_Interacoes, tempo_medio);

    // Ordenacao dos candidatos (sequencial)
    ordenarClassificados(candidatos, quantidade);

    if (gerarRelatorio(candidatos, quantidade, "classificados.csv") != 0)
    {
        fprintf(stderr, "Erro ao gerar relatorio.\n");
        return 1;
    }

    printf("Processamento concluido com sucesso. Relatorio gerado em 'classificados.csv'.\n");
    return 0;
}
