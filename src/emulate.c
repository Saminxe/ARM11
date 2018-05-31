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

// Update flags. Pre: Set == 1
void updateFlags(uint32_t result, int carry, State state) {
  setUnset(N, result >> 31, state);
  setUnset(Z, result == 0, state);
  setUnset(C, carry, state);
}

/* Check CSPR registers against conditions set according to the first four bits of the instr.
 * Return 1 if condition is met, 0 if it is not.*/
// If tests do not work, try casting cond to a uint8_t?
int checkInstrCond(State state, uint32_t instr) {
  uint8_t cond = instr >> 28;
  return checkCond(cond, state);
}

// Return bit at given position in instr.
int getInstrBit(uint32_t instr, int index) {
  assert(0 <= index && index <= 31);
  uint32_t mask = 1 << index;
  return !((instr & mask) == 0);
}

// Rotate right cyclically with bit 0 shifting into bit 31.
uint32_t ror(uint32_t value, uint8_t rotation) {
  return value >> (rotation % 32) | value << ((32 - rotation) % 32);
}

// Apply shift according to shift type.
uint32_t applyShiftType(uint32_t value, uint32_t instr, uint8_t amount, State state) {
  assert(0 <= amount && amount <= 32);
  int bit6 = getInstrBit(instr, 6);
  int bit5 = getInstrBit(instr, 5);
  uint32_t result = value;
  int carry = 0;
  if (!bit6) {
    if (!bit5) {
      // 00 lsl
      if (amount > 0)
        carry = getInstrBit(value, 32 - amount);
      result = value << amount;
    } else {
      // 01 lsr
      if (amount > 1)
        carry = getInstrBit(value, amount - 1);
      result = value >> amount;
    }
  } else {
    if (!bit5) {
      // 10 asr
      if (amount > 1)
        carry = getInstrBit(value, amount - 1);
      result = value >> amount | 0xFFFFFFFF << (32 - amount);
    } else {
      // 11 ror
      if (amount > 1)
        carry = getInstrBit(value, amount - 1);
      result = value >> amount | value << (32 - amount);
    }
  }
  return result;
}

// Check for overflow when performing a + b = res.
void checkOverflow(uint32_t a, uint32_t b, uint32_t res, State state) {
  int a31 = getInstrBit(a, 31);
  int b31 = getInstrBit(b, 31);
  int res31 = getInstrBit(res, 31);
  if ((a31 && b31) || (a31 && res31) || (b31 && res31)) {
    setUnset(C, 1, state);
  } else {
    setUnset(C, 0, state);
  }
}

// Check for borrowing for when performing a - b = res.
void checkBorrow(uint32_t a, uint32_t b, uint32_t res, State state) {
  int a31 = getInstrBit(a, 31);
  int b31 = getInstrBit(b, 31);
  int res31 = getInstrBit(res, 31);
  if ((!a31 && b31) || (a31 && b31 && res31) || (!a31 && res31)) {
    setUnset(C, 0, state);
  } else {
    setUnset(C, 1, state);
  }
}

void treatAsShiftRegister(State state, uint32_t instr)
{
  uint32_t oprand2 = (instr & 0x00000FFF); // Operand2
  uint8_t rm = instr & 0x0000000F;
  uint32_t value = state.registers[rm];
  uint8_t amount = 0x0;
  if (!getInstrBit(instr,4)) {
      // Bit 4 = 0; Shift by a constant amount.
      amount = instr & 0x00000F80;
  } else {
      // Bit 4 = 1; Shift specified by a register.
      uint8_t rs = instr & 0x00000F00;
      amount = state.registers[rs] & 0x0000000F;
  }
  oprand2 = applyShiftType(value, instr, amount, state);
}

/*** Processing Instructions ***/
/* All data processing instructions take the base address of the
  memory and registers, and the instruction as arguments */

