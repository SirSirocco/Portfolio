#include <stdio.h>
#include <stdlib.h>
#include "compilalinb.h"

#define TAM 1024

void dump(void* p, int n);

int main (void)
{
    unsigned char codigo[TAM];
    int num, p1 = 5, p2 = 5;
    FILE* f = fopen("script.linb", "r");
    
    if (!f)
        exit(EXIT_FAILURE);
    
    for (int i = 0; i < TAM; i++)
        codigo[i] = 0;

    funcp fptr = CompilaLinB(f, codigo);
    num = (*fptr)(p1, p2);
    printf("Retorno da funcao compilada: %d\n", num);

    fclose(f);

    return 0;
}

void dump(void* p, int n)
{
    unsigned char* q = (unsigned char*)p;

    while (n--)
    {
        printf("%p: %02x\n", q, *q);
        q++;
    }
}