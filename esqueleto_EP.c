#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Data Structures ---

// Nodes for the linked list of line numbers (Occurrences)
typedef struct Occurrence {
  int line_number;
  struct Occurrence *next;
} Occurrence;

// Node for the Index (Shared for List and BST)
typedef struct Node {
  char *word;
  struct Occurrence *lines_head; // Head of the list of occurrences
  struct Occurrence *lines_tail; // Tail for O(1) append
  int count; // Total occurrences (optional but good for stats)

  // Pointers for Linked List implementation
  struct Node *next;

  // Pointers for BST implementation
  struct Node *left;
  struct Node *right;
} Node;

// Global context for the Index
typedef struct {
  Node *head;    // For Linked List
  Node *root;    // For BST
  bool use_tree; // true = BST, false = Linked List

  long comparisons; // To track comparisons during current operation
  int unique_words;
} Index;

// Dynamic array for storing file lines
typedef struct {
  char **lines;
  int count;
  int capacity;
} TextStore;

// --- Function Prototypes ---
void init_index(Index *idx, bool use_tree);
void add_word(Index *idx, char *word, int line_num);
Node *create_node(char *word, int line_num);
void add_occurrence(Node *node, int line_num);
void search_word(Index *idx, char *word); // Prints results directly
void free_index(Index *idx);
int get_tree_height(Node *root);

void init_text_store(TextStore *store);
void add_line(TextStore *store, const char *line);
void free_text_store(TextStore *store);

char *normalize_word(char *token);
char *read_line(FILE *f);

// --- Globals ---
TextStore g_text;
Index g_index;

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Uso: %s <arquivo> <tipo_indice>\n", argv[0]);
    return 1;
  }

  char *filename = argv[1];
  char *type = argv[2];
  bool use_tree;

  if (strcmp(type, "lista") == 0) {
    use_tree = false;
  } else if (strcmp(type, "arvore") == 0) {
    use_tree = true;
  } else {
    fprintf(stderr, "Tipo de indice invalido! Use 'lista' ou 'arvore'.\n");
    return 1;
  }

  // 1. Load Text
  init_text_store(&g_text);
  FILE *f = fopen(filename, "r");
  if (!f) {
    perror("Erro ao abrir arquivo");
    return 1;
  }

  char *line;
  while ((line = read_line(f)) != NULL) {
    add_line(&g_text, line);
    free(line); // add_line makes a copy, so we free this one
  }
  fclose(f);

  // 2. Build Index
  init_index(&g_index, use_tree);
  long build_comparisons = 0; // Comparisons for build phase

  // Parse and Index
  for (int i = 0; i < g_text.count; i++) {
    // We need a copy because strtok modifies the string
    // But we want to keep original line in g_text.lines[i]
    // Actually, we can just process g_text.lines[i] carefully or copy it.
    // Let's copy it to a temp buffer for tokenization.
    char *temp_line = strdup(g_text.lines[i]);
    if (!temp_line)
      continue;

    // Tokenization logic:
    // We will manually iterate to handle complex delimiters if needed,
    // or just use strtok with all punctuation.
    // Prompt says: "Ignorar pontuacao... hifen trata como duas palavras... case
    // insensitive"

    char *ptr = temp_line;
    char *word_start = NULL;

    // Simple manual tokenizer to handle everything strictly
    while (*ptr) {
      if (isalpha((unsigned char)*ptr)) {
        if (!word_start)
          word_start = ptr;
      } else {
        if (word_start) {
          *ptr = '\0'; // Terminate word
          char *norm = normalize_word(word_start);
          // Add to index
          g_index.comparisons = 0; // Reset for this single insertion
          add_word(&g_index, norm, i);
          // to 1-based or 0-based.
          // Actually prompt output example:
          // 00001: ...
          // So lines are 1-indexed.
          // Array is 0-indexed.
          // We store 1-based in occurrences for easier printing, or just add 1
          // when printing. Let's store 0-based to match array index, add 1 on
          // print.
          build_comparisons += g_index.comparisons;

          word_start = NULL;
        }
      }
      ptr++;
    }
    // Last word if line ends with alpha
    if (word_start) {
      char *norm = normalize_word(word_start);
      g_index.comparisons = 0;
      add_word(&g_index, norm, i);
      build_comparisons += g_index.comparisons;
    }

    free(temp_line);
  }

  // 3. Print Initial Stats
  printf("Arquivo: '%s'\n", filename);
  printf("Tipo de indice: '%s'\n", type);
  printf("Numero de linhas no arquivo: %d\n", g_text.count);
  printf("Total de palavras unicas indexadas: %d\n", g_index.unique_words);
  if (use_tree) {
    printf("Altura da arvore: %d\n", get_tree_height(g_index.root));
  }
  printf("Numero de comparacoes realizadas para a construcao do indice: %ld\n",
         build_comparisons);

  // 4. Command Loop
  char cmd_buffer[100];
  while (1) {
    printf("> ");
    if (!fgets(cmd_buffer, sizeof(cmd_buffer), stdin))
      break;

    // Strip newline
    size_t len = strlen(cmd_buffer);
    if (len > 0 && cmd_buffer[len - 1] == '\n')
      cmd_buffer[len - 1] = '\0';
    if (len == 0)
      continue;

    // Parse command
    char cmd[20], arg[80];
    int scanned = sscanf(cmd_buffer, "%s %s", cmd, arg);

    if (scanned < 1)
      continue;

    if (strcmp(cmd, "fim") == 0) {
      break;
    } else if (strcmp(cmd, "busca") == 0) {
      if (scanned < 2) {
        // Should we handle "busca" without arg?
        // Treat as not found or error. Prompt doesn't specify.
        // Assuming arg is mandatory.
        continue;
      }
      char *search_term = normalize_word(arg);
      g_index.comparisons = 0;
      search_word(&g_index, search_term);
      // Search word prints the lines.
      printf("Numero de comparacoes: %ld\n", g_index.comparisons);
    } else {
      printf("Opcao invalida!\n");
    }
  }

  // Cleanup
  free_index(&g_index);
  free_text_store(&g_text);

  return 0;
}

