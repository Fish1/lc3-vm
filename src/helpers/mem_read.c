#include "stdint.h"

#include "../hardware/memory.c"

uint16_t mem_read(uint16_t addr) { return memory[addr]; }
