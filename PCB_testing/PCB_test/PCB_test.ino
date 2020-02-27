void setup() {
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(2, INPUT);
  Serial.begin(9600);
}

void loop() {
  Serial.print("TPS1: ");
  Serial.print(analogRead(A0));
  Serial.print("\t");
  Serial.print("TPS2: ");
  Serial.print(analogRead(A1));
  Serial.print("\t");
  Serial.print("Brake: ");
  Serial.print(analogRead(A2));
  Serial.print("\n");
  delay(10);
}
