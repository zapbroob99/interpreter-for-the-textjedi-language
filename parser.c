#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#define TEXT_TYPE "text"


void error(const char* message) {
    printf("Parser error: %s\n", message);
    exit(1);
}


void parseExpression(Token* tokens, int* index, SymbolTable* symbolTable);

void parsePrimary(Token* tokens, int* index, SymbolTable* symbolTable) {
    Token token = tokens[*index];

    if (token.type == INTEGER_CONSTANT || token.type == STRING_CONSTANT) {
        if (*index > 0 && tokens[*index - 1].type == KEYWORD && strcmp(tokens[*index - 1].lexeme, "output") == 0) {
            printf("Output: %s\n", token.lexeme);
        }
        (*index)++;
    } else if (token.type == IDENTIFIER) {
        SymbolEntry* entry = lookupEntry(symbolTable, token.lexeme);
        if (entry == NULL) {
            error("Undefined variable.");
        } else if (strcmp(entry->type, TEXT_TYPE) == 0) {
            printf("Output: %s\n", token.lexeme);
        } else {
            error("Invalid use of non-integer variable in expression.");
        }
        (*index)++;
    } else {
        error("Unexpected token in primary expression.");
    }
}

void parseUnary(Token* tokens, int* index, SymbolTable* symbolTable) {
    Token token = tokens[*index];

    if (token.lexeme[0] == '+' || token.lexeme[0] == '-') {
        (*index)++;
    }

    parsePrimary(tokens, index, symbolTable);
}

void parseTerm(Token* tokens, int* index, SymbolTable* symbolTable) {
    parseUnary(tokens, index, symbolTable);

    Token token = tokens[*index];

    while (strcmp(token.lexeme, "*") == 0 || strcmp(token.lexeme, "/") == 0) {
        (*index)++;
        parseUnary(tokens, index, symbolTable);
        token = tokens[*index];
    }
}

void parseExpression(Token* tokens, int* index, SymbolTable* symbolTable) {
    parseTerm(tokens, index, symbolTable);

    Token token = tokens[*index];

    while (strcmp(token.lexeme, "+") == 0 || strcmp(token.lexeme, "-") == 0) {
        (*index)++;
        parseTerm(tokens, index, symbolTable);
        token = tokens[*index];
    }
}

void trim(char *str) {
    // Trim leading whitespace
    int start = 0;
    while (str[start] == ' ' || str[start] == '\t' || str[start] == '\n') {
        start++;
    }

    // Trim trailing whitespace
    int end = strlen(str) - 1;
    while (end >= start && (str[end] == ' ' || str[end] == '\t' || str[end] == '\n')) {
        end--;
    }

    // Shift characters to the left
    int i;
    for (i = start; i <= end; i++) {
        str[i - start] = str[i];
    }
    str[i - start] = '\0';
}

