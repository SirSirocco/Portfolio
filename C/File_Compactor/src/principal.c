#include <stdlib.h>
#include "codifica.h"

int main(void)
{
    /* DECLARACAO DE VARIAVEIS */

    struct compactadora vCompactadora[] =
    {
        {'K',1,12},
        {'W',2,12},
        {'Y',3,12},
        {'Z',2,11},
        {'J',3,11},
        {'X',1,9},
        {'\n',1,8},
        {'H',1,7},
        {'F',36,7},
        {'G',37,7},
        {'B',1,6},
        {'Q',16,6},
        {',',17,6},
        {'V',19,6},
        {'.',49,6},
        {'P',48,6},
        {'D',1,5},
        {'C',17,5},
        {'O',16,5},
        {'M',25,5},
        {'R',1,4},
        {'L',7,4},
        {'N',6,4},
        {'A',5,4},
        {'T',9,4},
        {'S',14,4},
        {'U',15,4},
        {'I',13,4},
        {'E',1,3},
        {' ',5,3},
        {0, 0, 0},
        {4,0,12}
    };

    FILE* arqTexto = fopen("texto_in.txt", "r"),
        * arqBin = fopen("binario.bin", "wb");

    if (!arqTexto) exit(1);

    if (!arqBin) exit(2);

    /* COMPACTACAO */

    compacta(arqTexto, arqBin, vCompactadora);

    fclose(arqTexto);
    fclose(arqBin);

    /* DUMPING */

    if (!(arqBin = fopen("binario.bin", "rb"))) exit(3);
    if (!(arqTexto = fopen("texto_out.txt", "w"))) exit(4);

    fdump(arqBin);

    /* DESCOMPACTACAO */

    rewind(arqBin);

    descompacta(arqBin, arqTexto, vCompactadora);

    fclose(arqTexto);
    fclose(arqBin);

    return 0;
}