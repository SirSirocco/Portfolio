#include "grafos.h"


/* erro */
static void erro(int status)
{
    switch (status)
    {
    case 2:
        fprintf(stderr, "\nCodigo de erro %d: impossivel alocar memoria para grafo.\n", status);
        break;

    case 3:
        fprintf(stderr, "\nCodigo de erro %d: impossivel alocar memoria para lista de vizinhancas.\n", status);
        break;

    default:
        fprintf(stderr, "\nOcorreu um erro desconhecido.\n");
        break;
    }

    exit(status);
}


/* aviso */
static void aviso(int status)
{
    switch (status)
    {
    case 1:
        fprintf(stderr, "\nCodigo de aviso %d: numero de vertices invalido (deve ser um numero positivo).\n", status);
        break;

    default:
        break;
    }
}


/* criaNo */
static No* criaNo(int i, Peso peso, No* prox)
{
    No* no = (No*)malloc(sizeof(No));

    if (!no)
    {
        erro(3);
    }

    no->i = i;
    no->peso = peso;
    no->prox = prox;

    return no;
}


/* criaGrafo */
/*
Acoplamento:
    1. nVert: numero de vertices do grafo (deve ser maior que zero).
*/
Grafo* criaGrafo(int nVert)
{
    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));

    if (nVert <= 0)
    {
        aviso(1);
        free(grafo);
        return NULL;
    }

    if (!grafo)
    {
        erro(2);
    }

    grafo->vViz = (No**)malloc(nVert * sizeof(No*));

    if (!grafo->vViz)
    {
        erro(3);
    }

    for (int i = 0; i < nVert; i++)
    {
        grafo->vViz[i] = NULL; // Preenche vizinhos com NULL
    }

    grafo->nVert = nVert;
    grafo->nArest = 0;

    return grafo;
}


/* printaGrafo */
/*
Acoplamento:
    1. grafo:   ponteiro para o grafo a ser printado.
    2. DIGRAFO: booleana que indica de o grafo eh direcionado (DIGRAFO == TRUE) ou nao (DIGRAFO == FALSE).
*/
static void printaGrafo(Grafo* grafo, int DIGRAFO)
{
    No* no;
    int count = 0;
    for (int i = 0; i < grafo->nVert; i++)
    {
        printf("No %d:\n", i);
        no = grafo->vViz[i];

        while (no)
        {
            count += no->peso;
            printf("\t(%d, %d) - peso: %d\n", i, no->i, no->peso);
            no = no->prox;
        }
    }
    
    if (!DIGRAFO)
    {
      count /= 2;
    }
    
    printf("\nNumero de vertices: %d\n", grafo->nVert);
    printf("Numero de arestas: %d\n", grafo->nArest);
    printf("Peso total: %d\n\n", count);
}


/* printaMonografo */
/*
Objetivo:
    Wrapper da funcao printaGrafo com DIGRAFO == FALSE.
*/
void printaMonografo(Grafo* grafo)
{
  printaGrafo(grafo, FALSE);
}


/* printaDigrafo */
/*
Objetivo:
    Wrapper da funcao printaGrafo com DIGRAFO == TRUE.
*/
void printaDigrafo(Grafo* grafo)
{
  printaGrafo(grafo, TRUE);
}


/* printaArestas */
void printaArestas(Aresta* vArestas, int nArest)
{
    for (int i = 0; i < nArest; i++)
    {
        printf("%d  -  %d  -  %d\n", vArestas[i].i, vArestas[i].j, vArestas[i].peso);
    }
}


/* ordenaArestas */
void ordenaArestas(Aresta* vArestas, int nArest)
{
    Aresta aux;
    for (int i = 0; i < nArest - 1; i++)
    {
        for (int j = i + 1; j < nArest; j++)
        {
            if (vArestas[i].peso > vArestas[j].peso)
            {
                aux.i = vArestas[i].i;
                aux.j = vArestas[i].j;
                aux.peso = vArestas[i].peso;

                vArestas[i].i = vArestas[j].i;
                vArestas[i].j = vArestas[j].j;
                vArestas[i].peso = vArestas[j].peso;

                vArestas[j].i = aux.i;
                vArestas[j].j = aux.j;
                vArestas[j].peso = aux.peso;
            }
        }
    }
}


/* verificaArestaExistente */
/*
Objetivo:
    Retorna TRUE se a aresta ja existe no grafo e FALSE do contrario.
Funcionamento:
    A funcao considera as arestas (a, b) e (b, a) de maneira diferente.
*/
static int verificaArestaExistente(No* no, int i)
{
    while (no)
    {
        if (no->i == i)
        {
            return TRUE;
        }

        no = no->prox;
    }

    return FALSE;
}


