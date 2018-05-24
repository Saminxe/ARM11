#include <stdlib.h>
#include <stdio.h>
#define SP 13 //shared pointer
#define LR 14 //link register
#define PC 15 //program counter
#define CPSR 16 //flags register

//void loadInto(int *memory, )

int main(int argc, char **argv) {
  char *memory;
  long *registers;
  FILE *proc;
  int procSize;

  if (argc != 2) {
    printf("Usage = %s filename\n", argv[0]);
    return EXIT_FAILURE;
  }

  memory = (char *) calloc(65536, sizeof(char));
  registers = (long *) calloc(17, sizeof(long));
  proc = fopen(argv[1], "rb");

  if (proc == NULL) {
    printf("File Load Failure\n");
    return EXIT_FAILURE;
  }

  fseek(proc, 0, SEEK_END);
  procSize = ftell(proc);
  fseek(proc, 0, SEEK_SET);
  fread(memory, 1, procSize, proc);

  for (int i = 0; i < procSize; i++) {
    printf("Memory at %d = %x\n", i, memory[i]);
  }

  free(memory);
  free(registers);
  fclose(proc);

  return EXIT_SUCCESS;
}
