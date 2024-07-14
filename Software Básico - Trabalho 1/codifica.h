#ifndef __CODIFICA__
#define __CODIFICA__

#include <stdio.h>

struct compactadora { 
    char simbolo;             
    unsigned int codigo; 
    int tamanho; 
};

void compacta(FILE *arqTexto, FILE *arqBin, struct compactadora *v);
void descompacta(FILE *arqBin, FILE *arqTexto, struct compactadora *v);

#endif

// Esta parte foi separada para diferencia-la da secao acima, que ja tinha sido fornecida no enunciado do trabalho

/* INCLUDES */
#include <stdlib.h>

/* FUNCOES */
void fdump(FILE* arqBin);