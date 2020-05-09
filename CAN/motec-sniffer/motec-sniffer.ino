#include "mcp_can.h"

// Use pin 10 for CAN shield version 1.2
#define SPI_CS_PIN 9
// NOTE: Base Address for MOTEC M150 CAN is 0x07F0

MCP_CAN CAN(SPI_CS_PIN);

unsigned long id;
unsigned char buf[8];
unsigned char len;

void setup() {
  Serial.begin(115200);
  while (CAN_OK != CAN.begin(CAN_500KBPS)) {
    Serial.println("CAN INIT FAIL");
    Serial.println("TRY AGAIN");
    delay(1000);
  }
  Serial.println("Initialization Success");
}

void loop() {
  if (CAN_MSGAVAIL == CAN.checkReceive()) {
    CAN.readMsgBuf(&len, buf);
    id = CAN.getCanId();
    /* Serial.print("Getting Data from ID: 0x"); */
    /* Serial.println(id, HEX); */
    /* for (int i = 0; i < len; i++) { */
    /*   Serial.print(buf[i]); */
    /*   Serial.print("\t"); */
    /* } */
    /* Serial.println(); */
    if (id == 0x7F0) {
      float voltage = buf[2] * 0.1216;
      Serial.print("VOLTAGE: ");
      Serial.println(voltage);

      Serial.print("FAN STATUS ACTIVE: ");
      Serial.println(buf[0]);

      Serial.print("PDM GLOBAL ERROR: ");
      Serial.println(buf[1]);
    } else if (id == 0x118) {
      /* Serial.print("RPM: "); */
      /* Serial.println(buf[0] * 100); */

      /* Serial.print("TPS: "); */
      /* Serial.println(buf[1]); */

      /* Serial.print("SPEED (km/h): "); */
      /* Serial.println(buf[2]); */

      /* Serial.print("COOLANT TEMP: "); */
      /* Serial.println(buf[3]); */

      /* Serial.print("OIL TEMP: "); */
      /* Serial.println(buf[4]); */

      /* Serial.print("FUEL TEMP: "); */
      /* Serial.println(buf[5]); */

      /* Serial.print("TRANSMISSION TEMP: "); */
      /* Serial.println(buf[6]); */

      /* Serial.print("DIFFERENTIAL TEMP: "); */
      /* Serial.println(buf[7]); */
    } else if (id == 0x119) {
      /* Serial.print("FUEL PRESSURE (delta-kPa): "); */
      /* Serial.println(buf[0] * 10); */

      /* Serial.print("STEERING ANGLE: "); */
      /* Serial.println(buf[2] * 256 + buf[3]); */

      /* Serial.print("ENGINE STATE: "); */
      /* Serial.println(buf[4] & 0x1); */

      /* Serial.print("WARNING SOURCE: "); */
      /* Serial.println((buf[4] & 0x2) >> 1); */

      /* Serial.print("BRAKE STATE: "); */
      /* Serial.println((buf[4] & 0x4) >> 2); */

      /* Serial.print("ENGINE STATE: "); */
      /* Serial.println((buf[4] & 0x8) >> 3); */

      /* Serial.print("GEAR NEUTRAL SWITCH: "); */
      /* Serial.println((buf[4] & 0x10) >> 4); */

      /* Serial.print("CLUTCH SWITCH: "); */
      /* Serial.println((buf[4] & 0x20) >> 5); */

      /* Serial.print("CLUTCH STATE: "); */
      /* Serial.println((buf[4] & 0x40) >> 6); */

      /* Serial.print("DRIVER PIT SWITCH: "); */
      /* Serial.println((buf[4] & 0x80) >> 7); */

      /* Serial.print("ENGINE RUN SWITCH: "); // May be switched with engine
       * state */
      /* Serial.println((buf[5] & 0x1)); */

      /* Serial.print("DRIVER SWITCH 1"); */
      /* Serial.println((buf[5] & 0x2) >> 1); */

      /* Serial.print("DRIVER SWITCH 2"); */
      /* Serial.println((buf[5] & 0x4) >> 2); */

      /* Serial.print("DRIVER ROTARY SWITCH 1"); */
      /* Serial.println(buf[6]); */

      /* Serial.print("DRIVER ROTARY SWITCH 2"); */
      /* Serial.println(buf[7]); */
    }
  }
}
