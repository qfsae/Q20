#include "fcobs.h"

uint8_t fcobs_encode(uint8_t *data, uint8_t *dest, uint8_t data_len) {
  uint8_t zeroIdx = 0; // where the last 0 has been found
  for (int i = 0; i < data_len; i++) {
    if (data[i] != 0) {
      dest[i + 1] = data[i];
    } else {
      dest[zeroIdx] = i - zeroIdx + 1;
      zeroIdx = i - zeroIdx + 1;
    }
  }
  dest[data_len + 1] = 0; // add delimiter
  dest[zeroIdx] = data_len - zeroIdx + 1;
  return 0;
}

uint8_t fcobs_decode(uint8_t *data, uint8_t *dest, uint8_t data_len) {
  return 0;
}
