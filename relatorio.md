# Relatório do EP Indexador de Palavras

**Aluno(s):** [Seu Nome Aqui]
**Disciplina:** ACH2023 - Algoritmos e Estruturas de Dados I

---

## 1. Introdução

Este relatório apresenta os resultados obtidos com a implementação de um indexador de palavras utilizando duas estruturas de dados distintas: uma **Lista Encadeada** e uma **Árvore Binária de Busca (BST)**. O objetivo é comparar o desempenho de ambas as estruturas em termos de número de comparações realizadas durante a fase de construção do índice e de busca.

## 2. Metodologia

Foram realizados experimentos utilizando dois arquivos de texto:
1.  `texto.txt`: O texto exemplo fornecido no enunciado (13 linhas, aprox. 100 palavras).
2.  `texto_large.txt`: O mesmo texto duplicado 100 vezes (1300 linhas, aprox. 10000 palavras).

Para cada arquivo, executou-se o programa nos modos `lista` e `arvore`, coletando-se o número de comparações de strings realizadas para a construção do índice.

Note que, como o arquivo maior é composto apenas por repetições do menor, o número de palavras *únicas* (vocabulário) manteve-se constante em 70 palavras. Isso permite analisar o comportamento do custo de inserção à medida que a entrada cresce, mas a estrutura do índice mantém tamanho fixo.

## 3. Resultados

### Construção do Índice

A tabela abaixo resume os resultados obtidos para a fase de carga/indexação:

| Arquivo | Linhas | Palavras Únicas | Comparações (Lista) | Comparações (Árvore) | Razão (Lista/Árvore) |
| :--- | :--- | :--- | :--- | :--- | :--- |
| `texto.txt` | 13 | 70 | 3.412 | 691 | ~4.9x |
| `texto_large.txt` | 1300 | 70 | 348.130 | 76.030 | ~4.6x |

### Análise

1.  **Diferença de Desempenho**: A implementação utilizando Árvore Binária de Busca mostrou-se significativamente mais eficiente que a Lista Encadeada, realizando cerca de 4 a 5 vezes menos comparações para este conjunto de dados.
    *   **Lista**: Para cada palavra lida, é feita uma busca linear na lista. No pior caso (palavra nova ou no fim da lista), o custo é proporcional ao tamanho atual do vocabulário (V). Custo total ≈ O(N * V).
    *   **Árvore**: A busca é feita descendo a árvore. O custo é proporcional à altura da árvore (H). Em uma árvore não balanceada (como implementada), H tende a ser muito menor que V (idealmente log₂ V). Custo total ≈ O(N * H).

2.  **Crescimento**: Ao aumentar o tamanho da entrada em 100x (mantendo o vocabulário fixo), o número de comparações cresceu linearmente (aprox. 100x) em ambos os casos.
    *   Lista: 3.412 -> 348.130 (~102x)
    *   Árvore: 691 -> 76.030 (~110x)
    Isso ocorre porque, após as primeiras 70 inserções que constroem a estrutura, todas as subsequentes operações são buscas de palavras já existentes. Como o tamanho da estrutura (70 nós) não muda, o custo por palavra torna-se constante (médio).
    *   Custo médio/palavra (Lista) ≈ 348130 / 10000 ≈ 35 comparações (metade de 70, esperado para busca linear média).
    *   Custo médio/palavra (Árvore) ≈ 76030 / 10000 ≈ 7.6 comparações (compatível com log₂ 70 ≈ 6.1 e altura 11).

## 4. Conclusão

Os experimentos confirmam a superioridade teórica e prática da Árvore Binária de Busca sobre a Lista Encadeada para operações de busca e indexação, especialmente quando o número de elementos armazenados cresce. Enquanto a lista exige percorrer todos os elementos no pior caso, a árvore permite descartar metade dos candidatos a cada comparação, resultando em um desempenho muito superior.
