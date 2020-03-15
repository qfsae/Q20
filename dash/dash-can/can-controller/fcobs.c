#include "fcobs.h"

#define StartBlock() (code_data = dest++, code = 1)
#define FinishBlock() (*code_data = code)

uint8_t fcobs_encode(uint8_t *data, uint8_t *dest, uint8_t data_len) {
  const uint8_t *start = dest, *end = data + data_len;
  uint8_t code, *code_data; /* Where to insert the leading count */

  StartBlock();
  while (data < end) {
    if (code != 0xFF) {
      uint8_t c = *data++;
      if (c != 0) {
        *dest++ = c;
        code++;
        continue;
      }
    }
    FinishBlock();
    StartBlock();
  }
  FinishBlock();
  return dest - start;
}

uint8_t fcobs_decode(uint8_t *data, uint8_t *dest, uint8_t data_len) {
  const uint8_t *start = dest, *end = data + data_len;
  uint8_t code = 0xFF, copy = 0;

  for (; data < end; copy--) {
    if (copy != 0) {
      *dest++ = *data++;
    } else {
      if (code != 0xFF)
        *dest++ = 0;
      copy = code = *data++;
      if (code == 0)
        break; /* Source length too long */
    }
  }
  return dest - start;
}
