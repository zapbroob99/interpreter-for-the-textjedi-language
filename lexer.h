#ifndef LEXER_H
#define LEXER_H

typedef enum {
    KEYWORD,
    IDENTIFIER,
    INTEGER_CONSTANT,
    STRING_CONSTANT,
    OPERATOR,
    END_OF_LINE,
    COMMENT,
    BRACKET
} TokenType;

typedef struct {
    char lexeme[31];
    TokenType type;
} Token;

Token* tokenize(const char* code);
void printTokenType(TokenType type);
void printTokens(Token* tokens);

#endif /* LEXER_H */