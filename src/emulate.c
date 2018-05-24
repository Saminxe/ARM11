#include <stdlib.h>
#include <stdio.h>
#define SP 13 //shared pointer
#define LR 14 //link register
#define PC 15 //program counter
#define CPSR 16 //flags register

//void loadInto(char *memory, )

int main(int argc, char **argv) {
  int *memory;
  int *registers;
  //FILE *proc;

  if (argc != 2) {
    printf("Usage = %s filename\n", argv[0]);
    return EXIT_FAILURE;
  }

  memory = (int *) calloc(65536, sizeof(char));
  registers = (int *) calloc(17, sizeof(long));
  //proc = fopen(argv[1], "r");

  free(memory);
  free(registers);

  return EXIT_SUCCESS;
}
