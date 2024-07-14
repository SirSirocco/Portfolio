#include "dijkstra.h"


/* erro */
static void erro(int status)
{
    switch (status)
    {
    case 2:
        fprintf(stderr, "\nCodigo de erro %d: impossivel alocar memoria para construcao da tabela.\n", status);
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


/* compara */
/*
Objetivo:
    Retorna se a eh estritamente menor que b.
Acomplamento:
    1. a: valor inteiro ou INFINITY.
    2. b: valor inteiro ou INFINITY.
Retorno:
    Retorna TRUE ou FALSE a depender do resultado.
Restricoes:
    1. Supoe que a e b sejam nao negativos.
    2. a e b nao podem ser simultaneamente INFINITY.
*/
static int compara(int a, int b)
{
    if (a == INFINITY)
        return FALSE;

    if (b == INFINITY)
        return TRUE;

    return (a < b) ? TRUE : FALSE;
}


/* atualizaTabela */
/*
Objetivo:
    Atualiza os vetores com os valores a serem exibidos na tabela do algoritmo de Dijkstra.
Acomplamento:
    1. grafo:      grafo em que se esta aplicando o algoritmo de Dijkstra.
    2. visitados:  vetor que indica se os vertices ja foram visitados.
    3. distancias: vetor que guarda as distancias de cada vertice a origem.
    4. previos:    vetor que guarda o indice dos vertices previos.
    5. iCorr:      indice do vertice corrente.
*/
static void atualizaTabela(Grafo* grafo, char* visitados, int* distancias, int* previos, int iCorr)
{
    No* no = grafo->vViz[iCorr];
    int distTotal, distCorr = distancias[iCorr];

    while (no)
    {
        if (!visitados[no->i]) // Se noh ja tiver sido vistado, nao compara distancias
        {
            distTotal = distCorr + no->peso;
            if (compara(distTotal, distancias[no->i])) // Se distancia atual menor que distancia anterior, atualiza
            {
                distancias[no->i] = distTotal;
                previos[no->i] = iCorr; // iCorr passa a ser previo do noh
            }
        }

        no = no->prox;
    }
}


/* visita */
static void visita(char* visitados, int i)
{
    visitados[i] = TRUE;
}


/* procuraMenorNaoVisitado */
/*
Objetivo:
    Retorna indice do noh com menor distancia ainda nao visitado.
*/
static int procuraMenorNaoVisitado(int* distancias, char* visitados, int nVert)
{
    int candidato = 0;
    // Atualiza candidato enquanto ja tiver sido visitado e for menor que o numero de vertices - 1 (indice maximo de vViz)  
    while (candidato < nVert - 1 && visitados[candidato])
    {
        candidato++;
    }

    for (int i = candidato + 1; i < nVert; i++) // Compara com os restantes
    {
        if (visitados[i]) // Se ja visitado, ignora 
        {
            continue;
        }

        if (compara(distancias[i], distancias[candidato])) // Se a distancia atual for menor, atualiza candidato
        {
            candidato = i;
        }
    }

    return candidato;
}


/* printaVisitado */
/*
Objetivo:
    Funcao auxliar que imprime um elemento da coluna "Visitado".
*/
static void printaVisitado(char ch)
{
    if (ch == TRUE)
    {
        printf("   SIM    |");
    }

    else
    {
        printf("   NAO    |");
    }
}


/* printaDistancia */
/*
Objetivo:
    Funcao auxliar que imprime um elemento da coluna "Distancia".
*/
static void printaDistancia(int dist)
{
    if (dist == INFINITY)
    {
        printf("    oo     |"); // "oo" simboliza o infinito
    }

    else
    {
        printf("   %3d     |", dist);
    }
}


/* printaPrevio */
/*
Objetivo:
    Funcao auxliar que imprime um elemento da coluna "Previo".
*/
static void printaPrevio(int previo)
{
    if (previo == UNDEF)
    {
        printf("     -    \n"); // "-" simboliza ausencia de previo
    }

    else
    {
        printf("   %3d    \n", previo);
    }
}


/* printaTabela */
/*
Objetivo:
    Imprime a tabela do algoritmo de Dijkstra.
Acomplamento:
    1. nVisita:    numero da visita.
    2. visitados:  vetor que indica se os vertices ja foram visitados.
    3. distancias: vetor que guarda as distancias de cada vertice a origem.
    4. previos:    vetor que guarda o indice dos vertices previos.
    5. nVert:      numero de vertices do grafo.
*/
static void printaTabela(int nVisita, char* visitados, int* distancias, int* previos, int nVert)
{
    printf("Visita %d:\n", nVisita);
    printf("Indice | Visitado | Distancia | Previo\n");

    for (int i = 0; i < nVert; i++)
    {
        printf(" %3d   |", i);
        printaVisitado(visitados[i]);
        printaDistancia(distancias[i]);
        printaPrevio(previos[i]);
    }

    printf("\n");
}


/* dijkstra */
/*
Objetivo:
    Calcula a distancia minima entre um vertice especifico (origem) e os demais de um digrafo.
Acomplamento:
    1. grafo:   ponteiro para digrafo (grafo direcionado). Neste caso, (a, b) != (b, a).
    2. iOrigem: indice do vertice de origem.
Restricoes:
    1. Nao funcionara adequadamente para monografos.
Funcionamento:
    1. Imprime a tabela de Dijkstra a cada passo do algoritmo.
*/
void dijkstra(Grafo* grafo, int iOrigem)
{
    if (grafo->nVert <= 0)
    {
        aviso(1);
        return;
    }

    char* visitados = (char*)malloc(grafo->nVert * sizeof(char));
    int* distancias = (int*)malloc(grafo->nVert * sizeof(int)),
       * previos    = (int*)malloc(grafo->nVert * sizeof(int)),
         iCorr;

    if (!visitados || !distancias || !previos)
    {
        erro(2);
    }

    for (int i = 0; i < grafo->nVert; i++)
    {
        visitados[i] = 0;
        distancias[i] = INFINITY;
        previos[i] = UNDEF;
    }

    distancias[iOrigem] = 0;

    printaTabela(0, visitados, distancias, previos, grafo->nVert);
    atualizaTabela(grafo, visitados, distancias, previos, iOrigem);
    visita(visitados, iOrigem);

    for (int j = 1; j <= grafo->nVert; j++)
    {
        printaTabela(j, visitados, distancias, previos, grafo->nVert);
        iCorr = procuraMenorNaoVisitado(distancias, visitados, grafo->nVert);
        atualizaTabela(grafo, visitados, distancias, previos, iCorr);
        visita(visitados, iCorr);
    }

    free(visitados);
    free(distancias);
    free(previos);
}