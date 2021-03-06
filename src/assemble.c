#include "assemble.h"

/*** Debugging tools ***/
void printSymtab(SymbolTable s)
{
  int i = 0;
  do {
    printf("%s, 0x%x\n", s.symbols[i].name, s.symbols[i].val);
    i++;
  } while (i < s.size);
}

Type getType(OpCode op)
{
  if (0 <= op && op <= 9) return DATA;
  else if (10 <= op && op <= 11) return MULT;
  else if (12 <= op && op <= 13) return SDTR;
  else if (op == 14) return BRCH;
  else if (op == 15) return DATA;
  else return ERRO;
}

int noOfArgs(OpCode op)
{
  if (0 <= op && op <= 5) return 3;
  else if (6 <= op && op <= 9) return 2;
  else if (op == 10) return 3;
  else if (op == 11) return 4;
  else if (12 <= op && op <= 13) return 2;
  else if (op == 14) return 1;
  else if (op == 15) return 2;
  else return -1;
}

void printInstruction(OpCode opcode, Condition cond, char **args, int set)
{
  int expectedArgs = noOfArgs(opcode);
  int i = 0;
  if (opcode == ADD) printf("add");
  else if (opcode == SUB) printf("sub");
  else if (opcode == RSB) printf("rsb");
  else if (opcode == AND) printf("and");
  else if (opcode == EOR) printf("eor");
  else if (opcode == ORR) printf("orr");
  else if (opcode == MOV) printf("mov");
  else if (opcode == TST) printf("tst");
  else if (opcode == TEQ) printf("teq");
  else if (opcode == CMP) printf("cmp");
  else if (opcode == MUL) printf("mul");
  else if (opcode == MLA) printf("mla");
  else if (opcode == LDR) printf("ldr");
  else if (opcode == STR) printf("str");
  else if (opcode == B) printf("b");
  else if (opcode == LSL) printf("lsl");
  else printf("ERROR ");
  if (cond == EQ) printf("eq ");
  else if (cond == NE) printf("ne ");
  else if (cond == GE) printf("ge ");
  else if (cond == LT) printf("lt ");
  else if (cond == GT) printf("gt ");
  else if (cond == LE) printf("le ");
  else if (cond == AL) printf(" ");
  else printf("ERROR ");
  do {
    printf("%s ", args[i]);
    i++;
  } while (i < expectedArgs);
  if (set) printf(" SET CPSR");
  printf("\n");
}
/*** End of debugging tools ***/

int contains(char *string, char c) {
  int length = strlen(string);
  for (int i = 0; i < length; i++) {
    if (string[i] == c) return 1;
  }
  return 0;
}

int getRegister(char *reg)
{
  if (equals(reg, "r0")) return 0;
  else if (equals(reg, "r1")) return 1;
  else if (equals(reg, "r2")) return 2;
  else if (equals(reg, "r3")) return 3;
  else if (equals(reg, "r4")) return 4;
  else if (equals(reg, "r5")) return 5;
  else if (equals(reg, "r6")) return 6;
  else if (equals(reg, "r7")) return 7;
  else if (equals(reg, "r8")) return 8;
  else if (equals(reg, "r9")) return 9;
  else if (equals(reg, "r10")) return 10;
  else if (equals(reg, "r11")) return 11;
  else if (equals(reg, "r12")) return 12;
  else if (equals(reg, "sp")) return 13;
  else if (equals(reg, "lr")) return 14;
  else if (equals(reg, "pc")) return 15;
  else if (equals(reg, "cpsr")) return 16;
  else {
    printf("register does not exist\n");
    return -1;
  }
}

Type typeCheck(char *opcode) {
  char temp[5];
  strcpy(temp, opcode);
  temp[3] = 0;
  if (equals(temp, "add")
    || equals(temp, "sub")
    || equals(temp, "rsb")
    || equals(temp, "and")
    || equals(temp, "eor")
    || equals(temp, "orr")
    || equals(temp, "mov")
    || equals(temp, "tst")
    || equals(temp, "teq")
    || equals(temp, "cmp")
    || equals(temp, "lsl"))
    return DATA;
  else if (equals(temp, "mul") || equals(temp, "mla"))
    return MULT;
  else if (equals(temp, "ldr") || equals(temp, "str"))
    return SDTR;
  temp[1] = 0;
  if (equals(temp, "b")) return BRCH;
  return ERRO;
}

