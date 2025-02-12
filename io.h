#ifndef IO_H
#define IO_H

#include "processamento.h" // Inclui as definições de Candidato e Gabarito

// Funções para manipulação de arquivos CSV
int lerRespostas(const char *nomeArquivo, Candidato candidatos[], int *quantidade, const char *codigoDisciplina);
int lerGabarito(const char *nomeArquivo, Gabarito *gabarito);
int gerarRelatorio(Candidato candidatos[], int quantidade, const char *nomeArquivo);

#endif
