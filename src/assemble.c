#include "assemble.h"

/* Debug */
void printSymtab(SymbolTable s)
{
  int i = 0;
  do {
    printf("%s, %u\n", s.symbols[i].name, s.symbols[i].val);
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

void printInstruction(OpCode opcode, Condition cond, char **args)
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
  else if (cond == AL) printf("al ");
  else printf("ERROR ");
  do {
    printf("%s", args[i]);
    i++;
  } while (i < expectedArgs);
}

/* Debug End */

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
  else return -1;
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

Condition getCondition(char *cond)
{
  if (equals(cond, "eq")) return EQ;
  else if (equals(cond, "ne")) return NE;
  else if (equals(cond, "ge")) return GE;
  else if (equals(cond, "lt")) return LT;
  else if (equals(cond, "gt")) return GT;
  else if (equals(cond, "le")) return LE;
  else if (equals(cond, "al")) return AL;
  else return ER;
}

OpCode getOpcode(char* opcode)
{
  char temp[3];
  strcpy(temp, opcode);
  if (equals(temp, "add")) return ADD;
  else if (equals(temp, "sub")) return SUB;
  else if (equals(temp, "rsb")) return RSB;
  else if (equals(temp, "and")) return AND;
  else if (equals(temp, "eor")) return ORR;
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
      printf("%s\n", temp);
      return getCondition(temp);
    }
  } else if (type == ERRO) {
    printf("%s\n", "ERRO");
    return ERR;
  } else {
    if (strlen(opcode) == 3) {
      return AL;
    } else {
      strcpy(temp, opcode + 3);
      printf("%s\n", temp);
      return getCondition(temp);
    }
  }
  return AL;
}

int main(int argc, char **argv) {
  FILE *src;
  FILE *dest;
  char *buffer = calloc(BUFFER_SIZE, sizeof(char));
  uint32_t instruction = 0; // Temporary instruction
  uint32_t locctr = 0; // Starting address for memory allocations
  SymbolTable symtab = {0, calloc(DEFAULT_MAP_SIZE, sizeof(Symbol))};
  int size = 0; // Current pointer for symtab

  /* Tests for correct amount of input variables */
  if (argc != 3) {
    fprintf(stderr, "Usage = %s input output\n", argv[0]);
    return EXIT_FAILURE;
  }

  /* Opens the files */
  src = fopen(argv[1], "r");
  dest = fopen(argv[2], "wb");

  /* Checks the read file exists */
  if (src == NULL) {
    fprintf(stderr, "File Load Failure\n");
    return EXIT_FAILURE;
  }

  /* Symbol Tablulation Loop */
  while (fgets(buffer, BUFFER_SIZE, src) != NULL) {
    char *opcode = strtok(buffer, " ");
    char *operands = strtok(NULL, " ");
    if (operands != NULL) {
      operands[strlen(operands) - 1] = 0;
    }
    if (*opcode != ';') { // If it is not a comment
      if (opcode[strlen(opcode) - 2] == ':') { // If it is a label
        opcode[strlen(opcode) - 2] = 0;
        printf("Label = %s\n", opcode);
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
        if (typeCheck(opcode) != ERRO) {
          printf("Optype: %u, Condition: %u\n", typeCheck(opcode), condCheck(opcode));
          locctr += 32;
        }
      }
    }
  }

  fseek(src, 0, SEEK_SET); // Reset file the pointer
  locctr = 0; // Resets the location counter

  printSymtab(symtab);


  /* Translation Loop */
  while (fgets(buffer, BUFFER_SIZE, src) != NULL) {
    instruction = 0; // Resets the instruction;
    char *_opcode = strtok(buffer, " ");
    OpCode opcode = getOpcode(_opcode);
    char *optemp = strtok(NULL, " ");
    char *operands[4]; // max 4 operands
    int expected = noOfArgs(opcode);
    char *operand = strtok(optemp, ", ");
    for (int i = 0; i < expected; i++) {
      assert(operand != NULL);
      if (operand[0] == ';') {
        operands[i] = operand;
        operand = strtok(NULL, ", ");
      }
    }

    // TODO: Convert labels into memory addresses here using SYMTAB

    if ((0 <= opcode && opcode <= 5) || opcode == 15) {
      instruction = compute(opcode, operands[0], operands[1], operands[2]);
    }
    else if (opcode == 6) {
      instruction = move(operands[0], operands[1]);
    }
    else if (7 <= opcode && opcode <= 9) {
      instruction = flagger(opcode, operands[0], operands[1]);
    }
    else if (opcode == 10) {
      instruction = mul(operands[0], operands[1], operands[2]);
    }
    else if (opcode == 11) {
      instruction = mla(operands[0], operands[1], operands[2], operands[3]);
    }
    else if (12 <= opcode && opcode <= 13) {
      instruction = sdt(opcode, operands[0], operands[1], locctr);
    }
    else if (opcode == 14) {
      instruction = branch(operands[0], locctr);
    }
    else return EXIT_FAILURE;
    //instruction || 0; // TODO: or with conditions here

    uint8_t instr_array[4];
    instr_array[0] = 0xFF & instruction;
    instr_array[1] = (0xFF00 & instruction) >> 8;
    instr_array[2] = (0xFF0000 & instruction) >> 16;
    instr_array[3] = (0xFF000000 & instruction) >> 24;

    fwrite(instr_array, sizeof(uint8_t), 4, dest);

    locctr += 32;
  }


  fclose(src);
  fclose(dest);
  free(buffer);
}


