#include "assemble.h"

int main(int argc, char **argv) {
  FILE* src;
  char *buffer = calloc(BUFFER_SIZE, sizeof(char));
  char **tokens;

  /* Tests for correct amount of input variables */
  if (argc != 2) {
    fprintf(stderr, "Usage = %s filename\n", argv[0]);
    return EXIT_FAILURE;
  }

  /* Opens the file */
  src = fopen(argv[1], "r");

  /* Checks the file exists */
  if (src == NULL) {
    fprintf(stderr, "File Load Failure\n");
    return EXIT_FAILURE;
  }

  /* Tokenization Loop */
  while (fgets(buffer, BUFFER_SIZE, src) != NULL) {
    
  }



  fclose(src);
  free(buffer);

}