void parseAssignment(Token* tokens, int* index, SymbolTable* symbolTable) {
    Token token = tokens[*index];

    if (token.type == IDENTIFIER) {
        char identifier[31];
        strcpy(identifier, token.lexeme);
        (*index)++;
        token = tokens[*index];

        if (strcmp(token.lexeme, ":=") == 0) {
            (*index)++;
            int startIndex = *index;  // Starting index of the expression

            // Find the end of the expression
            while (tokens[*index].type != END_OF_LINE && strcmp(tokens[*index].lexeme, ";") != 0) {
                (*index)++;
            }

            // Construct the assignment expression
            char expression[100] = "";
            for (int i = startIndex; i < *index; i++) {
                strcat(expression, tokens[i].lexeme);
            }

            SymbolEntry* entry = lookupEntry(symbolTable, identifier);


            if (entry == NULL) {
                error("Undefined variable.");
            }




            else if (strcmp(entry->type, INTEGER_TYPE) == 0) {
                // Check if assigning to an integer variable
                int value;
                if (tokens[startIndex].type == INTEGER_CONSTANT) {
                    value = atoi(expression);
                } else if (tokens[startIndex].type == IDENTIFIER) {
                    SymbolEntry* identifierEntry = lookupEntry(symbolTable, tokens[startIndex].lexeme);
                    if (identifierEntry == NULL || strcmp(identifierEntry->type, INTEGER_TYPE) != 0) {
                        error("Invalid assignment to integer variable. Expected integer constant or integer variable.");
                    }
                    value = atoi(identifierEntry->value);
                } else {
                    error("Invalid assignment to integer variable. Expected integer constant or integer variable.");
                }
                // Handle addition and subtraction
                for (int i = startIndex + 1; i < *index; i += 2) {
                    if (strcmp(tokens[i].lexeme, "+") == 0) {
                        if (tokens[i + 1].type == INTEGER_CONSTANT) {
                            value += atoi(tokens[i + 1].lexeme);
                        } else if (tokens[i + 1].type == IDENTIFIER) {
                            SymbolEntry* identifierEntry = lookupEntry(symbolTable, tokens[i + 1].lexeme);
                            if (identifierEntry == NULL || strcmp(identifierEntry->type, INTEGER_TYPE) != 0) {
                                error("Invalid addition. Expected integer constant or integer variable.");
                            }
                            value += atoi(identifierEntry->value);
                        } else {
                            error("Invalid addition. Expected integer constant or integer variable.");
                        }
                    } else if (strcmp(tokens[i].lexeme, "-") == 0) {
                        if (tokens[i + 1].type == INTEGER_CONSTANT) {
                            value -= atoi(tokens[i + 1].lexeme);
                        } else if (tokens[i + 1].type == IDENTIFIER) {
                            SymbolEntry* identifierEntry = lookupEntry(symbolTable, tokens[i + 1].lexeme);
                            if (identifierEntry == NULL || strcmp(identifierEntry->type, INTEGER_TYPE) != 0) {
                                error("Invalid subtraction. Expected integer constant or integer variable.");
                            }
                            value -= atoi(identifierEntry->value);
                        } else {
                            error("Invalid subtraction. Expected integer constant or integer variable.");
                        }
                    } else {
                        error("Invalid operation. Expected '+' or '-' for addition or subtraction.");
                    }
                }

                sprintf(entry->value, "%d", value);
                printf("Assignment: %s := %d\n", identifier, value);
            } else if (strcmp(entry->type, TEXT_TYPE) == 0) {
                // Check if assigning to a text variable
                if (tokens[startIndex].type == STRING_CONSTANT) {
                    // Regular assignment with string constant
                    strcpy(entry->value, expression);
                } else if (tokens[startIndex].type == IDENTIFIER) {
                    // Assignment with another text variable
                    SymbolEntry* identifierEntry = lookupEntry(symbolTable, tokens[startIndex].lexeme);
                    if (identifierEntry == NULL || strcmp(identifierEntry->type, TEXT_TYPE) != 0) {
                        error("Invalid assignment to text variable. Expected string constant or text variable.");
                    }
                    strcpy(entry->value, identifierEntry->value);
                } else {
                    error("Invalid assignment to text variable. Expected string constant or text variable.");
                }

                // Check if subtraction operation is present
                char* subOperator = strstr(expression, "-");
                if (subOperator != NULL) {
                    // Extract the Minuend and Subtrahend
                    char minuend[100] = "";
                    char subtrahend[100] = "";
                    strncpy(minuend, expression, subOperator - expression);
                    strcpy(subtrahend, subOperator + 1);

                    // Remove leading and trailing whitespace from both strings
                    trim(minuend);
                    trim(subtrahend);

                    // Perform the subtraction operation
                    char* result = strstr(minuend, subtrahend);
                    if (result != NULL) {
                        // Remove the Subtrahend from the Minuend
                        strncpy(result, result + strlen(subtrahend), strlen(result + strlen(subtrahend)) + 1);
                        strcpy(entry->value, minuend);
                    } else {
                        // Substring not found, assign only the Minuend
                        strcpy(entry->value, minuend);
                    }
                }
                printf("Assignment: %s := %s\n", identifier, entry->value);
            }


            else {
                error("Invalid assignment to non-integer variable.");
            }
        } else {
            error("Expected assignment operator ':='.");
        }
    } else {
        error("Expected identifier in assignment statement.");
    }
}






