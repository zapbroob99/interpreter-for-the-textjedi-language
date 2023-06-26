#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"

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
    if (tokens != NULL) {
       // printf("\nStarting Lexical Analysis\n\n");
        printTokens(tokens);
       printf("\nStarting Parsing...\n\n");
        parse(tokens);
        free(tokens);
    } else {
        printf("Tokenization failed.\n");
    }

    // Free the memory allocated for the source code
    free(source_code);

    return 0;
}
