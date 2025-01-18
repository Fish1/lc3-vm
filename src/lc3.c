#include "signal.h"
#include "stdio.h"
#include "stdlib.h"

#include "./hardware/controller.c"
#include "./hardware/opcodes.c"

#include "./helpers/read_image.c"

#include "./operations/add.c"
#include "./operations/ld.c"
#include "./operations/ldi.c"
#include "./operations/st.c"
#include "./operations/trap.c"

#include "./platform/linux/disable_input_buffering.c"
#include "./platform/linux/restore_input_buffering.c"

void handle_interrupt(int signal) {
  restore_input_buffering();
  printf("\n");
  exit(-2);
}

int main(int argc, const char *argv[]) {

  signal(SIGINT, handle_interrupt);
  disable_input_buffering();

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

  while (running) {
    // fetch instruction
    uint16_t instr = mem_read(reg[R_PC]++);
    uint16_t op = instr >> 12;

    switch (op) {
    case OP_ADD:
      add(instr);
      break;
    case OP_LDI:
      ldi(instr);
      break;
    case OP_LD:
      ld(instr);
      break;
    case OP_ST:
      st(instr);
      break;
    case OP_TRAP:
      trap(instr);
      break;
    default: // bad op code
      printf("BAD OPCODE");
      abort();
      break;
    }
  }

  // shutdown
  restore_input_buffering();
}
