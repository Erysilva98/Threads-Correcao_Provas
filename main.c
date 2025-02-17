#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "io.h"
#include "processamento.h"
#include "utils.h"

#define MAX_CANDIDATOS 1000
#define NUM_QUESTOES 30
#define codigoDisciplina "0701"

int main(int argc, char *argv[])
{
    int num_threads = 4; // valor padrão
    if (argc > 1)
    {
        num_threads = atoi(argv[1]);
        if (num_threads <= 0)
        {
            fprintf(stderr, "Numero de threads inválido. Utilizando o valor padrao: 4\n");
            num_threads = 4;
        }
    }
    // Exibe o número de processadores disponíveis (opcional)
    int nprocs = omp_get_num_procs();
    printf("Processadores disponiveis: %d\n", nprocs);

    // Configura o OpenMP: desabilita ajuste dinâmico e define o número de threads
    omp_set_dynamic(0);
    omp_set_num_threads(num_threads);
    printf("Numero de threads utilizados: %d\n", num_threads);

    Candidato candidatos[MAX_CANDIDATOS];
    Gabarito gabarito;
    int quantidade = 0;

    fprintf(stdout, "Código de disciplina: %s\n", codigoDisciplina);

    // Leitura dos candidatos (filtra pelo código especificado)
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

    // Medição do tempo de processamento paralelizado
    double t_inicial = omp_get_wtime();
    calcularNotas(candidatos, quantidade, &gabarito);
    double t_final = omp_get_wtime();
    printf("Tempo de processamento paralelizado: %f segundos\n", t_final - t_inicial);

    // Ordenação dos candidatos (atualmente sequencial, via quickSort em utils.c :contentReference[oaicite:1]{index=1})
    ordenarClassificados(candidatos, quantidade);

    if (gerarRelatorio(candidatos, quantidade, "classificados.csv") != 0)
    {
        fprintf(stderr, "Erro ao gerar o relatório.\n");
        return 1;
    }

    printf("Processamento concluído com sucesso. Relatório gerado em 'classificados.csv'.\n");
    return 0;
}
