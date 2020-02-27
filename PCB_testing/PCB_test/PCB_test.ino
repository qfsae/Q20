void setup() {
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(2, INPUT);
  Serial.begin(9600);
}

float rawTPS1, TPS1_voltage;
float rawTPS2, TPS2_voltage;
float rawBrake, brake_voltage;

// Thresholds Currently set on BSPD 
float TPS1_threshold = 2.65;
float TPS2_threshold = 2.5;
float brake_threshold = 2.5;

int TPS1_on = 0;
int TPS2_on = 0;
int brake_on = 0;
int pre_delay = 0;
int post_delay = 0;

void loop() {
  rawTPS1 = analogRead(A0);
  TPS1_voltage = 5*rawTPS1 / 1023;

  rawTPS2 = analogRead(A1);
  TPS2_voltage = 5*rawTPS2 / 1023;

  rawBrake = analogRead(A2);
  brake_voltage = 5*rawBrake / 1023;

  if(TPS1_voltage > TPS1_threshold){ TPS1_on = 1; }
  else { TPS1_on = 0; }

  if(brake_voltage > brake_threshold){ brake_on = 1; }
  else { brake_on = 0; }

  if( ((TPS1_on) || (TPS2_on)) & (brake_on)){
    pre_delay = 1;
  } else { pre_delay = 0; }

  printResults();
  delay(10);
}

void printResults(){
  // TPS1
  Serial.print("TPS1: ");
  Serial.print(TPS1_voltage);
  Serial.print("\t");
  if(TPS1_on){ Serial.print("ON\t"); }
  else { Serial.print("OFF\t");  }

  // TPS2 
  Serial.print("TPS2: ");
  Serial.print(TPS2_voltage);
  Serial.print("\t");
  if(TPS2_on){ Serial.print("ON\t"); }
  else { Serial.print("OFF\t"); }

  // Brake
  Serial.print("Brake: ");
  Serial.print(brake_voltage);
  Serial.print("\t");
  if(brake_on){ Serial.print("ON\t"); }
  else { Serial.print("OFF\t"); }

  // FLAGS
  
  Serial.print("\n");
}
