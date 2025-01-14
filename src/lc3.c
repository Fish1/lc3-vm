#include "stdio.h"
#include "stdlib.h"

#include "./hardware/condition_flags.c"
#include "./hardware/memory.c"
#include "./hardware/opcodes.c"
#include "./hardware/register_store.c"

uint16_t sign_extend(uint16_t x, int bit_count) {
  if ((x >> (bit_count - 1)) & 1) {
    x |= (0xFFFF << bit_count);
  }
  return x;
}

void update_flags(uint16_t r) {
  if (reg[r] == 0) {
    reg[R_COND] = FL_ZRO;
  } else if (reg[r] >> 15) { // 1 in the left most bit indicates negative
    reg[R_COND] = FL_NEG;
  } else {
    reg[R_COND] = FL_POS;
  }
}

void add(uint16_t instr) {
  uint16_t r0 = (instr >> 9) & 0x7;
  uint16_t r1 = (instr >> 6) & 0x7;
  uint16_t imm_flag = (instr >> 5) & 0x1;

  if (imm_flag) {
    uint16_t imm5 = sign_extend(instr & 0x1f, 5);
    reg[r0] = reg[r1] + imm5;
  } else {
    uint16_t r2 = instr & 0x7;
    reg[r0] = reg[r1] + reg[r2];
  }

  update_flags(r0);
}

int main(int argc, const char *argv[]) {

  // check if there are at least 2 arguments
  if (argc < 2) {
    printf("lc3 [image-file1] ...\n");
    exit(2);
  }

  // load the image
  for (int j = 1; j < argc; ++j) {
    if (!read_image(argv[j])) {
      printf("failed to load image: %s\n", argv[j]);
      exit(1);
    }
  }

  // setup

  // set the z flag
  reg[R_COND] = FL_ZRO;

  // set the PC to starting position
  // x3000 is the default
  enum { PC_START = 0x3000 };
  reg[R_PC] = PC_START;

  int running = 1;
  while (running) {
    // fetch instruction
    uint16_t instr = mem_read(reg[R_PC]++);
    uint16_t op = instr >> 12;

    switch (op) {
    case OP_ADD:
      break;
    case OP_AND:
      break;
    case OP_NOT:
      break;
    default:
      // bad opcode
      break;
    }
  }

  // shutdown
}
