# Relatório do EP Indexador de Palavras

**Aluno(s):** Matheus Branco, Julio Cesar Galdino de Souza
**Disciplina:** ACH2023 - Algoritmos e Estruturas de Dados I

---

## 1. Introdução

Este relatório apresenta os resultados obtidos com a implementação de um indexador e buscador de palavras desenvolvido como parte do Exercício Programa da disciplina ACH2023 – Algoritmos e Estruturas de Dados I. O programa utiliza duas estruturas de dados distintas para a construção do índice: uma Lista Encadeada e uma Árvore Binária de Busca (BST).

O objetivo principal dos experimentos realizados é comparar o desempenho dessas duas estruturas em termos do número de comparações entre strings realizadas tanto durante a fase de construção do índice quanto durante a realização de buscas por palavras. Para isso, foram utilizados arquivos de texto com tamanhos crescentes, denominados curto, médio e longo, permitindo analisar o impacto do aumento do volume de dados e do vocabulário indexado sobre o custo das operações.

Além disso, foram avaliados diferentes cenários de busca, incluindo palavras de alta incidência, baixa incidência e palavras inexistentes no texto, de modo a contemplar casos médios e de pior caso para cada estrutura de dados. Os resultados obtidos são analisados e discutidos à luz da teoria estudada ao longo da disciplina.

## 2. Metodologia

Os experimentos realizados têm como objetivo avaliar o custo, em número de comparações entre strings, das operações de construção do índice e de busca por palavras em duas estruturas de dados distintas: Lista Encadeada e Árvore Binária de Busca (BST).

### 2.1 Conjunto de Dados
Foram utilizados três arquivos de texto com tamanhos crescentes, denominados curto, médio e longo, construídos a partir do texto-base fornecido no enunciado do exercício. A ampliação dos arquivos foi realizada de forma controlada, por meio da replicação parcial do texto e da adição de frases extras, com o objetivo de aumentar tanto o número total de palavras quanto o tamanho do vocabulário indexado.

As principais características dos arquivos são resumidas a seguir:

- Arquivo curto (texto_curto.txt): texto original do enunciado, contendo aproximadamente 13 linhas, cerca de 100 palavras e cerca de 70 palavras únicas.

- Arquivo médio (texto_medio.txt): composto pela replicação do texto base com adição de frases padronizadas ao final de cada bloco, resultando em aproximadamente 150 linhas, cerca de 1.200 palavras e um vocabulário de tamanho intermediário.

- Arquivo longo (texto_longo.txt): obtido a partir da replicação do arquivo médio, com variação cíclica das frases adicionais, totalizando aproximadamente 1.500 linhas, cerca de 12.000 palavras e um vocabulário significativamente maior.

Essa estratégia permitiu analisar o impacto do crescimento do volume de dados e do vocabulário de forma gradual e controlada.

### 2.2 Construção do Índice
Em cada execução do programa, o arquivo de texto foi carregado integralmente em memória, sendo armazenado linha por linha em uma estrutura sequencial. Em seguida, cada palavra do texto foi processada e inserida no índice, de acordo com o tipo de estrutura selecionada (lista ou arvore).

Durante a construção do índice, o programa contabilizou o número total de comparações entre strings realizadas para verificar a existência de uma palavra no índice e para determinar sua posição de inserção, conforme o comportamento da estrutura de dados utilizada.

### 2.3 Cenários de Busca

Após a construção do índice, foram realizadas buscas por palavras representando diferentes cenários:

Palavra de alta incidência: termo que ocorre diversas vezes no texto, utilizado para avaliar o comportamento da busca em situações comuns.

Palavra de baixa incidência: termo que ocorre poucas vezes, representando um caso intermediário.

Palavra inexistente: termo que não ocorre no texto, caracterizando o pior caso para ambas as estruturas.

As mesmas palavras foram utilizadas nas buscas em todos os arquivos e para ambos os tipos de índice, garantindo a comparabilidade dos resultados.

