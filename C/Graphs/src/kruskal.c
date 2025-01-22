#include "kruskal.h"


/* iniciaVertices */
/*
Objetivo:
    Inicializa os singletons que representam os vertices do grafo.
Acoplamento:
    1. vVertices: endereco inicial do vetor de singletons previamente criado.
    2. nVert:     tamanho do vetor de singletons.
*/
void iniciaVertices(NoConj* vVertices, int nVert)
{
    for (int i = 0; i < nVert; i ++)
    {
        (vVertices + i)->chave = i;
        (vVertices + i)->rep = (vVertices + i);
    }
}


/* printaVertices */
void printaVertices(NoConj* vVertices, int nVert)
{
    for (int i = 0; i < nVert; i ++)
    {
        printf("chave: %d, endereço: %p, rep: %p, chave rep: %d\n", (vVertices + i)->chave, (vVertices + i), (vVertices + i)->rep, (vVertices + i)->rep->chave);
    }
}


/* fusion */
/*
Objetivo:
    Faz a uniao por altura do conjunto de vertices que contem o indice num1 com o que contem num2.
Acoplamento:
    1. num1:      representa tanto o indice do elemento, quanto quanto o proprio elemento que sera
                  procurado o representante.
    2. num2:      representa tanto o indice do elemento, quanto quanto o proprio elemento que sera
                  procurado o representante.
    3. vVertices: endereco inicial do vetor que contem os conjuntos.
    4. nVert:     tamanho do vetor de conjuntos.
    
*/
void fusion(int num1, int num2, NoConj* vVertices, int nVert)
{
    int cont1 = 0, cont2 = 0;
    for (int i = 0; i < nVert; i++)
    {
        // Procura o represenante dos elementos
        if ((vVertices + i)->rep == (vVertices + num1)->rep)
        {
            cont1++;
        }

        else if ((vVertices+i)->rep == (vVertices + num2)->rep)
        {
            cont2++;
        }        
    }

    // Verifica as alturas
    if (cont1 > cont2)
    {
        (vVertices+num2)->rep = (vVertices+num1)->rep;
    }
    else
    {
        (vVertices+num1)->rep = (vVertices+num2)->rep;
    }
}


/* find */
/*
Objetivo:
    Retorna o endereco do representante do vertice associado ao indice num1.
Acoplamento:
    1. num1: indice do elemento em vVertices cujo representante se quer buscar.
    2. vVertices: vetor de singletons.
Funcionamento:
    Realiza o colapsamento de trajetoria.
*/
NoConj* find(int num1, NoConj* vVertices)
{
    if ((vVertices + num1)->rep == (vVertices+num1)) // Se elemento igual ao seu representante, retorna endereco do elemento
        return (vVertices + num1);
    // Do contrario, representante recebe representante do representante atual (necessario para colapsar trajetoria)
    (vVertices+num1)->rep = find((vVertices+num1)->rep->chave, vVertices);
    return (vVertices+num1)->rep;
}


/* criaArvMin */
/*
Objetivo:
    Preenche o grafo com a arvore geradora minima.
Acoplamento:
    1. vArestas:  vetor de arestas.
    2. nArest:    numero de elementos do vetor de arestas.
    3. vVertices: vetor que ontem os singletons.
    4. nVert:     tamanho do vetor de singletons.
*/
Grafo* criaArvMinima(Aresta* vArestas, int nArest, NoConj* vVertices, int nVert)
{
    Grafo* arvMin = criaGrafo(nVert);
    int maior = 0, menor = 0, aux = 0;
    Aresta aresta;
    for (int i = 0; i < nArest; i++)
    {
        maior = vArestas[i].i;
        menor = vArestas[i].j;
        // Se eles nao estao no mesmo conjunto, ou seja, fazem parte de florestas diferentes
        if (find(maior, vVertices) != find(menor, vVertices))
        {
            if (menor > maior)
            {
                aux = menor;
                menor = maior;
                maior = aux;
            }

            aresta.i = maior;
            aresta.j = menor;
            aresta.peso = vArestas[i].peso;
            
            adicionaArestaMonografo(arvMin, &aresta, 1);          

            // Junta os elementos em um conjunto so 
            fusion(maior, menor, vVertices, nVert);                
        }
    }
    
    return arvMin;
}