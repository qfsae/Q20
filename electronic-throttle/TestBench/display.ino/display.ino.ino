#include <LiquidCrystal.h> //LCD Library
LiquidCrystal lcd(2, 3, 4, 5, 6, 7); //initializing LCD, input pins are

//Analog Pins 
const int TPS1Pin   =   A0;  //Pin of TPS1
const int TPS2Pin   =   A1;  //Pin of TPS2
const int APPS1Pin  =   A2; 
const int APPS2Pin  =   A3;
const int BrakePres =   A4;

int TPS1 = 0;
int TPS2 = 0;
int APPS1 = 0;
int APPS2 = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(TPS1Pin, INPUT);
  pinMode(TPS2Pin, INPUT);
  pinMode(APPS1Pin, INPUT);
  pinMode(APPS2Pin, INPUT);
  pinMode(BrakePres, INPUT);
  lcd.begin(16, 2); //initialize the size of the LCD screen
  lcd.print(" ETC Test Bench "); //title print
  lcd.setCursor(2, 1); // set cursor to second line
  lcd.print("  ELEC 344  ");
  delay(2000);
  lcd.clear();
//  lcd.setCursor(1, 1);
  lcd.print("TPS:");
  lcd.setCursor(0, 1);
  lcd.print("APPS: ");
}

void loop() {
  lcd.clear();
  lcd.print("TPS:");
  lcd.setCursor(0, 1);
  lcd.print("APPS: ");
  // put your main code here, to run repeatedly:
  TPS1 = analogRead(TPS1Pin);
  Serial.println(TPS1);
  TPS1 = map(TPS1, 830, 115, 0, 100);
  APPS1 = analogRead(APPS1Pin);
  APPS1 = map(APPS1, 600, 113, 0, 100);
  if (APPS1 < 0){ APPS1 = 0; }
//  Serial.print(analogRead(A0));
//  Serial.print("\t");
//  Serial.print(analogRead(A1));
//  Serial.print("\t");
//  Serial.print(analogRead(A2));
//  Serial.print("\t");   
//  Serial.print(analogRead(A3));
//  Serial.println();
  if (TPS1 < 100){
    lcd.setCursor(8, 0);
    lcd.print("");
  }
  lcd.setCursor(5, 0);
  lcd.print(" ");
  lcd.print(TPS1);
  lcd.print("%");
  lcd.setCursor(6, 1);
  lcd.print(APPS1);
  lcd.print("%");
  delay(50);
}