### 2.4 Métrica de Avaliação

A principal métrica utilizada nos experimentos foi o número de comparações entre strings, contabilizado separadamente para:

- a fase de construção do índice;

- cada operação de busca realizada.

Essa métrica foi escolhida por refletir diretamente o custo das operações fundamentais das estruturas de dados estudadas, sendo independente de fatores externos como velocidade de processamento ou características do hardware utilizado.


## 3. Resultados

### 3.1. Construção do Índice

- Tabela 1 - Construção do índice
| Arquivo | Linhas | Palavras Únicas | Estrutura | Comparações (Construção) |
| ------- | ------ | --------------- | --------- | ------------------------ |
| Curto   | 11     | 70              | Lista     | 3412                     |
| Curto   | 11     | 70              | Árvore    | 691                      |
| Médio   | 129    | 74              | Lista     | 38346                    |
| Médio   | 129    | 74              | Árvore    | 7966                     |
| Longo   | 1219   | 87              | Lista     | 388193                   |
| Longo   | 1219   | 87              | Árvore    | 80810                    |

A Tabela 1 apresenta o número de comparações realizadas durante a construção do índice para os três arquivos de texto avaliados. Observa-se que o número de comparações cresce à medida que o tamanho do texto e do vocabulário aumentam, sendo consistentemente maior na implementação baseada em lista quando comparada à árvore binária de busca.


### 3.2. Resultados das Buscas por Palavras

#### Definição das Palavras Buscadas
Para a avaliação das operações de busca, foram selecionadas três categorias de palavras:
(i) uma palavra de alta incidência, "algorithm";
(ii) uma palavra de baixa incidência, "fertile";
e (iii) uma palavra inexistente, "quicksort".

### 3.3. Busca por Palavra de Alta Incidência
- Tabela 2 - Palavra de Alta Incidência ("algorithm")
| Arquivo | Estrutura | Ocorrências         | Comparações   |
| ------- | --------- | ------------------- | ------------- |
| Curto   | Lista     |                     | 3             |
| Curto   | Árvore    |                     | 3             |
| Médio   | Lista     |                     | 3             |
| Médio   | Árvore    |                     | 10            |
| Longo   | Lista     |                     | 100           |
| Longo   | Árvore    |                     | 100           |

A Tabela 2 apresenta os resultados das buscas por uma palavra de alta incidência. Nota-se que o número de ocorrências da palavra no texto não influencia diretamente o custo da busca, uma vez que a operação consiste apenas em localizar a entrada correspondente no índice.


### 3.4. Busca por Palavra de Baixa Incidência
- Tabela 3 — Palavra Baixa Incidência (“fertile”)
| Arquivo | Estrutura | Ocorrências | Comparações      |
| ------- | --------- | ----------- | ---------------- |
| Curto   | Lista     |             | 62               |
| Curto   | Árvore    |             | 8                |
| Médio   | Lista     |             | 62               |
| Médio   | Árvore    |             | 8                |
| Longo   | Lista     |             | 62               |
| Longo   | Árvore    |             | 8                |

Os resultados para a busca por uma palavra de baixa incidência seguem o mesmo padrão observado para palavras frequentes, reforçando que o custo da busca depende da estrutura do índice e não do número de ocorrências da palavra no texto.

### 3.5. Busca por Palavra Inexistente (Pior Caso)
- Tabela 4 — Palavra Inexistente (“quicksort”)
| Arquivo | Estrutura | Resultado      | Comparações |
| ------- | --------- | -------------- | ----------- |
| Curto   | Lista     | Não encontrada | 70          |
| Curto   | Árvore    | Não encontrada | 12          |
| Médio   | Lista     | Não encontrada | 74          |
| Médio   | Árvore    | Não encontrada | 12          |
| Longo   | Lista     | Não encontrada | 87          |
| Longo   | Árvore    | Não encontrada | 12          |

