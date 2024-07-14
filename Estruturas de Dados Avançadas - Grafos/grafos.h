#pragma once

/* DEPENDENCIAS EXTERNAS */
#include <stdio.h>
#include <stdlib.h>

/* DEFINICOES AUXILIARES */
#define TRUE 1
#define FALSE 0
typedef int Peso;

/* ESTRUTURAS */
struct node
{
	int i;
	Peso peso;
	struct node* prox;
};

struct grafo
{
	struct node** vViz; // Vetor de ponteiros para as listas de vizinhancas
	int nVert;					// Numero de vertices
	int nArest;					// Numero de arestas
};

struct aresta
{
	int i;							// Se aresta for dirigida, representa a origem
	int j;							// Se aresta for dirigida, representa o destino
	Peso peso;
};

typedef struct node No;
typedef struct grafo Grafo;
typedef struct aresta Aresta;

/* FUNCOES */
Grafo* criaGrafo(int nVert);
void printaMonografo(Grafo* grafo);
void printaDigrafo(Grafo* grafo);
void printaArestas(Aresta* vArestas, int nArest);
void ordenaArestas(Aresta* vArestas, int nArest);
void adicionaArestaMonografo(Grafo* grafo, Aresta* arestas, int nArest);
void adicionaArestaDigrafo(Grafo* grafo, Aresta* arestas, int nArest);
void liberaGrafo(Grafo* grafo);