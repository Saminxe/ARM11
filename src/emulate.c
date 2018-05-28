#include "emulate.h"

/*
██████╗ ██╗   ██╗████████╗███████╗     ██████╗ ██████╗ ██████╗ ███████╗██████╗     ██╗███████╗    ██╗     ██╗████████╗████████╗██╗     ███████╗    ███████╗███╗   ██╗██████╗ ██╗ █████╗ ███╗   ██╗
██╔══██╗╚██╗ ██╔╝╚══██╔══╝██╔════╝    ██╔═══██╗██╔══██╗██╔══██╗██╔════╝██╔══██╗    ██║██╔════╝    ██║     ██║╚══██╔══╝╚══██╔══╝██║     ██╔════╝    ██╔════╝████╗  ██║██╔══██╗██║██╔══██╗████╗  ██║
██████╔╝ ╚████╔╝    ██║   █████╗      ██║   ██║██████╔╝██║  ██║█████╗  ██████╔╝    ██║███████╗    ██║     ██║   ██║      ██║   ██║     █████╗      █████╗  ██╔██╗ ██║██║  ██║██║███████║██╔██╗ ██║
██╔══██╗  ╚██╔╝     ██║   ██╔══╝      ██║   ██║██╔══██╗██║  ██║██╔══╝  ██╔══██╗    ██║╚════██║    ██║     ██║   ██║      ██║   ██║     ██╔══╝      ██╔══╝  ██║╚██╗██║██║  ██║██║██╔══██║██║╚██╗██║
██████╔╝   ██║      ██║   ███████╗    ╚██████╔╝██║  ██║██████╔╝███████╗██║  ██║    ██║███████║    ███████╗██║   ██║      ██║   ███████╗███████╗    ███████╗██║ ╚████║██████╔╝██║██║  ██║██║ ╚████║
╚═════╝    ╚═╝      ╚═╝   ╚══════╝     ╚═════╝ ╚═╝  ╚═╝╚═════╝ ╚══════╝╚═╝  ╚═╝    ╚═╝╚══════╝    ╚══════╝╚═╝   ╚═╝      ╚═╝   ╚══════╝╚══════╝    ╚══════╝╚═╝  ╚═══╝╚═════╝ ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝
*/

/*** Debugging tools ***/
void printMemoryComposition(uint8_t *memory, int size)
{
    for (int i = 0; i < size; i++)
      printf("Memory at %d = %02x\n", i, memory[i]);
}

void printRegisterComposition(uint32_t *registers)
{
  for (int i = 0; i < 13; i++)
    printf("E%02d: %08x\n", i, registers[i]);
  printf("ESP: %08x\n", registers[SP]);
  printf("ELR: %08x\n", registers[LR]);
  printf("EPC: %08x\n", registers[PC]);
  printf("CPSR: %08x\n", registers[CPSR]);
}

void condChecks(uint32_t *registers, uint32_t cpsrState)
{
  *(registers + CPSR) = cpsrState;
  printf("CPSR: %x\n", cpsrState);
  printf("Code 0000 returns %d\n", checkCond(0x0, registers));
  printf("Code 0001 returns %d\n", checkCond(0x1, registers));
  printf("Code 1010 returns %d\n", checkCond(0xA, registers));
  printf("Code 1011 returns %d\n", checkCond(0xB, registers));
  printf("Code 1100 returns %d\n", checkCond(0xC, registers));
  printf("Code 1101 returns %d\n", checkCond(0xD, registers));
  printf("Code 1110 returns %d\n\n", checkCond(0xE, registers));
}
/*** End of debugging tools ***/

/*** Generic pattern matcher ***/
int patternMatcher(uint32_t instr, uint32_t pattern, uint32_t mask)
{
  uint32_t modified = ((~(instr ^ pattern)) & mask) | ~(mask);
  if (modified == 0) return 0;
  if (((modified + 1) & modified) == 0) return 1;
  return 0;
}

/*** Helper functions for Processing Instructions ***/

/* Check CSPR registers against a given condition code.
 * Return 1 if condition is met, 0 if it is not.*/
