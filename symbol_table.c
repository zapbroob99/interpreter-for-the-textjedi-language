#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

SymbolTable* createSymbolTable() {
    SymbolTable* table = malloc(sizeof(SymbolTable));


    initializeSymbolTable(table);
    return table;
}


void initializeSymbolTable(SymbolTable* table) {
    table->entries = NULL;
    table->size = 0;
    table->capacity = 0;
}

void destroySymbolTable(SymbolTable* table) {
    free(table->entries);
    table->entries = NULL;
    table->size = 0;
    table->capacity = 0;
}

void insertEntry(SymbolTable* table, const char* name, const char* type,const char* value) {
    SymbolEntry entry;
    strcpy(entry.name, name);
    strcpy(entry.type, type);
    strcpy(entry.value, value);  // Assign the value to the entry

    if (table->size >= table->capacity) {
        // Expand the symbol table capacity if needed
        int newCapacity = table->capacity == 0 ? 1 : table->capacity * 2;
        SymbolEntry* newEntries = (SymbolEntry*)realloc(table->entries, newCapacity * sizeof(SymbolEntry));
        if (newEntries == NULL) {
            printf("Error: Failed to allocate memory for symbol table.\n");
            exit(1);
        }
        table->entries = newEntries;
        table->capacity = newCapacity;
    }

    table->entries[table->size++] = entry;
}

SymbolEntry* lookupEntry(SymbolTable* table, const char* name) {
    for (int i = 0; i < table->size; i++) {
        if (strcmp(table->entries[i].name, name) == 0) {
            return &table->entries[i];
        }
    }
    return NULL;
}
