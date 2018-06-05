#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <inttypes.h>

#define SP 13 //shared pointer
#define LR 14 //link register
#define PC 15 //program counter
#define CPSR 16 //flags register
#define N 0x8
#define Z 0x4
#define C 0x2
#define V 0x1

#define MAX_MEMORY_ADDRESS 65536

typedef struct {
  uint8_t *memory;
  uint32_t *registers;
} State;

void printState(State state);
void condChecks(State state, uint32_t cpsrState);
void output(State state);
int patternMatcher(uint32_t instr, uint32_t pattern, uint32_t mask);
void setUnset(int flag, int set, State state);
void updateFlags(uint32_t result, int carry, State state);
int checkInstrCond(State state, uint32_t instr);
int getInstrBit(uint32_t instr, int position);
uint32_t ror(uint32_t value, uint8_t rotation);
uint32_t applyShiftType(uint32_t value, uint32_t instr, uint8_t amount, int set, State state);
void checkOverflow(uint32_t a, uint32_t b, uint32_t res, State state);
void checkBorrow(uint32_t a, uint32_t b, uint32_t res, State state);
void dataProcess(State state, uint32_t instr);
void multiply(State state, uint32_t instr);
void singleDataTransfer(State state, uint32_t instr);
void branchDataTransfer(State state, uint32_t instr);
int process(State state);
