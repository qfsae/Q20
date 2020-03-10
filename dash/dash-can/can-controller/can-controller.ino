#include "mcp_can.h"
#include <SPI.h>

<<<<<<< HEAD
#define BODY_LENGTH 4
=======
#define BODY_LENGTH 8
>>>>>>> 43b08cf1f50674faea6cce34ea99dde48f36b35a

#define SPI_CS_PIN 10

MCP_CAN CAN(SPI_CS_PIN);

unsigned char msg[8] = {0, 0, 0, 0, 0, 0, 0, 0};

int endMarker = -20000;
char *c = (char *)&endMarker;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting");
  while (CAN.begin(CAN_500KBPS) != CAN_OK) {
    Serial.println("CAN BUS init failure");
    Serial.println("Trying again");
    delay(100);
  }
  Serial.println("CAN Bus Initialized!");

<<<<<<< HEAD
  // add endmarkers to message
  int endMarker = -20000;
  char *c = (char *)&endMarker;

  msg[4] = c[0];
  msg[5] = c[1];

  msg[0] = 200;
  msg[1] = 200;
=======
>>>>>>> 43b08cf1f50674faea6cce34ea99dde48f36b35a
}

void writeMsg() {
  for (int i = 0; i < BODY_LENGTH; i++) {
    Serial.write(msg[i]);
  }
  Serial.write(c[0]);
  Serial.write(c[1]);
}

void loop() {
<<<<<<< HEAD
  unsigned char len = 0;
  unsigned char buf[8];
  if (CAN_MSGAVAIL == CAN.checkReceive()) {
    CAN.readMsgBuf(&len, buf);
    unsigned long id = CAN.getCanId();
    // for testing sending just battery voltage is fine
    // but more will need to be added
    if (id == 0x7F0) {
      msg[2] = buf[2];
      writeMsg();
      //Serial.println("GOT BAT VOlTAGE");
    }
  }
  //delay(75);
=======
    unsigned char len = 0;
    if (CAN_MSGAVAIL == CAN.checkReceive()) {
      CAN.readMsgBuf(&len, msg);
      unsigned long id = CAN.getCanId();
     if (id == 0x7F0) {
     	// msg[2] is the battery voltage divided by 0.1216
	writeMsg();
     }
   }
>>>>>>> 43b08cf1f50674faea6cce34ea99dde48f36b35a
}
