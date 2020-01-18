// CAN Testbench
// runs between two CAN shields and adjusts the brightness of an LED
// connected to the receiver based off potentiometer readings from the sender

#include "mcp_can.h"
#include <SPI.h>

#define SPI_CS_PIN 9

// Set to 0 to run receiver code
#define SENDING 0

// LED and Potentiometer Definitions
#define LED_CATHODE 7
#define LED_PWM 6

#define POT_VCC A0
#define POT_WIPER A1
#define POT_GND A2

MCP_CAN CAN(SPI_CS_PIN);

void setup() {
  Serial.begin(115200);

  // SETUP LED FOR OUTPUT
  pinMode(LED_CATHODE, OUTPUT);
  pinMode(LED_PWM, OUTPUT);
  digitalWrite(LED_CATHODE, LOW);

  // SETUP POTENTIOMETER FOR BRIGHTNESS READ
  pinMode(POT_VCC, OUTPUT);
  pinMode(POT_WIPER, INPUT); // WIPER PIN
  pinMode(POT_GND, OUTPUT);
  digitalWrite(POT_VCC, HIGH);
  digitalWrite(POT_GND, LOW);

  while (CAN.begin(CAN_500KBPS) != CAN_OK) {
    Serial.println("CAN BUS init failure");
    Serial.println("Trying again");
    delay(100);
  }
  Serial.println("CAN Bus Initialized!");
}

void loop() {
  unsigned char len = 0;

  // messages have max length of 8 bytes
  unsigned char buf[8];

  // Check if there is a message available
  if (!SENDING && CAN_MSGAVAIL == CAN.checkReceive()) {
    CAN.readMsgBuf(&len, buf);
    unsigned long id = CAN.getCanId();
    Serial.print("Getting Data from ID: ");
    Serial.println(id, HEX);

    for (int i = 0; i < len; i++) {
      Serial.print(buf[i]);
      Serial.print("\t");
    }
    // Brightness of LED depends on potentiometer position on the sending CAN
    // node
    Serial.print("writing to LED: ");
    Serial.println(buf[0]);
    analogWrite(LED_PWM, buf[0]);
    Serial.print("\n");
    Serial.println("END OF MESSAGE");
  } else {
    unsigned char message[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    unsigned long sendingID = 0x00;
    unsigned char reading = map(analogRead(POT_WIPER), 0, 1023, 0, 255);
    Serial.println(reading);
    message[0] = reading;
    CAN.sendMsgBuf(sendingID, 0, 8, message);
    delay(100);
  }
}