int checkCond(uint8_t cond, uint32_t *registers)
{
  uint8_t cpsr = *(registers + CPSR) >> 28;
  switch (cond) {
    case (0x0): return (cpsr & Z) == 0x4;
    case (0x1): return (cpsr & Z) == 0x0;
    case (0xA): return (cpsr & N) >> 3 == (cpsr & V);
    case (0xB): return (cpsr & N) >> 3 != (cpsr & V);
    case (0xC): return ((cpsr & N) >> 3 == (cpsr & V)) && ((cpsr & Z) == 0x0);
    case (0xD): return ((cpsr & Z) == 0x4) || ((cpsr & N) >> 3 != (cpsr & V));
    case (0xE): return 1;
    default: printf("ERROR: Condition code %c is not acceptable\n", cond);
  }
  return -1;
}

/* Sets/unsets the desired flag in CPSR.
  1 = Set, 0 = Unset. */
void setUnset(int flag, int set, uint32_t *registers)
{
  if (set) {
    switch (flag) {
      case (V): registers[CPSR] |= 0x10000000; break;
      case (C): registers[CPSR] |= 0x20000000; break;
      case (Z): registers[CPSR] |= 0x40000000; break;
      case (N): registers[CPSR] |= 0x80000000; break;
    }
  } else {
    switch (flag) {
      case (V): registers[CPSR] &= 0xEFFFFFFF; break;
      case (C): registers[CPSR] &= 0xDFFFFFFF; break;
      case (Z): registers[CPSR] &= 0xBFFFFFFF; break;
      case (N): registers[CPSR] &= 0x7FFFFFFF; break;
    }
  }
}


/* Check CSPR registers against conditions set according to the first four bits of the instr.
 * Return 1 if condition is met, 0 if it is not.*/
int checkInstrCond(uint32_t *registers, uint32_t instr) {
  uint8_t cond = instr >> 28;
  return checkCond(cond, registers);
}

/*** Processing Instructions ***/
/* All data processing instructions take the base address of the
  memory and registers, and the instruction as arguments */

// Return bit at given position in instr.
int getInstrBit(uint32_t instr, int position) {
  assert(0 <= position && position <= 31);
  uint32_t mask = 1 << position;
  return !((instr & mask) == 0);
}

void dataProcess(uint8_t *memory, uint32_t *registers, uint32_t instr)
{
  if (!checkInstrCond(registers, instr)) return;
  //printf("This is a data processing instruction\n");

  int immediate = getInstrBit(instr, 25); // I
  uint8_t opcode = (instr & 0x1E000000) >> 21; // OpCode
  int set = getInstrBit(instr,20); // S
  uint8_t rn = (instr & 0x000F0000) >> 16; // Rn
  uint8_t rd = (instr & 0x0000F000) >> 12; // Rd
  uint8_t oprand2 = (instr & 0x00000FFF); // Operand2

  // If Operand2 is an immediate value (I = 1)
  if (immediate) {
    
  }
  // If Operand2 is a register (I = 0)

  // Opcode instructions
  int opcode3 = getInstrBit(instr, 24);
  int opcode2 = getInstrBit(instr, 23);
  int opcode1 = getInstrBit(instr, 22);
  int opcode0 = getInstrBit(instr, 21);
  if (opcode3) {
    if (opcode2) {
      if (opcode1) {
        if (opcode0) {
          //1111
        }
        //1110
      }
      if (opcode0) {
        //1101
        registers[rd] = oprand2;
      }
      //1100
      registers[rd] = registers[rn] | oprand2;
    }
    if (opcode1) {
      if (opcode0) {
        //1011
      }
      //1010
      registers[rn] - oprand2;
    }
    if (opcode0){
      //1001
      registers[rn] ^ oprand2;
    }
    //1000
    registers[rn] & oprand2;
  }
  if (opcode2) {
    if (opcode1) {
      if (opcode0) {
        //0111
      }
      //0110
    }
    if (opcode0) {
      //0101
    }
    //0100
    registers[rd] = registers[rn] + oprand2;
  }
  if (opcode1) {
    if (opcode0) {
      //0011
      registers[rd] = oprand2 - registers[rn];
    }
    //0010
    registers[rd] = registers[rn] - oprand2;
  }
  if (opcode0){
    //0001
    registers[rd] = registers[rn] ^ oprand2;
  }
  //0000
  registers[rd] = registers[rn] & oprand2;
}

