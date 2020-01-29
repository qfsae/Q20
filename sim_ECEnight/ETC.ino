/* ETC Stuff for Sim for ECE Night
- Need to test blips work properly
- Need to rewire tps 4 pin connector
- Recalibrate values 
- Talk over CAN to the Dash
*/

#include <PID_v1.h>
#include "mcp_can.h"
#include <SPI.h>

#define SPI_CS_PIN 9 // double check if 9 or 10 for specific shield
MCP_CAN CAN(SPI_CS_PIN);

// DOUBLE CHECK ALL OF THIS
const int TPS1Pin = A3;  //Pin of TPS1
const int TPS2Pin = A5;  //Pin of TPS2
const int torquePin = 9; //PWM pin to motor driver
const int directionPin = 4; //directionPin

int rpm;
float batVolt = 13.22;
int gear;
int engTemp = 25;

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];            // temporary array for use when parsing

boolean newData = false;

void downshift();
void upshift();

//PID tunings
const double Kp = 1;
const double Ki = 0.00001;
const double Kd = 0.00;

//ETB Constants
const int TPS1Close = 860;
const int TPS1Open = 65;
const int TPS2Close = 90;
const int TPS2Open = 971;

int direction; 
double Input;
double OutputTorque;

int TPS1;
int TPS2;
int TPSdiff;
float tps, APPS1;

PID myPID(&Input, &OutputTorque, 0, Kp, Ki, Kd, DIRECT);

void setup() {
  Serial.begin(115200);
  while (CAN.begin(CAN_500KBPS) != CAN_OK) {
    delay(100);
  }
  TCCR0B = TCCR0B & B11111000 | B00000010; // for PWM frequency of 7812.50 Hz

  // Set PID inputs
  Input = 0;
  DesiredPosition = 0;
  myPID.SetMode(AUTOMATIC);

  pinMode(TPS1Pin, INPUT);
  pinMode(TPS2Pin, INPUT);
  pinMode(directionPin, OUTPUT);
  pinMode(torquePin, OUTPUT);
  pinMode(13, OUTPUT);
  myPID.SetSampleTime(10);
}

int lastGear = 0;

void loop() {
    // Read from CAN shield. 
    if (lastGear >  gear && gear != 0){ // downshift
      downshift();
    }
    else if (lastGear < gear && gear != 0){
      upshift();
    }
  
  TPS1 = analogRead(TPS1Pin);
  Input = map(TPS1, TPS1Close, TPS1Open, 0, 100); // Map it to a number between 0 and 100%

  APPS1 = tps*100; // It would read this from CAN

  if (APPS1 < 0){ APPS1 = 0; }

  Input = APPS1 - Input;
  if(Input < 0){
    myPID.SetControllerDirection(DIRECT);
    digitalWrite(directionPin,LOW);
    direction = 1; 
  }
  else {
    direction = 0;
    myPID.SetControllerDirection(REVERSE);
    digitalWrite(directionPin,HIGH);
  }

  myPID.Compute();
  analogWrite(torquePin, OutputTorque);
  lastGear = gear;
}

void downshift(){
  
}

void upshift(){
  
}

//void downshift(){
//  
//  myPID.Compute();
//  analogWrite(torquePin, OutputTorque);
//}