/*** Lookup Functions ***/
int symtabContains(SymbolTable m, char *k) {
  for (int i = 0; i < m.size; i++) {
    if (m.symbols[i].name == k)
      return 1;
  }
  return 0;
}



uint8_t getKeyVal(SymbolTable m, char *k) {
  for (int i = 0; i < m.size; i++) {
    if (m.symbols[i].name == k)
      return m.symbols[i].val;
  }
  return -1;
}


//don't touch this Sam is writing bad code

//check if strings are equals
//switch statement on char array
//return the machine code

int equals(char *a, char *b) {
  int length = strlen(a);
  int length2 = strlen(b);
  if (length != length2) {
    return 0;
  }
  for (int i = 0; i < length; i++) {
    if (a[i] != b[i]) return 0;
  }
  return 1;
}


//the code below is subject to change depending if we actually need to keep track of
// the instruction format, available addressing modes and length information

//OPTAB (operation code table)
int optab(char* a)
{
  if(equals(a, "add")) {
    return 0x800000;
  } else if (equals(a, "sub")) {
    return 0x400000;
  } else if (equals(a, "rsb")) {
    return 0x600000;
  } else if (equals(a, "and")) {
    return 0x0;
  } else if (equals(a, "eor")) {
    return 0x200000;
  } else if (equals(a, "orr")) {
    return 0x1800000;
  } else if (equals(a, "mov")) {
    return 0x1A00000;
  } else if (equals(a, "tst")) {
    return 0x1000000;
  } else if (equals(a, "teq")) {
    return 0x1200000;
  } else if (equals(a, "cmp")) {
    return 0x1400000;
  } else if (equals(a, "mul")) {
    return 0xE0000090;
  } else if (equals(a, "mla")) {
    return 0xE0200090;
  } else if (equals(a, "ldr")) {
    return 0x4100000;
  } else if (equals(a, "str")) {
    return 0x4000000;
  } else if (equals(a, "b")) {
    return 0xEA000000;
  } else if (equals(a, "lsl")) {
    return 0x1A00000;           //unsure
  }
  return -1;
}

int optabCheck(char* a)
{
  if(optab(a) != -1) {
    return 1;
  }
  return 0;
}

/*** FILL OUT YOUR OWN INSTRUCTIONS ください　どうもありがとうございました。***/
/*** Data Processing Instructions ***/
uint32_t compute(OpCode opcode, char *rd, char *rn, char *operand2)
{
  // TODO: return 28-bit instruction for add, eor, sub, rsb, add, orr
  return 0;
}

uint32_t move(char *rd, char *operand2)
{
  // TODO: return 28-bit instruction for mov
  return 0;
}

uint32_t flagger(OpCode opcode, char *rn, char *operand2)
{
  // TODO: return 28-bit instruction for tst, teq, cmp
  return 0;
}

uint32_t shift(char *rn, char *expression)
{
  // TODO: return 28-bit instruction for lsl
  return 0;
}

/*** Multiply Instructions ***/
uint32_t mul(char *rd, char *rm, char *rs)
{
  // TODO: return 28-bit instruction for mul
  return 0;
}

uint32_t mla(char *rd, char *rm, char *rs, char *rn)
{
  // TODO: return 28-bit instruction for mla
  return 0;
}

/*** Single Data Transfer Instructions ***/
uint32_t sdt(OpCode opcode, char *rd, char *address, int locctr)
{
  // TODO: return 28-bit instruction for ldr, str
  // locctr is the location of the instruction in memory, hence = PC - 8.
  return 0;
}

/*** Branch Instructions ***/
uint32_t branch(char *address, int locctr)
{
  // TODO: return 28-bit instruction for ldr, str
  // address will be fed in from the symbol table.
  // locctr is the location of the instruction in memory, hence = PC - 8.
  return 0;
}