void parseVariableDeclaration(Token* tokens, int* index, SymbolTable* symbolTable) {
    Token token = tokens[*index];

    if (strcmp(token.lexeme, "new") == 0) {
        (*index)++;
        token = tokens[*index];

        if (token.type == KEYWORD) {
            char keyword[31];
            strcpy(keyword, token.lexeme);
            (*index)++;
            token = tokens[*index];

            if (token.type == IDENTIFIER) {
                insertEntry(symbolTable, token.lexeme, keyword,token.lexeme);
                // Create DeclarationNode
                printf("Variable Declaration: %s %s\n", keyword, token.lexeme);
                (*index)++;
            } else {
                error("Expected identifier in variable declaration statement.");
            }
        } else {
            error("Expected keyword 'int' or 'text' in variable declaration statement.");
        }
    } else {
        error("Expected 'new' keyword in variable declaration statement.");
    }
}

void parseInputStatement(Token* tokens, int* index, SymbolTable* symbolTable) {
    Token token = tokens[*index];

    if (strcmp(token.lexeme, "input") == 0) {
        (*index)++;
        token = tokens[*index];

        if (token.type == IDENTIFIER) {
            SymbolEntry* entry = lookupEntry(symbolTable, token.lexeme);
            if (entry == NULL) {
                error("Undefined variable.");
            } else if (strcmp(entry->type, TEXT_TYPE) == 0) {
                char promptText[101];
                (*index)++;
                token = tokens[*index];

                if (strcmp(token.lexeme, "prompt") == 0) {
                    (*index)++;
                    token = tokens[*index];

                    if (token.type == STRING_CONSTANT) {
                        strncpy(promptText, token.lexeme, sizeof(promptText) - 1);
                        promptText[sizeof(promptText) - 1] = '\0';

                        printf("%s: ", promptText);
                        fgets(entry->value, sizeof(entry->value), stdin);
                        entry->value[strcspn(entry->value, "\n")] = '\0';  // Remove trailing newline
                        printf("Assigned %s := %s\n", entry->name, entry->value);  // Print assignment statement

                        (*index)++;
                    } else {
                        error("Expected string constant after 'prompt' keyword.");
                    }
                } else {
                    error("Expected 'prompt' keyword after identifier in input statement.");
                }
            } else {
                error("Invalid use of non-text variable in input statement.");
            }
        } else {
            error("Expected identifier in input statement.");
        }
    } else {
        error("Expected 'input' keyword in input statement.");
    }
}



void parseOutputStatement(Token* tokens, int* index, SymbolTable* symbolTable) {
    Token token = tokens[*index];

    if (strcmp(token.lexeme, "output") == 0) {
        (*index)++;
        token = tokens[*index];

        if (token.type == IDENTIFIER) {
            SymbolEntry* entry = lookupEntry(symbolTable, token.lexeme);
            if (entry == NULL) {
                error("Undefined variable.");
            } else if (strcmp(entry->type, TEXT_TYPE) == 0) {
                printf("Output: %s\n", entry->value);  // Print the value from the symbol table entry
            } else {
                error("Invalid use of non-text variable in output statement.");
            }
            (*index)++;
        } else if (token.type == STRING_CONSTANT) {
            printf("Output: %s\n", token.lexeme);
            (*index)++;
        } else {
            error("Expected identifier or string constant in output statement.");
        }
    } else {
        error("Expected 'output' keyword in output statement.");
    }
}


