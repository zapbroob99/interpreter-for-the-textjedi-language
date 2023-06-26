#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef struct {
    char name[31];
    char type[10];
    char value[99999];
} SymbolEntry;

typedef struct {
    SymbolEntry* entries;
    int size;
    int capacity;
} SymbolTable;

#define INTEGER_TYPE "int"  // Define INTEGER_TYPE constant
#define TEXT_TYPE "string"  // Define INTEGER_TYPE constant
SymbolTable* createSymbolTable();
void initializeSymbolTable(SymbolTable* table);
void destroySymbolTable(SymbolTable* table);
void insertEntry(SymbolTable* table, const char* name, const char* type,const char* value);
SymbolEntry* lookupEntry(SymbolTable* table, const char* name);

#endif
