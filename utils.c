#include <string.h>
#include <stdlib.h>
#include "utils.h"

// Função auxiliar: particiona o array para o QuickSort
static int partition(Candidato candidatos[], int low, int high)
{
    float pivot = candidatos[high].mediaFinal;
    int i = low - 1;
    int j;
    for (j = low; j < high; j++)
    {
        // Ordena de forma decrescente: candidato com maior média vem primeiro
        if (candidatos[j].mediaFinal > pivot)
        {
            i++;
            Candidato temp = candidatos[i];
            candidatos[i] = candidatos[j];
            candidatos[j] = temp;
        }
    }
    Candidato temp = candidatos[i + 1];
    candidatos[i + 1] = candidatos[high];
    candidatos[high] = temp;
    return i + 1;
}

void quickSort(Candidato candidatos[], int low, int high)
{
    if (low < high)
    {
        int pi = partition(candidatos, low, high);
        quickSort(candidatos, low, pi - 1);
        quickSort(candidatos, pi + 1, high);
    }
}

void removeQuotes(char *str)
{
    int len = strlen(str);
    if (len >= 2 && str[0] == '\"' && str[len - 1] == '\"')
    {
        // Remove as aspas deslocando o conteúdo para a esquerda
        memmove(str, str + 1, len - 2);
        str[len - 2] = '\0';
    }
}
