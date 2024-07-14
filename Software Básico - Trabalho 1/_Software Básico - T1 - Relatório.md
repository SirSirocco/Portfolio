**Integrantes:**

* Pedro de Almeida Barizon - 2211350 - 3WA
* Guilherme Riechert Senko - 2011478 - 3WA

**Nota:** 9.8

# RELATÓRIO DE IMPLEMENTAÇÃO DE COMPACTAÇÃO E DESCOMPACTAÇÃO USANDO CÓDIGOS LIVRES DE PREFIXO – INF1018

## INTRODUÇÃO

Este trabalho foi desenvolvido para a disciplina de Software Básico com o objetivo de implementar um sistema de compactação e descompactação de texto usando códigos livres de prefixo. Este método permite uma compressão de dados sem perda utilizando códigos de comprimento variável — cuja eficiência sobressai especialmente em contextos em que certos símbolos aparecem com mais frequência do que outros.

## METODOLOGIA

A estrutura principal utilizada foi a compactadora, que mapeia cada símbolo a um código binário e ao tamanho desse código.

```
struct compactadora {
    char simbolo;
    unsigned int codigo;
    int tamanho;
};
```

## FUNÇÕES IMPLEMENTADAS

* `compacta(FILE *arqTexto, FILE *arqBin, struct compactadora *v)`: Compacta o texto lido de arqTexto usando a tabela fornecida em forma de vetor e grava o resultado binário em arqBin.
* `descompacta(FILE *arqBin, FILE *arqTexto, struct compactadora *v)`: Lê o binário de arqBin e reconstrói o texto original em arqTexto usando a tabela fornecida.

## TESTES IMPLEMENTADOS

Para assegurar a eficácia e a eficiência das funções de compactação e descompactação implementadas, realizamos uma série de testes rigorosos. Cada teste foi projetado para avaliar aspectos específicos da funcionalidade e robustez das operações.

### 1. Teste de Compactação Básica

* **Objetivo:** Verificar a capacidade da função compacta de transformar corretamente um texto simples em sua forma binária usando o código fornecido no enunciado.
* **Procedimento:** Utilizamos uma string definida como "A, B, C, ..., Z\n" para representar uma sequência simples e previsível.
* **Verificação:** Após a compactação, fizemos um dump do arquivo binário resultante e comparamos manualmente este output com a saída esperada segundo a tabela fornecida no enunciado.

### 2. Teste de Descompactação e Integridade

* **Objetivo:** Assegurar que a função descompacta pode reverter o processo de compactação, restaurando o texto original a partir do binário.
* **Procedimento:** Após a compactação dos dados, utilizamos a função descompacta para recriar o texto a partir do binário gerado e comparar com o texto original.
* **Verificação:** A saída de texto foi verificada para corresponder exatamente ao texto de entrada, garantindo a integridade dos dados.

### 3. Testes com Diferentes Padrões de Texto

* **Objetivo:** Testar a robustez das funções com diferentes padrões de texto que incluem repetições e espaços, como em "\n\nAABBCCDD...,,\n\n".
* **Procedimento:** Compactamos e descompactamos este texto complexo para verificar se os códigos sem prefixos tratam corretamente os símbolos e espaços repetidos.
* **Verificação:** Analisamos o texto descompactado para confirmar que todos os caracteres e símbolos estavam corretamente posicionados e representados.

### 4. Teste com Arquivos de Texto Reais

* **Objetivo:** Avaliar o desempenho das funções em um cenário de uso mais realista, usando arquivos de texto de maior porte.
* **Procedimento:** Utilizamos arquivos de texto como o "BOM\nDIA\n" e um texto mais longo de 30 linhas para testar a compactação e descompactação.
* **Verificação:** O foco estava em verificar a eficiência da compactação em termos de tamanho do arquivo e a precisão da descompactação.

### 5. Teste de Conformidade com o Enunciado

* **Objetivo:** Garantir que as implementações seguem as especificações dadas no enunciado do trabalho, utilizando os códigos e estruturas fornecidos.
* **Procedimento:** Seguimos estritamente as instruções e o exemplo de código do enunciado para desenvolver e testar as funções.
* **Verificação:** A implementação foi revisada para assegurar que todas as especificações do enunciado foram cumpridas, incluindo a maneira correta de abertura e manuseio dos arquivos.

### 6. Teste com Tabela Qualquer

* **Objetivo:** Garantir que as funções operam com sucesso independentemente da tabela de códigos livres de prefixo fornecida.
* **Procedimento:** Definimos uma tabela diferente da fornecida pelo enunciado. Em seguida, compactamos "LUKE, EU SOU SEU PAI\n", vindo de um arquivo de texto, e fizemos um dump do binário gerado. Logo após, descompactamo-lo, escrevendo o resultado em um outro arquivo de texto.
* **Verificação:** Verificamos tanto que o dump correspondia ao esperado quanto que o conteúdo de ambos os arquivos de texto era idêntico, o que atestou o bom desempenho das funções envolvidas.
* **Observação**: O método empregado para definir os códigos sem prefixos foi dos mais simples: consideramos a árvore de prefixos mais desbalanceada possível, de modo que todo nó à esquerda fosse uma folha. Dessa forma, foram gerados os códigos 0, 10, 110, 1110, e assim sucessivamente, que correspondem às somas 2^(n + 1) – 2, com n natural. Tal propriedade facilitou a criação da tabela de compactação.

Estes testes, todos exitosos, foram essenciais para confirmar que tanto a compactação quanto a descompactação funcionam de maneira confiável e eficiente, tratando diferentes tipos de entrada e garantindo a integridade dos dados após o processo.

## CONCLUSÃO

A implementação do algoritmo para compactação e descompactação de textos em C alcançou sucesso, atendendo a todos os requisitos e objetivos estabelecidos pelo enunciado do trabalho. Os resultados obtidos nas diversas fases de teste demonstram a eficiência do algoritmo em compressão de dados sem perda, validando a escolha de técnicas de codificação livre de prefixo para este propósito.
