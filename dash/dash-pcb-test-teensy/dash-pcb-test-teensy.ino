#define BAUD_RATE 1000000
#define USerial Serial1
#define BODY_LENGTH 4

unsigned char n = 0;
unsigned char currentMsg[BODY_LENGTH];
unsigned char data[BODY_LENGTH];

void arrCpy(unsigned char *src, unsigned char *dest) {
  for (int i = 0; i < BODY_LENGTH; i++) {
    dest[i] = src[i];  
  }
}

void recvData() {
  int endMarker = -20000;
  char *endBytes = (char *)&endMarker;
  //Serial.println("RUN");
  if (USerial.available() >= 2) {
    //Serial.println("RECV");
    char a = USerial.read();
    char b = USerial.read();
    if (a == endBytes[0] && b == endBytes[1]) {
      // The message has ended
      n = 0;
      //arrCpy(data, currentMsg);
      Serial.println(data[1]);
      
    } else {
      data[n] = a;
      data[n + 1] = b;
      n += 2;
    }
  }
}

void printCurrentMsg() {
  for (int i = 0; i < BODY_LENGTH; i++) {
    Serial.print(currentMsg[i]);
    Serial.print('\t');
  }
  Serial.println();
  Serial.println("END");
}

void setup() {
  // put your setup code here, to run once:
  USerial.begin(BAUD_RATE);
  Serial.begin(BAUD_RATE);
  delay(500);
  Serial.println("STARTING");
}

void loop() {
  // put your main code here, to run repeatedly:
  recvData();
}
