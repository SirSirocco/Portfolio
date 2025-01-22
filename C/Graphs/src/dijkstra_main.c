/* DEPENDENCIAS EXTERNAS */
#include <stdio.h>
#include "grafos.h"
#include "dijkstra.h"


int main(void)
{
  /* DECLARACAO DE VARIAVEIS E CONSTRUCAO DO DIGRAFO */
  Aresta vArestas[] =
  {
      {0, 1, 2},
      {0, 4, 1},
      {1, 4, 7},
      {1, 2, 3},
      {2, 3, 4},
      {3, 4, 5},
      {4, 2, 6}
  };
  int nVert = 5,
      nArest = sizeof(vArestas) / sizeof(vArestas[0]);
  Grafo* grafo = criaGrafo(nVert);
  adicionaArestaDigrafo(grafo, vArestas, nArest);

  /* EXIBICAO DO GRAFO */
  printf("# GRAFO A SER EXAMINADO #\n\n");
  printaDigrafo(grafo);

  /* ALGORITMO DE DIJKSTRA */
  printf("# APLICACAO DO ALGORITMO DE DIJKSTRA #\n\n");
  dijkstra(grafo, 0);

  /* TERMINO DO PROGRAMA */  
  liberaGrafo(grafo);
	return 0;
}