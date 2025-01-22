/* Pedro de Almeida Barizon 2211350 3WA */
/* Guilherme Riechert Senko 2011478 3WA */


/*** HEADER ***/
#include "compilalinb.h"


/*** DEPENDENCIAS EXTERNAS ***/
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


/*** DEFINICOES AUXILIARES ***/
#define MAX_LIN 64 // 50 + margem de seguranca
#define TRUE 1
#define FALSE 0
#define SIZE sizeof(int)


/*** ESTRUTURAS ***/
struct jmp_pendente
{
    int posAtual;
    int linhaDestino;
    struct jmp_pendente* prox;
};

typedef struct jmp_pendente JmpPendente; // Representa jump a ser resolvido na amarracao


/*** TABELA DE COMANDOS DE MAQUINA ***/

/** NOMENCLATURA **/
// Receptor <= varp1 op varp2

/** CORRESPONDENCIA ASSEMBLY-LinB **/
/*
    eax          := varp1 e aux
    ecx          := varp2
    edi          := p1
    esi          := p2
    -4*i(%rbp)   := vi, com i >= 1
*/

/** TAMANHOS **/
#define LEAVE           1
#define MOV_RSP_RBP     3
#define PUSH_RBP        1
#define RET             1
#define SUB_RSP_16      4

#define JMP             6

#define CMP             3

#define MOV_AUX_P       2
#define MOV_AUX_V       6

#define MOV_C_AUX       5
#define MOV_P_AUX       2
#define MOV_V_AUX       6

#define MOV_C_ECX       5
#define MOV_P_ECX       2
#define MOV_V_ECX       6

#define ADD_ECX         2
#define SUB_ECX         2
#define MUL_ECX         3

/** COMANDOS **/

    /* Entrada e Saida */
unsigned char
    leave[LEAVE]                = { 0xc9 },
    mov_rsp_rbp[MOV_RSP_RBP]    = { 0x48, 0x89, 0xe5 },
    push_rbp[PUSH_RBP]          = { 0x55 },
    ret[RET]                    = { 0xc3 },
    sub_rsp_16[SUB_RSP_16]      = { 0x48, 0x83, 0xec, 0x10 },

    /* Salto */
    jne[JMP]                    = { 0x0f, 0x85, 0x00, 0x00, 0x00, 0x00 },

    /* Comparacao (com zero) */
    cmp_aux[CMP]                = { 0x83, 0xf8, 0x00 },
    cmp_p1[CMP]                 = { 0x83, 0xff, 0x00 },
    cmp_p2[CMP]                 = { 0x83, 0xfe, 0x00 },

    /* Movimentacao (MOV_ORIGEM_DESTINO) */
    mov_aux_p1[MOV_AUX_P]       = { 0x89, 0xc7 },
    mov_aux_p2[MOV_AUX_P]       = { 0x89, 0xc6 },
    mov_aux_v[MOV_AUX_V]        = { 0x89, 0x85, 0x00, 0x00, 0x00, 0x00 },

    mov_c_aux[MOV_C_AUX]        = { 0xb8, 0x00, 0x00, 0x00, 0x00 },
    mov_p1_aux[MOV_P_AUX]       = { 0x89, 0xf8 },
    mov_p2_aux[MOV_P_AUX]       = { 0x89, 0xf0 },
    mov_v_aux[MOV_V_AUX]        = { 0x8b, 0x85, 0x00, 0x00, 0x00, 0x00 },

    mov_c_ecx[MOV_C_ECX]        = { 0xb9, 0x00, 0x00, 0x00, 0x00 },
    mov_p1_ecx[MOV_P_ECX]       = { 0x89, 0xf9 },
    mov_p2_ecx[MOV_P_ECX]       = { 0x89, 0xf1 },
    mov_v_ecx[MOV_V_ECX]        = { 0x8b, 0x8d, 0x00, 0x00, 0x00, 0x00 },

    /* Adicao (ADD_ORIGEM_DESTINO) */
    add_ecx_aux[ADD_ECX]        = { 0x01, 0xc8 },

    /* Subtracao (SUB_ORIGEM_DESTINO) */
    sub_ecx_aux[SUB_ECX]        = { 0x29, 0xc8 },

    /* Multiplicacao (IMUL_ORIGEM_DESTINO) */
    mul_ecx_aux[MUL_ECX]        = { 0x0f, 0xaf, 0xc1 };


