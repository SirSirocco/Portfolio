#pragma once

/* DEPENDENCIAS EXTERNAS */
#include <stdio.h>
#include <stdlib.h>
#include "grafos.h"

/* DEFINICOES AUXILIARES */
#define TRUE 1
#define FALSE 0
#define INFINITY -1 // Infinito
#define UNDEF -2    // Indica vertice previo indefinido (undefined)

/* FUNCOES */
void dijkstra(Grafo* grafo, int iOrigem);