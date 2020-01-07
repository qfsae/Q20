#include "Arduino.h"

#include "DEFS.h"

#include "ECU.h"

#define DEBUG_MODE 0

ECU::ECU() {}

float readUnsigned(unsigned char lowByte, unsigned char highByte,
                   float factor) {
  return ((highByte * 256) + lowByte) * factor;
}

float readSigned(unsigned char lowByte, unsigned char highByte, float factor) {
  int reading = (highByte * 256) + lowByte;
  if (reading > 32767) {
    reading -= 65536;
  }
  return reading * factor;
}

// Updates the public variables on the object when a new message is received
// Running this update function in the loop of an Arduino sketch gives easy
// access to the latest ECU values
void ECU::update(unsigned long id, unsigned char msgBuf[],
                 unsigned char msgLen) {

  // update ECU and PDM data here
  if (DEBUG_MODE) {
    Serial.print("Observed ID ");
    Serial.println(id, HEX);
    Serial.print("MSG Length ");
    Serial.println(msgLen);
  }

  switch (id) {
  case PE1:
    this->rpm = readUnsigned(msgBuf[0], msgBuf[1], 1);
    this->tps = readSigned(msgBuf[2], msgBuf[3], 0.1);
    this->openTime = readSigned(msgBuf[4], msgBuf[5], 0.1);
    this->ignitionAngle = readSigned(msgBuf[6], msgBuf[7], 0.1);
    break;
  case PE2:
    this->pressure = readSigned(msgBuf[0], msgBuf[1], 0.01);
    this->MAP = readSigned(msgBuf[2], msgBuf[3], 0.01);
    this->lambda = readSigned(msgBuf[4], msgBuf[5], 0.01);
    this->pressureUnit = readUnsigned(msgBuf[6], msgBuf[7], 1);
    break;
  case PE3:
    for (int i = 0; i < 8; i += 2) {
      this->analogInputs[i / 2] = readSigned(msgBuf[i], msgBuf[i + 1], 0.001);
    }
    break;
  case PE4:
    for (int i = 0; i < 8; i += 2) {
      this->analogInputs[4 + i / 2] =
          readSigned(msgBuf[i], msgBuf[i + 1], 0.001);
    }
    break;
  case PE5:
    for (int i = 0; i < 8; i += 2) {
      this->frequencies[i / 2] = readSigned(msgBuf[i], msgBuf[i + 1], 0.2);
    }
    break;
  case PE6:
    this->batVoltage = readSigned(msgBuf[0], msgBuf[1], 0.01);
    this->airTemp = readSigned(msgBuf[2], msgBuf[3], 0.1);
    this->coolantTemp = readSigned(msgBuf[4], msgBuf[5], 0.1);
    this->tempUnit = readUnsigned(msgBuf[6], msgBuf[7], 1);
    break;
  case PE7:
    for (int i = 0; i < 4; i += 2) {
      this->thermistors[i / 2] = readSigned(msgBuf[i], msgBuf[i + 1], 0.1);
    }
    this->versionMajor = readUnsigned(msgBuf[4], 0, 1);
    this->versionMinor = readUnsigned(msgBuf[5], 0, 1);
    this->versionBuild = readUnsigned(msgBuf[6], 0, 1);
    break;
  }
}

void makeNames(String a, String b, String c, String d, String *arr) {
  arr[0] = a;
  arr[1] = b;
  arr[2] = c;
  arr[3] = d;
}

void makeVals(float a, float b, float c, float d, float *arr) {
  arr[0] = a;
  arr[1] = b;
  arr[2] = c;
  arr[3] = d;
}

void printMsg(String names[], float vals[], int n) {
  for (int i = 0; i < n; i++) {
    Serial.print(names[i]);
    Serial.print(" = ");
    Serial.print(vals[i]);
    Serial.print(", ");
  }
  Serial.println();
}

void ECU::debugPrint(unsigned long id) {
  String names[4];
  float vals[4];

  // 4 values are transmitted in each ECU CAN message
  int valNum = 4;
  switch (id) {
  case PE1:
    makeNames("RPM", "TPS", "OPEN_TIME(ms)", "IGNIT_ANGLE(deg)", names);
    makeVals(this->rpm, this->tps, this->openTime, this->ignitionAngle, vals);
    printMsg(names, vals, valNum);
    break;
  case PE2:
    makeNames("PRESSURE", "MAP", "LAMBDA", "0 - psi / 1 - kpa", names);
    makeVals(this->pressure, this->MAP, this->lambda, this->pressureUnit, vals);
    printMsg(names, vals, valNum);
    break;
  case PE3:
    makeNames("AIN1", "AIN2", "AIN3", "AIN4", names);
    printMsg(names, this->analogInputs, valNum);
    break;
  case PE4:
    makeNames("AIN5", "AIN6", "AIN7", "AIN8", names);
    printMsg(names, &this->analogInputs[4], valNum);
    break;
  case PE5:
    makeNames("FREQ1", "FREQ2", "FREQ3", "FREQ4", names);
    printMsg(names, this->frequencies, valNum);
    break;
  case PE6:
    makeNames("BAT", "AIR_TEMP", "COOL_TEMP", "0 - F / 1 - C", names);
    makeVals(this->batVoltage, this->airTemp, this->coolantTemp, this->tempUnit,
             vals);
    printMsg(names, vals, valNum);
    break;
  case PE7:
    String customNames[] = {"THERMIS1", "THERMIS2", "VMAJOR", "VMINOR",
                            "VBUILD"};
    float customVals[] = {this->thermistors[0], this->thermistors[1],
                          this->versionMajor, this->versionMinor,
                          this->versionBuild};
    printMsg(customNames, customVals, 5);
    break;
  }
}