void dataProcess(State state, uint32_t instr)
{
  if (!checkInstrCond(state, instr))  return;
  //printf("This is a data processing instruction\n");

  int immediate = getInstrBit(instr, 25); // I
  int set = getInstrBit(instr,20); // S
  uint8_t rn = (instr & 0x000F0000) >> 16; // Rn
  uint8_t rd = (instr & 0x0000F000) >> 12; // Rd
  uint32_t oprand2 = (instr & 0x00000FFF); // Operand2

  // If Operand2 is an immediate value (I = 1)
  if (immediate) {
    // Rotation amount is twice the value in the 4 bit rotation field.
    uint8_t rotate = (instr & 0x00000F00) >> 7;
    uint32_t imm = instr & 0x000000FF;
    // Rotate right by "rotate"
    oprand2 = ror(imm, rotate);
  } else {
  // If Operand2 is a register (I = 0)
    treatAsShiftRegister(state, instr);
    if (set)
      setUnset(C, carry, state);
  }

  // Apply Opcode instructions
  int opcode3 = getInstrBit(instr, 24);
  int opcode2 = getInstrBit(instr, 23);
  int opcode1 = getInstrBit(instr, 22);
  int opcode0 = getInstrBit(instr, 21);
  uint32_t result = 0x0;
  if (opcode3) {
    if (opcode2) {
      if (opcode1) {
        if (opcode0) {
          //1111
        } else {
          //1110
        }
      } else {
        if (opcode0) {
          //1101 mov
          state.registers[rd] = oprand2;
        } else {
          //1100 orr
          state.registers[rd] = (result = state.registers[rn] | oprand2);
        }
      }
    } else {
      if (opcode1) {
        if (opcode0) {
          //1011
        } else {
          //1010 cmp
          result = state.registers[rn] - oprand2;
          if (set)
            checkBorrow(state.registers[rn], oprand2, result, state);
        }
      } else {
        if (opcode0) {
          //1001 teq
          result = state.registers[rn] ^ oprand2;
        } else {
          //1000 tst
          result = state.registers[rn] & oprand2;
        }
      }
    }
  } else {
    if (opcode2) {
      if (opcode1) {
        if (opcode0) {
          //0111
        } else {
          //0110
        }
      } else {
        if (opcode0) {
          //0101
        } else {
          //0100 add
          state.registers[rd] = (result = state.registers[rn] + oprand2);
          if (set)
            checkOverflow(state.registers[rn], oprand2, result, state);
        }
      }
    } else {
      if (opcode1) {
        if (opcode0) {
          //0011 rsb
          state.registers[rd] = (result = oprand2 - state.registers[rn]);
          if (set)
            checkBorrow(oprand2, state.registers[rn], result, state);
        } else {
          //0010 sub
          state.registers[rd] = (result = state.registers[rn] - oprand2);
          if (set)
            checkBorrow(state.registers[rn], oprand2, result, state);
        }
      } else {
        if (opcode0) {
          //0001 eor
          state.registers[rd] = (result = state.registers[rn] ^ oprand2);
        } else {
          //0000 and
          state.registers[rd] = (result = state.registers[rn] & oprand2);
        }
      }
    }
  }

  // Set N and Z flags;
  if (set) {
    setUnset(N, result >> 31, state);
    setUnset(Z, result == 0, state);
  }
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

//If tests do not work, try casting shifted ints instead of directly assigning types?
void singleDataTransfer(State state, uint32_t instr)
{
  printf("This is an SDT instruction\n");
  if (checkInstrCond(state, instr)) {
    uint16_t offset = (instr << 20) >> 4;
    uint32_t RnRdOffset = (instr << 12) >> 12;
    uint8_t RnRd = RnRdOffset >> 12;
    uint8_t Rn = RnRd >> 4;
    uint8_t Rd = (RnRd << 4) >> 4;
    if (getInstrBit(instr, 25) == 1) {
      // I = 1; interpret offset as shifted register
      treatAsShiftRegister(state, instr);
    } else {
      // I = 0; interpret offset as unsigned immediate offset
    }
    if (getInstrBit(instr, 24) == 1) {
      // P = 1; offset is added/subtracted to base register before transferring data
    } else {
      // P = 0; offset is added/subtracted to base register after transferring data
    }
    if (getInstrBit(instr, 23) == 1) {
      // U = 1; offset added to base register
    } else {
      // U = 0; offset subtracted from base register
    }

    if (getInstrBit(instr, 20) == 1) {
      // L = 1; word loaded from memory
    } else {
      // L = 0; word stored into memory
    }
  }
}

void branchDataTransfer(State state, uint32_t instr)
{
  printf("This is a branch instruction\n");
  if (checkInstrCond(state, instr)) {
     int32_t offset = instr & 0xFFFFFF;
     offset = offset << 2;
     int checkSign = offset >> 25;
     if (checkSign) {
        offset = offset | 0xFC000000;
     }
     state.registers[PC] += offset;
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

  //printMemoryComposition(memory, procSize);
  //printRegisterComposition(registers);

  /*for (int i = 0; i <= 16; i++) {
    uint32_t cpsrState = 0x10000000 * i;
    condChecks(registers, cpsrState);
  }*/

  /*
  uint32_t getInstrBitTest = 0x00000001;
  for (int i = 31; i >= 0; i--) {
      printf("%d", getInstrBit(getInstrBitTest, i));
  }

  uint32_t imm = 0x000000FF;
  uint8_t rotate = 0x1;
  uint32_t rotated = imm >> rotate | imm << (32 - rotate);
  printf("%" PRIu32 "\n", imm);

*/

  /* End of debugging*/

  free(state.memory);
  free(state.registers);
  fclose(proc);

  return EXIT_SUCCESS;
}
