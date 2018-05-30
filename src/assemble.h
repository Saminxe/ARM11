#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define BUFFER_SIZE 512
#define DEFAULT_MAP_SIZE 120
#define PC 15
#define CSPR 16

typedef struct {
  char *name;
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

typedef struct{
//    case ("add"): return 0x800000;
//    case ("sub"): return 0x400000;
//    case ("rsb"): return 0x600000;
//    case ("and"): return 0x0;
//    case ("eor"): return 0x200000;
//    case ("orr"): return 0x1800000;
//    case ("mov"): return 0x1A00000
//    case ("tst"): return 0x1000000;
//    case ("teq"): return 0x1200000;
//    case ("cmp"): return 0x1400000;
//    case ("mul"): return 0xE0000090;
//    case ("mla"): return 0xE0200090;
//    case ("ldr"): return
//    case ("str"): return
//    case ("beq"): return 0xA000000;
//    case ("bne"): return 0x1A000000;
//    case ("bge"): return 0xAA000000;
//    case ("blt"): return 0xBA000000;
//    case ("bgt"): return 0xCA000000;
//    case ("ble"): return 0xDA000000;
//    case ("b"): return 0xEA000000;
//    //I've treated b as al
//    case ("lsl"): return; //unsure
//    case ("andeq"): return 0x00000000;
    int ADD;
    int SUB;
    int RSB;
    int AND;
    int EOR;
    int ORR;
    int MOV;
    int TST;
    int TEQ;
    int CMP;
    int MUL;
    int MLA;
    int LDR;
    int STR;
    int BEQ;
    int BNE;
    int BGE;
    int BLT;
    int BGT;
    int BLE;
    int B;
    int LSL;
    int ANDEQ;
} OP;

int contains(char *string, char c);
int symtabContains(SymbolTable m, char *k);
void symtabAdd(SymbolTable m, Symbol e);
uint8_t getKeyVal(SymbolTable m, char *k);