int hasCondition(char *opcode)
{
  return (
    opcode[strlen(opcode) - 1] != 's' &&
    !(*opcode == 'b' && strlen(opcode) == 1) &&
    !(*opcode != 'b' && strlen(opcode) == 3)
  );
}

Condition getCondition(char *opcode)
{
  if (hasCondition(opcode)) {
    char cond[2];
    strcpy(cond, opcode + strlen(opcode) - 2); // last two chars of opcode
    cond[2] = 0;
    if (equals(cond, "eq")) return EQ;
    else if (equals(cond, "ne")) return NE;
    else if (equals(cond, "ge")) return GE;
    else if (equals(cond, "lt")) return LT;
    else if (equals(cond, "gt")) return GT;
    else if (equals(cond, "le")) return LE;
    else if (equals(cond, "al")) return AL;
    else return ER;
  } else {
    return AL;
  }
}

OpCode getOpcode(char* opcode)
{
  char temp[3];
  strcpy(temp, opcode);
  temp[3] = 0;
  if (equals(temp, "add")) return ADD;
  else if (equals(temp, "sub")) return SUB;
  else if (equals(temp, "rsb")) return RSB;
  else if (equals(temp, "and")) return AND;
  else if (equals(temp, "eor")) return EOR;
  else if (equals(temp, "orr")) return ORR;
  else if (equals(temp, "mov")) return MOV;
  else if (equals(temp, "tst")) return TST;
  else if (equals(temp, "teq")) return TEQ;
  else if (equals(temp, "cmp")) return CMP;
  else if (equals(temp, "mul")) return MUL;
  else if (equals(temp, "mla")) return MLA;
  else if (equals(temp, "ldr")) return LDR;
  else if (equals(temp, "str")) return STR;
  else if (equals(temp, "lsl")) return LSL;
  else {
    temp[1] = 0;
    if (equals(temp, "b")) return B;
    else return ERR;
  }
}

Condition condCheck(char* opcode)
{
  char temp[2];
  Type type = typeCheck(opcode);
  if (type == BRCH) {
    if (strlen(opcode) == 1) {
      return AL;
    } else {
      strcpy(temp, opcode + 1);
      return getCondition(temp);
    }
  } else if (type == ERRO) {
    return ERR;
  } else {
    if (strlen(opcode) == 3) {
      return AL;
    } else {
      strcpy(temp, opcode + 3);
      return getCondition(temp);
    }
  }
  return AL;
}

char *isLabel(char *string)
{
  char _string[DEFAULT_STRLEN];
  strcpy(_string, string);
  if (contains(_string, ':')) {
    char *label = strtok(_string, ":");
    return label;
  } else {
    return 0;
  }
}

uint32_t conditionToBinary(Condition cond)
{
  if (cond == EQ) return 0x0;
  else if (cond == NE) return 0x10000000;
  else if (cond == GE) return 0xA0000000;
  else if (cond == LT) return 0xB0000000;
  else if (cond == GT) return 0xC0000000;
  else if (cond == LE) return 0xD0000000;
  else if (cond == AL) return 0xE0000000;
  else return 0xFFFFFFFF; // error
}

int isSet(char *opcode)
{
  if (hasCondition(opcode)) {
    if (strlen(opcode) == 6 || strlen(opcode) == 4) {
      return (opcode[strlen(opcode) - 3] == 's');
    }
  } else {
    if (strlen(opcode) == 4 || strlen(opcode) == 2) {
      return (opcode[strlen(opcode) - 1] == 's');
    }
  }
  return 0;
}

