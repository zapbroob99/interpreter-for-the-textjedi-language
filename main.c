#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
int main() {
    FILE* file = fopen("source.tj", "r");
    if (file == NULL) {
        printf("Failed to open file.\n");
        return 1;
    }
    // Get the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);
    // Allocate memory to hold the source code
    char* source_code = malloc(file_size + 1);
    if (source_code == NULL) {
        printf("Failed to allocate memory.\n");
        fclose(file);
        return 1;
    }
    // Read the source code from the file
    size_t read_size = fread(source_code, 1, file_size, file);
    source_code[read_size] = '\0';

    // Close the file
    fclose(file);
    Token* tokens = tokenize(source_code);
    printf("\nStarting Lexical Analysis\n\n");
    printTokens(tokens);
    free(tokens);
}