/*** FUNCOES ***/

/** error **/
/*
    Objetivo:   Exibir mensagem de erro em caso de falha na compilacao feita pela CompilaLinB e
                interromper o programa com "exit".
    Acoplamento:
                1. msg:     string (read only) que indica a mensagem de erro a ser exibida.
                2. line:    numero da linha em que surgiu o erro.
*/
static void error(const char* msg, int line)
{
    fprintf(stderr, "Erro %s na linha %d\n", msg, line);
    exit(EXIT_FAILURE);
}


/** leAteFinalLinha **/
/*
    Objetivo:   Ler ate final da linha (inclusive o '\n') ou fim do arquivo.
    Acoplamento:
                1. f: ponteiro para arquivo a ser lido.
    Hipoteses: 
                1. Supoe que o arquivo ja esteja aberto em modo que permita leitura.
    Restricoes:
                1. Nao fecha o arquivo.
*/
static void leAteFinalLinha(FILE* f)
{
    int ch;
    do
    {
        ch = fgetc(f);
    } while (ch != '\n' && ch != EOF);
}


/** escreveInstr **/
/*
    Objetivo:   Escrever instrucao no array de bytes "codigo" a partir da posicao apontada por "pos" (*pos).
    Acoplamento:
                1. instr:       endereco inicial do array que guarda o codigo de maquina a ser escrito.
                2. instrTam:    numero de bytes a serem escritos.
                3. codigo:      endereco inicial do array em que serah escrito o codigo de maquina.
                4. pos:         ponteiro para a posicao de "codigo" em que se comecarah a escrever.           
    Funcionamento:
                1. Atualiza o valor de *pos.
*/
static void escreveInstr(unsigned char* instr, int instrTam, unsigned char* codigo, int* pos)
{
    for (int i = 0; i < instrTam; i++)
        codigo[(*pos)++] = instr[i];
}


/** escreveConst **/
/*
    Objetivo:   Escrever instrucao no array de bytes "codigo" a partir da posicao apontada por "pos" (*pos).
    Acoplamento:
                1. cte:         constante a ser escrita.
                2. codigo:      endereco inicial do array em que serah escrito o codigo de maquina.
                3. pos:         ponteiro para a posicao de "codigo" em que se comecarah a escrever.
    Hipoteses: 
                1. Supoe que a maquina seja little endian.
    Funcionamento:
                1. Atualiza o valor de *pos.
*/
static void escreveConst(int cte, unsigned char* codigo, int* pos)
{
    int size = sizeof(int);
    union
    {
        int i;
        char byte[SIZE];
    } u;

    u.i = cte;

    for (int j = 0; j < size; j++)
        codigo[(*pos)++] = u.byte[j];
}


/** escreveInstrComConst **/
/*
    Objetivo:   Encapsular a escreveInstr e a escreveConst, a fim de tornar o codigo mais limpo.
    Acoplamento:
                1. instr:       endereco inicial do array que guarda o codigo de maquina a ser escrito.
                2. instrTam:    numero de bytes a serem escritos.
                3. cte:         constante a ser escrita.
                4. codigo:      endereco inicial do array em que serah escrito o codigo de maquina.
                5. pos:         ponteiro para a posicao de "codigo" em que se comecarah a escrever.
    Hipoteses: 
                1. Supoe que a maquina seja little endian.
    Funcionamento:
                1. Atualiza o valor de *pos.
*/
static void escreveInstrComConst(unsigned char* instr, int instrTam, int cte, unsigned char* codigo, int* pos)
{
    escreveInstr(instr, instrTam - sizeof(int), codigo, pos);
    escreveConst(cte, codigo, pos);
}


/** constroiPilha **/
/*
    Objetivo:   Escrever os comandos para construcao da pilha.
    Acoplamento:
                1. codigo:      endereco inicial do array em que serah escrito o codigo de maquina.
                2. pos:         ponteiro para a posicao de "codigo" em que se comecarah a escrever.
    Funcionamento:
                1. Atualiza o valor de *pos.
                2. Variaveis locais armazenadas na pilha.
*/
static void constroiPilha(unsigned char* codigo, int* pos)
{
    escreveInstr(push_rbp, PUSH_RBP, codigo, pos);
    escreveInstr(mov_rsp_rbp, MOV_RSP_RBP, codigo, pos);
    escreveInstr(sub_rsp_16, SUB_RSP_16, codigo, pos);
}


