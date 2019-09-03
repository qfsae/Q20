	//Arduino Code for Formula SAE Team MCU (Master Control Unit)
/*
// PIN DEFINITIONS 
//Analog
A0. TPS1_Signal
A1. TPS2_Signal 
A2. APPS1_Signal
A3. APPS2_Signal
A4. tractionSig_Signal

//PWM
PWM3. Can Intercept
PWM4. TPS_Speed

//Digital
18. DownButtonSignal (TX1)
19. UpButtonSignal	(RX1)
22. TPS_Dir
23. ShiftCut_Ard
24. ETC Error Flag
25. EngNeut_Ard
26. UpSol
27. DownSol
50. CAN_CS
51. MOSI
52. MISO
53. SCK

*/ 
#include <PID_v1.h>
//----------------------------Pin Definitions--------------------------------
//Analog Pins 
const int TPS1Pin 	= 	A0;  //Pin of TPS1
const int TPS2Pin 	= 	A1;  //Pin of TPS2
const int APPS1Pin 	=	A2; // 
const int APPS2Pin 	= 	A3;
const int tractionSig = A4;

//PWM
const int canInt 	=	3;
const int TPS_Speed = 	4; //PWM pin to motor driver

//Digital Pins
const int downBut 	= 	18; //Button from Paddle Shifts
const int upBut 	= 	19;
const int TPS_Dir 	= 	22; //TPS_Direction Signal
const int shiftCut 	= 	23;
const int ETC_Flag	=	24;
const int engNeut 	= 	25;
const int upSol		=	27; //Signal to upshift solenoid
const int downSol	= 	26; //Signal to downshift solenoid 
const int tachSig; //???
//--------------------------------------------------------------------------

//-------------------------------Delays-------------------------------------
const int downShiftDelay 	= 	300; //from Q17 need to retune 
const int upShiftDelay		= 	300;
const int numberRPMSamples = 5;
long engineRpm = 0;
//--------------------------------------------------------------------------

//----------------------------Thresholds------------------------------------
const int APPS1UpperThresh 	= 	0;
const int APPS2UpperThresh 	=	0;
const int TPS1UpperThresh 	= 	0;
const int TPS2UpperThresh 	= 	0;

const int APPS1LowerThresh 	= 	0;
const int APPS2LowerThresh 	= 	0;
const int TPS1LowerThresh 	= 	0;
const int TPS2LowerThresh 	= 	0;

//ETC Stuff
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
const int APPS1Close = 0;
const int APPS1Open = 1024;
const int APPS2Close = 0;
const int APPS2Open = 1024;

double Input;
double OutputTorque;
double DesiredPosition;
int TPS1;
int TPS2;
int TPSdiff;
int APPS1;
int APPS2;
int APPSdiff;

PID myPID(&Input, &OutputTorque, &DesiredPosition, Kp, Ki, Kd, DIRECT); //PID system for ETC
//--------------------------------------------------------------------------

//Function Protocols 
void updateETB();
void CANInterupt();
void checkSignals();
void manualMode();
void manualDownShift();
void manualUpShift();
int getRPM();
void blipDown();
void neutralFind();
//--------------------------------------------------------------------------

void setup(){
	Init();
	Input = 0;
	DesiredPosition = 0;
	myPID.SetMode(AUTOMATIC);
	myPID.SetSampleTime(10);
	Serial.begin(9600);
}

void Init(){
	//Function that will initailize all pin modes and starting values 
	
	//Analog
	pinMode(TPS1Pin, INPUT);
	pinMode(TPS2Pin, INPUT);
	pinMode(APPS1Pin, INPUT);
	pinMode(APPS2Pin, INPUT);
	pinMode(tractionSig, INPUT);
	
	//PWM Pins
//	attachInterrupt(canInt, CANInterupt, HIGH); // need to write function for can intercept
	//attachInterrupt(upBut, manualUpShift, LOW);
	//attachInterrupt(downBut, manualDownShift, LOW);
	
	//Digital
	pinMode(downBut, INPUT);
	pinMode(upBut, INPUT);
	pinMode(TPS_Dir, OUTPUT);
	pinMode(shiftCut, OUTPUT);
	pinMode(ETC_Flag, OUTPUT);
	pinMode(engNeut, OUTPUT);
	pinMode(upSol, OUTPUT);
	pinMode(downSol, OUTPUT);
	pinMode(TPS_Dir, OUTPUT);
	pinMode(TPS_Speed, OUTPUT); 
	digitalWrite(upSol, LOW);
	digitalWrite(downSol, LOW);	
	
}

void loop(){
//  digitalWrite(upSol, HIGH);
//  digitalWrite(downSol, HIGH);
//
//  Serial.print(digitalRead(upBut));
//  Serial.print("\t");
//  Serial.print(digitalRead(downBut));
//  Serial.println();
//	//checkSignals();
//	//updateETB();
	manualShifting();
}