// --- Implementation Helpers ---

char *normalize_word(char *token) {
  // Modify in place (lowercase)
  for (int i = 0; token[i]; i++) {
    token[i] = tolower((unsigned char)token[i]);
  }
  return token;
}

void init_text_store(TextStore *store) {
  store->count = 0;
  store->capacity = 100;
  store->lines = malloc(store->capacity * sizeof(char *));
}

void add_line(TextStore *store, const char *line) {
  if (store->count >= store->capacity) {
    store->capacity *= 2;
    store->lines = realloc(store->lines, store->capacity * sizeof(char *));
  }
  store->lines[store->count++] = strdup(line);
}

void free_text_store(TextStore *store) {
  for (int i = 0; i < store->count; i++) {
    free(store->lines[i]);
  }
  free(store->lines);
}

void init_index(Index *idx, bool use_tree) {
  idx->head = NULL;
  idx->root = NULL;
  idx->use_tree = use_tree;
  idx->comparisons = 0;
  idx->unique_words = 0;
}

Node *create_node(char *word, int line_num) {
  Node *n = malloc(sizeof(Node));
  n->word = strdup(word);
  n->count = 1;
  n->next = NULL;
  n->left = NULL;
  n->right = NULL;

  n->lines_head = malloc(sizeof(Occurrence));
  n->lines_head->line_number = line_num;
  n->lines_head->next = NULL;
  n->lines_tail = n->lines_head;

  return n;
}

void add_occurrence(Node *node, int line_num) {
  // Only add if not same as last to avoid duplicates on same line?
  // Prompt: "linhas em que a palavra aparece".
  // "Existem X ocorrências". If word appears twice on same line, does it count
  // as 2 occurrences? Example: "some value, or set of values, as input and
  // produces some value, or set of values," Prompt output: "Existem 2
  // ocorrenciacias da palavra 'set' na(s) seguinte(s) linha(s):" 00002: some
  // value... It lists line 2 once. So we should count occurrences (total count)
  // but maybe verify unique lines list? Wait, "Total de ocorrencias". If "set"
  // appears twice on line 2, count is 2? The example says "Existem 2
  // ocorrencias... na(s) seguinte(s) linha(s): 00002: ...". And it displays
  // line 2 ONLY ONCE. So:
  // 1. We increment word count for EVERY match.
  // 2. We add line number to list ONLY IF it's different from the last added
  // line (since we process sequentially).

  node->count++;

  if (node->lines_tail->line_number != line_num) {
    Occurrence *occ = malloc(sizeof(Occurrence));
    occ->line_number = line_num;
    occ->next = NULL;
    node->lines_tail->next = occ;
    node->lines_tail = occ;
  }
}

// --- Implementation ---

// Recursive insertion for BST
Node *insert_bst(Node *root, char *word, int line_num, Index *idx) {
  if (!root) {
    idx->unique_words++;
    return create_node(word, line_num);
  }

  idx->comparisons++;
  int cmp = strcmp(word, root->word);

  if (cmp == 0) {
    add_occurrence(root, line_num);
  } else if (cmp < 0) {
    root->left = insert_bst(root->left, word, line_num, idx);
  } else {
    root->right = insert_bst(root->right, word, line_num, idx);
  }
  return root;
}

