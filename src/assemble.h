#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define BUFFER_SIZE 512

typedef struct {
  char *key;
  uint8_t val;
} Entry;

typedef struct {
  int size;
  Entry entries[];
} Map;

int contains(char *string, char c);
int mapContainsKey(Map m, char *k);
uint8_t getKeyVal(Map m, char *k);
