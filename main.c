#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "processamento.h"
#include "utils.h"

#define MAX_CANDIDATOS 1000
#define NUM_QUESTOES 30
#define codigoDisciplina "\"0701\""

int main()
{
    Candidato candidatos[MAX_CANDIDATOS];
    Gabarito gabarito;
    int quantidade = 0;

    // Leitura dos dados de entrada
    if (lerRespostas("respostas.csv", candidatos, &quantidade, codigoDisciplina) != 0)
    {
        fprintf(stderr, "Erro ao ler respostas.csv\n");
        return 1;
    }

    printf("Quantidade de candidatos filtrados: %d\n", quantidade);

    if (lerGabarito("gabarito.csv", &gabarito) != 0)
    {
        fprintf(stderr, "Erro ao ler gabarito.csv\n");
        return 1;
    }

    // Processamento: cálculo de notas e verificação de respostas
    calcularNotas(candidatos, quantidade, &gabarito);

    // Ordenação dos candidatos por média final (ordem decrescente)
    ordenarClassificados(candidatos, quantidade);

    // Geração do relatório final em "classificados.csv"
    if (gerarRelatorio(candidatos, quantidade, "classificados.csv") != 0)
    {
        fprintf(stderr, "Erro ao gerar o relatório.\n");
        return 1;
    }

    printf("Processamento concluído com sucesso. Relatório gerado em 'classificados.csv'.\n");
    return 0;
}