void parseWriteStatement(Token* tokens, int* index, SymbolTable* symbolTable) {
    Token token = tokens[*index];

    if (strcmp(token.lexeme, "write") == 0) {
        (*index)++;
        token = tokens[*index];

        if (token.type == STRING_CONSTANT || token.type == IDENTIFIER) {
            char* fileName = tokens[*index + 2].lexeme;  // The lexeme of the identifier after "to"
            FILE* file = fopen(fileName, "w");
            if (file == NULL) {
                error("Failed to open file for writing.");
            }

            if (token.type == STRING_CONSTANT) {
                fprintf(file, "%s\n", token.lexeme);
                printf("Writing Successfull\n");
            } else {
                SymbolEntry* entry = lookupEntry(symbolTable, token.lexeme);
                if (entry == NULL) {
                    error("Undefined variable.");
                } else if (strcmp(entry->type, TEXT_TYPE) == 0) {
                    fprintf(file, "%s\n", entry->value);
                    printf("Writing Successfull\n");
                } else {
                    error("Invalid use of non-text variable in write statement.");
                }
            }

            fclose(file);

            (*index) += 3;  // Skip the string constant or identifier and "to" keyword
        } else {
            error("Expected string constant or identifier in write statement.");
        }
    } else {
        error("Expected 'write' keyword in write statement.");
    }
}



void parseSizeFunction(Token* tokens, int* index, SymbolTable* symbolTable) {
    Token token = tokens[*index];

    if (strcmp(token.lexeme, "size") == 0) {
        (*index)++;
        token = tokens[*index];

        if (token.type == BRACKET && token.lexeme[0] == '(') {
            (*index)++;
            token = tokens[*index];

            if (token.type == IDENTIFIER) {
                SymbolEntry* entry = lookupEntry(symbolTable, token.lexeme);
                if (entry == NULL) {
                    error("Undefined variable.");
                } else if (strcmp(entry->type, TEXT_TYPE) == 0) {
                    printf("Size of %s: %d\n", token.lexeme, strlen(entry->value));
                } else {
                    error("Invalid use of non-text variable in size function.");
                }
                (*index)++;
            } else {
                error("Expected identifier as argument in size function.");
            }

            token = tokens[*index];
            if (token.type == BRACKET && token.lexeme[0] == ')') {
                (*index)++;
                return;
            } else {
                error("Expected closing parenthesis in size function.");
            }
        } else {
            error("Expected opening parenthesis in size function.");
        }
    } else {
        error("Expected 'size' keyword in size function.");
    }
}


char* parseSubsFunction(Token* tokens, int* index, SymbolTable* symbolTable) {
    Token token = tokens[*index];

    if (strcmp(token.lexeme, "subs") == 0) {
        (*index)++;
        token = tokens[*index];

        if (strcmp(token.lexeme, "(") == 0) {
            (*index)++;
            token = tokens[*index];

            if (token.type == IDENTIFIER) {
                char identifier[31];
                strcpy(identifier, token.lexeme);
                (*index)++;
                token = tokens[*index];

                if (strcmp(token.lexeme, ",") == 0) {
                    (*index)++;
                    token = tokens[*index];

                    if (token.type == INTEGER_CONSTANT) {
                        int arg2 = atoi(token.lexeme);
                        (*index)++;
                        token = tokens[*index];

                        if (strcmp(token.lexeme, ",") == 0) {
                            (*index)++;
                            token = tokens[*index];

                            if (token.type == INTEGER_CONSTANT) {
                                int arg3 = atoi(token.lexeme);
                                (*index)++;
                                token = tokens[*index];

                                if (strcmp(token.lexeme, ")") == 0) {
                                    (*index)++;
                                    SymbolEntry* entry = lookupEntry(symbolTable, identifier);
                                    if (entry == NULL || strcmp(entry->type, TEXT_TYPE) != 0) {
                                        error("Invalid subs function. Expected text variable.");
                                    }

                                    char* text = entry->value;
                                    int textLength = strlen(text);
                                    if (arg2 < 0 || arg2 >= textLength || arg3 < 0 || arg3 > textLength) {
                                        error("Invalid subs function arguments. Out of range.");
                                    }

                                    // Create a substring of the text
                                    char* substring = malloc((arg3 - arg2 + 1) * sizeof(char));
                                    strncpy(substring, text + arg2, arg3 - arg2);
                                    substring[arg3 - arg2] = '\0';

                                    printf("Subs function: %s := subs(%s, %d, %d)\n", identifier, identifier, arg2, arg3);
                                    printf("Substring: %s\n", substring);

                                    return substring;
                                } else {
                                    error("Expected closing parenthesis ')'.");
                                }
                            } else {
                                error("Invalid subs function argument. Expected integer constant.");
                            }
                        } else {
                            error("Expected comma ','.");
                        }
                    } else {
                        error("Invalid subs function argument. Expected integer constant.");
                    }
                } else {
                    error("Expected comma ','.");
                }
            } else {
                error("Invalid subs function argument. Expected identifier.");
            }
        } else {
            error("Expected opening parenthesis '('.");
        }
    } else {
        error("Expected identifier for subs function.");
    }

    return NULL; // Return NULL in case of an error
}



