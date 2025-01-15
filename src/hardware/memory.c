#include "stdint.h"
#define MEMORY_MAX (1 << 16)
uint16_t memory[MEMORY_MAX]; /* 675536 locations */

uint16_t mem_read(uint16_t addr) {
	return memory[addr];
}
