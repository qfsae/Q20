const int upShiftButton = 0;
const int downShiftButton = 10;
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
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(upShiftButton) == LOW){
    
    doUpShift();
  }
//  if(digitalRead(downShiftButton) == LOW){
//    doDownShift();
//  }
}

void doUpShift(){
  Serial.println("Upshifting");
  digitalWrite(shiftCut, HIGH);
  digitalWrite(upShiftSolenoid, HIGH);
  delay(upShiftDelay);
  
  digitalWrite(upShiftSolenoid, LOW); 
  digitalWrite(shiftCut, LOW); 
}

void doDownShift(){
  Serial.println("Downshifting");
  digitalWrite(shiftCut, HIGH);
  digitalWrite(downShiftSolenoid, HIGH);
  delay(downShiftDelay);
  
  digitalWrite(downShiftSolenoid, LOW); 
  digitalWrite(shiftCut, LOW);
}
