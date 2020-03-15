// COBS Testbench
// The sketch tests the COBS (Consistent Overhead Byte Stuffing) library
// By: Ethan Peterson
// March 15, 2020

#include "fcobs.h"

#define BAUD_RATE 1000000

// True for the transmit code to run
#define SENDING 1

// Length of a data packet without COBS overhead in bytes
#define BODY_LENGTH 4

// initialize the send buffer with some test data
uint8_t sendBuf[BODY_LENGTH] = {11, 22, 0, 33};
uint8_t encodedBuf[BODY_LENGTH + 2];

void printBuf(uint8_t *buf, uint8_t len) {
  for (int i = 0; i < len; i++) {
    Serial.print(buf[i]);
    Serial.print('\t');
  }
  Serial.println();
}

void setup() {
  Serial.begin(BAUD_RATE);
  Serial.println("Starting");
  Serial.println("Encoding The Data: ");
  printBuf(sendBuf, BODY_LENGTH);
  fcobs_encode(sendBuf, encodedBuf, BODY_LENGTH);
  Serial.println("Encoded Data: ");
  printBuf(encodedBuf, BODY_LENGTH + 2);
}

void loop() {
  if (SENDING) {
    // write the encoded buf to UART
  } else {
    // do a decode of serial data here
  }
}
