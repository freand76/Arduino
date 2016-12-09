#include "Arduino.h"
#include "MultiShield.h"

uint32_t nextMillis;

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
	Multi.setLedSegment(0xfeed, LEDSEG_HEX);
    }

    if (Multi.isButtonPressed(BUTTON_2)) {
	Multi.setLedSegment(-10, LEDSEG_DEC);
    }

    if (Multi.isButtonPressed(BUTTON_3)) {
	Multi.setLedSegmentError();
    }

}
