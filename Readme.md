# Projeto Correção de Provas

Este projeto implementa uma solução para o processamento dos resultados de um concurso público, utilizando a linguagem C e técnicas de programação paralela com OpenMP.

## Descrição

O programa realiza as seguintes tarefas:

- **Leitura e filtragem:**  
  Lê o arquivo `respostas.csv` e filtra os candidatos com base no código de disciplina especificado (por exemplo, "0701").

- **Comparação e processamento:**  
  Compara as respostas dos candidatos com o gabarito oficial presente no arquivo `gabarito.csv` e calcula:
  - Percentual de acerto de cada questão.
  - Valor de cada questão de acordo com o grau de dificuldade.
  - Notas parciais para os componentes:
    - Língua Portuguesa (questões 1 a 10)
    - Matemática e Lógica (questões 11 a 20)
    - Conhecimento Específico (questões 21 a 30)
  - Média final de cada candidato, que é a soma das notas dos três componentes.

- **Ordenação e geração de relatórios:**  
  Os candidatos são ordenados de forma decrescente pela média final (usando o algoritmo QuickSort) e são gerados os relatórios:
  - `classificados.csv`: lista final dos candidatos classificados.
  - `filtro.csv`: candidatos filtrados pelo código de disciplina.

- **Paralelismo:**  
  As etapas de verificação das respostas e cálculo das notas são paralelizadas utilizando OpenMP, possibilitando a configuração do número de threads via linha de comando.

## Estrutura do Projeto

- **main.c:**  
  Contém a função `main`, que configura o número de threads, realiza a leitura dos arquivos, chama as funções de processamento e gera os relatórios.

- **io.c / io.h:**  
  Funções para manipulação dos arquivos CSV (leitura dos dados de entrada e geração dos arquivos de saída).

- **processamento.c / processamento.h:**  
  Contém as funções que realizam o processamento dos dados dos candidatos (comparação, cálculo dos percentuais, notas e médias).

- **utils.c / utils.h:**  
  Funções auxiliares, como a implementação do algoritmo de ordenação QuickSort e remoção de aspas de strings.

## Como Compilar

Certifique-se de ter um compilador com suporte a OpenMP (como o GCC). Para compilar o projeto, execute:

```bash
gcc -fopenmp -o projeto main.c io.c processamento.c utils.c

```

## Como Executar ( Windows )

O programa aceita um parâmetro na linha de comando para definir o número de threads a serem utilizadas. Por exemplo:

```bash

projeto.exe 4

```

## Medição de Desempenho
Para avaliar o desempenho do processamento paralelizado, o tempo de execução da função calcularNotas é medido com omp_get_wtime(). Devido ao baixo volume de dados (121 candidatos), os tempos individuais podem ser muito curtos. Para obter uma medida mais significativa, o processamento pode ser repetido (por exemplo, 1000 iterações) e o tempo médio calculado.

## Considerações sobre o Paralelismo

- **Overhead:**
    Em cargas de trabalho pequenas, o overhead de criação e sincronização das threads pode superar o ganho de desempenho, fazendo com que o tempo médio aumente à medida que mais threads são utilizadas.

- **Granularidade:**
    Para evidenciar os benefícios do paralelismo, pode ser necessário aumentar o número de candidatos ou agrupar mais operações em cada thread.