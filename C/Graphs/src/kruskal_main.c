/* DEPENDENCIAS EXTERNAS */
#include <stdio.h>
#include "grafos.h"
#include "kruskal.h"

/* DEFINICOES AUXILIARES */
#define N_VERT 5

int main(void)
{
  /* DECLARACAO DE VARIAVEIS */
  Aresta vArestas [] = 
  {
    {0, 1, 7}, 
    {0, 4, 1}, 
    {1, 2, 4}, 
    {1, 4, 2}, 
    {2, 3, 5}, 
    {2, 4, 6}, 
    {3, 4, 6}, 
  };
  Grafo* grafo, * arvMin;
  int nVert = 5,
      nArest = sizeof(vArestas) / sizeof(vArestas[0]);
  NoConj vVertices [N_VERT];

  /* CONSTRUCAO DO GRAFO */
  grafo = criaGrafo(nVert);
  printf("# GRAFO VAZIO #\n\n");
  printaMonografo(grafo);
  adicionaArestaMonografo(grafo, vArestas, nArest);
  
  /* EXIBICAO DO GRAFO */
  printf("# GRAFO QUE REPRESENTA AS CIDADES E SUAS ESTRADAS #\n\n");
  printaMonografo(grafo);

  /* ALGORITMO DE KRUSKAL */
  iniciaVertices(vVertices, grafo->nVert);
  
  // printaVertices(vVertices, grafo->nVert);
  // printf("\n\n");
  // printaArestas(vArestas, grafo->nArest);
  
  ordenaArestas(vArestas, grafo->nArest);
  
  // printf("\n\n");
  // printaArestas(vArestas, grafo->nArest);
  // printf("\n\n");
  
  /* ARVORE GERADORA MINIMA */
  printf("# SISTEMA DE ESTRADAS COM MENOR DISTANCIA #\n\n");
  arvMin = criaArvMinima(vArestas, nArest, vVertices, nVert);
  
  // printaVertices(vVertices, grafo->nVert);
  // printf("\n\n");
  
  printaMonografo(arvMin);
  
  /* TERMINO DO PROGRAMA */
  liberaGrafo(grafo);
  liberaGrafo(arvMin);
  return 0;
}