int main(int argc, char **argv) {
  FILE *src;
  FILE *dest;
  char *buffer = calloc(BUFFER_SIZE, sizeof(char));
  uint32_t instruction = 0; // Temporary instruction
  uint32_t locctr = 0; // Starting address for memory allocations
  SymbolTable symtab = {0, calloc(DEFAULT_MAP_SIZE, sizeof(Symbol))};
  int size = 0; // Current pointer for symtab
  static Buffer post_buffer = {0};

  // Tests for correct amount of input variables
  if (argc != 3) {
    fprintf(stderr, "Usage = %s input output\n", argv[0]);
    free(buffer);
    free(symtab.symbols);
    return EXIT_FAILURE;
  }

  // Opens the files
  src = fopen(argv[1], "r");
  dest = fopen(argv[2], "wb");

  // Checks the read file exists
  if (src == NULL) {
    fprintf(stderr, "File Load Failure\n");
    free(buffer);
    free(symtab.symbols);
    return EXIT_FAILURE;
  }

  // Symbol Tabulation Loop
  while (fgets(buffer, BUFFER_SIZE, src) != NULL) {
    char *_opcode = strtok(buffer, " ");
    char *operands = strtok(NULL, " ");
    if (operands != NULL) {
      operands[strlen(operands) - 1] = 0;
    }
    if (*_opcode != ';') { // If it is not a comment
      if (contains(_opcode, ':')) { // If it is a label
        char opcode[DEFAULT_STRLEN];
        strcpy(opcode, isLabel(_opcode));
        if (symtabContains(symtab, opcode)) { // If the label is in symbtab
          fprintf(stderr, "Duplicate symbol, aborting assembly.\n");
          return -1;
        } else {
          strcpy(symtab.symbols[size].name, opcode);
          symtab.symbols[size].val = locctr;
          size++;
          symtab.size = size;
        }
      } else {
        if (typeCheck(_opcode) != ERRO) {
          locctr += INSTRUCTION_WIDTH;
        }
      }
    }
  }

  const int program_end = locctr;
  fseek(src, 0, SEEK_SET); // Reset file the pointer
  locctr = 0; // Resets the location counter

  printf("SYMBOL TABLE:\n");
  printSymtab(symtab);
  printf("\n");

  // Translation Loop
  while (fgets(buffer, BUFFER_SIZE, src) != NULL) {
    instruction = 0; // Resets the instruction;
    char *_opcode = strtok(buffer, " ");
    if (*_opcode != ';' && *_opcode > 32) {
      if (contains(_opcode, ':')) {
        char opcode[DEFAULT_STRLEN];
        strcpy(opcode, isLabel(_opcode));
        uint32_t labelAddress = getKeyVal(symtab, opcode);
        printf("%s has address 0x%x\n", opcode, labelAddress);
      } else {
        OpCode opcode = getOpcode(_opcode);
        Condition cond = getCondition(_opcode);
        int set = isSet(_opcode);
        char *_operands = strtok(NULL, "\n");
        char *operands[4];
        operands[0] = strtok(_operands, ", ");
        for (int i = 1; i < noOfArgs(opcode) - 1; i++) {
          operands[i] = strtok(NULL, ", ");
        }

        if (noOfArgs(opcode) >= 2) {
          char *final = strtok(NULL, "\n;");
          int shift = 0;
          if (*final == ' ') shift = 1;
          operands[noOfArgs(opcode) - 1] =  final + shift;
          // magic 1 saves the day again
        }

        printInstruction(opcode, cond, operands, set);

        if (0 <= opcode && opcode <= 5) {
          instruction = compute(opcode, operands[0], operands[1], operands[2], set);
        }
        else if (opcode == 6) {
          instruction = move(operands[0], operands[1], set);
        }
        else if (7 <= opcode && opcode <= 9) {
          instruction = flagger(opcode, operands[0], operands[1]);
        }
        else if (opcode == 10) {
          instruction = mul(operands[0], operands[1], operands[2], set);
        }
        else if (opcode == 11) {
          instruction = mla(operands[0], operands[1], operands[2], operands[3], set);
        }
        else if (12 <= opcode && opcode <= 13) {
          uint64_t transfer = sdt(opcode, operands[0], operands[1],  locctr, program_end, post_buffer);
          if (transfer & (0xFFFFFFFF00000000)) {
            uint32_t buffervalue = (transfer & (0xFFFFFFFF00000000)) >> 32;
            instruction = transfer & 0xFFFFFFFF;
            post_buffer.buffer[post_buffer.size] = buffervalue;
            post_buffer.size++;
          } else {
            instruction = transfer & 0xFFFFFFFF;
          }
        }
        else if (opcode == 14) {
          instruction = branch(operands[0], locctr, symtab);
        }
        else if (opcode == 15) {
          char _op2[80];
          strcpy(_op2, operands[0]);
          strcat(_op2, ", lsl ");
          strcat(_op2, operands[1]);
          printf("%s\n", _op2);
          instruction = move(operands[0], _op2, set);
        }
        else return EXIT_FAILURE;

        instruction |= conditionToBinary(cond);

        uint8_t instr_array[4];
        instr_array[0] = 0xFF & instruction;
        instr_array[1] = (0xFF00 & instruction) >> 8;
        instr_array[2] = (0xFF0000 & instruction) >> 16;
        instr_array[3] = (0xFF000000 & instruction) >> 24;

        fwrite(instr_array, sizeof(uint8_t), 4, dest);

        locctr += INSTRUCTION_WIDTH;
      }
    }
  }

  printf("%u\n", post_buffer.size);

  for (int i = 0; i < post_buffer.size; i++)
  {
    uint32_t instruction = post_buffer.buffer[i];
    printf("%x\n", instruction);
    uint8_t instr_array[4];
    instr_array[0] = 0xFF & instruction;
    instr_array[1] = (0xFF00 & instruction) >> 8;
    instr_array[2] = (0xFF0000 & instruction) >> 16;
    instr_array[3] = (0xFF000000 & instruction) >> 24;
    fwrite(instr_array, sizeof(uint8_t), 4, dest);
  }

  fclose(src);
  fclose(dest);
  free(buffer);
  free(symtab.symbols);
  return EXIT_SUCCESS;
}


