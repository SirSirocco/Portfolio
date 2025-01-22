**Team Members:**

- Pedro de Almeida Barizon - 2211350 - 3WA
- Guilherme Riechert Senko - 2011478 - 3WA

**Grade:** 9.8

# IMPLEMENTATION REPORT: TEXT COMPRESSION AND DECOMPRESSION USING PREFIX-FREE CODES – INF1018

## INTRODUCTION

This project was developed for the Basic Software course with the goal of implementing a text compression and decompression system using prefix-free codes. This method enables lossless data compression using variable-length codes, particularly efficient in contexts where certain symbols appear more frequently than others.

## METHODOLOGY

The main structure used was the `compactadora`, which maps each symbol to a binary code and its length.

```c
struct compactadora {
    char simbolo;
    unsigned int codigo;
    int tamanho;
};
```

## IMPLEMENTED FUNCTIONS

- `compacta(FILE *arqTexto, FILE *arqBin, struct compactadora *v)`: Compresses the text read from `arqTexto` using the provided table (in vector form) and writes the binary result to `arqBin`.
- `descompacta(FILE *arqBin, FILE *arqTexto, struct compactadora *v)`: Reads the binary data from `arqBin` and reconstructs the original text in `arqTexto` using the provided table.

## IMPLEMENTED TESTS

To ensure the effectiveness and efficiency of the implemented compression and decompression functions, we conducted a series of rigorous tests. Each test was designed to evaluate specific aspects of the functionality and robustness of the operations.

### 1. Basic Compression Test

- **Objective:** Verify the ability of the `compacta` function to correctly transform simple text into its binary form using the code provided in the assignment.
- **Procedure:** A predefined string "A, B, C, ..., Z\n" was used to represent a simple and predictable sequence.
- **Verification:** After compression, the resulting binary file was dumped and manually compared to the expected output according to the table provided in the assignment.

### 2. Decompression and Integrity Test

- **Objective:** Ensure that the `descompacta` function can reverse the compression process, restoring the original text from the binary data.
- **Procedure:** After data compression, the `descompacta` function was used to recreate the text from the generated binary and compare it to the original text.
- **Verification:** The decompressed text was verified to match the input text exactly, ensuring data integrity.

### 3. Tests with Different Text Patterns

- **Objective:** Test the robustness of the functions with different text patterns, including repetitions and spaces, such as "\n\nAABBCCDD...,\n\n".
- **Procedure:** This complex text was compressed and decompressed to verify that prefix-free codes correctly handled repeated symbols and spaces.
- **Verification:** The decompressed text was analyzed to confirm that all characters and symbols were correctly positioned and represented.

### 4. Real Text File Tests

- **Objective:** Evaluate the performance of the functions in a more realistic usage scenario using larger text files.
- **Procedure:** Text files like "BOM\nDIA\n" and a longer text with 30 lines were used to test compression and decompression.
- **Verification:** The focus was on verifying the compression efficiency in terms of file size and the accuracy of the decompression.

### 5. Compliance Test with the Assignment

- **Objective:** Ensure the implementations follow the specifications outlined in the assignment, using the provided codes and structures.
- **Procedure:** The instructions and example code from the assignment were strictly followed during development and testing.
- **Verification:** The implementation was reviewed to ensure that all the specifications were met, including the correct way to open and handle files.

### 6. Test with Custom Table

- **Objective:** Ensure that the functions operate successfully regardless of the prefix-free code table provided.
- **Procedure:** A table different from the one provided in the assignment was defined. The text "LUKE, I AM YOUR FATHER\n" from a text file was compressed, and the resulting binary was dumped. It was then decompressed, and the result was written to another text file.
- **Verification:** Both the binary dump and the content of the text files were verified to ensure that the functions performed as expected.
- **Observation:** The method used to define the prefix-free codes was straightforward: the most unbalanced prefix tree possible was considered, with every left node being a leaf. This generated codes like 0, 10, 110, 1110, and so on, corresponding to sums of 2^(n + 1) – 2, with n as a natural number. This property simplified the creation of the compression table.

These successful tests confirmed that both compression and decompression work reliably and efficiently, handling various input types and ensuring data integrity throughout the process.

## CONCLUSION

The implementation of the text compression and decompression algorithm in C was successful, meeting all the requirements and objectives outlined in the assignment. The results obtained during the various testing phases demonstrate the algorithm's efficiency in lossless data compression, validating the use of prefix-free encoding techniques for this purpose.
