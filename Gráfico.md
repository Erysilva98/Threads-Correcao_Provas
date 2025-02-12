        [Leitura de Arquivos]
                  │
                  ▼
     [Filtragem dos Candidatos]
                  │
                  ▼
   [Comparação das Respostas com o Gabarito] -- (paralelizável por candidato)
                  │
                  ▼
  [Cálculo dos Acertos por Questão] 
                  │
                  ▼
       [Cálculo dos Percentuais p(n)]
                  │
                  ▼
       ┌────────────────────────┐
       │ Para cada componente:  │
       │  - Determina p(M)      │
       │  - Calcula GD(n)       │
       │  - Soma GD's           │
       │  - Calcula P(n)        │
       └────────────────────────┘  -- (paralelizável entre os 3 grupos)
                  │
                  ▼
     [Cálculo das Notas dos Candidatos] -- (paralelizável por candidato)
                  │
                  ▼
           [Ordenação dos Candidatos]
                  │
                  ▼
         [Geração do Relatório CSV]
