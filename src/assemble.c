#include "assemble.h"

int contains(char *string, char c)
{
  int length = strlen(string);
  for (int i = 0; i < length; i++) {
    if (string[i] == c) return 1;
  }
  return 0;
}

int main(int argc, char **argv) {
  FILE* src;
  char *buffer = calloc(BUFFER_SIZE, sizeof(char));
  Map symbolTable;

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
    char *nextToken = strtok(buffer, " ,");
    while (nextToken) {
      if (contains(nextToken, ':')) {
        printf("LABEL: ");
      }
      printf("%s\n", nextToken);
      nextToken = strtok(NULL, " ,");
    }
  }


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
int mapContainsKey(Map m, char *k)
{
  for (int i = 0; i < m.size; i++) {
    if (m.entries[i].key == k)
      return 1;
  }
  return 0;
}

void addToMap(Map m, Entry e)
{
  m.entries[m.size] = e;
  m.size++;
}

uint8_t getKeyVal(Map m, char *k) {
  for (int i = 0; i < m.size; i++) {
    if (m.entries[i].key == k)
      return m.entries[i].val;
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


//OPTAB (operation code table)
//int OPMap(int cond)
//{
//  switch(cond) {
//    case (OP.ADD): return 0x800000;
//    case (OP.SUB): return 0x400000;
//    case (OP.RSB): return 0x600000;
//    case (OP.AND): return 0x0;
//    case (OP.EOR): return 0x200000;
//    case (OP.ORR): return 0x1800000;
//    case (OP.MOV): return 0x1A00000
//    case (OP.TST): return 0x1000000;
//    case (OP.TEQ): return 0x1200000;
//    case (OP.CMP): return 0x1400000;
//    case (OP.MUL): return 0xE0000090;
//    case (OP.MLA): return 0xE0200090;
//    case (OP.LDR): return
//    case (OP.STR): return
//    case (OP.BEQ): return 0xA000000;
//    case (OP.BNE): return 0x1A000000;
//    case (OP.BGE): return 0xAA000000;
//    case (OP.BLT): return 0xBA000000;
//    case (OP.BGT): return 0xCA000000;
//    case (OP.BLE): return 0xDA000000;
//    case (OP.B): return 0xEA000000;
//        //I've treated b as al
//    case (OP.LSL): return 0x0; //unsure
//    case (OP.ANDEQ): return 0x00000000;
//
//  }






  //here's some more legit code tbh


uint8_t opcode = (instr & 0x1E000000) >> 21; // OpCode

uint8_t rn = (instr & 0x000F0000) >> 16; // Rn
uint8_t rd = (instr & 0x0000F000) >> 12; // Rd
uint32_t oprand2 = (instr & 0x00000FFF); // Operand2
uint8_t rm = (instr & 0x000000FF);







  int add(State a, State b, State c) {
    a.registers[rd] = b.registers[rn] + c.registers[oprand2];
  }


  int mov(State a, int const b) {
    a.memory = (uint8_t) b;
  }
























