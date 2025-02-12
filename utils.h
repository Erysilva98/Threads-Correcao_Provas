#ifndef UTILS_H
#define UTILS_H

#include "processamento.h"

// Função de ordenação (QuickSort) para ordenar os candidatos
void quickSort(Candidato candidatos[], int low, int high);

// Função auxiliar para remover aspas de uma string
void removeQuotes(char *str);

#endif