void parseAsStringFunction(Token* tokens, int* index, SymbolTable* symbolTable) {
    Token token = tokens[*index];

    if (token.type == IDENTIFIER) {
        char identifier[31];
        strcpy(identifier, token.lexeme);
        (*index)++;
        token = tokens[*index];

        if (strcmp(token.lexeme, "(") == 0) {
            (*index)++;
            token = tokens[*index];

            if (token.type == IDENTIFIER) {
                char argIdentifier[31];
                strcpy(argIdentifier, token.lexeme);
                (*index)++;
                token = tokens[*index];

                if (strcmp(token.lexeme, ")") == 0) {
                    (*index)++;

                    SymbolEntry* entry = lookupEntry(symbolTable, identifier);
                    if (entry == NULL) {
                        error("Undefined variable.");
                    } else if (strcmp(entry->type, TEXT_TYPE) == 0) {
                        SymbolEntry* argEntry = lookupEntry(symbolTable, argIdentifier);
                        if (argEntry == NULL || strcmp(argEntry->type, INTEGER_TYPE) != 0) {
                            error("Invalid argument. Expected integer variable.");
                        }

                        sprintf(entry->value, "%d", atoi(argEntry->value));
                        printf("As String function: %s := %s\n", identifier, entry->value);
                    } else {
                        error("Invalid conversion. Expected text variable.");
                    }
                } else {
                    error("Expected closing parenthesis ')'.");
                }
            } else {
                error("Expected identifier for function argument.");
            }
        } else {
            error("Expected opening parenthesis '('.");
        }
    } else {
        error("Expected identifier for function call.");
    }
}


void parseStatement(Token* tokens, int* index, SymbolTable* symbolTable) {
    Token token = tokens[*index];

    if (token.type == KEYWORD) {
        if (strcmp(token.lexeme, "new") == 0) {
            parseVariableDeclaration(tokens, index, symbolTable);
        } else if (strcmp(token.lexeme, "input") == 0) {
            parseInputStatement(tokens, index, symbolTable);
        } else if (strcmp(token.lexeme, "output") == 0) {
            parseOutputStatement(tokens, index, symbolTable);
        } else if(strcmp(token.lexeme, "size")==0){
            parseSizeFunction(tokens, index, symbolTable);
        } else if(strcmp(token.lexeme, "write")==0){
            parseWriteStatement(tokens,index,symbolTable);
        }

        else {
            error("Invalid keyword in statement.");
        }
    } else if (token.type == IDENTIFIER) {
        parseAssignment(tokens, index, symbolTable);
    } else {
        error("Invalid token in statement.");
    }

    token = tokens[*index];

    if (strcmp(token.lexeme, ";") == 0) {
        (*index)++;
    } else {
        error("Expected ';' at the end of the statement.");
    }
}

void parse(Token* tokens) {
    SymbolTable* symbolTable = createSymbolTable();

    int index = 0;

    while (tokens[index].type != END_OF_LINE) {
        parseStatement(tokens, &index, symbolTable);
    }

    destroySymbolTable(symbolTable);
}

void trim(char* str);
