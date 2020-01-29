#include <PID_v1.h>

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
double DesiredPosition;
int TPS1;
int TPS2;
int TPSdiff;
float tps, APPS1;

PID myPID(&Input, &OutputTorque, 0, Kp, Ki, Kd, DIRECT);

void setup() {
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
  
  Serial.begin(115200);
}

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

//============

void parseData() {      // split the data into its parts
    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    int temp = atoi(strtokIndx); // copy it to messageFromPC
    if(temp != 0) {
      rpm = temp;
    }
 
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    int temp2 = atoi(strtokIndx); // convert this part to an float
    if(temp2 != 0) {
      gear = temp2;               
    }

    strtokIndx = strtok(NULL, ",");
    float tempf = atof(strtokIndx);     // convert this part to a int
    if(tempf != 0.0) {
      tps = tempf;
    }

}

int lastGear = 0;

void loop() {
    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
        parseData();
        //showParsedData();
        newData = false;
    }
    if (lastGear >  gear && gear != 0){ // downshift
      downshift();
    }
    else if (lastGear < gear && gear != 0){
      upshift();
    }
  
  TPS1 = constrain(analogRead(TPS1Pin), TPS1Open, TPS1Close); // Range: 931 - 50 // Range: 931 - 50
  Input = map(TPS1, TPS1Close, TPS1Open, 0, 100);

  APPS1 = tps*100;
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
