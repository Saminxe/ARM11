#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define BUFFER_SIZE 512
#define DEFAULT_MAP_SIZE 120
#define PC 15
#define CSPR 16

typedef struct {
  char name[20];
  uint8_t val;
} Symbol;

typedef struct {
  int size;
  Symbol *symbols;
} SymbolTable;


typedef struct {
    uint8_t *memory;
    uint32_t *registers;
} State;


int contains(char *string, char c);
int symtabContains(SymbolTable m, char *k);
void symtabAdd(SymbolTable m, Symbol e);
uint8_t getKeyVal(SymbolTable m, char *k);