/** attPrimeiroOperando **/
/*
    Objetivo:   Escrever o comando referente ao primeiro operando da atribuicao.
    Acoplamento:
                1. var:         tipo do operando.
                2. idx:         se "var" nao for '$', representa indice do operando. Do contrario,
                                representa uma constante.
                3. codigo:      endereco inicial do array em que serah escrito o codigo de maquina.
                4. pos:         ponteiro para a posicao de "codigo" em que se comecarah a escrever.
    Funcionamento:
                1. Atualiza o valor de *pos.
*/
static void attPrimeiroOperando(char var, int idx, unsigned char* codigo, int* pos)
{
    switch (var)
    {
        case 'p': /* parametro */
            if (idx == 1)
                escreveInstr(mov_p1_aux, MOV_P_AUX, codigo, pos);
            
            else if (idx == 2)
                escreveInstr(mov_p2_aux, MOV_P_AUX, codigo, pos);
            
            break;
    
        case 'v': /* variavel local */
            escreveInstrComConst(mov_v_aux, MOV_V_AUX, -4 * idx, codigo, pos);
            break;
        
        case '$': /* constante */
            escreveInstrComConst(mov_c_aux, MOV_C_AUX, idx, codigo, pos); // idx farah papel de constante
            break;
    }
}


/** attSegundoOperando **/
/*
    Objetivo:   Escrever o comando referente ao segundo operando da atribuicao.
    Acoplamento:
                1. var:         tipo do operando.
                2. idx:         se "var" nao for '$', representa indice do operando. Do contrario,
                                representa uma constante.
                3. codigo:      endereco inicial do array em que serah escrito o codigo de maquina.
                4. pos:         ponteiro para a posicao de "codigo" em que se comecarah a escrever.
    Funcionamento:
                1. Atualiza o valor de *pos.
*/
static void attSegundoOperando(char var, int idx, unsigned char* codigo, int* pos)
{
    switch (var)
    {
        case 'p': /* parametro */
            if (idx == 1)
                escreveInstr(mov_p1_ecx, MOV_P_ECX, codigo, pos);
            
            else if (idx == 2)
                escreveInstr(mov_p2_ecx, MOV_P_ECX, codigo, pos);
            
            break;
    
        case 'v': /* variavel local */
            escreveInstrComConst(mov_v_ecx, MOV_V_ECX, -4 * idx, codigo, pos);
            break;
        
        case '$': /* constante */
            escreveInstrComConst(mov_c_ecx, MOV_C_ECX, idx, codigo, pos); // idx farah papel de constante
            break;
    }
}


/** attOperacao **/
/*
    Objetivo:   Escrever o comando referente a operacao a ser executada entre os operando da atribuicao.
    Acoplamento:
                1. op:          tipo da operacao.
                3. codigo:      endereco inicial do array em que serah escrito o codigo de maquina.
                4. pos:         ponteiro para a posicao de "codigo" em que se comecarah a escrever.
    Funcionamento:
                1. Atualiza o valor de *pos.
*/
static void attOperacao(char op, unsigned char* codigo, int* pos)
{
    switch (op)
    {
        case '+':
            escreveInstr(add_ecx_aux, ADD_ECX, codigo, pos);
            break;
    
        case '-':
            escreveInstr(sub_ecx_aux, SUB_ECX, codigo, pos);
            break;
        
        case '*':
            escreveInstr(mul_ecx_aux, MUL_ECX, codigo, pos);
            break;
    }
}


/** attReceptor **/
/*
    Objetivo:   Escrever o comando referente ao receptor da atribuicao.
    Acoplamento:
                1. var:         tipo do receptor.
                2. idx:         indice do receptor.
                3. codigo:      endereco inicial do array em que serah escrito o codigo de maquina.
                4. pos:         ponteiro para a posicao de "codigo" em que se comecarah a escrever.
    Funcionamento:
                1. Atualiza o valor de *pos.
*/
static void attReceptor(char var, int idx, unsigned char* codigo, int* pos)
{
    switch (var)
    {
        case 'p': /* parametro */
            if (idx == 1)
                escreveInstr(mov_aux_p1, MOV_AUX_P, codigo, pos);

            else if (idx == 2)
                escreveInstr(mov_aux_p2, MOV_AUX_P, codigo, pos);

            break;
    
        case 'v': /* variavel local */
            escreveInstrComConst(mov_aux_v, MOV_AUX_V, -4 * idx, codigo, pos);
            break;
    }
}


