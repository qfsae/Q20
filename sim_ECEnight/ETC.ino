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
const int APPS1Pin = A0;
const int APPS2Pin = A1;
const int BrakePres = A2;
const int TPS1Pin = A3;  //Pin of TPS1
const int TPS2Pin = A4;  //Pin of TPS2
const int torquePin = 9; //PWM pin to motor driver
const int directionPin = 4; //directionPin

const int upButton = 2;
const int downButton = 3;
const int shiftCut = 10;
const int upLED = 11;
const int downLED = 12;

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
  pinMode(shiftCut, OUTPUT);
  pinMode(upLED, OUTPUT);
  pinMode(downLED, OUTPUT);
  digitalWrite(shiftCut, HIGH);
  digitalWrite(upLED, HIGH);
  digitalWrite(downLED, HIGH);

  while (CAN.begin(CAN_500KBPS) != CAN_OK) { delay(100); }

  digitalWrite(shiftCut, LOW);
  digitalWrite(upLED, LOW);
  digitalWrite(downLED, LOW);

  TCCR0B = TCCR0B & B11111000 | B00000010; // for PWM frequency of 7812.50 Hz
  pinMode(upButton, INPUT);
  pinMode(downButton, INPUT);
  attachInterrupt(digitalPinToInterrupt(downButton), downshift, FALLING);
  attachInterrupt(digitalPinToInterrupt(upbutton), upshift, FALLING);
  
  // ETC Inputs
  pinMode(APPS1Pin, INPUT);
  pinMode(TPS1Pin, INPUT);
  pinMode(TPS2Pin, INPUT);
  pinMode(BrakePres, INPUT);
  pinMode(BSPDFlag, INPUT);

  // ETC Outputs
  pinMode(directionPin, OUTPUT);
  pinMode(torquePin, OUTPUT);
  pinMode(13, OUTPUT);
  // Set PID inputs
  Input = 0;
  DesiredPosition = 0;
  myPID.SetMode(AUTOMATIC);


  myPID.SetSampleTime(10);
}

int lastGear = 0;

void loop() {
    BSPD = analogRead(BSPDFlag);
    if(BSPDFlag > 410){ flag_start = millis(); }
    if(BSPDFlag > 410 && millis() > flag_start + 1000){
      brickProgram();
    }
    Brake = analogRead(BrakePres);
    Brake = map(Brake, BrakeLow, BrakeHigh, 0, 100); // Map to 0-100%

    // Read from CAN shield. 
    if (lastGear >  gear && gear != 0){ // downshift
      downshift();
    }
    else if (lastGear < gear && gear != 0){
      upshift();
    }
  
  TPS1 = constrain(analogRead(TPS1Pin), TPS1Open, TPS1Close);
  TPS2 = constrain(analogRead(TPS2Pin), TPS2Close, TPS2Open);
  TPSDiff = (TPS1Close - TPS1Open) - ((TPS1 - TPS1Open) + (TPS2 - TPS2Close));
  if(TPSdiff > ((TPS1Close - TPS1Open)*0.1)){
    // differ more than 10%
    analogWrite(torquePin, 0); // kill throttle body, this should be better
  }

  Input = map(TPS1, TPS1Close, TPS1Open, 0, 1024); // Map it to a number between 0 and 100%
  if(SIM){
    // put stuff to receive from CAN here
    receiveCAN();
    APPS1 = tps*100; // It would read this from CAN
  } else {
    APPS1 = map(analogRead(APPS1Pin), APPS1Close, APPS1Open, 0, 1024);
  }

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

void receiveCAN(){
  // Need to receive tps and gear over CAN
}

void transmitCAN(){
  // Need to transmit TPS, Brake, Error, Flag, etc)

}

void brickProgram(){
  while(1){
    // Rip
  }
}

void downshift(){
  digitalWrite(shiftCut, HIGH);
  digitalWrite(downLED, HIGH);
  int startingTPS = TPS1;
  int startAPPS = APPS1;
  while(TPS1 > TPS1Open + 50){
    Input = map(TPS1, TPS1Close, TPS1Open, 0, 1024){
    DesiredPosition = TPS1Open;
    myPID.SetControllerDirection(REVERSE);
    digitalWrite(directionPin, HIGH);
    myPID.Compute();
    analogWrite(torquePin, OutputTorque);
    TPS1 = constrain(analogRead(TPS1Pin), TPS1Open, TPS1Close);
    }
  } delay(60);
  while(TPS1 < startingTPS){
    Input = map(TPS1, TPS1Close, TPS1Open, 0, 1024);
    DesiredPosition = startAPPS;
    myPID.SetControllerDirection(DIRECT);
    digitalWrite(directionPin, LOW);
    myPID.Compute();
    analogWrite(torquePin, OutputTorque);
    TPS1 = constrain(analogRead(TPS1Pin), TPS1Open, TPS1Close);
  }
  digitalWrite(shiftCut, LOW);
  digitalWrite(downLED, LOW);
}

void upshift(){
  digitalWrite(shiftCut, HIGH);
  digitalWrite(downLED, HIGH);
  int startingTPS = TPS1;
  int startAPPS = APPS1;
  while(TPS1 < TPS1Close - 50){
    Input = map(TPS1, TPS1Close, TPS1Open, 0, 1024){
    DesiredPosition = TPS1Close;
    myPID.SetControllerDirection(DIRECT);
    digitalWrite(directionPin, LOW);
    myPID.Compute();
    analogWrite(torquePin, OutputTorque);
    TPS1 = constrain(analogRead(TPS1Pin), TPS1Open, TPS1Close);
    }
  } delay(60);
  while(TPS1 < startingTPS){
    Input = map(TPS1, TPS1Close, TPS1Open, 0, 1024);
    DesiredPosition = startAPPS;
    myPID.SetControllerDirection(REVERSE);
    digitalWrite(directionPin, HIGH);
    myPID.Compute();
    analogWrite(torquePin, OutputTorque);
    TPS1 = constrain(analogRead(TPS1Pin), TPS1Open, TPS1Close);
  }
  digitalWrite(shiftCut, LOW);
  digitalWrite(upLED, LOW);
}