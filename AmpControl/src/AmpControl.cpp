#include "Arduino.h"
#include "NexaTx.h"

#define LEDPIN           13               // the number of the LED pin
#define TURN_OFF_DELAY   (15*60)          // time until power off when no audio
#define NEXA_REMOTE_ID   0x55d836         // RemoteID
#define NEXA_UNIT_ID     2                // UnitID
#define NEXA_RETRANSMITS 10               // Send On and Off signals 10 times

typedef enum {
    POWER_OFF,
    POWER_ON,
    POWER_ON_NO_AUDIO,
} fsm_state_t;

long turnOffCounter = -1;
fsm_state_t fsm_state = POWER_OFF;
NexaTx nexaTx;

boolean checkAudio() {
    unsigned long value = 0;
    
    for (int idx = 0; idx < 1000; idx++) {
	int val = analogRead(A0);
	value = value + val;
    }
    
    if (value > 10) {
	return true;
    }

    return false;
}

void turnOn() {
    digitalWrite(LEDPIN, HIGH);
    Serial.println("ON");
    nexaTx.transmit(NEXA_REMOTE_ID, NEXA_GROUP_OFF, NEXA_ON, NEXA_UNIT_ID, NEXA_RETRANSMITS);
}

void turnOff() {
    digitalWrite(LEDPIN, LOW);
    Serial.println("OFF");
    nexaTx.transmit(NEXA_REMOTE_ID, NEXA_GROUP_OFF, NEXA_OFF, NEXA_UNIT_ID, NEXA_RETRANSMITS);
}

void setup() 
{ 
    Serial.begin(115200); 
    Serial.println("AmpControl\n");
    Serial.println(__DATE__);   
    pinMode(LEDPIN, OUTPUT);      
    pinMode(A0, INPUT);
    analogReference(INTERNAL);
    digitalWrite(A0, LOW);
    digitalWrite(LEDPIN, LOW);
    checkAudio();
    nexaTx.init();
} 

void loop() 
{ 
    delay(1000);
    switch(fsm_state) {
    case POWER_OFF :
	if (checkAudio()) {
	    fsm_state = POWER_ON; 
	    turnOn();    
	}
	break;
    case POWER_ON :
	if (!checkAudio()) {
	    turnOffCounter = TURN_OFF_DELAY;
	    fsm_state = POWER_ON_NO_AUDIO; 
	}
	break;
    case POWER_ON_NO_AUDIO :
	if (checkAudio()) {
	    fsm_state = POWER_ON;
	    turnOn(); 
	} else {
	    turnOffCounter--;
	    Serial.print("Timer: ");
	    Serial.println(turnOffCounter);
	    if (turnOffCounter == 0) {
		fsm_state = POWER_OFF;
		turnOff();
	    }
	}
	break;
    }
}
