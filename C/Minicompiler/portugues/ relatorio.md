**Integrantes:**

- Pedro de Almeida Barizon - Matricula: 2211350 - Turma: 3WA
- Guilherme Riechert Senko - Matricula: 2011478 - Turma: 3WA

**Nota:** 9.8

# RELATÓRIO DE IMPLEMENTACAO: COMPILADOR PARA A LINGUAGEM BÁSICA LINB INF1018 - Software Básico (2024.1)

## Introdução

Este relatório descreve o desenvolvimento e a implementação de um compilador para a linguagem básica LinB,
conforme especificado no enunciado do segundo trabalho da disciplina de Software Básico. O objetivo principal
foi criar uma função `CompilaLinB` em C, capaz de ler um código fonte LinB de um arquivo de texto, traduzir para
código de máquina x86-64 e armazená-lo em uma área de memória fornecida como parâmetro.

## Descrição da Linguagem LinB

A linguagem LinB é uma linguagem de programação simples que suporta operações aritméticas básicas, atribuições,
desvios condicionais e retorno de função. As instruções da linguagem são:

- Atribuição: `varp '<=' expr`
- Operação aritmética: `varpc op varpc`
- Desvio: `'if' varp num`
- Retorno: `'ret'`

Variáveis locais são representadas como `v1`, `v2` etc., e parâmetros como `p1`, `p2`. Constantes inteiras são prefixadas com `$`.

## Estrutura da Função CompilaLinB

```
typedef int (*funcp) ();
funcp CompilaLinB (FILE *f, unsigned char codigo[]);
```

O parâmetro `f` é um descritor de arquivo aberto para leitura, e `codigo` é o vetor em que o código de máquina será armazenado.
O código gerado segue as convenções de C/Linux para passagem de parâmetros, valor de retorno e salvamento de registradores.

## Metodologia

A função CompilaLinB foi desenvolvida utilizando as seguintes etapas:

1. **Leitura do Código LinB:** A função lê o código fonte LinB linha por linha do arquivo fornecido.
2. **Tradução para Código de Máquina:** Cada linha é analisada e traduzida para uma instrução de máquina correspondente.
3. **Armazenamento:** O código de máquina gerado é armazenado na área de memória fornecida pelo segundo parâmetro da função.
4. **Retorno de Ponteiro:** A função retorna um ponteiro para função que aponta para o início do código de máquina gerado.

## Implementação e Testes

A implementação foi feita seguindo os passos abaixo:

1. **Prólogo e Epílogo:** Implementação do prólogo e epílogo padrão para preparação (construção da pilha) e finalização (`ret`) da função.
2. **Atribuições:** Implementação das atribuições de variáveis locais e parâmetros.
3. **Operações Aritméticas:** Tradução das operações aritméticas básicas (+, -, \*).
4. **Desvios Condicionais:** Implementação da instrução `if` para desvios baseados em condições.

### Exemplo de Teste

```
Função LinB:
v1 <= p1 + $1
ret

Tradução para código de máquina:
movl 	%edi, %eax 	/* Move p1 (primeiro operando) para %eax */
movl	$1, %ecx	/* Move $1 (segundo operando) para %ecx */
addl 	%ecx, %eax	/* Realiza operacao */
movl	%eax, -4(%rbp)	/* Move resultado para v1 */

/* Termino do programa */
movl	-4(%rbp), %eax	/* Move v1 para %eax, porque v1 eh variavel de retorno em LinB, assim como %eax eh o registrador de retorno de Assembly */
leave
ret
```

## Resultados dos Testes

Todos os testes de funções LinB realizados obtiveram êxito, incluindo-se operações aritméticas simples, desvios
condicionais e funções recursivas. Abaixo, seguem exemplos de funções traduzidas e compiladas com sucesso:

```
1. Soma Simples:
   v1 <= p1 + $1
   ret
```

```
2. Fatorial:
   v1 <= $0 + $1          	// 1: f = 1
   if p1 4           		// 2: if (n != 0) desvia para linha 4
   ret                   	// 3: retorna f
   v1 <= v1 * p1          	// 4: f = f * n
   p1 <= p1 - $1          	// 5: n = n - 1
   if p1 4               	// 6: if (n != 0) desvia para linha 4
   ret                   	// 7: retorna f
```

```
3. Diferença de Quadrados:
   v1 <= p1 + p2          	// 1: i = x + y
   p1 <= p1 - p2          	// 2: x = x - y
   v1 <= v1 * p1          	// 3: i = i * x
   ret                   	// 4: retorna i
```

```
4. Soma de Quadrados de 1 a x:
   v1 <= $0 + $0          	// 1: sum = 0
   v2 <= $1 + $0          	// 2: i = 1
   p1 <= p1 + $1          	// 3: x = x + 1
   v3 <= p1 - v2          	// 4: aux = x - i  (será zero se x == i)
   if v3 7               	// 5: if (aux != 0) desvia para linha 7
   ret                   	// 6: retorna sum
   v3 <= v2 * v2          	// 7: aux = i * i
   v1 <= v1 + v3          	// 8: sum = sum + aux
   v2 <= v2 + $1          	// 9: i = i + 1
   if v2 4               	// 10: if (i != 0) desvia para linha 4 (sempre desviará)
```

## Conclusão

A função `CompilaLinB` foi desenvolvida com sucesso, cumprindo todos os requisitos estabelecidos no enunciado do trabalho.
A tradução das instruções LinB para código de máquina x86-64 foi realizada de forma eficiente, e os testes realizados demonstraram
a funcionalidade correta do compilador. Este projeto proporcionou uma experiência valiosa na construção de um microcompilador e no
entendimento profundo das traduções de alto nível para baixo nível.

## Instruções de Compilação

Para compilar o código, utilize o comando:

`gcc -Wall -Wa,--execstack -c compilalinb.c`

ou

`gcc -Wall -Wa,--execstack -o programa.exe principal.c compilalinb.c`

O primeiro comando apenas gerará um .obj, que poderá ser amarrado a outros objetos para gerar um executável. O segundo comando gerará
o executável `programa.exe` a partir da compilação e da amarração de `principal.c`, que contém a main, com `compilalinb.c`.

Executado qualquer desses comandos, o compilador gcc não exibirá qualquer mensagem de saída, de maneira a indicar que a compilação
foi bem sucedida.
