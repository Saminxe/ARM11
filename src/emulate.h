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

void printMemoryComposition(uint8_t *memory, int size);
void printRegisterComposition(uint32_t *registers);
void condChecks(uint32_t *registers, uint32_t cpsrState);
int patternMatcher(uint32_t instr, uint32_t pattern, uint32_t mask);
int checkCond(uint8_t cond, uint32_t *registers);
int checkInstrCond(uint32_t *registers, uint32_t instr);
int getInstrBit(uint32_t instr, int position);
void setUnset(int flag, int set, uint32_t *registers);
void dataProcess(uint8_t *memory, uint32_t *registers, uint32_t instr);
void multiply(uint8_t *memory, uint32_t *registers, uint32_t instr);
void singleDataTransfer(uint8_t *memory, uint32_t *registers, uint32_t instr);
void branchDataTransfer(uint8_t *memory, uint32_t *registers, uint32_t instr);
void process(uint8_t *memory, uint32_t *registers);
