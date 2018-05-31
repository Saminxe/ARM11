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


//OPTAB (operation code table)
int OPMap(State state, uint32_t instr)
{
  uint8_t opcode = (instr & 0x1E000000) >> 21; // OpCode
  uint8_t rn = (instr & 0x000F0000) >> 16; // Rn
  uint8_t rd = (instr & 0x0000F000) >> 12; // Rd
  uint32_t oprand2 = (instr & 0x00000FFF); // Operand2
  uint8_t rm = (instr & 0x000000FF); //Rm
  uint8_t rs = (instr & 0x00000F00); //Rs

  void add() {
    state.registers[rd] = state.registers[rn] + state.registers[oprand2];
  }

  void mov() {
    state.registers[rd] = state.registers[opcode];
  }

  void adn() {
    state.registers[rd] = state.registers[rn] & state.registers[opcode];
  }

  void eor() {
    state.registers[rd] = state.registers[rn] ^ state.registers[opcode];
  }

  void sub() {
    state.registers[rd] = state.registers[rn] - state.registers[opcode];
  }

  void rsb() {
    state.registers[rd] = state.registers[opcode] - state.registers[rn];
  }

  void orr() {
    state.registers[rd] = state.registers[rn] | state.registers[opcode];
  }

  void tst() {
    state.registers[CSPR] = state.registers[rn] & state.registers[opcode];
  }

  void teq() {
    state.registers[CSPR] = state.registers[rn] ^ state.registers[opcode];
  }

  void cmp() {
    state.registers[CSPR] = state.registers[rn] - state.registers[opcode];
  }

  void mul() {
    state.registers[rd] = state.registers[rm] * state.registers[rs];
  }

  void mla() {
    mul() + state.registers[rn];
  }

  void ldr() {
    if(instr < 0xFF) {
      move();
    }
    state.registers[rd] = state.memory;
  }

  void str() {
    state.memory = state.registers[rd];
  }

  void beq() {
    state.registers[PC] = state.memory;
  }

  void bne() {
    state.registers[PC] = state.memory;
  }

  void bge() {
    state.registers[PC] = state.memory;
  }

  void blt() {
    state.registers[PC] = state.memory;
  }

  void bgt() {
    state.registers[PC] = state.memory;
  }

  void ble() {
    state.registers[PC] = state.memory;
  }

  void b() {
    state.registers[PC] = state.memory;
  }

  void lsl() {
    state.registers[rd] = state.registers[rd] << 0; //shifted left but no clue how much
  }

  void andeq() {
  }

  switch(instr) {
    case (OP.ADD):
      add(state);
      return 0x800000;
    case (OP.SUB):
      sub(state);
      return 0x400000;
    case (OP.RSB):
      rsb(state);
      return 0x600000;
    case (OP.AND):
      adn(state);
      return 0x0;
    case (OP.EOR):
      eor(state);
      return 0x200000;
    case (OP.ORR):
      orr(state);
      return 0x1800000;
    case (OP.MOV):
      mov(state, instr);
      return 0x1A00000;
    case (OP.TST):
      tst();
      return 0x1000000;
    case (OP.TEQ):
      teq();
      return 0x1200000;
    case (OP.CMP):
      cmp();
      return 0x1400000;
    case (OP.MUL):
      mul();
      return 0xE0000090;
    case (OP.MLA):
      mla();
      return 0xE0200090;
    case (OP.LDR):
      ldr();
      return 0x4100000;
    case (OP.STR):
      str();
      return 0x4000000;
    case (OP.BEQ):
      beq();
      return 0xA000000;
    case (OP.BNE):
      bne();
      return 0x1A000000;
    case (OP.BGE):
      bge();
      return 0xAA000000;
    case (OP.BLT):
      blt();
      return 0xBA000000;
    case (OP.BGT):
      bgt();
      return 0xCA000000;
    case (OP.BLE):
      ble();
      return 0xDA000000;
    case (OP.B):
      b();
      return 0xEA000000;
        //I've treated b as al
    case (OP.LSL):
      lsl();
      return 0x0; //unsure
    case (OP.ANDEQ):
      andeq();
      return 0x00000000;
    default: return 0;
  }


}
