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
      printf("Memory at %d = %2x\n", i, memory[i]);
}

void printRegisterComposition(unsigned long *registers)
{
  for (int i = 0; i < 13; i++)
    printf("E%d: %lx\n", i, registers[i]);
  printf("ESP: %lx\n", registers[SP]);
  printf("ELR: %lx\n", registers[LR]);
  printf("EPC: %lx\n", registers[PC]);
  printf("CPSR: %lx\n", registers[CPSR]);
}



/* End of debugging tools */

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

  //printMemoryComposition(memory, procSize);
  //printRegisterComposition(registers);

  free(memory);
  free(registers);
  fclose(proc);

  return EXIT_SUCCESS;
}
