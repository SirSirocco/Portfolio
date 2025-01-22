**Members:**

- Pedro de Almeida Barizon - Registration: 2211350 - Class: 3WA
- Guilherme Riechert Senko - Registration: 2011478 - Class: 3WA

**Grade:** 9.8

# IMPLEMENTATION REPORT: COMPILER FOR THE LINB BASIC LANGUAGE INF1018 - Basic Software (2024.1)

## Introduction

This report describes the development and implementation of a compiler for the basic LinB language, as specified in the statement of the second assignment for the Basic Software course. The main objective was to create a `CompilaLinB` function in C, capable of reading LinB source code from a text file, translating it into x86-64 machine code, and storing it in a memory area provided as a parameter.

## Description of the LinB Language

The LinB language is a simple programming language that supports basic arithmetic operations, assignments, conditional jumps, and function returns. The language's instructions are as follows:

- Assignment: `varp '<=' expr`
- Arithmetic operation: `varpc op varpc`
- Jump: `'if' varp num`
- Return: `'ret'`

Local variables are represented as `v1`, `v2`, etc., and parameters as `p1`, `p2`. Integer constants are prefixed with `$`.

## Structure of the CompilaLinB Function

```c
typedef int (*funcp) ();
funcp CompilaLinB (FILE *f, unsigned char codigo[]);
```

The parameter `f` is a file descriptor opened for reading, and `codigo` is the vector where the machine code will be stored. The generated code follows C/Linux conventions for parameter passing, return value, and register saving.

## Methodology

The `CompilaLinB` function was developed using the following steps:

1. **Reading LinB Code:** The function reads the LinB source code line by line from the provided file.
2. **Translation to Machine Code:** Each line is analyzed and translated into a corresponding machine instruction.
3. **Storage:** The generated machine code is stored in the memory area provided by the function's second parameter.
4. **Pointer Return:** The function returns a function pointer that points to the start of the generated machine code.

## Implementation and Tests

The implementation followed these steps:

1. **Prologue and Epilogue:** Implementation of standard prologue and epilogue for preparation (stack setup) and termination (`ret`) of the function.
2. **Assignments:** Implementation of local variable and parameter assignments.
3. **Arithmetic Operations:** Translation of basic arithmetic operations (+, -, \*).
4. **Conditional Jumps:** Implementation of the `if` instruction for condition-based jumps.

### Test Example

```c
LinB Function:
v1 <= p1 + $1
ret

Translation to machine code:
movl    %edi, %eax    /* Move p1 (first operand) to %eax */
movl    $1, %ecx      /* Move $1 (second operand) to %ecx */
addl    %ecx, %eax    /* Perform operation */
movl    %eax, -4(%rbp) /* Move result to v1 */

/* Program end */
movl    -4(%rbp), %eax /* Move v1 to %eax, as v1 is the return variable in LinB, and %eax is the return register in Assembly */
leave
ret
```

## Test Results

All tests of LinB functions were successful, including simple arithmetic operations, conditional jumps, and recursive functions. Below are examples of functions successfully translated and compiled:

```c
1. Simple Sum:
   v1 <= p1 + $1
   ret
```

```c
2. Factorial:
   v1 <= $0 + $1            // 1: f = 1
   if p1 4                  // 2: if (n != 0) jump to line 4
   ret                      // 3: return f
   v1 <= v1 * p1            // 4: f = f * n
   p1 <= p1 - $1            // 5: n = n - 1
   if p1 4                  // 6: if (n != 0) jump to line 4
   ret                      // 7: return f
```

```c
3. Difference of Squares:
   v1 <= p1 + p2            // 1: i = x + y
   p1 <= p1 - p2            // 2: x = x - y
   v1 <= v1 * p1            // 3: i = i * x
   ret                      // 4: return i
```

```c
4. Sum of Squares from 1 to x:
   v1 <= $0 + $0            // 1: sum = 0
   v2 <= $1 + $0            // 2: i = 1
   p1 <= p1 + $1            // 3: x = x + 1
   v3 <= p1 - v2            // 4: aux = x - i (will be zero if x == i)
   if v3 7                  // 5: if (aux != 0) jump to line 7
   ret                      // 6: return sum
   v3 <= v2 * v2            // 7: aux = i * i
   v1 <= v1 + v3            // 8: sum = sum + aux
   v2 <= v2 + $1            // 9: i = i + 1
   if v2 4                  // 10: if (i != 0) jump to line 4 (will always jump)
```

## Conclusion

The `CompilaLinB` function was successfully developed, meeting all the requirements outlined in the assignment. The translation of LinB instructions to x86-64 machine code was performed efficiently, and the conducted tests demonstrated the correct functionality of the compiler. This project provided valuable experience in building a microcompiler and deeply understanding high-to-low-level translations.

## Compilation Instructions

To compile the code, use the command:

`gcc -Wall -Wa,--execstack -c compilalinb.c`

or

`gcc -Wall -Wa,--execstack -o programa.exe principal.c compilalinb.c`

The first command will generate a .obj file, which can be linked to other objects to create an executable. The second command will generate the executable `programa.exe` from the compilation and linking of `principal.c`, which contains the main function, with `compilalinb.c`.

Executing any of these commands, the gcc compiler will not display any output messages, indicating that the compilation was successful.
