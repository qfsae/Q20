#ifndef FCOBS_h
#define FCOBS_h
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * encode a byte buffer into a cobs packet to send over UART
 *
 * @param data     A byte buffer of the raw information that will be encoded
 *                 using COBS.
 * @param dest     The destination byte buffer for the encoded bytes.
 * @param data_len the length of the data buffer
 */
uint8_t fcobs_encode(uint8_t *data, uint8_t *dest, uint8_t data_len);

/**
 * decode a byte buffer of COBS encoded data into the original data
 *
 * @param data     The COBS encoded data.
 * @param dest     The destination buffer for the decoded data.
 * @param data_len The length of the encoded buffer.
 */
uint8_t fcobs_decode(uint8_t *data, uint8_t *dest, uint8_t data_len);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
