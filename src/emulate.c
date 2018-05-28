#include "emulate.h"

/*
██████╗ ██╗   ██╗████████╗███████╗     ██████╗ ██████╗ ██████╗ ███████╗██████╗     ██╗███████╗    ██╗     ██╗████████╗████████╗██╗     ███████╗    ███████╗███╗   ██╗██████╗ ██╗ █████╗ ███╗   ██╗
██╔══██╗╚██╗ ██╔╝╚══██╔══╝██╔════╝    ██╔═══██╗██╔══██╗██╔══██╗██╔════╝██╔══██╗    ██║██╔════╝    ██║     ██║╚══██╔══╝╚══██╔══╝██║     ██╔════╝    ██╔════╝████╗  ██║██╔══██╗██║██╔══██╗████╗  ██║
██████╔╝ ╚████╔╝    ██║   █████╗      ██║   ██║██████╔╝██║  ██║█████╗  ██████╔╝    ██║███████╗    ██║     ██║   ██║      ██║   ██║     █████╗      █████╗  ██╔██╗ ██║██║  ██║██║███████║██╔██╗ ██║
██╔══██╗  ╚██╔╝     ██║   ██╔══╝      ██║   ██║██╔══██╗██║  ██║██╔══╝  ██╔══██╗    ██║╚════██║    ██║     ██║   ██║      ██║   ██║     ██╔══╝      ██╔══╝  ██║╚██╗██║██║  ██║██║██╔══██║██║╚██╗██║
██████╔╝   ██║      ██║   ███████╗    ╚██████╔╝██║  ██║██████╔╝███████╗██║  ██║    ██║███████║    ███████╗██║   ██║      ██║   ███████╗███████╗    ███████╗██║ ╚████║██████╔╝██║██║  ██║██║ ╚████║
╚═════╝    ╚═╝      ╚═╝   ╚══════╝     ╚═════╝ ╚═╝  ╚═╝╚═════╝ ╚══════╝╚═╝  ╚═╝    ╚═╝╚══════╝    ╚══════╝╚═╝   ╚═╝      ╚═╝   ╚══════╝╚══════╝    ╚══════╝╚═╝  ╚═══╝╚═════╝ ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝
*/

/*** Debugging tools ***/
void printState(State state)
{
  int i = 0;
  do {
    printf("Memory at %d = %02x\n", i, state.memory[i]);
    i++;
  } while ((state.memory[i] != 0) && (i < 65536));
  for (int j = 0; j < 13; j++)
    printf("E%02d: %08x\n", j, state.registers[j]);
  printf("ESP: %08x\n", state.registers[SP]);
  printf("ELR: %08x\n", state.registers[LR]);
  printf("EPC: %08x\n", state.registers[PC]);
  printf("CPSR: %08x\n", state.registers[CPSR]);
}


void condChecks(State state, uint32_t cpsrState)
{
  state.registers[CPSR] = cpsrState;
  printf("CPSR: %x\n", cpsrState);
  printf("Code 0000 returns %d\n", checkCond(0x0, state));
  printf("Code 0001 returns %d\n", checkCond(0x1, state));
  printf("Code 1010 returns %d\n", checkCond(0xA, state));
  printf("Code 1011 returns %d\n", checkCond(0xB, state));
  printf("Code 1100 returns %d\n", checkCond(0xC, state));
  printf("Code 1101 returns %d\n", checkCond(0xD, state));
  printf("Code 1110 returns %d\n\n", checkCond(0xE, state));
}
/*** End of debugging tools ***/

/*** Generic pattern matcher ***/
int patternMatcher(uint32_t instr, uint32_t pattern, uint32_t mask)
{
  uint32_t modified = ((~(instr ^ pattern)) & mask) | ~(mask);
  if (modified == 0) return 0;
  if (((modified + 1) & modified) == 0) return 1;
  return 0;
}

/*** Helper functions for Processing Instructions ***/

/* Check CSPR registers against a given condition code.
 * Return 1 if condition is met, 0 if it is not.*/
int checkCond(uint8_t cond, State state)
{
  uint8_t cpsr = state.registers[CPSR] >> 28;
  switch (cond) {
    case (0x0): return (cpsr & Z) == 0x4;
    case (0x1): return (cpsr & Z) == 0x0;
    case (0xA): return (cpsr & N) >> 3 == (cpsr & V);
    case (0xB): return (cpsr & N) >> 3 != (cpsr & V);
    case (0xC): return ((cpsr & N) >> 3 == (cpsr & V)) && ((cpsr & Z) == 0x0);
    case (0xD): return ((cpsr & Z) == 0x4) || ((cpsr & N) >> 3 != (cpsr & V));
    case (0xE): return 1;
    default: printf("ERROR: Condition code %c is not acceptable\n", cond);
  }
  return -1;
}

/* Sets/unsets the desired flag in CPSR.
  1 = Set, 0 = Unset. */
void setUnset(int flag, int set, State state)
{
  if (set) {
    switch (flag) {
      case (V): state.registers[CPSR] |= 0x10000000; break;
      case (C): state.registers[CPSR] |= 0x20000000; break;
      case (Z): state.registers[CPSR] |= 0x40000000; break;
      case (N): state.registers[CPSR] |= 0x80000000; break;
    }
  } else {
    switch (flag) {
      case (V): state.registers[CPSR] &= 0xEFFFFFFF; break;
      case (C): state.registers[CPSR] &= 0xDFFFFFFF; break;
      case (Z): state.registers[CPSR] &= 0xBFFFFFFF; break;
      case (N): state.registers[CPSR] &= 0x7FFFFFFF; break;
    }
  }
}


