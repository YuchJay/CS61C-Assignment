#include "lfsr.h"
#include <stdint.h>

void lfsr_calculate(uint16_t *reg) {
  uint16_t feedback = ((*reg >> 0) & 1) ^ ((*reg >> 2) & 1) ^
                      ((*reg >> 3) & 1) ^ ((*reg >> 5) & 1);

  *reg = (*reg >> 1) | (feedback << 15);
}
