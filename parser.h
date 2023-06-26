#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "symbol_table.h"

void error(const char* message);
void parse(Token* tokens);

#endif
