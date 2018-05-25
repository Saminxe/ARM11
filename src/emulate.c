#include <stdlib.h>
#include <stdio.h>
#define SP 13 //shared pointer
#define LR 14 //link register
#define PC 15 //program counter
#define CPSR 16 //flags register

/*** Debugging tools ***/
void printMemoryComposition(unsigned char *memory, int size)
{
    for (int i = 0; i < size; i++)
      printf("Memory at %d = %02x\n", i, memory[i]);
}

void printRegisterComposition(unsigned long *registers)
{
  for (int i = 0; i < 13; i++)
    printf("E%02d: %08lx\n", i, registers[i]);
  printf("ESP: %08lx\n", registers[SP]);
  printf("ELR: %08lx\n", registers[LR]);
  printf("EPC: %08lx\n", registers[PC]);
  printf("CPSR: %08lx\n", registers[CPSR]);
}

/*** End of debugging tools ***/

/*** Processing Instructions ***/
/* All data processing instructions take the base address of the
  memory and registers, and the instruction as arguments */
void dataProcess(unsigned char *memory, unsigned long *registers, unsigned long instr)
{
  printf("This is a data processing instruction\n");
}

void multiply(unsigned char *memory, unsigned long *registers, unsigned long instr)
{
  printf("This is a multiply instruction\n");
}

void singleDataTransfer(unsigned char *memory, unsigned long *registers, unsigned long instr)
{
  printf("This is an SDT instruction\n");
}

void branchDataTransfer(unsigned char *memory, unsigned long *registers, unsigned long instr)
{
  printf("This is a branch instruction\n");
}

/*** Pipeline ***/
void fetch(unsigned char *memory, unsigned long *registers)
{
  unsigned long pc = *(registers + PC);
  unsigned long instr = memory[pc + 3] |
    memory[pc + 2] << 8 |
    memory[pc + 1] << 16 |
    memory[pc] << 24;
  if (~(instr ^ 0x90) & 0x0FC000F0)
    multiply(memory, registers, instr);
  else if (~(instr ^ 0x0) & 0x0C000000)
    dataProcess(memory, registers, instr);
  else if (~(instr ^ 0x04000000) & 0x0C600000)
    singleDataTransfer(memory, registers, instr);
  else if (~(instr ^ 0x0A000000) & 0x0F000000)
    branchDataTransfer(memory, registers, instr);
  else printf("not a valid instruction u schmuck");
}

//yappy test

int main(int argc, char **argv)
{
  unsigned char *memory;
  unsigned long *registers;
  FILE *proc;
  int procSize;

  if (argc != 2) {
    printf("Usage = %s filename\n", argv[0]);
    return EXIT_FAILURE;
  }

  memory = (unsigned char *) calloc(65536, sizeof(char));
  registers = (unsigned long *) calloc(17, sizeof(long));
  proc = fopen(argv[1], "rb");

  if (proc == NULL) {
    printf("File Load Failure\n");
    return EXIT_FAILURE;
  }

  fseek(proc, 0, SEEK_END);
  procSize = ftell(proc);
  fseek(proc, 0, SEEK_SET);
  fread(memory, sizeof(char), procSize, proc);

  fetch(memory, registers);

  printMemoryComposition(memory, procSize);
  printRegisterComposition(registers);

  free(memory);
  free(registers);
  fclose(proc);

  return EXIT_SUCCESS;
}
