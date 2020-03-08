#define USerial Serial1
#define BODY_LENGTH 8

int endMarker = -20000;
char *c = (char *)&endMarker;

int n = 0;
unsigned char buf[BODY_LENGTH];
unsigned char msg[BODY_LENGTH];

void arrCopy(unsigned char *src, unsigned char *dest) {
  for (int i = 0; i < BODY_LENGTH; i++) {
    dest[i] = src[i];
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  USerial.begin(115200);
  delay(500);
  Serial.println("STARTING");
}

void loop() {
  // put your main code here, to run repeatedly:
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
  // TEST BY FETCHING BATTERY VOLTAGE
  Serial.println(msg[2]);
}
