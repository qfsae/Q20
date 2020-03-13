#define USerial Serial1

#define BODY_LENGTH 8

int endMarker = -20000;
char *c = (char *)&endMarker;

int n = 0;
unsigned char buf[BODY_LENGTH];
unsigned char msg[BODY_LENGTH];

void arrCopy(unsigned char *src, unsigned char *dest) {
  for (int i = 0; i < n; i++) {
    dest[i] = src[i];
  }
}

void setup() {
  // put your setup code here, to run once:
  //USerial.begin(115200);
  Serial.begin(115200);
  delay(500);
  Serial.println("STARTING");
  USerial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println("TESTING");
  if (USerial.available() >= 2) {
    Serial.println("RECV");
    char a = USerial.read();
    char b = USerial.read();
    if (a == c[0] && b == c[1]) {
      arrCopy(buf, msg); // copy parsed data packet into msg array
      n = 0; // reset counter
    } else {
      buf[n] = a;
      buf[n + 1] = b;
      n += 2;
    }
  }
  float voltage = msg[2] * 0.1216;
  Serial.println(voltage);
  delay(100);
}