void add_word(Index *idx, char *word, int line_num) {
  if (idx->use_tree) {
    idx->root = insert_bst(idx->root, word, line_num, idx);
  } else {
    // Linked List Insert
    if (!idx->head) {
      idx->head = create_node(word, line_num);
      idx->unique_words++;
      return;
    }

    Node *curr = idx->head;
    Node *prev = NULL;

    while (curr) {
      idx->comparisons++;
      int cmp = strcmp(word, curr->word);

      if (cmp == 0) {
        add_occurrence(curr, line_num);
        return;
      }
      // For sorted list? Prompt doesn't specify if list must be sorted.
      // "sugere-se... lista sequencial". Actually for the INDEX, prompt says
      // "lista (o tipo exato a ser usado fica a criterio de voces)". Unordered
      // list is O(N) search. Sorted list is O(N) insert, O(N) search (unless
      // sequential array + binary search). Simplest is Unordered list with
      // Append or Prepend. But for Search to be slightly efficient or standard,
      // we usually just append. Let's assume unordered list for simplicity
      // unless sorting is beneficial. However, to check if it exists, we
      // traverse.

      // Optimization: If we keep it sorted, we can fail early on search, but
      // insert is still O(N). Let's stick to simple traversal.

      prev = curr;
      curr = curr->next;
    }

    // Not found, append.
    // If we want to maintain order (alphabetical), we would insert in place.
    // Prompt doesn't require alphabetical order for *list*, but "arvore"
    // implies sorted output? Actually prompt example commands don't show list
    // printing order. Let's just append for now to be safe and simple.

    prev->next = create_node(word, line_num);
    idx->unique_words++;
  }
}

// Helper to print lines
void print_occurrences(Node *node) {
  if (!node)
    return;

  printf("Existem %d ocorrencias da palavra '%s' na(s) seguinte(s) linha(s):\n",
         node->count, node->word);

  Occurrence *occ = node->lines_head;
  while (occ) {
    // g_text is global, we can access it.
    // occ->line_number is 0-indexed in our store logic?
    // In tokenizer loop: `add_word(&g_index, norm, i);` where i is 0...count-1.
    // Prompt output: "00001: ...".
    // So we print (line_number + 1) formatted as %05d.

    printf("%05d: %s\n", occ->line_number + 1, g_text.lines[occ->line_number]);
    occ = occ->next;
  }
}

void search_word(Index *idx, char *word) {
  if (idx->use_tree) {
    Node *curr = idx->root;
    while (curr) {
      idx->comparisons++;
      int cmp = strcmp(word, curr->word);
      if (cmp == 0) {
        print_occurrences(curr);
        return;
      } else if (cmp < 0) {
        curr = curr->left;
      } else {
        curr = curr->right;
      }
    }
  } else {
    Node *curr = idx->head;
    while (curr) {
      idx->comparisons++;
      if (strcmp(word, curr->word) == 0) {
        print_occurrences(curr);
        return;
      }
      curr = curr->next;
    }
  }
  printf("Palavra '%s' nao encontrada.\n", word);
}

void free_occurrences(Occurrence *head) {
  while (head) {
    Occurrence *tmp = head;
    head = head->next;
    free(tmp);
  }
}

void free_nodes_recursive(Node *node) {
  if (!node)
    return;
  free_nodes_recursive(node->left);  // For List, left is NULL
  free_nodes_recursive(node->right); // For List, right is NULL (except next)

  // For List, we need to handle 'next' differently if we use this same func.
  // Better to have separate free or careful logic.
  // Since Node has next/left/right, let's split logic.
}

void free_tree(Node *root) {
  if (!root)
    return;
  free_tree(root->left);
  free_tree(root->right);
  free(root->word);
  free_occurrences(root->lines_head);
  free(root);
}

void free_list(Node *head) {
  while (head) {
    Node *tmp = head;
    head = head->next;
    free(tmp->word);
    free_occurrences(tmp->lines_head);
    free(tmp);
  }
}

void free_index(Index *idx) {
  if (idx->use_tree) {
    free_tree(idx->root);
  } else {
    free_list(idx->head);
  }
}

int get_tree_height(Node *root) {
  if (!root)
    return -1; // Empty tree height? Usually -1 or 0. Def: height of leaf is 0.
               // Height of single node tree is 0.
  // Prompt "Altura da arvore".
  // Data structures texts vary. CLRS: "height of a node is number of edges on
  // longest path to leaf". 1 node -> 0 edges -> height 0. Empty -> -1.
  int hl = get_tree_height(root->left);
  int hr = get_tree_height(root->right);
  return (hl > hr ? hl : hr) + 1;
}

char *read_line(FILE *f) {
  int capacity = 128;
  int pos = 0;
  char *buffer = malloc(capacity * sizeof(char));
  if (!buffer) return NULL;

  int c;
  while (1) {
    c = fgetc(f);
    if (c == EOF || c == '\n') break;

    if (pos >= capacity - 1) {
      capacity *= 2;
      char *new_buf = realloc(buffer, capacity * sizeof(char));
      if (!new_buf) {
        free(buffer);
        return NULL; // Memory error
      }
      buffer = new_buf;
    }
    buffer[pos++] = (char)c;
  }

  if (pos == 0 && c == EOF) {
    free(buffer);
    return NULL; // End of file
  }

  buffer[pos] = '\0';
  return buffer;
}