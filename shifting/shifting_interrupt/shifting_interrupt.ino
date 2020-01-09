// Interrupt Pins
const int upShiftButton = 2;
const int downShiftButton = 3;

const int upShiftSolenoid = 1;
const int downShiftSolenoid = 11;
const int shiftCut = 13;
int upShiftDelay = 200;
int downShiftDelay = 200;

void setup() {
  // put your setup code here, to run once:
  pinMode(upShiftButton,INPUT);
  pinMode(downShiftButton,INPUT);
  pinMode(upShiftSolenoid,OUTPUT);
  pinMode(downShiftSolenoid,OUTPUT);
  pinMode(shiftCut, OUTPUT);                                          
  digitalWrite(shiftCut, LOW);
  attachInterrupt(digitalPinToInterrupt(2), doUpShift, LOW);
  attachInterrupt(digitalPinToInterrupt(3), doDownShift,LOW);
  Serial.begin(9600);
}

void loop() {
//  if(digitalRead(upShiftButton) == LOW){
//    doUpShift();
//  }
//  if(digitalRead(downShiftButton) == LOW){
//    doDownShift();
//  }
Serial.println("In Loop");
}

void doUpShift(){
  Serial.println("Upshifting");
  delay(100);
//  digitalWrite(shiftCut, HIGH);
//  digitalWrite(upShiftSolenoid, HIGH);
//  delay(upShiftDelay);
//  digitalWrite(upShiftSolenoid, LOW); 
//  digitalWrite(shiftCut, LOW); 
}

void doDownShift(){
  Serial.println("Downshifting");
  delay(100);
//  digitalWrite(shiftCut, HIGH);
//  digitalWrite(downShiftSolenoid, HIGH);
//  delay(downShiftDelay);
//  
//  digitalWrite(downShiftSolenoid, LOW); 
//  digitalWrite(shiftCut, LOW);
}