/* Check CSPR registers against conditions set according to the first four bits of the instr.
 * Return 1 if condition is met, 0 if it is not.*/
int checkInstrCond(State state, uint32_t instr) {
  uint8_t cond = instr >> 28;
  return checkCond(cond, state);
}

/*** Processing Instructions ***/
/* All data processing instructions take the base address of the
  memory and registers, and the instruction as arguments */

// Return bit at given position in instr.
int getInstrBit(uint32_t instr, int position) {
  assert(0 <= position && position <= 31);
  uint32_t mask = 1 << position;
  return !((instr & mask) == 0);
}

void dataProcess(State state, uint32_t instr)
{
  if (!checkInstrCond(state, instr)) { return; }
}

void multiply(State state, uint32_t instr)
{
  int acc = (instr & 0x00200000) >> 21;
  int set = (instr & 0x00100000) >> 20;
  uint8_t rd = (instr & 0x000F0000) >> 16;
  uint8_t rn = (instr & 0x0000F000) >> 12;
  uint8_t rs = (instr & 0x00000F00) >> 8;
  uint8_t rm = (instr & 0x0000000F);
  if (!checkInstrCond(state, instr)) return;
  state.registers[rd] = state.registers[rm] * state.registers[rs];
  if (acc) {
    state.registers[rd] += state.registers[rn];
    //printf("Register E%02u = %u * %u + %u = %u\n", rd, state.registers[rm], state.registers[rs], state.registers[rn], state.registers[rd]);
  }
//  else printf("Register E%02u = %u * %u = %u\n", rd, state.registers[rm], state.registers[rs], state.registers[rd]);
  if (set) {
    setUnset(N, state.registers[rd] >> 31, state);
    setUnset(Z, state.registers[rd] == 0, state);
  }
}

void singleDataTransfer(State state, uint32_t instr)
{
  printf("This is an SDT instruction\n");
}

void branchDataTransfer(State state, uint32_t instr)
{
  printf("This is a branch instruction\n");
  if (checkInstrCond(state, instr)) {
     int32_t offset = instr & 0xFFFFFF;
     offset <<2;
     int checkSign = offset >> 25;
     if (checkSign) {
        offset | 0xFC000000;
     }
     //PC += offset;
     state.registers = (uint32_t*) PC;
  }
}

/*** Pipeline ***/
void process(State state)
{
  const uint32_t DATA_PROCESS_PATTERN = 0x00000000;
  const uint32_t DATA_PROCESS_MASK = 0x0C000000;
  const uint32_t MULTIPLY_PATTERN = 0x00000090;
  const uint32_t MULTIPLY_MASK = 0x0FC00090;
  const uint32_t SDT_PATTERN = 0x04000000;
  const uint32_t SDT_MASK = 0x0C060000;
  const uint32_t BRANCH_PATTERN = 0x0A000000;
  const uint32_t BRANCH_MASK = 0x0F000000;
  do {
    uint32_t instr = state.memory[state.registers[PC]] |
      state.memory[state.registers[PC] + 1] << 8 |
      state.memory[state.registers[PC] + 2] << 16 |
      state.memory[state.registers[PC] + 3] << 24;
    //printf("%x\n", instr);
    state.registers[PC] += 4;
    if (instr == 0) break;
    else if (patternMatcher(instr, MULTIPLY_PATTERN, MULTIPLY_MASK))
      multiply(state, instr);
    else if (patternMatcher(instr, DATA_PROCESS_PATTERN, DATA_PROCESS_MASK))
      dataProcess(state, instr);
    else if (patternMatcher(instr, SDT_PATTERN, SDT_MASK))
      singleDataTransfer(state, instr);
    else if (patternMatcher(instr, BRANCH_PATTERN, BRANCH_MASK))
      branchDataTransfer(state, instr);
    else printf("not a valid instruction u schmuck\n");
    printState(state);
  } while (state.registers[PC] < 65536);
}

int main(int argc, char **argv)
{
  State state = {(uint8_t *) calloc(65536, sizeof(char)),
      (uint32_t *) calloc(17, sizeof(long))};
  FILE *proc;
  int procSize;

  if (argc != 2) {
    printf("Usage = %s filename\n", argv[0]);
    return EXIT_FAILURE;
  }

  proc = fopen(argv[1], "rb");

  if (proc == NULL) {
    printf("File Load Failure\n");
    return EXIT_FAILURE;
  }

  fseek(proc, 0, SEEK_END);
  procSize = ftell(proc);
  fseek(proc, 0, SEEK_SET);
  fread(state.memory, sizeof(uint8_t), procSize, proc);

  /* Register Tests
  state.registers[0] = 4;
  state.registers[1] = 5;
  state.registers[2] = 0xffff0000;
  */

  process(state);

  /* Debugging */

  /*for (int i = 0; i <= 16; i++) {
    uint32_t cpsrState = 0x10000000 * i;
    condChecks(registers, cpsrState);
  }*/

  /*
  uint32_t getInstrBitTest = 0x00000001;
  for (int i = 31; i >= 0; i--) {
      printf("%d", getInstrBit(getInstrBitTest, i));
  }
  */

  /* End of debugging*/

  free(state.memory);
  free(state.registers);
  fclose(proc);

  return EXIT_SUCCESS;
}