void multiply(uint8_t *memory, uint32_t *registers, uint32_t instr)
{
  int acc = (instr & 0x00200000) >> 21;
  int set = (instr & 0x00100000) >> 20;
  uint8_t rd = (instr & 0x000F0000) >> 16;
  uint8_t rn = (instr & 0x0000F000) >> 12;
  uint8_t rs = (instr & 0x00000F00) >> 8;
  uint8_t rm = (instr & 0x0000000F);
  //printf("This is a multiply instruction\n");
  if (!checkInstrCond(registers, instr)) return;
  registers[rd] = registers[rm] * registers[rs];
  if (acc)
    registers[rd] += registers[rn];
  if (set) {
    setUnset(N, registers[rd] >> 31, registers);
    setUnset(Z, registers[rd] == 0, registers);
  }
}

void singleDataTransfer(uint8_t *memory, uint32_t *registers, uint32_t instr)
{
  printf("This is an SDT instruction\n");
}

void branchDataTransfer(uint8_t *memory, uint32_t *registers, uint32_t instr)
{
  printf("This is a branch instruction\n");
  /* Sam, use checkIntstrCond */
  /*instr = calloc(32, sizeof(long));
  uint8_t cond = instr >> 28;*/
}

/*** Pipeline ***/
void process(uint8_t *memory, uint32_t *registers)
{
  const uint32_t DATA_PROCESS_PATTERN = 0x00000000;
  const uint32_t DATA_PROCESS_MASK = 0x0C000000;
  const uint32_t MULTIPLY_PATTERN = 0x00000090;
  const uint32_t MULTIPLY_MASK = 0x0FC00090;
  const uint32_t SDT_PATTERN = 0x04000000;
  const uint32_t SDT_MASK = 0x0C060000;
  const uint32_t BRANCH_PATTERN = 0x0A000000;
  const uint32_t BRANCH_MASK = 0x0F000000;
  do {
    uint32_t instr = memory[*(registers + PC)] |
      memory[*(registers + PC) + 1] << 8 |
      memory[*(registers + PC) + 2] << 16 |
      memory[*(registers + PC) + 3] << 24;
    //printf("%x\n", instr);
    *(registers + PC) += 4;
    if (instr == 0) break;
    else if (patternMatcher(instr, MULTIPLY_PATTERN, MULTIPLY_MASK))
      multiply(memory, registers, instr);
    else if (patternMatcher(instr, DATA_PROCESS_PATTERN, DATA_PROCESS_MASK))
      dataProcess(memory, registers, instr);
    else if (patternMatcher(instr, SDT_PATTERN, SDT_MASK))
      singleDataTransfer(memory, registers, instr);
    else if (patternMatcher(instr, BRANCH_PATTERN, BRANCH_MASK))
      branchDataTransfer(memory, registers, instr);
    else printf("not a valid instruction u schmuck\n");
  } while (*(registers + PC) < 65536);
}

int main(int argc, char **argv)
{
  uint8_t *memory;
  uint32_t *registers;
  FILE *proc;
  int procSize;

  if (argc != 2) {
    printf("Usage = %s filename\n", argv[0]);
    return EXIT_FAILURE;
  }

  memory = (uint8_t *) calloc(65536, sizeof(char));
  registers = (uint32_t *) calloc(17, sizeof(long));
  proc = fopen(argv[1], "rb");

  if (proc == NULL) {
    printf("File Load Failure\n");
    return EXIT_FAILURE;
  }

  fseek(proc, 0, SEEK_END);
  procSize = ftell(proc);
  fseek(proc, 0, SEEK_SET);
  fread(memory, sizeof(char), procSize, proc);

  /* Register tests
  registers[0] = 4;
  registers[1] = 5;
  registers[2] = 0xffff0000;
  */
  
  process(memory, registers);

  /* Debugging */
  //printMemoryComposition(memory, procSize);
  //printRegisterComposition(registers);

  /*for (int i = 0; i <= 16; i++) {
    uint32_t cpsrState = 0x10000000 * i;
    condChecks(registers, cpsrState);
  }*/

  /*
  uint32_t getInstrBitTest = 0x00000001;
  for (int i = 31; i >= 0; i--) {
      printf("%d", getInstrBit(getInstrBitTest, i));
  }
  */


  /* End of debugging*/

  free(memory);
  free(registers);
  fclose(proc);

  return EXIT_SUCCESS;
}
