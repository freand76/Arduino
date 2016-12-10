#include "MultiShield.h"

MultiShield Multi;

ISR(TIMER0_COMPA_vect) {
    Multi.updateLedSegment();
    if (Multi.waitCounter > 0) {
	Multi.waitCounter--;
    }
    Multi.millisCounter++;

    for (int idx = 0; idx < NOF_LEDS; idx++) {
	if (Multi.ledIntensity[idx] == 0) {
	    digitalWrite(ledPinSelector[idx], HIGH);
	} else {
	    uint16_t tmp = Multi.ledCounter[idx];
	    tmp += Multi.ledIntensity[idx];

	    if (tmp > 255) {
		digitalWrite(ledPinSelector[idx], LOW);
	    } else {
		digitalWrite(ledPinSelector[idx], HIGH);
	    }
	    Multi.ledCounter[idx] = tmp & 0xff;
	}
    }
}

void MultiShield::init() {
    digitalWrite(ledPinSelector[LED_1], HIGH);
    digitalWrite(ledPinSelector[LED_2], HIGH);
    digitalWrite(ledPinSelector[LED_3], HIGH);
    digitalWrite(ledPinSelector[LED_4], HIGH);
    digitalWrite(BUZZER_PIN, HIGH);

    pinMode(ledPinSelector[LED_1], OUTPUT);
    pinMode(ledPinSelector[LED_2], OUTPUT);
    pinMode(ledPinSelector[LED_3], OUTPUT);
    pinMode(ledPinSelector[LED_4], OUTPUT);

    for (int idx = 0; idx < NOF_LEDS; idx++) {
	ledIntensity[idx] = 0;
    }

    for (int idx = 0; idx < NOF_BUTTONS; idx++) {
	pinMode(buttonPinSelector[idx], INPUT);
    }

    pinMode(BUZZER_PIN, OUTPUT);

    pinMode(LEDSEG_LATCH, OUTPUT);
    pinMode(LEDSEG_CLK,   OUTPUT);
    pinMode(LEDSEG_DATA,  OUTPUT);
    for (int idx = 0; idx < 4; idx++) {
	ledSegmentValue[idx] = SEG_EMPTY;
    }
    millisCounter = 0;
    waitCounter = 0;

    //set timer0 interrupt at 2kHz
    TCCR0A = 0;// set entire TCCR0A register to 0
    TCCR0B = 0;// same for TCCR0B
    TCNT0  = 0;// initialize counter value to 0
    // set compare match register for 2khz increments
    OCR0A = 248;// = (16*10^6) / (1000*64) - 1 (must be <256)
    // turn on CTC mode
    TCCR0A |= (1 << WGM01);
    // Set CS01 and CS00 bits for 64 prescaler
    TCCR0B |= (1 << CS01) | (1 << CS00);
    // enable timer compare interrupt
    TIMSK0 |= (1 << OCIE0A);

    sei();
}

void MultiShield::waitMillis(int millis) {
    noInterrupts();
    waitCounter = millis;
    interrupts();

    while(waitCounter > 0) {
    }
}

uint32_t MultiShield::getCurrentMillis() {
    return millisCounter;
}

void MultiShield::ledState(ledpin_t led, ledState_t state, uint8_t intensity) {
    if (state == LED_OFF) {
	ledIntensity[led] = 0x00;
    } else {
	ledIntensity[led] = intensity;
    }
}

void MultiShield::setLedSegment(int32_t value) {
    if ((value > 9999) || (value < -999)) {
    } else {
	int16_t tmp = value;
	bool neg = false;

	if (tmp < 0) {
	    neg = true;
	    tmp = -tmp;
	}

	for (int idx = 0; idx < NOF_SEG_SYMBOLS; idx++) {
	    if ((tmp == 0) && (neg) && (idx != 0)) {
		ledSegmentValue[idx] = SEG_DASH;
		neg = false;
	    } else if ((tmp == 0) && (idx != 0)) {
		ledSegmentValue[idx] = SEG_EMPTY;
	    } else {
		ledSegmentValue[idx] = tmp % 10;
	    }
	    tmp = tmp / 10;
	}
    }
}

void MultiShield::setLedSegmentHex(int32_t value) {
    if ((value > 65535) || (value < 0)) {
	setLedSegmentError();
    } else {
	int16_t tmp = value;
	for (int idx = 0; idx < NOF_SEG_SYMBOLS; idx++) {
	    ledSegmentValue[idx] = tmp & 0x0f;
	    tmp = tmp >> 4;
	}
    }
}

void MultiShield::setLedSegmentError() {
    ledSegmentValue[3] = SEG_e;
    ledSegmentValue[2] = SEG_r;
    ledSegmentValue[1] = SEG_r;
    ledSegmentValue[0] = SEG_EMPTY;
}

void MultiShield::clearLedSegment() {
    for (int idx = 0; idx < NOF_SEG_SYMBOLS; idx++) {
	ledSegmentValue[idx] = SEG_EMPTY;
    }
}

void MultiShield::updateLedSegment() {
    uint8_t ledSegmentPos = millisCounter & 0x03;

    digitalWrite(LEDSEG_LATCH, LOW);
    shiftOut(LEDSEG_DATA, LEDSEG_CLK, MSBFIRST, ledSegValToSegment[ledSegmentValue[ledSegmentPos]]);
    shiftOut(LEDSEG_DATA, LEDSEG_CLK, MSBFIRST, ledSegSelector[ledSegmentPos] );
    digitalWrite(LEDSEG_LATCH, HIGH);
}

bool MultiShield::isButtonPressed(buttonpin_t button) {
    if (digitalRead(buttonPinSelector[button]) == LOW) {
	return true;
    }
    return false;
}