/* enlistaAresta */
/*
Objetivo:
    Adiciona a aresta com o indice de destino na lista de vizinhancas do
    indice de origem.
Funcionamento:
    A funcao considerara as aresta (a, b) e (b, a) como diferentes.
*/
static void enlistaAresta(Grafo* grafo, int idxOrigem, int idxDestino, Peso peso)
{
    No* aux = grafo->vViz[idxOrigem], * novo = criaNo(idxDestino, peso, NULL);

    if (!aux)
    {
        grafo->vViz[idxOrigem] = novo;
    }

    else if (!aux->prox)
    {
        if (aux->peso > peso)
        {
            novo->prox = aux;
            grafo->vViz[idxOrigem] = novo;
        }

        else
        {
            aux->prox = novo;
        }
    }

    else
    {
        while (aux->prox && peso > aux->prox->peso)
        {
            aux = aux->prox;
        }

        novo->prox = aux->prox;
        aux->prox = novo;
    }
}


/* enlistaArestaIJ */
/*
Objetivo:
    Wrapper da funcao enlistaAresta, que encapsula os indices sob a forma
    de uma estrutura aresta.
Funcionamento:
    O indice de origem eh "i", e o de destino eh "j".
*/
static void enlistaArestaIJ(Grafo* grafo, Aresta aresta)
{
    enlistaAresta(grafo, aresta.i, aresta.j, aresta.peso);
}


/* enlistaArestaJI */
/*
Objetivo:
    Wrapper da funcao enlistaAresta, que encapsula os indices sob a forma
    de uma estrutura aresta.
Funcionamento:
    O indice de origem eh "j", e o de destino eh "i".
*/
static void enlistaArestaJI(Grafo* grafo, Aresta aresta)
{
    enlistaAresta(grafo, aresta.j, aresta.i, aresta.peso);
}


/* adicionaAresta */
/*
Objetivo:
    Atualiza o grafo passado como argumento, adicionando elementos nas listas de
    vizinhanca.
Acoplamento:
    1. grafo:   ponteiro para o grafo a ser atualizado.
    2. arestas: vetor de arestas.
    3. nArest:  numero de elementos do vetor de arestas.
    4. DIGRAFO: booleana que determina se o grafo eh digrafo (direcionado) ou monografo (Nao direcionado)
Funcionamento:
    1. DIGRAFO == TRUE -> grafo eh digrafo. Neste caso, a aresta (a, b) sera considerada diferente da
       (b, a). Ou seja, um elemento de arestas dara origem a apenas um elemento nas listas de vizinhanca.
    2. DIGRAFO == FALSE -> grafo eh monografo. Neste caso, a aresta (a, b) sera considerada igual a (b, a).
       Ou seja, um elemento de arestas dara origem a dois elementos nas listas de vizinhancas: um elemento na
       lista de a e outro na lista de b.
    3. Atualiza grafo->nArest toda vez que uma aresta eh inclusa. Se for monografo, (a, b) e (b, a) contam como
       uma aresta.
*/
static void adicionaAresta(Grafo* grafo, Aresta* arestas, int nArest, int DIGRAFO)
{
    for (int k = 0; k < nArest; k++)
    {
        if (verificaArestaExistente(grafo->vViz[arestas[k].i], arestas[k].j))
        {
            continue;
        }
        
        enlistaArestaIJ(grafo, arestas[k]);

        if (!DIGRAFO)
        {
            enlistaArestaJI(grafo, arestas[k]);
        }

        grafo->nArest++;
    }
}


/* adicionaArestaMonografo */
/*
Objetivo:
    Wrapper da adicionaAresta com DIGRAFO == FALSE.
*/
void adicionaArestaMonografo(Grafo* grafo, Aresta* arestas, int nArest)
{
    adicionaAresta(grafo, arestas, nArest, FALSE);
}


/* adicionaArestaDigrafo */
/*
Objetivo:
    Wrapper da adicionaAresta com DIGRAFO == TRUE.
*/
void adicionaArestaDigrafo(Grafo* grafo, Aresta* arestas, int nArest)
{
    adicionaAresta(grafo, arestas, nArest, TRUE);
}


/* liberaGrafo */
/*
Objetivo:
    Libera os nohs das listas de vizinhancas, o vetor de ponteiros para as listas (vViz) e
    o grafo em si.
*/
void liberaGrafo(Grafo* grafo)
{
    No* aux;

    for (int i = 0; i < grafo->nVert; i++)
    {
        while (grafo->vViz[i])
        {
            aux = grafo->vViz[i];
            grafo->vViz[i] = grafo->vViz[i]->prox;
            free(aux);
        }
    }

    free(grafo->vViz);
    free(grafo);
}