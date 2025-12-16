# README - EP AED: Indexador e Buscador de Palavras

## Como compilar

Para compilar o programa, utilize o compilador `gcc` no terminal. O programa é composto por um único arquivo fonte `esqueleto_EP.c`.

Comando:
```bash
gcc -Wall -Wextra esqueleto_EP.c -o ep
```

Isso gerará um executável chamado `ep` (ou `ep.exe` no Windows).

## Como executar

O programa deve ser executado via linha de comando recebendo dois argumentos obrigatórios:
1. O nome do arquivo de texto a ser indexado.
2. O tipo de índice a ser utilizado: `lista` ou `arvore`.

Exemplos:

**Modo Lista:**
```bash
./ep texto.txt lista
```

**Modo Árvore (BST):**
```bash
./ep texto.txt arvore
```

Após a execução, o programa exibirá estatísticas sobre a indexação e aguardará comandos do usuário (`busca <palavra>` ou `fim`).

## Estrutura dos Arquivos

- `esqueleto_EP.c`: Código fonte principal.
- `texto.txt`: Arquivo de texto exemplo para testes.
- `relatorio.pdf`: Relatório de experimentos e análise (gerado a partir do md).