/** atribui **/
/*
    Objetivo:   Encapsular as funcoes com prefixo "att", para deixar o codigo mais limpo.
    Acoplamento:
                1. var:         endereco inicial do vetor de tipos.
                2. idx:         endereco inicial do vetor de indices.
                3. op:          tipo da operacao.
                4. codigo:      endereco inicial do array em que serah escrito o codigo de maquina.
                5. pos:         ponteiro para a posicao de "codigo" em que se comecarah a escrever.
    Funcionamento:
                1. Atualiza o valor de *pos.
*/
static void atribui(char* var, int* idx, char op, unsigned char* codigo, int* pos)
{
    attPrimeiroOperando(var[1], idx[1], codigo, pos);
    attSegundoOperando(var[2], idx[2], codigo, pos);
    attOperacao(op, codigo, pos);
    attReceptor(var[0], idx[0], codigo, pos);
}


/** retorna **/
/*
    Objetivo:   Escrever os comandos para encerramento da funcao sendo escrita.
    Acoplamento:
                1. codigo:      endereco inicial do array em que serah escrito o codigo de maquina.
                2. pos:         ponteiro para a posicao de "codigo" em que se comecarah a escrever.
    Hipoteses:
                1. Em LinB, retorno da funcao eh escrito em v1 (primeira variavel local).
                2. Nas convencoes do Assembly 86x64 da Intel, retorno da funcao eh escrito em %eax (aux).
    Funcionamento:
                1. Atualiza o valor de *pos.
*/
static void retorna(unsigned char* codigo, int* pos)
{
    escreveInstrComConst(mov_v_aux, MOV_V_AUX, -4, codigo, pos);
    escreveInstr(leave, LEAVE, codigo, pos);
    escreveInstr(ret, RET, codigo, pos);
}


/** jmpComparacao **/
/*
    Objetivo:   Escrever o comando de comparacao com zero, que condicionarah o jump.
    Acoplamento:
                1. var:         tipo do operando.
                2. idx:         indice do operando.
                3. codigo:      endereco inicial do array em que serah escrito o codigo de maquina.
                4. pos:         ponteiro para a posicao de "codigo" em que se comecarah a escrever.
    Funcionamento:
                1. Atualiza o valor de *pos.
*/
static void jmpComparacao(char var, int idx, unsigned char* codigo, int* pos)
{
    switch (var)
    {
        case 'p': /* parametro */
            if (idx == 1)
                escreveInstr(cmp_p1, CMP, codigo, pos);

            else if (idx == 2)
                escreveInstr(cmp_p2, CMP, codigo, pos);

            break;
        
        case 'v': /* variavel local */
            escreveInstrComConst(mov_v_aux, MOV_V_AUX, -4 * idx, codigo, pos);
            escreveInstr(cmp_aux, CMP, codigo, pos);
            break;
    }
}


/** geraJmpPendente **/
/*
    Objetivo:   Gera noh do tipo JmpPendente e empilha na pilha de jumps pendentes.
    Acoplamento:
                1. posAtual:        posicao imediatamente depois de onde o salto foi escrito
                                    pelo compilador.
                2. linhaDestino:    numero da linha para a qual saltar.
                3. prox:            ponteiro para o topo da pilha de jumps pendentes.
    Funcionamento:
                1. Se houver erro de memoria, encerra o programa com "exit".
*/
static JmpPendente* geraJmpPendente(int posAtual, int linhaDestino, JmpPendente* prox)
{
    JmpPendente* jmpPend = (JmpPendente*)malloc(sizeof(JmpPendente));

    if (!jmpPend)
        exit(EXIT_FAILURE);
    
    jmpPend->posAtual = posAtual;
    jmpPend->linhaDestino = linhaDestino;
    jmpPend->prox = prox; // Empilha

    return jmpPend;
}


