#include <stdlib.h>
#include <stdio.h>
#define FLAGS 16

int main(int argc, char **argv) {
  memory = calloc(65536, sizeof(char));
  registers = calloc(17, sizeof(long));

  return EXIT_SUCCESS;
}
