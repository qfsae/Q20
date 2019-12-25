/*
  Testing Sketch for the QCAN Library
  Library will build atop the CAN shield library to retrieve ECU and PDM
  messages This sketch will use the underlying library to make a CAN sniffer
  which will print the ECU and PDM messages

  TODO: Formalize this into a MOCK ECU class which can take a custom data struct
  updated in a real time from another source such as number gen or racing sim
  NOTE: This is complete spaghetti code that I have simply using to test my
  library's to read the simulated ECU messages as I go along
*/

#include "DEFS.h"
#include "ECU.h"
#define SENDING 0
// Boolean to indicate we are using extended CAN Addresses
#define EXTENDED 1
#define SPI_CS_PIN 9

MCP_CAN CAN(SPI_CS_PIN);
ECU ECU;

void setup() {
  Serial.begin(115200);
  while (CAN_OK != CAN.begin(CAN_250KBPS)) {
    Serial.println("CAN INIT FAIL");
    Serial.println("TRY AGAIN");
    delay(10000);
  }
  Serial.println("Initialization Success");
  if (SENDING) {
    Serial.print("SENDING ADDR = ");
    Serial.println(PE1, HEX);
  }
  ECU.begin(CAN);
}

void testPE1() {
  int fakeRPM = 200;
  int fakeTPS = 50;
  int fakeOpenTime = 70;
  int fakeAngle = 200;
  unsigned char testMessage[8];
  testMessage[0] = fakeRPM;
  testMessage[2] = fakeTPS;
  testMessage[4] = fakeOpenTime;
  testMessage[6] = fakeAngle;
  CAN.sendMsgBuf(PE1, 1, 8, testMessage);
}

void printPE1() {
  Serial.print("RPM = ");
  Serial.print(ECU.rpm);

  Serial.print(", TPS = ");
  Serial.print(ECU.tps);

  Serial.print(", OPEN_TIME = ");
  Serial.print(ECU.openTime);

  Serial.print(", IGNIT_ANGLE = ");
  Serial.println(ECU.ignitionAngle);
}

void testPE2() {
  int fakePressure = 200;
  int fakeMAP = 200;
  int fakeLambda = 100;
  // 0 - psi, 1 - kpa
  int fakeUnit = 1;
  unsigned char message[8];
  message[0] = fakePressure;
  message[2] = fakeMAP;
  message[4] = fakeLambda;
  message[6] = fakeUnit;
  message[7] = 0;
  CAN.sendMsgBuf(PE2, 1, 8, message);
}

void printPE2() {
  Serial.print("pressure = ");
  Serial.print(ECU.pressure);

  Serial.print(", MAP = ");
  Serial.print(ECU.MAP);

  Serial.print(", LAMBDA = ");
  Serial.print(ECU.lambda);

  Serial.print(", UNIT = ");
  Serial.println(ECU.pressureUnit);
}

void testPE3() {
  unsigned char message[8];
  message[0] = 200;
  message[2] = 100;
  message[4] = 50;
  message[6] = 100;
  message[7] = 0;
  CAN.sendMsgBuf(PE3, 1, 8, message);
}

void printPE3() {
  Serial.print("INPUT 1 = ");
  Serial.print(ECU.analogInputs[0]);

  Serial.print(", INPUT 2 = ");
  Serial.print(ECU.analogInputs[1]);

  Serial.print(", INPUT 3 = ");
  Serial.print(ECU.analogInputs[2]);

  Serial.print(", INPUT 4 = ");
  Serial.println(ECU.analogInputs[3]);
}

void testPE4() {
  unsigned char message[8];
  message[0] = 200;
  message[2] = 100;
  message[4] = 50;
  message[6] = 25;
  message[7] = 0;
  CAN.sendMsgBuf(PE4, 1, 8, message);
}

void printPE4() {
  Serial.print("INPUT 5 = ");
  Serial.print(ECU.analogInputs[4]);

  Serial.print(", INPUT 6 = ");
  Serial.print(ECU.analogInputs[5]);

  Serial.print(", INPUT 7 = ");
  Serial.print(ECU.analogInputs[6]);

  Serial.print(", INPUT 8 = ");
  Serial.println(ECU.analogInputs[7]);
}

void testPE5() {
  unsigned char message[8];
  for (int i = 0; i < 8; i++) {
    message[i] = 0;
  }
  message[0] = 200;
  message[2] = 100;
  message[4] = 50;
  message[6] = 25;
  CAN.sendMsgBuf(PE5, 1, 8, message);
}
void printPE5() {
  Serial.print("INPUT 1 = ");
  Serial.print(ECU.frequencies[0]);

  Serial.print(", INPUT 2 = ");
  Serial.print(ECU.frequencies[1]);

  Serial.print(", INPUT 3 = ");
  Serial.print(ECU.frequencies[2]);

  Serial.print(", INPUT 4 = ");
  Serial.println(ECU.frequencies[3]);
}

void loop() {
  if (SENDING) {
    // SEND FAKE MESSAGES HERE
    testPE5();
  } else {
    ECU.update();
    printPE5();
  }
}
