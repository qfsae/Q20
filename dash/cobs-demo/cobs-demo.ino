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

uint8_t sendBuf[BODY_LENGTH] = {11, 22, 0, 33};
uint8_t encodedBuf[BODY_LENGTH + 2];

void setup() {
  Serial.begin(BAUD_RATE);
  Serial.println("Starting");
}

void loop() {
  if (SENDING) {
    fcobs_encode(sendBuf, encodedBuf, 4);
    // write the encoded buf to UART
  } else {
    // do a decode of serial data here
  }
}
