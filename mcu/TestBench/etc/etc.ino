544#include <PID_v1.h>

const int TPS1Pin = A0;  //Pin of TPS1
const int TPS2Pin = A5;  //Pin of TPS2
const int APPS1Pin = A2; // Gas pedal Potentiometer
const int torquePin = 9; //PWM pin to motor driver
const int directionPin = 4; //directionPin

//PID tunings
const double Kp = 1;
const double Ki = 0.00001;
const double Kd = 0.00;

//ETB Constants
const int TPS1Close = 931;
const int TPS1Open = 50;
const int TPS2Close = 90;
const int TPS2Open = 971;

//APPS Constants
const int APPS1Close = 700;
const int APPS1Open = 90;
const int APPS2Close = 0;
const int APPS2Open = 1024;

int direction; 
double Input;
double OutputTorque;
double DesiredPosition;
int TPS1;
int TPS2;
int TPSdiff;
int APPS1;
int APPS2;
int APPSdiff;

PID myPID(&Input, &OutputTorque, 0, Kp, Ki, Kd, DIRECT);

void setup() {
  TCCR0B = TCCR0B & B11111000 | B00000010; // for PWM frequency of 7812.50 Hz

  // Set PID inputs
  Input = 0;
  DesiredPosition = 0;
  myPID.SetMode(AUTOMATIC);

  pinMode(APPS1Pin, INPUT);
  pinMode(TPS1Pin, INPUT);
  pinMode(TPS2Pin, INPUT);
  pinMode(directionPin, OUTPUT);
  pinMode(torquePin, OUTPUT);

  myPID.SetSampleTime(10);
  
  Serial.begin(115200);
}

void loop() {

  TPS1 = constrain(analogRead(TPS1Pin), TPS1Open, TPS1Close); // Range: 931 - 50
  TPS2 = constrain(analogRead(TPS2Pin), TPS2Close, TPS2Open); // Range: 90 - 971
  TPSdiff = (TPS1Close - TPS1Open) - ((TPS1 - TPS1Open) + (TPS2 - TPS2Close));
  if(TPSdiff > ((TPS1Close - TPS1Open)*0.1)){ // TPS1 and TPS2 differ by more than 10%
    analogWrite(torquePin, 0);
    Serial.println("ERROR 1");
  }

  Input = map(TPS1, TPS1Close, TPS1Open, 0, 1024);

  //APPS1 = constrain(analogRead(APPS1Pin), APPS1Close, APPS1Open); // Range: 0 - 1024
  //APPS1 = map(APPS1, APPS1Open, APPS1Close, 0, 1024);
  APPS1 = map(analogRead(APPS1Pin), APPS1Close, APPS1Open, 0, 1024);
  if (APPS1 < 0){ APPS1 = 0; }
  //APPS2 = analogRead(APPS2Pin); // Range: 0 - 1024
  //APPSdiff = APPS1 - APPS2;
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
  //DesiredPosition = APPS1;

  myPID.Compute();
  analogWrite(torquePin, OutputTorque);
  
  Serial.print("TPS1: ");
  Serial.print(TPS1);
  Serial.print(" TPS2: ");
  Serial.print(TPS2);
  Serial.print(" TPSdiff: ");
  Serial.print(TPSdiff);
  Serial.print(" APPS1: ");
  Serial.print(APPS1);
  Serial.print(" Input: ");
  Serial.print(Input);
  Serial.print(" Output: ");
  Serial.print(OutputTorque);
  Serial.print(" Direction: ");
  if(direction == 1){
    Serial.println("CLOSE");
  } else {
  Serial.println("OPEN");
  }
}
