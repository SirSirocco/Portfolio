/* Pedro de Almeida Barizon 2211350 3WA */
/* Guilherme Riechert Senko 2011478 3WA */

/* HEADER */

#include "codifica.h"


/* DEFINES */

#define I_MAX 31 // Indice maximo do vetor com as structs compactadoras, que corresponde a posicao do EOT (End of Transmission)


/* FUNCOES */

/* fdump */
// -> Faz o dump do arquivo binario passado como argumento;
// -> Supoe que o arquivo esteja aberto em modo escrita;
// -> Nao fecha o arquivo nem faz rewind.

void fdump(FILE* arqBin)
{
    unsigned char* buffer = (unsigned char*)malloc(1 * sizeof(unsigned char));

    if (!buffer)
    {
        printf("\n\nErro: Memoria insuficiente. Impossivel fazer dump do arquivo.\n\n");
        return;
    }

    while (1)
    {
        fread(buffer, 1, 1, arqBin);

        if (feof(arqBin))
            break;

        printf("%02X ", *buffer);
    }

    free(buffer);
}


/* buscaCh */
// -> Retorna o ponteiro para a estrutura cujo campo "simbolo" tem o valor do
//    caractere buscado;
// -> Supoe que o vetor passado como argumento esteja nao vazio (do contrario,
//    havera erro de derreferenciacao de ponteiro nulo ou de segmentation fault);
// -> Funcao estatica por possuir carater auxiliar a funcao "compacta".

static struct compactadora* buscaCh(char ch, struct compactadora* v)
{
    while (v->simbolo != ch)
        v++;

    return v;
}


/* compacta */
// -> Compacta o arquivo texto ao usar os codigos passados por meio do
//    vetor de structs compactadoras, escrevendo o resultado no arquivo binario;
// -> Supoe que o arquivo texto esteja aberto em modo de leitura ("r") ou equivalente;
// -> Supoe que o arquivo binario esteja aberto em modo de escrita binaria ("wb") ou equivalente;
// -> Nao fecha os arquivos nem faz rewind;
// -> Supoe que o ultimo elemento do vetor seja referente ao EOT (End of Transmission);
// -> Obs.: O algoritmo e valido para codigos de qualquer tamanho.

void compacta(FILE* arqTexto, FILE* arqBin, struct compactadora* v)
{
    unsigned char buffer = 0;
    int
        count = 0, // := bits ja copiados no buffer
        ch,
        tam,       // := numero de bits do conteudo a ser copiado ao buffer
        subtam;    // := numero de bits da leva menor do conteudo
    struct compactadora* ptr;

    while ((ch = fgetc(arqTexto)) != EOF)
    {
        if (ch == 13) // Se ch == CR (Carriage Return), reinicia o loop, para que \n não seja contado como dois caracteres (CR + LF)
            continue;

        ptr = buscaCh(ch, v);
        tam = ptr->tamanho;

        if ((tam + count) <= 8) // Se buffer ainda suportar o que sera copiado
        {
            buffer |= (ptr->codigo << (8 - count - tam)); // Vai preenchendo buffer do bit mais significativo para o menos

            count += tam;

            if (count == 8) // Se buffer cheio, escreve no arquivo binario e esvazia
            {
                fwrite(&buffer, 1, 1, arqBin);
                count = 0;
                buffer = 0;
            }
        }

        else // Caso em que buffer nao suporta todo o conteudo a ser copiado
        {
            // Solucao: copiar em levas menores

            while (tam > 0) // Enquanto houver algo a ser copiado
            {
                // subtam corresponde ao tamanho da leva menor
                // subtam sera o minimo dentre o que ainda deve ser copiado e o quanto o buffer ainda pode receber

                subtam = (tam < 8 - count) ? tam : (8 - count);
                tam -= subtam;

                buffer |= ((ptr->codigo >> tam) << (8 - count - subtam));

                count += subtam;

                if (count == 8)
                {
                    fwrite(&buffer, 1, 1, arqBin);
                    count = 0;
                    buffer = 0;
                }
            }
        }
    }

    // Escrita do EOT (processo identico ao visto acima)

    ptr = v + I_MAX;
    tam = ptr->tamanho;

    if ((tam + count) <= 8)
        buffer |= (ptr->codigo << (8 - count - tam));

    else
    {
        while (tam > 0)
        {
            subtam = (tam < 8 - count) ? tam : (8 - count);
            tam -= subtam;

            buffer |= ((ptr->codigo >> tam) << (8 - count - subtam));

            count += subtam;

            if (count == 8)
            {
                fwrite(&buffer, 1, 1, arqBin);

                if (!tam) // Se os 8 ultimos bits a serem escritos preencherem perfeitamente o buffer, nao ha necessidade de esvazia-lo ao fim da funcao em #
                    return;

                count = 0;
                buffer = 0;
            }
        }
    }

    fwrite(&buffer, 1, 1, arqBin); // # Esvazia o buffer pela ultima vez, mesmo que nao tenha sido antes preenchido totalmente
}