/*** Lookup Functions ***/
int symtabContains(SymbolTable m, char *k) {
  for (int i = 0; i < m.size; i++) {
    if (m.symbols[i].name == k)
      return 1;
  }
  return 0;
}

uint32_t getKeyVal(SymbolTable m, char *k) {
  for (int i = 0; i < m.size; i++) {
    if (equals(m.symbols[i].name, k))
      return m.symbols[i].val;
  }
  return -1;
}

int equals(char *a, char *b) {
  return (strcmp(a, b) == 0);
}

uint32_t circularShift(int lr, uint32_t input, int n)
{
  const unsigned int mask = (CHAR_BIT*sizeof(input) - 1);
  n &= mask;
  if (lr) {
    return (input << n) | (input >> ((-n) & mask));
  } else {
    return (input >> n) | (input << ((-n) & mask));
  }
}

uint32_t getRotate(uint32_t input)
{
  uint8_t rots = 0;
  while (rots <= 30) {
    if (input & (~0xFF)) {
      input = circularShift(1, input, 2);
      rots += 2;
    } else break;
  }
  if (rots > 30) return -1;
  return (input | (rots << 7)) & 0xFFF;
}

Shift getShift(char *shift)
{
  if (equals(shift, "lsl")) return ShSL;
  else if (equals(shift, "lsr")) return ShLSR;
  else if (equals(shift, "asr")) return ShASR;
  else if (equals(shift, "ror")) return ShROR;
  else return ShERR;
}

uint32_t processOperand2(char *operand2)
{
  uint32_t result = 0;
  if (*operand2 == '#') {
    char *ptr;
    long int imm = strtol(operand2 + 1, &ptr, 0);
    printf("%ld\n", imm);
    result |= (getRotate(imm) & 0xFFF);
    result |= (1 << 25);
  } else {
    char rm[3];
    char shift[DEFAULT_STRLEN];
    sscanf(operand2, "%[^, ] %*[,] %[^\n]", rm, shift);
    uint8_t Rm = getRegister(rm);
    printf("%s = %d\n", rm, Rm);
    if (*shift > 32) { //if it is not trash
      char _shift_type[3];
      Shift shift_type;
      char _shift_operand[DEFAULT_STRLEN];
      long int shift_operand;
      char *shiftptr;
      sscanf(shift, "%[^ ] %s", _shift_type, _shift_operand);
      shift_type = getShift(_shift_type);
      switch (shift_type) {
        case ShSL: break;
        case ShLSR: result |= 1 << 5; break;
        case ShASR: result |= 1 << 6; break;
        case ShROR: result |= 3 << 5; break;
        default: result |= 0xFFFFFFFF; break;
      }
      if (*_shift_operand == '#') {
        shift_operand = strtol(_shift_operand + 1, &shiftptr, 0);
        result |= (shift_operand & 0x1F) << 7;
      } else {
        shift_operand = getRegister(_shift_operand);
        result |= (shift_operand << 8) | (1 << 4);
      }
    }
    result |= Rm;
  }
  printf("%x\n", result);
  return result;
}

