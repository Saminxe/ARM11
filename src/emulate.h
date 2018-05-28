#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#define SP 13 //shared pointer
#define LR 14 //link register
#define PC 15 //program counter
#define CPSR 16 //flags register
#define N 0x8
#define Z 0x4
#define C 0x2
#define V 0x1

typedef struct {
  uint8_t *memory;
  uint32_t *registers;
} State;

void printState(State state);
void condChecks(State state, uint32_t cpsrState);
int patternMatcher(uint32_t instr, uint32_t pattern, uint32_t mask);
int checkCond(uint8_t cond, State state);
int checkInstrCond(State state, uint32_t instr);
int getInstrBit(uint32_t instr, int position);
void setUnset(int flag, int set, State state);
void dataProcess(State state, uint32_t instr);
void multiply(State state, uint32_t instr);
void singleDataTransfer(State state, uint32_t instr);
void branchDataTransfer(State state, uint32_t instr);
void process(State state);
