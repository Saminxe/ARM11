#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

#define BUFFER_SIZE 512
#define DEFAULT_MAP_SIZE 120
#define PC 15
#define CSPR 16
#define DEFAULT_STRLEN 64

typedef enum _instr{DATA, MULT, SDTR, BRCH, ERRO = -1} Type;
typedef enum _cond{EQ, NE, GE, LT, GT, LE, AL, ER = -1} Condition;
typedef enum _opcode{ERR = -1, ADD, SUB, RSB, AND, EOR, ORR, MOV, TST, TEQ, CMP,
  MUL, MLA, LDR, STR, B, LSL} OpCode;
typedef enum {ShSL, ShLSR, ShASR, ShROR, ShERR} Shift;
  // All possible opcodes excluding conditions.

typedef struct {
  char name[20];
  uint32_t val;
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
uint32_t getKeyVal(SymbolTable m, char *k);
int optab(char* a);
int optabCheck(char* a);
int equals(char *a, char *b);

/* Bytecode generators */
uint32_t compute(OpCode opcode, char *rd, char *rn, char *operand2, int set);
uint32_t move(char *rd, char *operand2, int set);
uint32_t flagger(OpCode opcode, char *rn, char *operand2);
uint32_t shift(char *rn, char *expression, int set);
uint32_t mul(char *rd, char *rm, char *rs, int set);
uint32_t mla(char *rd, char *rm, char *rs, char *rn, int set);
uint32_t sdt(OpCode opcode, char *rd, char *rn, char *address, int locctr);
uint32_t branch(char *address, int locctr, SymbolTable symtab);
