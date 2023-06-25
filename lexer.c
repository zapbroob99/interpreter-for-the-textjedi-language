#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "lexer.h"

Token* tokenize(const char* code) {
    Token* tokens = malloc(sizeof(Token) * 100);
    int tokenCount = 0;
    int codeLength = strlen(code);
    int i = 0;
    int openBracketsCount = 0;

    while (i < codeLength) {
        // Skip whitespaces
        if (code[i] == ' ' || code[i] == '\t' || code[i] == '\n') {
            i++;
            continue;
        }

        // Check for keywords and identifiers
        if (isalpha(code[i])) {
            int j = 0;
            while (isalnum(code[i]) || code[i] == '_') {
                tokens[tokenCount].lexeme[j] = code[i];
                i++;
                j++;
                if (j >= 30) {
                    printf("Lexical error: Identifier exceeds maximum size.\n");
                    free(tokens);
                    return NULL;
                }
            }
            tokens[tokenCount].lexeme[j] = '\0';
            if (strcmp(tokens[tokenCount].lexeme, "new") == 0 || strcmp(tokens[tokenCount].lexeme, "int") == 0 ||
                strcmp(tokens[tokenCount].lexeme, "text") == 0 || strcmp(tokens[tokenCount].lexeme, "size") == 0 ||
                strcmp(tokens[tokenCount].lexeme, "subs") == 0 || strcmp(tokens[tokenCount].lexeme, "locate") == 0 ||
                strcmp(tokens[tokenCount].lexeme, "insert") == 0 || strcmp(tokens[tokenCount].lexeme, "override") == 0 ||
                strcmp(tokens[tokenCount].lexeme, "read") == 0 || strcmp(tokens[tokenCount].lexeme, "write") == 0 ||
                strcmp(tokens[tokenCount].lexeme, "from") == 0 || strcmp(tokens[tokenCount].lexeme, "to") == 0 ||
                strcmp(tokens[tokenCount].lexeme, "input") == 0 || strcmp(tokens[tokenCount].lexeme, "output") == 0 ||
                strcmp(tokens[tokenCount].lexeme, "asText") == 0 || strcmp(tokens[tokenCount].lexeme, "asString") == 0) {
                tokens[tokenCount].type = KEYWORD;
            } else {
                tokens[tokenCount].type = IDENTIFIER;
            }
            tokenCount++;
            continue;
        }

        // Check for integer constants
        if (isdigit(code[i])) {
            int j = 0;
            while (isdigit(code[i])) {
                tokens[tokenCount].lexeme[j] = code[i];
                i++;
                j++;
                if (j >= 30) {
                    printf("Lexical error: Integer constant exceeds maximum size.\n");
                    free(tokens);
                    return NULL;
                }
            }
            tokens[tokenCount].lexeme[j] = '\0';
            tokens[tokenCount].type = INTEGER_CONSTANT;
            tokenCount++;
            continue;
        }

        // Check for string constants
        if (code[i] == '\"') {
            int j = 0;
            i++;
            while (code[i] != '\"' && i < codeLength) {
                if (j >= 30) {
                    printf("Lexical error: String constant exceeds maximum size.\n");
                    free(tokens);
                    return NULL;
                }
                tokens[tokenCount].lexeme[j] = code[i];
                i++;
                j++;
            }
            tokens[tokenCount].lexeme[j] = '\0';
            if (code[i] == '\"') {
                tokens[tokenCount].type = STRING_CONSTANT;
            } else {
                printf("Lexical error: String constant not terminated.\n");
                free(tokens);
                return NULL;
            }
            tokenCount++;
            i++;
            continue;
        }

        // Check for operators and end of line mark
        if (code[i] == '+' || code[i] == '-' || (code[i] == ':' && code[i + 1] == '=')) {
            if (code[i] == ':') {
                tokens[tokenCount].lexeme[0] = code[i];
                tokens[tokenCount].lexeme[1] = code[i + 1];
                tokens[tokenCount].lexeme[2] = '\0';
                i += 2;

            } else {
                tokens[tokenCount].lexeme[0] = code[i];
                tokens[tokenCount].lexeme[1] = '\0';
                i++;
            }
            tokens[tokenCount].type = OPERATOR;
            tokenCount++;
            continue;
        }
        // Check for brackets
        if (code[i] == '(' || code[i] == ')') {
            tokens[tokenCount].lexeme[0] = code[i];
            tokens[tokenCount].lexeme[1] = '\0';
            tokens[tokenCount].type = BRACKET;
            tokenCount++;

            if (code[i] == '(') {
                openBracketsCount++;
            } else if (code[i] == ')') {
                if (openBracketsCount <= 0) {
                    printf("Lexical error: Unterminated bracket ')'\n");
                    free(tokens);
                    return NULL;
                }
                openBracketsCount--;
            }

            i++;
            continue;
        }


        // Check for end of line mark
        if (code[i] == ';') {
            tokens[tokenCount].lexeme[0] = code[i];
            tokens[tokenCount].lexeme[1] = '\0';
            tokens[tokenCount].type = END_OF_LINE;
            tokenCount++;
            i++;
            continue;
        }

        // Check for comments
        if (code[i] == '/' && code[i + 1] == '*') {
            i += 2;
            while (!(code[i] == '*' && code[i + 1] == '/')) {
                i++;
                if (i >= codeLength) {
                    printf("Lexical error: Comment not terminated.\n");
                    free(tokens);
                    return NULL;
                }
            }
            i += 2;
            continue;
        }

        // Invalid character
        printf("Lexical error: Invalid character '%c'.\n", code[i]);
        free(tokens);
        return NULL;
    }
    // Check for unterminated brackets
    if (openBracketsCount > 0) {
        printf("Lexical error: Unterminated bracket \n");
        free(tokens);
        return NULL;
    }

    tokens[tokenCount].lexeme[0] = '\0';
    tokens[tokenCount].type = END_OF_LINE;
    tokenCount++;

    return tokens;
}

void printTokenType(TokenType type) {
    switch (type) {
        case KEYWORD:
            printf("Type: Keyword\n");
            break;
        case IDENTIFIER:
            printf("Type: Identifier\n");
            break;
        case INTEGER_CONSTANT:
            printf("Type: Integer Constant\n");
            break;
        case STRING_CONSTANT:
            printf("Type: String Constant\n");
            break;
        case OPERATOR:
            printf("Type: Operator\n");
            break;
        case END_OF_LINE:
            printf("Type: End of Line\n");
            break;
        case COMMENT:
            printf("Type: Comment\n");
            break;
        case BRACKET:
            printf("Type: Bracket\n");
            break;
        default:
            printf("Type: Unknown\n");
            break;

    }
}

void printTokens(Token* tokens) {
    int i = 0;
    while (tokens[i].lexeme[0] != '\0') {
        printf("Lexeme: %s, ", tokens[i].lexeme);
        printTokenType(tokens[i].type);
        i++;
    }
}