/** salta **/
/*
    Objetivo:   Encapsular as funcoes com prefixo "jmp", para deixar o codigo mais limpo.
    Acoplamento:
                1. var:         tipo do operando.
                2. idx:         indice do operando.
                3. num:         numero da linha para a qual saltar.
                4. prox:        topo da pilha de jumps pendentes. 
                5. codigo:      endereco inicial do array em que serah escrito o codigo de maquina.
                6. pos:         ponteiro para a posicao de "codigo" em que se comecarah a escrever.
    Funcionamento:
                1. Atualiza o valor de *pos.
*/
static JmpPendente* salta(char var, int idx, int num, JmpPendente* prox, unsigned char* codigo, int* pos)
{
    JmpPendente* jmpPend;

    jmpComparacao(var, idx, codigo, pos);
    escreveInstr(jne, JMP, codigo, pos);
    jmpPend = geraJmpPendente(*pos, num, prox); // Empilha e atualiza topo da pilha de jumps pendentes

    return jmpPend;
}


/** resolveJmps **/
/*
    Objetivo:   Amarrar os enderecos dos jumps pendentes e liberar essas estruturas.
    Acoplamento:
                1. jmpPend:     ponteiro para o topo da pilha de jumps pendentes.
                2. lineVector:  endereco inicial do vetor que guarda os offsets em bytes de cada linha.
                3. codigo:      endereco inicial do array em que serah escrito o codigo de maquina.
    Hipoteses:
                1. A linha i eh armazenada na posicao (i - 1) no vetor de linhas, porque a contagem de linhas
                   comeca em 1.
*/
static void resolveJmps(JmpPendente* jmpPend, int* lineVector, unsigned char* codigo)
{
    int delta, pos;
    JmpPendente* aux;

    while (jmpPend)
    {
        delta = lineVector[jmpPend->linhaDestino - 1] - jmpPend->posAtual; // delta = pos_destino - pos_atual
        pos = jmpPend->posAtual - sizeof(int);
        escreveConst(delta, codigo, &pos);

        aux = jmpPend;
        jmpPend = jmpPend->prox;
        free(aux);
    }
}


/** CompilaLinB **/
/*
    Objetivo:   Compilar script em LinB.
    Acoplamento:
                1. f:           ponteiro para arquivo com script em LinB.
                2. codigo:      endereco inicial do array em que serah escrito o codigo de maquina
                                do script compilado.
    Hipoteses: 
                1. Supoe que o arquivo ja esteja aberto em modo que permita leitura.
    Restricoes:
                1. Nao fecha o arquivo.
*/
funcp CompilaLinB(FILE* f, unsigned char codigo[])
{
    int
        ch,
        line = 1,               // Contagem das linhas comeca em 1
        pos = 0,                // Offset de bytes em relacao ao endereco inicial de "codigo"
        num,
        idx[3],
        lineVector[MAX_LIN];    // Vetor que armazena o offset em bytes de cada linha em relacao ao endereco inicial de "codigo"
                                // ( Linha 1 -> lineVector[0] ) e, em geral, ( Linha i -> LineVector[i - 1] )
    char
        op,
        var[3];

    JmpPendente* jmpPend = NULL;

    constroiPilha(codigo, &pos);

    while ((ch = fgetc(f)) != EOF)  // Leitura do primeiro caracter da linha
    {
        lineVector[line - 1] = pos; // Computa offsets de bytes
        switch (ch)                 // Identifica comando
        {
            case 'r': /* retorno */
            {
                if (fscanf(f, "et%c", var + 0) != 1 && !feof(f))
                    error("comando invalido", line);
                retorna(codigo, &pos);
                break;
            }

            case 'v':  /* atribuicao */

            case 'p':
            {
                if (fscanf(f, "%d <= %c%d %c %c%d", idx + 0, var + 1, idx + 1, &op, var + 2, idx + 2) != 6)
                    error("comando invalido", line);
                var[0] = ch;
                atribui(var, idx, op, codigo, &pos);
                break;
            }

            case 'i':  /* desvio */
            {
                if (fscanf(f, "f %c%d %d", var + 0, idx + 0, &num) != 3)
                    error("comando invalido", line);
                jmpPend = salta(var[0], idx[0], num, jmpPend, codigo, &pos);
                break;
            }

            default: error("comando desconhecido", line);
        }

        line++;
        leAteFinalLinha(f);
    }

    resolveJmps(jmpPend, lineVector, codigo);

    return (funcp)codigo;
}