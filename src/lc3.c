#include "stdio.h"
#include "stdlib.h"

#include "./hardware/memory.c"
#include "./hardware/opcodes.c"

#include "./helpers/mem_read.c"
#include "./helpers/read_image.c"
#include "./helpers/sign_extend.c"
#include "./helpers/update_flags.c"

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
