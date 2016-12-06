#include "Arduino.h"
#include "MultiShield.h"

void setup() { 
    Serial.begin(115200); 
    Serial.println("MultiShield");
    Serial.println(__DATE__);   
    Multi.init();
} 

int value = 0;

void loop() {
    Multi.setLedSegment(value);
    value++;
    Multi.waitMillis(1000);
}
