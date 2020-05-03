#include <mcp_can.h>
#include <mcp_can_dfs.h>
#include <SPI.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN    6                                                 // Set NeoPixel Data Pin 
#define LED_COUNT  8                                                 // # of NeoPixels
#define SPI_CS_PIN 10                                                // Set CS Pin for CAN

MCP_CAN CAN(SPI_CS_PIN);                                             // Init CAN library
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);   // Init NeoPixel Strip

void setup() {
  /* 
   *  Set up CAN 
   */
  Serial.begin(115200);

  while (CAN_OK != CAN.begin(CAN_250KBPS))
  {
    Serial.println("CAN BUS Shield init fail");
    Serial.println("Init CAN BUS Shield again");
  }
  Serial.println("CAN BUS Shield init OK!");

  /*
   * Set up shift lights
   */
   strip.begin();
   strip.show();                                                      // Initialize all pixels to 'off' 
}

//declare status variables to retrieve from CAN
int rpm = 0;
float tps = 0;
float batVolt = 0;

//read data from CAN and update variables 
void readCAN() {
  unsigned char len = 0;
  unsigned char buf[8];
  Serial.println("In loop");

  if(CAN_MSGAVAIL == CAN.checkReceive()) 
  {
    CAN.readMsgBuf(&len, buf);                                         // read data,    len: data length;   buf: data buffer

    unsigned int canId = CAN.getCanId();

    if (canId == 0xF048)                                               // get RPM, TPS from CAN
    {
      rpm = buf[1]*256 + buf[0];
      tps = buf[3]*256 + buf[2];
      if (tps > 32767) tps -= 65536;
      tps = tps*0.1;
    }
    else if (canId == 0xF548)
    {
      batVolt = buf[1]*256 + buf[0];
      if (batVolt > 32767)  batVolt -= 65536;
      batVolt=batVolt*0.01;
//      Coolant = buf[5]*256 + buf[4];
//      if (Coolant > 32767)  Coolant -= 65536;
//      Coolant=Coolant*0.1;
    }
  }
}

//update shift lights based on rpm
void shiftLights() {                                                   // increment by 625 rpm
  if(rpm < 1000 && rpm > 500)  strip.setPixelColor(0, 0, 255, 0);      // last interval show all lights red
  if(rpm < 1500) strip.setPixelColor(1, 0, 255, 0);
  if(rpm < 2000) strip.setPixelColor(2, 0, 255, 0);
  if(rpm < 2500) strip.setPixelColor(3, 0, 255, 0);
  if(rpm < 3000) strip.setPixelColor(4, 0, 255, 0);
  if(rpm < 3500) strip.setPixelColor(5, 0, 0, 255);
  if(rpm < 4000) strip.setPixelColor(6, 0, 0, 255);
  if(rpm < 4500) strip.setPixelColor(7, 0, 0, 255);
  if(rpm < 5000) strip.setPixelColor(8, 0, 0, 255);
  if(rpm < 5500) strip.setPixelColor(9, 0, 0, 255);
  if(rpm < 6000) strip.setPixelColor(10, 255, 0, 0);
  if(rpm < 6500) strip.setPixelColor(11, 255, 0, 0);
  if(rpm < 7000) strip.setPixelColor(12, 255, 0, 0);
  if(rpm < 7500) strip.setPixelColor(13, 255, 0, 0);
  if(rpm < 8000) strip.setPixelColor(14, 255, 0, 0);
  if(rpm < 8500) batchPixels(0, 15, 255, 0, 0);

  strip.show();
}

void batchPixels(int startPixel, int endPixel, int red, int green, int blue) {
  for(int i = startPixel; i <= endPixel; i++) {
    strip.setPixelColor(i, red, green, blue);
  }
}

void loop() {
  readCAN();
  shiftLights();

  Serial.print(rpm);
  Serial.print("\t");
  Serial.print(tps);
  Serial.print("\t");
  Serial.print(batVolt);
}