/* pot */
// -> Retorna b elevado a exp;
// -> Usada para a construcao da mascara de bits em funcao do tamanho do 
//    buffer na "descompacta";
// -> Funcao estatica por possuir carater auxiliar a funcao "descompacta".

static unsigned int pot(unsigned int b, unsigned int exp)
{
    if (!exp)
        return 1;

    return b * pot(b, exp - 1);
}


/* buscaCod */
// -> Retorna o ponteiro para a estrutura cujos campos "codigo" e "tamanho" tenham
//    os valores procurados;
// -> Se nao encontrar, retorna NULL;
// -> Supoe que o vetor passado como argumento esteja nao vazio (do contrario,
//    havera erro de derreferenciacao de ponteiro nulo ou de segmentation fault);
// -> Funcao estatica por possuir carater auxiliar a funcao "descompacta".

static struct compactadora* buscaCod(unsigned int cod, unsigned int tam, struct compactadora* v)
{
    int pos = 0;

    while (pos <= I_MAX && (v[pos].codigo != cod || v[pos].tamanho != tam))
        pos++;

    if (pos > I_MAX)
        return NULL;

    return v + pos;
}


/* little2big */
// -> Mapeia um inteiro sem sinal da forma 0x78563412 em 0x12345678;
// -> O nome alude as convencoes Little e Big-Endian, porque o objetivo
//    da funcao e atuar no buffer para que seja preenchido do byte mais
//    significativo para o menos (Big), ao contrario do que faz a "fwrite" (Little);
// -> Funcao estatica por possuir carater auxiliar a funcao "descompacta".

unsigned int little2big(unsigned int ui)
{
    return (ui << 24) | (ui >> 24) | ((ui & 0x0000FF00) << 8) | ((ui & 0x00FF0000) >> 8);
}


/* descompacta */
// -> Descompacta o arquivo binario ao usar os codigos passados por meio do
//    vetor de structs compactadoras, escrevendo o resultado no arquivo texto;
// -> Supoe que o arquivo binario esteja aberto em modo de leitura binaria ("rb") ou equivalente;
// -> Supoe que o arquivo texto esteja aberto em modo de escrita ("w") ou equivalente;
// -> Nao fecha os arquivos nem faz rewind;
// -> Supoe que o ultimo elemento do vetor seja referente ao EOT (End of Transmission);
// -> Supoe que os codigos sao livres de prefixos;
// -> Supoe que os codigos tem tamanho maximo de 32 bits (isto e, o tamanho do buffer);
// -> Obs.: Se desejar aumentar para 64 bits, declare um buffer do tipo unsigned long long
//    e faca as modificacoes correspondentes na little2big. O resto do codigo nao demandara
//    modificacoes, por estar em funcao de sizeof(buffer).

void descompacta(FILE* arqBin, FILE* arqTexto, struct compactadora* v)
{
    unsigned int
        buffer = 0,
        size = sizeof(buffer),
        mask = pot(2, 8 * size - 1), // := Mascara para capturar apenas o bit mais significativo do buffer
        tam = 0,
        countBuffer = 0,
        cod;
    struct compactadora* vAtual;

    fread(&buffer, 1, size, arqBin);
    buffer = little2big(buffer); // Faz o buffer ser preenchido do bit mais significativo para o menos


    while (1)
    {
        cod = (buffer & mask) >> (8 * size - 1); // Captura o bit mais significativo e atribui seu valor absoluto (0 ou 1) a cod
        tam = 1;

        buffer <<= 1; // Remove bit do buffer, abrindo espaco a partir do bit menos significativo. Ex. com 4 bits: 1011 -> 0110
        countBuffer++;

        if (countBuffer == 8) // Quando um byte ficar vago, le do arquivo binario
        {
            fread(&buffer, 1, 1, arqBin);
            countBuffer = 0;
        }

        while (!(vAtual = buscaCod(cod, tam, v))) // Enquanto nao achar codigo, ou seja, enquanto vAtual == NULL
        {
            cod = (cod << 1) | ((buffer & mask) >> (8 * size - 1)); // Shifta o codigo 1 bit para a esquerda e copia o proximo bit vindo do buffer para o espaco deixado  
            tam++;

            buffer <<= 1;
            countBuffer++;

            if (countBuffer == 8)
            {
                fread(&buffer, 1, 1, arqBin);
                countBuffer = 0;
            }
        }

        if (vAtual == (v + I_MAX))
            break;

        fprintf(arqTexto, "%c", vAtual->simbolo);
    }

    return;
}