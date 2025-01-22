#pragma once

/* DEPENDENCIAS EXTERNAS */
#include <stdio.h>
#include <stdlib.h>
#include "grafos.h"

/* ESTRUTURAS */
struct noConj           // Singleton
{
    int chave;
    struct noConj* rep; // Representante do conjunto
};

typedef struct noConj NoConj;

/* FUNCOES */
void iniciaVertices(NoConj* vVertices, int nVert);
void printaVertices(NoConj* vVertices, int nVert);
void fusion(int num1, int num2, NoConj* vVertices, int nVert);
NoConj* find(int num1, NoConj* vVertices);
Grafo* criaArvMinima(Aresta* vArestas, int nArest, NoConj* vVertices, int nVert);