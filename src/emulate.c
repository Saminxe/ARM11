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
  printf("CPSR: %x\n", state.registers[CPSR]);
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

void output(State state)
{
  int i = 0;
  uint32_t huiyi = 0xffffff;
  printf("Registers:\n");
  for (int j = 0; j < 10; j++)
    printf("$%d  : %10d (0x%08x)\n", j, state.registers[j], state.registers[j]);
  for (int j = 10; j < 13; j++)
      printf("$%d : %10d (0x%08x)\n", j, state.registers[j], state.registers[j]);
  printf("PC  : %10d (0x%08x)\n", state.registers[PC], state.registers[PC]);
  printf("CPSR: %10d (0x%08x)\n", state.registers[CPSR], state.registers[CPSR]);
  printf("Non-zero memory:\n");
  do {
    huiyi = state.memory[i] << 24 | state.memory[i+1] << 16
      | state.memory[i+2] << 8 | state.memory[i+3];
    if (huiyi != 0)
      printf("0x%08x: 0x%08x\n", i, huiyi);
    i += 4;
  } while (i < 65536);
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
uint32_t applyShiftType(uint32_t value, uint32_t instr, uint8_t amount, int set, State state) {
  if(!(0 <= amount && amount <= 32)) printf("Amount: %d/n", amount);
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
  if (set)
    setUnset(C, carry, state);
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

uint32_t endianConversion(uint32_t num) {
  return ((num >> 24) & 0xff) |
      ((num << 8) & 0xff0000) |
      ((num >> 8) & 0xff00) |
      ((num << 24) & 0xff000000);
}

/*** Processing Instructions ***/
/* All data processing instructions take the base address of the
  memory and registers, and the instruction as arguments */

void dataProcess(State state, uint32_t instr)
{
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
    uint8_t rm = instr & 0x0000000F;
    uint32_t value = state.registers[rm];
    uint8_t amount = 0x0;
    if (!getInstrBit(instr,4)) {
      // Bit 4 = 0; Shift by a constant amount.
      amount = instr & 0x00000F80;
    } else {
      // Bit 4 = 1; Shift specified by a register.
      uint8_t rs = instr & 0x00000F00;
      amount = state.registers[rs] & 0x000000FF;
    }
    oprand2 = applyShiftType(value, instr, amount, set, state);
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
          //printf("%d\n", result);
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
  //printf("This is a multiply instruction\n");
  int acc = (instr & 0x00200000) >> 21;
  int set = (instr & 0x00100000) >> 20;
  uint8_t rd = (instr & 0x000F0000) >> 16;
  uint8_t rn = (instr & 0x0000F000) >> 12;
  uint8_t rs = (instr & 0x00000F00) >> 8;
  uint8_t rm = (instr & 0x0000000F);
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
  //printf("This is an SDT instruction\n");
  uint8_t Rn = (instr & 0x000F0000) >> 16; // Rn
  uint8_t Rd = (instr & 0x0000F000) >> 12; // Rd
  uint32_t offset = 0;
  if (Rn == PC) {
    Rn = state.registers[PC] + 8;
  }
  if (getInstrBit(instr, 25) == 1) {
    // I = 1; interpret offset as shifted register
    uint8_t rm = instr & 0x0000000F;
    uint32_t value = state.registers[rm];
    uint8_t amount = 0x0;
    if (!getInstrBit(instr,4)) {
      // Bit 4 = 0; Shift by a constant amount.
      amount = instr & 0x00000F80;
    } else {
      // Bit 4 = 1; Shift specified by the bottom byte of Rs.
      uint8_t rs = instr & 0x00000F00;
      amount = state.registers[rs] & 0x000000FF;
    }
    offset = applyShiftType(value, instr, amount, 0, state);
  } else {
    // I = 0; interpret offset as unsigned immediate offset
    offset = instr & 0x00000FFF;
  }
  //printf("Offset: %u\n", offset);
  uint32_t tempReg = state.registers[Rn];
  //printf("Before: %u\n", tempReg);
  if (getInstrBit(instr, 23) == 1) {
    // U = 1; offset added to base register
    tempReg += offset;
  } else {
    // U = 0; offset subtracted from base register
    tempReg -= offset;
  }
  //printf("After: %u\n", tempReg);
  if (getInstrBit(instr, 20) == 1) {
    // L = 1; word loaded from memory
    if (getInstrBit(instr, 24) == 1) {
      // P = 1; offset is added/subtracted to base register before transferring data
      if (tempReg > 65536) printf("Error: Out of bounds memory access at address 0x%08x\n", tempReg);
      else {
        state.registers[Rd] = state.memory[tempReg] |
          state.memory[tempReg + 1] << 8 |
          state.memory[tempReg + 2] << 16 |
          state.memory[tempReg + 3] << 24;
      }
    } else {
      // P = 0; offset is added/subtracted to base register after transferring data
      if (state.registers[Rn] > 65536) printf("Error: Out of bounds memory access at address 0x%08x\n", state.registers[Rn]);
      else {
        state.registers[Rd] = state.memory[state.registers[Rn]] |
          state.memory[state.registers[Rn] + 1] << 8 |
          state.memory[state.registers[Rn] + 2] << 16 |
          state.memory[state.registers[Rn] + 3] << 24;
        state.registers[Rn] = tempReg;
      }
    }
  } else {
    // L = 0; word stored into memory
    if (getInstrBit(instr, 24) == 1) {
      // P = 1; offset is added/subtracted to base register before transferring data
      if (tempReg > 65536) printf("Error: Out of bounds memory access at address 0x%08x\n", tempReg);
      else
        state.memory[tempReg] = state.registers[Rd] & 0x000000FF;
        state.memory[tempReg + 1] = (state.registers[Rd] & 0x0000FF00) >> 8;
        state.memory[tempReg + 2] = (state.registers[Rd] & 0x00FF0000) >> 16;
        state.memory[tempReg + 3] = (state.registers[Rd] & 0xFF000000) >> 24;
    } else {
      // P = 0; offset is added/subtracted to base register after transferring data
      if (state.registers[Rn] > 65536) printf("Error: Out of bounds memory access at address 0x%08x\n", state.registers[Rn]);
      else {
        state.memory[state.registers[Rn]] = state.registers[Rd] & 0x000000FF;
        state.memory[state.registers[Rn] + 1] = (state.registers[Rd] & 0x0000FF00) >> 8;
        state.memory[state.registers[Rn] + 2] = (state.registers[Rd] & 0x00FF0000) >> 16;
        state.memory[state.registers[Rn] + 3] = (state.registers[Rd] & 0xFF000000) >> 24;
      }
    }
  }
}

void branchDataTransfer(State state, uint32_t instr)
{
  //printf("This is a branch instruction\n");
  int32_t offset = instr & 0xFFFFFF;
  offset = offset << 2;
  int checkSign = offset >> 25;
  if (checkSign) {
    offset = offset | 0xFC000000;
  }
  //printf("%d\n", offset);
  state.registers[PC] += offset;
  //state.registers = (uint32_t*) PC;
}

/*** Pipeline ***/
int process(State state)
{
  const uint32_t DATA_PROCESS_PATTERN = 0x00000000;
  const uint32_t DATA_PROCESS_MASK = 0x0C000000;
  const uint32_t MULTIPLY_PATTERN = 0x00000090;
  const uint32_t MULTIPLY_MASK = 0x0FC00090;
  const uint32_t SDT_PATTERN = 0x04000000;
  const uint32_t SDT_MASK = 0x0C600000;
  const uint32_t BRANCH_PATTERN = 0x0A000000;
  const uint32_t BRANCH_MASK = 0x0F000000;
  int isFetched = 0;
  int isDecoded = 0;
  uint32_t fetched = 0;
  uint32_t decoded = 1;

  do {
    if (!isFetched && !isDecoded) {
      fetched = state.memory[state.registers[PC]] |
        state.memory[state.registers[PC] + 1] << 8 |
        state.memory[state.registers[PC] + 2] << 16 |
        state.memory[state.registers[PC] + 3] << 24;
      state.registers[PC] += 4;
      isFetched++;
    } else if (isFetched && !isDecoded) {
      decoded = fetched;
      isDecoded++;
      fetched = state.memory[state.registers[PC]] |
        state.memory[state.registers[PC] + 1] << 8 |
        state.memory[state.registers[PC] + 2] << 16 |
        state.memory[state.registers[PC] + 3] << 24;
      state.registers[PC] += 4;
    } else if (isFetched && isDecoded) {
      if (checkInstrCond(state, decoded)) {
        if (patternMatcher(decoded, BRANCH_PATTERN, BRANCH_MASK)) {
          branchDataTransfer(state, decoded);
          isDecoded = 0;
        } else {
          if (patternMatcher(decoded, MULTIPLY_PATTERN, MULTIPLY_MASK))
            multiply(state, decoded);
          else if (patternMatcher(decoded, DATA_PROCESS_PATTERN, DATA_PROCESS_MASK))
            dataProcess(state, decoded);
          else if (patternMatcher(decoded, SDT_PATTERN, SDT_MASK))
            singleDataTransfer(state, decoded);
          else {printf("not a valid instruction u schmuck\n"); return 1;};
          decoded = fetched;
        }
        fetched = state.memory[state.registers[PC]] |
          state.memory[state.registers[PC] + 1] << 8 |
          state.memory[state.registers[PC] + 2] << 16 |
          state.memory[state.registers[PC] + 3] << 24;
        state.registers[PC] += 4;
      } else {
        decoded = fetched;
        fetched = state.memory[state.registers[PC]] |
          state.memory[state.registers[PC] + 1] << 8 |
          state.memory[state.registers[PC] + 2] << 16 |
          state.memory[state.registers[PC] + 3] << 24;
        state.registers[PC] += 4;
      }
    }
    //printf("%x\n", instr);
  } while (state.registers[PC] < 65536 && decoded != 0);
  return 0;
}

int main(int argc, char **argv)
{
  State state = {(uint8_t *) calloc(65536, sizeof(char)),
      (uint32_t *) calloc(17, sizeof(long))}; // Allocate the machine state
  FILE *proc; // Allocate the stream to be read
  int procSize; // Initialize the size of the file
  int succ = 0;
  if (argc != 2) {
    printf("Usage = %s filename\n", argv[0]);
    return EXIT_FAILURE;
  } // Check the correct number of arguments
  proc = fopen(argv[1], "rb");
  if (proc == NULL) {
    printf("File Load Failure\n");
    return EXIT_FAILURE;
  } // Check the file isn't null
  fseek(proc, 0, SEEK_END);
  procSize = ftell(proc); // Get the size of the file
  fseek(proc, 0, SEEK_SET); // Reset the seeker
  fread(state.memory, sizeof(uint8_t), procSize, proc); // Read the file inbto memory
  succ = process(state); // Call the pipeline loop
  output(state); // Print the result of the pipeline
  free(state.memory); // Free the memory
  free(state.registers); // Free the registers
  fclose(proc); // Close the file
  return succ; // Return success
}
