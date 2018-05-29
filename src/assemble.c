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