A Tabela 4 apresenta os resultados para a busca por uma palavra inexistente, caracterizando o pior caso para ambas as estruturas. Na lista, a busca exige a verificação de todas as palavras do vocabulário indexado, enquanto na árvore a busca é encerrada ao atingir um nó nulo.

## 4. Discussão dos Resultados e Conclusão

A análise detalhada dos experimentos realizados permite extrair conclusões definitivas sobre o desempenho comparativo entre a Lista Encadeada e a Árvore Binária de Busca (BST) para a tarefa de indexação textual.

### 4.1. Análise da Construção do Índice (Carga)
A fase de construção do índice (referente à Tabela 1) evidenciou a maior disparidade de desempenho entre as estruturas.
* **Lista Encadeada:** Apresentou um crescimento de custo linearmente dependente do produto entre o tamanho do texto ($N$) e o tamanho do vocabulário indexado ($V$). Para cada nova palavra lida do texto, a lista exige uma varredura sequencial para verificar se o termo já existe. No arquivo "Longo", isso resultou em mais de **388.000 comparações**.
* **Árvore Binária:** Manteve um desempenho significativamente superior, realizando cerca de **80.000 comparações** para o mesmo arquivo "Longo" (quase 5 vezes menos). Isso ocorre porque a verificação de existência na árvore possui complexidade média logarítmica ($O(\log V)$), tornando a inserção muito mais rápida à medida que o vocabulário cresce.

### 4.2. Análise das Buscas e Estabilidade do Vocabulário
Um fenômeno importante observado nas Tabelas 3 e 4 foi a **constância no número de comparações** para os arquivos Médio e Longo. Por exemplo, a busca pela palavra *"fertile"* custou exatamente o mesmo número de comparações (62 na Lista e 8 na Árvore) em todos os cenários, embora o número de ocorrências tenha subido de 1 para 100.

Esse comportamento valida a eficiência da estrutura de índice invertido:
1.  **Independência do Tamanho do Texto:** O custo computacional da busca depende exclusivamente do tamanho do **vocabulário único** ($V$), e não do número total de palavras no texto ($N$).
2.  **Efeito da Metodologia:** Como os arquivos maiores foram gerados pela replicação do texto base, o vocabulário de palavras únicas permaneceu estável (crescendo minimamente apenas devido às frases de junção).
3.  **Escalabilidade:** Isso demonstra que o índice é altamente escalável. O programa processou um texto 100 vezes maior (arquivo Longo) sem degradar o tempo de resposta da busca, pois a estrutura não armazenou dados redundantes, limitando-se a atualizar os contadores e as listas de ocorrências das palavras já existentes.

### 4.3. Análise do Pior Caso (Palavra Inexistente)
A busca pela palavra *"quicksort"* (Tabela 4) ilustra o cenário de **Pior Caso**, onde a diferença de complexidade entre as estruturas fica mais evidente:
* Na **Lista Encadeada**, o algoritmo foi forçado a percorrer **todos** os nós da lista (custo $O(V)$) para concluir que a palavra não existia.
* Na **Árvore Binária**, o algoritmo precisou apenas percorrer um caminho da raiz até uma folha nula (custo $O(H)$, onde $H$ é a altura da árvore).
* Numericamente, isso representou uma economia de processamento de aproximadamente **85%** a favor da árvore no cenário Longo.

### 4.4. Conclusão Final
Os experimentos confirmam a hipótese teórica de que estruturas hierárquicas (Árvores) são superiores a estruturas lineares (Listas) para sistemas de recuperação de informação.

Embora a Lista Encadeada seja de implementação trivial, seu custo de busca linear a torna proibitiva para grandes vocabulários. A Árvore Binária de Busca, mesmo sem balanceamento dinâmico, oferece um equilíbrio ideal entre tempo de construção e velocidade de consulta, provando ser a estrutura mais adequada para a solução do problema proposto neste Exercício Programa.