//ETC Function
void updateETB(){
	//ETC Stuff -> Look into making this a function 
	TPS1 = constrain(analogRead(TPS1Pin), TPS1Open, TPS1Close); // Range: 931 - 50
	TPS2 = constrain(analogRead(TPS2Pin), TPS2Close, TPS2Open); // Range: 90 - 971
	TPSdiff = (TPS1Close - TPS1Open) - ((TPS1 - TPS1Open) + (TPS2 - TPS2Close));

	if(TPSdiff > ((TPS1Close - TPS1Open)*0.1)){ // TPS1 and TPS2 differ by more than 10%
		analogWrite(TPS_Speed, 0);
		Serial.println("ERROR 1");
	}

	Input = map(TPS1, TPS1Close, TPS1Open, 0, 1024);

	APPS1 = constrain(analogRead(APPS1Pin), APPS1Close, APPS1Open); // Range: 0 - 1024
	//APPS2 = analogRead(APPS2Pin); // Range: 0 - 1024
	//APPSdiff = APPS1 - APPS2;
  
	if(Input < APPS1){
		myPID.SetControllerDirection(DIRECT);
		digitalWrite(TPS_Dir,LOW);    
	}
	else	{
		myPID.SetControllerDirection(REVERSE);
		digitalWrite(TPS_Dir,HIGH);
	}


	DesiredPosition = APPS1;
	myPID.Compute();
	//Update Write Function AFTER Compute
	analogWrite(TPS_Speed, OutputTorque);
}//updateETB()

//Shifting Stuff
void manualShifting () {
  //Upshift 
  int time; 
  if (!digitalRead(upBut)) {
    Serial.println("Manual UP Shift Requested");
    time = millis();
    manualUpshift();
    time = millis() - time;
    Serial.print("Shift Time:  ");
    Serial.println(time);
    }    
    
    //Downshift
    if(!digitalRead(downBut)) {
      Serial.println("Manual DOWN Shift Requested");
      time = millis();
      
      manualDownshift();
      time = millis() - time;
      Serial.print("Shift Time:  ");  
      Serial.println(time);
    }
   
}

void manualDownshift () {
      digitalWrite(shiftCut, HIGH);
      digitalWrite(downSol, HIGH);
      delay(downShiftDelay);
	  
      digitalWrite(shiftCut, LOW); 
      digitalWrite(downSol, LOW);  
}

void manualUpshift() {
      digitalWrite(shiftCut, HIGH);
      digitalWrite(upSol, HIGH);
      delay(upShiftDelay);
	    
      digitalWrite(upSol, LOW); 
      digitalWrite(shiftCut, LOW); 
      delay(300);
}

int getRPM () {

//returns the average frequency recorded
//PE3 outputs a 30% duty cycle

  long lowTimeTotal = 0;
  long highTimeTotal = 0;
  long measureNum = 0;
  long periodTotal = 0;
  long highTimeAvg = 0;

  for(byte j = 0; j <= numberRPMSamples; j++){    
     highTimeTotal = highTimeTotal + pulseIn(tachSig ,HIGH, 60);
     measureNum++;
    }
    
  highTimeAvg = highTimeTotal / numberRPMSamples;
  if (highTimeAvg>0 ){
    engineRpm = 5/highTimeAvg;
  }
  return engineRpm;

}

void blipDown () {
  //wait for tps to reach tpsThres
  //manualDownshift(); 
   
}

void neutralFind() {
  manualDownshift(); 
  manualDownshift(); 
  manualDownshift();
      
      //digitalWrite(shiftCutPin, HIGH);
      digitalWrite(upSol, HIGH);
      delay(upShiftDelay/2);
      digitalWrite(upSol, LOW); 
      //digitalWrite(shiftCutPin, LOW);
}

byte getGear () {
  byte gear = 0;
  return gear;
}

void CANInterupt(){
//	Not sure what the CANInterupt is doing 
}

void checkSignals(){
	//Test each individual signal to make sure that it is plausible
	//Send out ETC Flag
	//Not fully sure what to do here
	
	if(APPS1 > APPS1UpperThresh || APPS1 < APPS1LowerThresh){
		digitalWrite(ETC_Flag, HIGH);
	}
	
	if(APPS2 > APPS2UpperThresh || APPS2 < APPS2LowerThresh){
		digitalWrite(ETC_Flag, HIGH);
	}
	
	if(TPS1 > TPS1UpperThresh || TPS1 < TPS1LowerThresh){
		digitalWrite(ETC_Flag, HIGH);
	}
	
	if(TPS2 > TPS2UpperThresh || TPS2 < TPS2LowerThresh){
		digitalWrite(ETC_Flag, HIGH);
	}
	
}

//--------------------------------------------------------------------------
