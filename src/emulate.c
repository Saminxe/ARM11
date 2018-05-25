#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#define SP 13 //shared pointer
#define LR 14 //link register
#define PC 15 //program counter
#define CPSR 16 //flags register

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
int checkCond(uint8_t cond, uint32_t *registers) {
  uint8_t cpsr = *(registers + CPSR) >> 28;
  uint8_t N = 0x8;
  uint8_t Z = 0x4;
  //uint8_t C = 0x2;
  uint8_t V = 0x1;

  switch (cond) {
    case (0x0): return cpsr & Z;
    case (0x1): return (cpsr & Z) != 0x0;
    case (0xA): return (cpsr & N) >> 3 == (cpsr & V);
    case (0xB): return (cpsr & N) >> 3 != (cpsr & V);
    case (0xC): return ((cpsr & N) >> 3 == (cpsr & V)) && ((cpsr & Z) == 0x1);
    case (0xD): return ((cpsr & Z) == 0x1) || ((cpsr & N) >> 3 != (cpsr & V));
    case (0xE): return 1;
    default: printf("ERROR: Condition code %c is not acceptable\n", cond);
  }
  return -1;
}

int checkInstrCond(uint32_t *registers, uint32_t instr) {
  uint8_t cond = instr >> 28;
  return checkCond(cond, registers);
}

/*** Processing Instructions ***/
/* All data processing instructions take the base address of the
  memory and registers, and the instruction as arguments */
void dataProcess(uint8_t *memory, uint32_t *registers, uint32_t instr)
{
  printf("This is a data processing instruction\n");
}

void multiply(uint8_t *memory, uint32_t *registers, uint32_t instr)
{
  printf("This is a multiply instruction\n");
}

void singleDataTransfer(uint8_t *memory, uint32_t *registers, uint32_t instr)
{
  printf("This is an SDT instruction\n");
}

void branchDataTransfer(uint8_t *memory, uint32_t *registers, uint32_t instr)
{
  printf("This is a branch instruction\n");
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
  uint32_t pc = *(registers + PC);
  uint32_t instr = memory[pc] |
    memory[pc + 1] << 8 |
    memory[pc + 2] << 16 |
    memory[pc + 3] << 24;
  printf("%x\n", instr);
  if (patternMatcher(instr, MULTIPLY_PATTERN, MULTIPLY_MASK))
    multiply(memory, registers, instr);
  else if (patternMatcher(instr, DATA_PROCESS_PATTERN, DATA_PROCESS_MASK))
    dataProcess(memory, registers, instr);
  else if (patternMatcher(instr, SDT_PATTERN, SDT_MASK))
    singleDataTransfer(memory, registers, instr);
  else if (patternMatcher(instr, BRANCH_PATTERN, BRANCH_MASK))
    branchDataTransfer(memory, registers, instr);
  else printf("not a valid instruction u schmuck");
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

  process(memory, registers);
  *(registers + CPSR) = 0xF0000000;

  printMemoryComposition(memory, procSize);
  printRegisterComposition(registers);

  printf("%d\n", checkCond(0,registers));

  free(memory);
  free(registers);
  fclose(proc);

  return EXIT_SUCCESS;
}
