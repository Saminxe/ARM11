#include "emulate.h"

/*
██████╗ ██╗   ██╗████████╗███████╗     ██████╗ ██████╗ ██████╗ ███████╗██████╗     ██╗███████╗    ██╗     ██╗████████╗████████╗██╗     ███████╗    ███████╗███╗   ██╗██████╗ ██╗ █████╗ ███╗   ██╗
██╔══██╗╚██╗ ██╔╝╚══██╔══╝██╔════╝    ██╔═══██╗██╔══██╗██╔══██╗██╔════╝██╔══██╗    ██║██╔════╝    ██║     ██║╚══██╔══╝╚══██╔══╝██║     ██╔════╝    ██╔════╝████╗  ██║██╔══██╗██║██╔══██╗████╗  ██║
██████╔╝ ╚████╔╝    ██║   █████╗      ██║   ██║██████╔╝██║  ██║█████╗  ██████╔╝    ██║███████╗    ██║     ██║   ██║      ██║   ██║     █████╗      █████╗  ██╔██╗ ██║██║  ██║██║███████║██╔██╗ ██║
██╔══██╗  ╚██╔╝     ██║   ██╔══╝      ██║   ██║██╔══██╗██║  ██║██╔══╝  ██╔══██╗    ██║╚════██║    ██║     ██║   ██║      ██║   ██║     ██╔══╝      ██╔══╝  ██║╚██╗██║██║  ██║██║██╔══██║██║╚██╗██║
██████╔╝   ██║      ██║   ███████╗    ╚██████╔╝██║  ██║██████╔╝███████╗██║  ██║    ██║███████║    ███████╗██║   ██║      ██║   ███████╗███████╗    ███████╗██║ ╚████║██████╔╝██║██║  ██║██║ ╚████║
╚═════╝    ╚═╝      ╚═╝   ╚══════╝     ╚═════╝ ╚═╝  ╚═╝╚═════╝ ╚══════╝╚═╝  ╚═╝    ╚═╝╚══════╝    ╚══════╝╚═╝   ╚═╝      ╚═╝   ╚══════╝╚══════╝    ╚══════╝╚═╝  ╚═══╝╚═════╝ ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝
*///

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
  } while (i < MAX_MEMORY_ADDRESS);
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
int checkInstrCond(State state, uint32_t instr)
{
  uint8_t cond = instr >> 28;
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
  return 0;
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

// Loads 32-bit digit from memory address
uint32_t load(State state, uint32_t select) {
  if (0 <= select && select <= MAX_MEMORY_ADDRESS) {
    return state.memory[select] |
      state.memory[select + 1] << 8 |
      state.memory[select + 2] << 16 |
      state.memory[select + 3] << 24;
  } else if (select == 0x20200000) {
    printf("One GPIO pin from 0 to 9 has been accessed\n");
  } else if (select == 0x20200004) {
    printf("One GPIO pin from 10 to 19 has been accessed\n");
  } else if (select == 0x20200008) {
    printf("One GPIO pin from 20 to 29 has been accessed\n");
  }
  return select;
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
    uint8_t amount = 0;
    if (!getInstrBit(instr,4)) {
      // Bit 4 = 0; Shift by a constant amount.
      amount = (instr & 0x00000F80) >> 7;
    } else {
      // Bit 4 = 1; Shift specified by the bottom byte of Rs.
      uint8_t rs = (instr & 0x00000F00) >> 8;
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
  int acc = (instr & 0x00200000) >> 21;
  int set = (instr & 0x00100000) >> 20;
  uint8_t rd = (instr & 0x000F0000) >> 16;
  uint8_t rn = (instr & 0x0000F000) >> 12;
  uint8_t rs = (instr & 0x00000F00) >> 8;
  uint8_t rm = (instr & 0x0000000F);
  state.registers[rd] = state.registers[rm] * state.registers[rs];
  if (acc) {
    state.registers[rd] += state.registers[rn];
  }
  if (set) {
    setUnset(N, state.registers[rd] >> 31, state);
    setUnset(Z, state.registers[rd] == 0, state);
  }
}

// Return 1 if address out of bound, 0 otherwise.
int checkMemoryBounds(uint32_t address) {
  if (address > MAX_MEMORY_ADDRESS &&
    address != 0x20200000 &&
    address != 0x20200004 &&
    address != 0x20200008 &&
    address != 0x2020001C &&
    address != 0x20200028) {
    printf("Error: Out of bounds memory access at address 0x%08x\n", address);
    return 1;
  }
  return 0;
}

void singleDataTransfer(State state, uint32_t instr) {
  uint8_t Rn = (instr & 0x000F0000) >> 16; // Rn
  uint8_t Rd = (instr & 0x0000F000) >> 12; // Rd
  int I = getInstrBit(instr, 25);
  int P = getInstrBit(instr, 24);
  int U = getInstrBit(instr, 23);
  int L = getInstrBit(instr, 20);
  uint32_t offset = 0;
  uint32_t tempReg = state.registers[Rn];
  if (I) {
    // I = 1; interpret offset as shifted register
    uint8_t rm = instr & 0x0000000F;
    uint32_t value = state.registers[rm];
    uint8_t amount = 0x0;
    if (!getInstrBit(instr,4)) {
      // Bit 4 = 0; Shift by a constant amount.
      amount = (instr & 0x00000F80) >> 7;
    } else {
      // Bit 4 = 1; Shift specified by the bottom byte of Rs.
      uint8_t rs = (instr & 0x00000F00) >> 8;
      amount = state.registers[rs] & 0x000000FF;
    }
    offset = applyShiftType(value, instr, amount, 0, state);
  } else {
    // I = 0; interpret offset as unsigned immediate offset
    offset = instr & 0x00000FFF;
  }
  if (U) {
    // U = 1; offset added to base register
    tempReg += offset;
  } else {
    // U = 0; offset subtracted from base register
    tempReg -= offset;
  }
  // printf("Tempreg: %u\n", tempReg);
  // Check addresses are not out of bound
  if (!(checkMemoryBounds(tempReg) || checkMemoryBounds(state.registers[Rn]))) {
    if (L) {
      // L = 1; word loaded from memory
      if (P) {
        // P = 1; offset is added/subtracted to base register before transferring data
        state.registers[Rd] = load(state, tempReg);
      } else {
        // P = 0; offset is added/subtracted to base register after transferring data
        state.registers[Rd] = load(state, state.registers[Rn]);
        state.registers[Rn] = tempReg;
      }
    } else if (P) {
      // L = 0; word stored into memory
      // P = 1; offset is added/subtracted to base register before transferring data
      if (tempReg == 0x20200000) {
        printf("One GPIO pin from 0 to 9 has been accessed\n");
        return;
      } else if (tempReg == 0x20200004) {
        printf("One GPIO pin from 10 to 19 has been accessed\n");
        return;
      } else if (tempReg == 0x20200008) {
        printf("One GPIO pin from 20 to 29 has been accessed\n");
        return;
      } else if (tempReg == 0x20200028) {
        printf("PIN OFF\n");
        return;
      } else if (tempReg == 0x2020001C) {
        printf("PIN ON\n");
        return;
      }
      state.memory[tempReg] = state.registers[Rd] & 0x000000FF;
      state.memory[tempReg + 1] = (state.registers[Rd] & 0x0000FF00) >> 8;
      state.memory[tempReg + 2] = (state.registers[Rd] & 0x00FF0000) >> 16;
      state.memory[tempReg + 3] = (state.registers[Rd] & 0xFF000000) >> 24;

    } else {
      // P = 0; offset is added/subtracted to base register after transferring data
      if (state.registers[Rn] == 0x20200000) {
        printf("One GPIO pin from 0 to 9 has been accessed\n");
        state.registers[Rn] = tempReg;
        return;
      } else if (state.registers[Rn] == 0x20200004) {
        printf("One GPIO pin from 10 to 19 has been accessed\n");
        state.registers[Rn] = tempReg;
        return;
      } else if (state.registers[Rn] == 0x20200008) {
        printf("One GPIO pin from 20 to 29 has been accessed\n");
        state.registers[Rn] = tempReg;
        return;
      } else if (state.registers[Rn] == 0x20200028) {
        printf("PIN OFF\n");
        state.registers[Rn] = tempReg;
        return;
      } else if (state.registers[Rn] == 0x2020001C) {
        printf("PIN ON\n");
        state.registers[Rn] = tempReg;
        return;
      }
      state.memory[state.registers[Rn]] = state.registers[Rd] & 0x000000FF;
      state.memory[state.registers[Rn] + 1] = (state.registers[Rd] & 0x0000FF00) >> 8;
      state.memory[state.registers[Rn] + 2] = (state.registers[Rd] & 0x00FF0000) >> 16;
      state.memory[state.registers[Rn] + 3] = (state.registers[Rd] & 0xFF000000) >> 24;
      state.registers[Rn] = tempReg;
    }
  }
}

void branchDataTransfer(State state, uint32_t instr)
{
  int32_t offset = instr & 0xFFFFFF;
  offset = offset << 2;
  int checkSign = offset >> 25;
  if (checkSign) {
    offset = offset | 0xFC000000;
  }
  state.registers[PC] += offset;
}

/*** Pipeline ***/
int process(State state)
{
  int isFetched = 0;
  int isDecoded = 0;
  uint32_t fetched = 1;
  uint32_t decoded = 1;
  do {
    if (!isFetched && !isDecoded) {
      isFetched++;
    } else if (isFetched && !isDecoded) {
      decoded = fetched;
      isDecoded++;
    } else if (isFetched && isDecoded) {
      if (checkInstrCond(state, decoded)) {
        if (patternMatcher(decoded, 0x0A000000, 0x0F000000)) {
          branchDataTransfer(state, decoded);
          isDecoded = 0;
        } else {
          if (patternMatcher(decoded, 0x00000090, 0x0FC00090))
            multiply(state, decoded);
          else if (patternMatcher(decoded, 0x00000000, 0x0C000000))
            dataProcess(state, decoded);
          else if (patternMatcher(decoded, 0x04000000, 0x0C600000))
            singleDataTransfer(state, decoded);
          else {printf("not a valid instruction u schmuck\n"); return 1;};
          decoded = fetched;
        }
      } else {
        decoded = fetched;
      }
    }
    fetched = load(state, state.registers[PC]);
    state.registers[PC] += 4;
  } while (state.registers[PC] < MAX_MEMORY_ADDRESS && decoded != 0);
  return 0;
}

int main(int argc, char **argv)
{
  State state = {(uint8_t *) calloc(MAX_MEMORY_ADDRESS, sizeof(char)),
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
  fread(state.memory, sizeof(uint8_t), procSize, proc); // Read the file into memory
  succ = process(state); // Call the pipeline loop
  /* Part 3:
   * write mem[0x2020 0004] | 0x0004 0000 into memory address 0x2020 0004‬ to set gpio 16 to output
   * 0x2020 0028 to clear, 0x2020 001C to set (turn on)
   * clear gpio 16: write 0x‭8000‬ into 0x2020 0028, to turn on write 0x8000 to 0x2020 001C
   */
  output(state); // Print the result of the pipeline
  free(state.memory); // Free the memory
  free(state.registers); // Free the registers
  fclose(proc); // Close the file
  return succ; // Return success
}
