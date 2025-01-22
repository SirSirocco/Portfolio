**Objetivo:** Implementar dois dos principais algoritmos relacionados a grafos: o algoritmo de Kruskal e o de Dijkstra.

## Estrutura dos Programas

### Programa de Kruskal

O programa é composto de cinco módulos:

- `kruskal_main.c`, responsável pela execução do programa que realiza o algoritmo de Kruskal;
- `grafos.c`, responsável pela manipulação de grafos;
- `grafos.h`, responsável pelas declarações das funções do grafos.c e das estruturas node, grafo e aresta;
- `kruskal.c`, responsável pelo algoritmo de Kruskal;
- `kruskal.h`, responsável pelas declarações das funções do kruskal.c e da estrutura noConj.

### Programa de Dijkstra

O programa é composto de cinco módulos:

- `dijkstra_main.c`, responsável pela execução do programa que realiza o algoritmo de Dijkstra;
- `grafos.c`, responsável pela manipulação de grafos;
- `grafos.h`, responsável pelas declarações das funções do grafos.c e das estruturas node, grafo e aresta;
- `dijkstra.c`, responsável pelo algoritmo de Dijkstra;
- `dijkstra.h`, responsável pelas declarações das funções do dijkstra.c.

### Estruturas Utilizadas

### `grafos.c`

```
struct node
{
int i;
Peso peso;
struct node* prox;
};

struct grafo
{
struct node** vViz;
int nVert;
int nArest;
};

struct aresta
{
int i;
int j;
Peso peso;
};
typedef struct node No;
typedef struct grafo Grafo;
typedef struct aresta Aresta;
```

### `kruskal.c`

```
struct noConj
{
int chave;
struct noConj* rep;
};
typedef struct noConj NoConj;
```

### Funções Utilizadas

### `grafos.c`

```
Grafo* criaGrafo(int nVert);
void printaMonografo(Grafo* grafo);
void printaDigrafo(Grafo* grafo);
void printaArestas(Aresta* vArestas, int nArest);
void ordenaArestas(Aresta* vArestas, int nArest);
void adicionaArestaMonografo(Grafo* grafo, Aresta* arestas, int nArest);
void adicionaArestaDigrafo(Grafo* grafo, Aresta* arestas, int nArest);
void liberaGrafo(Grafo* grafo);
```

### `kruskal.c`

```
void iniciaVertices(NoConj* vVertices, int nVert);
void printaVertices(NoConj* vVertices, int nVert);
void fusion(int num1, int num2, NoConj* vVertices, int nVert);
NoConj* find(int num1, NoConj* vVertices);
Grafo* criaArvMinima(Aresta* vArestas, int nArest, NoConj* vVertices, int nVert);
```

### `dijkstra.c`

```
void dijkstra(Grafo* grafo, int iOrigem);
```

## Solução

### Kruskal

Ao início, são declarados o vetor de arestas, um vetor de estruturas do tipo `NoConj`, que representa os _singletons_, duas variáveis do tipo ponteiro para `Grafo` e duas do tipo `int`, que contêm as informações dos grafos. Uma das variáveis do tipo `Grafo` recebe o endereço do grafo gerado pela criaGrafo, e depois a função printaMonografo o exibe. Após a exibição do grafo inteiro, é iniciado o algoritmo de Kruskal. Inicialmente, o vetor de _singletons_ é iniciado e o de arestas é ordenado. Depois, é chamada a função `criaArvMinima`. Nela, um grafo é iniciado, ou seja, é alocado apenas o vetor de ponteiros que irão apontar para as listas encadeadas de cada vértice. Ao percorrer o vetor de arestas, para cada uma delas, ele realiza um _find_ dos vértices que formam essa aresta e, para isso, ele chama a função `find`. Essa função funciona recursivamente e procura o endereço do representante do elemento. Se os dois representantes de ambos os vértices forem diferentes, é chamada a função `adicionaArestaMonografa` que vai adicionar um novo elemento na lista encadeada da posição do vértice. Depois é chamada a função `fusion` que, levando em conta a altura, coloca os dois vértices no mesmo conjunto. Após o final da iteração das arestas, o grafo que representa a árvore é retornado para a outra variável do tipo ponteiro para `Grafo` e, novamente, é chamada a `printaMonografo`, que exibe a árvore mínima retornada. Por fim, as memórias de ambos os grafos são liberadas com a função `liberaGrafo`.

### Dijkstra

Ao início, é construído o grafo com base em um conjunto de arestas previamente definido. Exibe-se o grafo antes e depois das inserções. Em seguida, aplica-se a dijkstra ao grafo, passando-se o índice zero como origem. A função imprime uma tabela com as condições iniciais (Visita 0) e, a cada nova visita a um vértice pelo algoritmo, exibe a tabela de Dijkstra atualizada. Evidentemente, a última tabela printada é aquela com as distâncias mínimas à origem.

## Observações e Conclusões

Os algoritmos de Kruskal e de Dijkstra revelaram-se surpreendentemente fáceis de serem implementados — ainda mais com o prévio desenvolvimento do módulo `grafos.c` —, com pleno funcionamento e êxito em todos os testes. Quanto a dificuldades de implementação, estas, sendo pequenas, foram prontamente resolvidas com o auxílio do depurador. Por fim, os programas foram compilados sem erro usando-se o ambiente de desenvolvimento do replit, que se mostrou conveniente por permitir edição simultânea do código pelos dois integrantes do grupo.
