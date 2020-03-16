volatile uint8_t fcobs_decode(volatile uint8_t *data, volatile uint8_t *dest, volatile uint8_t data_len) {
  const volatile uint8_t *start = dest, *end = data + data_len;
  volatile uint8_t code = 0xFF, copy = 0;

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