uint32_t processExpression(char *expression)
{
  printf("Expression: %s\n", expression);
  uint32_t result = 0;
  if (*expression == '#') {
    char *ptr;
    long int imm = strtol(expression + 1, &ptr, 0);
    printf("%ld\n", imm);
    if (imm < 0) {
      imm = abs(imm);
      result |= imm;
    } else {
      result |= 1 << 23; // u = 1;
      result |= imm;
    }
  } else {
    char rm[3];
    char shift[DEFAULT_STRLEN];
    sscanf(expression, "%[^, ] %*[,] %[^\n]", rm, shift);
    uint8_t Rm = getRegister(rm);
    printf("%s = %d\n", rm, Rm);
    if (*shift > 32) { //if it is not trash
      char _shift_type[3];
      Shift shift_type;
      char _shift_operand[DEFAULT_STRLEN];
      long int shift_operand;
      char *shiftptr;
      sscanf(shift, "%[^ ] %s", _shift_type, _shift_operand);
      shift_type = getShift(_shift_type);
      switch (shift_type) {
        case ShSL: break;
        case ShLSR: result |= 1 << 5; break;
        case ShASR: result |= 1 << 6; break;
        case ShROR: result |= 3 << 5; break;
        default: result |= 0xFFFFFFFF; break;
      }
      if (*_shift_operand == '#') {
        shift_operand = strtol(_shift_operand + 1, &shiftptr, 0);
        result |= (shift_operand & 0x1F) << 7;
      } else {
        shift_operand = getRegister(_shift_operand);
        result |= (shift_operand << 8) | 1;
      }
    }
    result |= Rm;
    result |= 1 << 25; // i = 1;
  }
  return result;
}

/*** Data Processing Instructions ***/
uint32_t compute(OpCode opcode, char *rd, char *rn, char *operand2, int set)
{
  uint32_t instruction = 0;
  uint32_t op2 = 0;
  uint8_t opc;
  switch (opcode) {
    case AND: opc = 0x0; break;
    case EOR: opc = 0x1; break;
    case SUB: opc = 0x2; break;
    case RSB: opc = 0x3; break;
    case ADD: opc = 0x4; break;
    case ORR: opc = 0xC; break;
    default: opc = 0xF; break;
  }
  op2 = processOperand2(operand2);
  uint8_t Rd = getRegister(rd);
  uint8_t Rn = getRegister(rn);
  instruction |= (opc << 21);
  instruction |= op2;
  if (set) instruction |= (1 << 20);
  instruction |= (Rd << 12);
  instruction |= (Rn << 16);
  return instruction;
}

uint32_t move(char *rd, char *operand2, int set)
{
  uint32_t instruction = 0;
  uint8_t Rd = getRegister(rd);
  uint32_t op2 = processOperand2(operand2);
  printf("%x\n", op2);
  instruction |= 0xD << 21;
  instruction |= Rd << 12;
  instruction |= op2;
  if (set) instruction |= (1 << 20);
  return instruction;
}

uint32_t flagger(OpCode opcode, char *rn, char *operand2)
{
  uint32_t instruction = 0;
  uint32_t op2 = 0;
  uint8_t opc;
  uint8_t Rn = getRegister(rn);
  switch (opcode) {
    case TST: opc = 0x8; break;
    case TEQ: opc = 0x9; break;
    case CMP: opc = 0xA; break;
    default: opc = 0xF; break;
  }
  op2 = processOperand2(operand2);
  instruction |= (opc << 21);
  instruction |= Rn << 16;
  instruction |= op2;
  instruction |= 1 << 20;
  return instruction;
}

uint32_t shift(char *rn, char *expression, int set)
{
  // TODO: return 28-bit instruction for lsl
  return 0;
}

/*** Multiply Instructions ***/
uint32_t mul(char *rd, char *rm, char *rs, int set)
{
  uint8_t Rd = getRegister(rd);
  uint8_t Rm = getRegister(rm);
  uint8_t Rs = getRegister(rs);
  uint32_t instruction = 0;
  if (Rd == -1 && Rm == -1 && Rs == -1) return -1;
  else {
    instruction |= Rm;
    instruction |= (0x9 << 4);
    instruction |= (Rs << 8);
    instruction |= (Rd << 16);
  }
  if (set)
    instruction |= (1 << 20);
  return instruction;
}

