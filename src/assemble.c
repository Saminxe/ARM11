#include "assemble.h"

/* Debug */
void printSymtab(SymbolTable s) {
  int i = 0;
  do {
    printf("%s, %u\n", s.symbols[i].name, s.symbols[i].val);
    i++;
  } while (i < s.size);
}

int contains(char *string, char c)
{
  int length = strlen(string);
  for (int i = 0; i < length; i++) {
    if (string[i] == c) return 1;
  }
  return 0;
}

//operation code table
int optab(char *string) {
  return 1;
}

int main(int argc, char **argv)
{
  FILE* src;
  char *buffer = calloc(BUFFER_SIZE, sizeof(char));
  uint8_t locctr = 0; // Starting address for memory allocations
  SymbolTable symtab = {0, calloc(DEFAULT_MAP_SIZE, sizeof(Symbol))};
  int size = 0; // Current pointer for symtab

  /* Tests for correct amount of input variables */
  if (argc != 3) {
    fprintf(stderr, "Usage = %s input output\n", argv[0]);
    return EXIT_FAILURE;
  }

  /* Opens the file */
  src = fopen(argv[1], "r");

  /* Checks the file exists */
  if (src == NULL) {
    fprintf(stderr, "File Load Failure\n");
    return EXIT_FAILURE;
  }

  /* Symbol Tablulation Loop */
  while (fgets(buffer, BUFFER_SIZE, src) != NULL) {
    char *opcode = strtok(buffer, " ");
    char *operands = strtok(NULL, " ");
    if (*opcode != ';') { // If it is not a comment
      if (opcode[strlen(opcode) - 2] == ':') { // If it is a label
        opcode[strlen(opcode) - 2] = 0;
        printf("Label = %s\n", opcode);
        if (symtabContains(symtab, opcode)) { // If the label is in symbtab
          fprintf(stderr, "Duplicate symbol, aborting assembly.\n");
          return -1;
        } else {
          strcpy(symtab.symbols[size].name, opcode);
          symtab.symbols[size].val = locctr;
          size++;
          symtab.size = size;
        }
      } else if (optab(opcode)) {
        printf("Opcode = %s, Operands = %s\n", opcode, operands);
        locctr += 32; // Length of instruction
      } else {
        printf("Unrecognized opcode %s\n", opcode);
      }
    }
  }

  printSymtab(symtab);


  //operation code table
  int OPTAB() {

  }


  //location counter
  int LOCCTR() {

  }


  //symbol table
  int SYMTAB() {

  }





  /* Translation Loop */
  while (fgets(buffer, BUFFER_SIZE, src) != NULL) {

  }

  fclose(src);
  free(buffer);



}

/*** Lookup Functions ***/
int symtabContains(SymbolTable m, char *k)
{
  for (int i = 0; i < m.size; i++) {
    if (m.symbols[i].name == k)
      return 1;
  }
  return 0;
}

uint8_t getKeyVal(SymbolTable m, char *k) {
  for (int i = 0; i < m.size; i++) {
    if (m.symbols[i].name == k)
      return m.symbols[i].val;
  }
  return -1;
}


//int branchInstruction() {
//  char[](label);
//  label =
//}








//don't touch this Sam is writing bad code

//check if strings are equals
//switch statement on char array
//return the machine code

int equals(char* a, char* b)
{
  int length = strlen(a);
  int length2 = strlen(b);
  if (length != length2) {
    return 0;
  }
  for (int i = 0; i < length; i++) {
    if (a[i] != b[i]) return 0;
  }
  return 1;
}



enum mneumonic {ADD, SUB, RSB, AND, EOR, ORR, MOV, TST, TEQ, CMP, MUL, MLA, LDR, STR, BEQ, BNE, BGE, BLT, BGT, BLE, B, LSL, ANDEQ};


//the code below is subject to change depending if we actually need to keep track of
// the instruction format, available addressing modes and length information

//OPTAB (operation code table)
int OPMap(State state, uint32_t instr)
{
  switch(mneumonic) {
    case (ADD): return 0x800000;
    case (SUB): return 0x400000;
    case (RSB): return 0x600000;
    case (AND): return 0x0;
    case (EOR): return 0x200000;
    case (ORR): return 0x1800000;
    case (MOV): return 0x1A00000;
    case (TST): return 0x1000000;
    case (TEQ): return 0x1200000;
    case (CMP): return 0x1400000;
    case (MUL): return 0xE0000090;
    case (MLA): return 0xE0200090;
    case (LDR): return 0x4100000;
    case (STR): return 0x4000000;
    case (BEQ): return 0xA000000;
    case (BNE): return 0x1A000000;
    case (BGE): return 0xAA000000;
    case (BLT): return 0xBA000000;
    case (BGT): return 0xCA000000;
    case (BLE): return 0xDA000000;
    case (B):   return 0xEA000000;
        //I've treated b as al
    case (LSL): return 0x0; //unsure
    case (ANDEQ): return 0x00000000;
    default:    return 0;
  }
}





