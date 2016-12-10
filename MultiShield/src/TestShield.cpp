#include "Arduino.h"
#include "MultiShield.h"
#include "Wire.h"

uint32_t nextMillis;

void setup_6050() {
    Wire.begin();
    Wire.beginTransmission(0x68);
    Wire.write(0x6B);  // PWR_MGMT_1 register
    Wire.write(0);     // set to zero (wakes up the MPU-6050)
    Wire.endTransmission(true);
}

void sample_6050() {
    int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

    Wire.beginTransmission(0x68);
    Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(0x68,14,true);  // request a total of 14 registers
    AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
    AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
    Serial.print("AcX = "); Serial.print(AcX);
    Serial.print(" | AcY = "); Serial.print(AcY);
    Serial.print(" | AcZ = "); Serial.print(AcZ);
    Serial.println();

    int deg = (int)(atan2(AcX, AcZ)*180/3.14159265);
    Serial.println(deg);
}

void setup() {
    Serial.begin(115200);
    Serial.println("MultiShield");
    Serial.println(__DATE__);
    Multi.init();

    Multi.ledState( LED_1, LED_ON);
    Multi.ledState( LED_2, LED_ON, 0x80);
    Multi.ledState( LED_3, LED_ON, 0x40);
    Multi.ledState( LED_4, LED_ON);

    nextMillis = Multi.getCurrentMillis();

    //setup_6050();
}

int value = 0;

void loop() {
    nextMillis += 100;
    while(Multi.getCurrentMillis() < nextMillis) {
	/* Just Wait */
    }

    for (int idx = 0; idx < 4; idx++) {
	Multi.ledState( LED_1, LED_ON, 0x30 + value);
	Multi.ledState( LED_2, LED_ON, 0x20 + value);
	Multi.ledState( LED_3, LED_ON, 0x10 + value);
	Multi.ledState( LED_4, LED_ON, 0x00 + value);
    }
    value = value + 0x10;

    if (Multi.isButtonPressed(BUTTON_1)) {
	Multi.setLedSegmentHex(0xfeed);
    }

    if (Multi.isButtonPressed(BUTTON_2)) {
	Multi.setLedSegment(-10);
    }

    if (Multi.isButtonPressed(BUTTON_3)) {
	Multi.setLedSegmentError();
    }


    //sample_6050();
}
