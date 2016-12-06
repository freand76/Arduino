#include "Arduino.h"
#include "MultiShield.h"

#define LEDPIN           13               // the number of the LED pin
#define TURN_OFF_DELAY   (15*60)          // time until power off when no audio
#define NEXA_REMOTE_ID   0x55d836         // RemoteID
#define NEXA_UNIT_ID     2                // UnitID
#define NEXA_RETRANSMITS 10               // Send On and Off signals 10 times

int latchPin = 4;
int clockPin = 7;
int dataPin  = 8;

void setup() { 
    Serial.begin(115200); 
    Serial.println("MultiShield");
    Serial.println(__DATE__);   
    Multi.init();
} 

int value = 1375;
int count = 0;
void loop() {
    Multi.setLedSegment(value);
    value++;
    Multi.waitMillis(100);
}