uint32_t mla(char *rd, char *rm, char *rs, char *rn, int set)
{
  uint8_t Rd = getRegister(rd);
  uint8_t Rm = getRegister(rm);
  uint8_t Rs = getRegister(rs);
  uint8_t Rn = getRegister(rn);
  uint32_t instruction = 0;
  if (Rd == -1 && Rm == -1 && Rs == -1) return -1;
  else {
    instruction |= Rm;
    instruction |= (0x9 << 4);
    instruction |= (Rs << 8);
    instruction |= (Rn << 12);
    instruction |= (Rd << 16);
    instruction |= (1 << 21);
  }
  if (set)
    instruction |= (1 << 20);
  return instruction;
}

/*** Single Data Transfer Instructions ***/

uint64_t sdt(OpCode opcode, char *rd, char *address, int locctr, const int program_end, Buffer post_buffer)
{
  int pc = locctr + (2 * INSTRUCTION_WIDTH);
  uint32_t instruction = 0;
  uint8_t Rd = getRegister(rd);
  printf("%s = %d\n", rd, Rd);
  uint8_t Rn = 0;
  int i, p, u, l;
  i = p = u = l = 0;
  int offset = 0;
  uint64_t loadvalue = 0;
  if (*address == '=') {
    i = 1;
    if (opcode == LDR) {
      char *ptr;
      uint32_t expression = strtol(address + 1, &ptr, 0);
      l = 0;
      if (expression <= 0xFF) {
        char operand2[80];
        strcpy(operand2, "#");
        strcat(operand2, address + 1);
        return move(rd, operand2, 0);
      } else {
        loadvalue = expression;
        i = 0;
        p = u = 1;
        Rn = PC;
        offset = ((INSTRUCTION_WIDTH * post_buffer.size) + program_end - pc) / 8;
      }
    } else {
      printf("Error, using immediate with STR\n");
      return -1;
    }
  } else {
    char rn[3];
    char expression[DEFAULT_STRLEN];
    if (address[3] == ']' || address[4] == ']') {
      sscanf(address, "%*[[] %[^]] %*[], ] %s ", rn, expression);
      printf("%s\n", expression);
      if (*expression > 32) {
        p = 0;
      } else {
        p = 1;
      }
    } else {
      sscanf(address, "%*[[] %[^,] %*[, ] %[^]] ", rn, expression);
      p = 1;
    }
    if (*expression == '+') {
      char _expr[DEFAULT_STRLEN];
      strcpy(_expr, expression + 1);
      strcpy(expression, _expr);
      u = 1;
    } else if (*expression == '-') {
      char _expr[DEFAULT_STRLEN];
      strcpy(_expr, expression + 1);
      strcpy(expression, _expr);
      u = 0;
    } else if (*expression == 'r' || *expression <= 32) {
      u = 1;
    }
    if (*expression > 32) offset = processExpression(expression);
    printf("%x\n", offset);
    Rn = getRegister(rn);
  }
  if (opcode == LDR) l = 1;
  instruction |= 1 << 26;
  instruction |= i << 25;
  instruction |= p << 24;
  instruction |= u << 23;
  instruction |= l << 20;
  instruction |= Rn << 16;
  instruction |= Rd << 12;
  instruction |= offset;
  return (loadvalue << 32) | instruction;
}



/*** Branch Instructions ***/
uint32_t branch(char *label, int locctr, SymbolTable symtab)
{
  uint32_t instruction = 0;
  printf("Current Locctr: %u\n", locctr);
  int32_t address = getKeyVal(symtab, label);     // this is the converted address of the label
  address = (address - (locctr + (2 * INSTRUCTION_WIDTH))) / 8;
  address = address >> 2;
  instruction |= (address & 0xFFFFFF);
  instruction |= (0xA << 24);
  // address will be fed in from the symbol table.
  // locctr is the location of the instruction in memory, hence = PC - 8.
  return instruction;
}

// if you want to be a butterfly, you have to be butterfly soup first
