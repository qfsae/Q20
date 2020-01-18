#include "mcp_can.h"

#define SPI_CS_PIN 9

MCP_CAN CAN(SPI_CS_PIN);

void setup() {
//  while (CAN.begin(CAN_250KBPS) != CAN_OK) {
//    Serial.println("CAN BUS init failure");
//    Serial.println("Trying again");
//    delay(100);
//  }
//  Serial.println("CAN Bus Initialized!")
Serial.begin(115200);
Serial.write(5);
Serial.write('\n');
Serial.write(5);
Serial.write('\n');
}

void loop() {

//   unsigned char len = 0;
//   unsigned char buf[8];
//   if (CAN_MSGAVAIL == CAN.checkReceive()) {
//     CAN.readMsgBuf(&len, buf);
//     unsigned long id = CAN.getCanId();
//     Serial.print("Getting Data from ID: ");
//     Serial.println(id, HEX);

//     for (int i = 0; i < len; i++) {
//       Serial.print(buf[i]);
//       Serial.print("\t");
//     }
//     Serial.println();
//   }  
  
  // new line character will terminate messages
}