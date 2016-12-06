#include "MultiShield.h"

MultiShield Multi;

ISR(TIMER0_COMPA_vect) {
    Multi.updateLedSegment();
    if (Multi.waitCounter > 0) {
	Multi.waitCounter--;
    }
}

void MultiShield::waitMillis(int millis) {
    noInterrupts();
    Multi.waitCounter = millis;
    interrupts();

    while(Multi.waitCounter > 0) {
    }
}

void MultiShield::init() {
    digitalWrite(LED_0, HIGH);
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, HIGH);
    digitalWrite(LED_3, HIGH);
    
    pinMode(LED_0, OUTPUT);
    pinMode(LED_1, OUTPUT);
    pinMode(LED_2, OUTPUT);
    pinMode(LED_3, OUTPUT);

    pinMode(LEDSEG_LATCH, OUTPUT);
    pinMode(LEDSEG_CLK,   OUTPUT);
    pinMode(LEDSEG_DATA,  OUTPUT);
    ledSegmentValue = 0;
    ledSegmentType = LEDSEG_DEC;
    ledSegmentPos = 0;
    
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

void MultiShield::ledState(ledpin_t led, ledState_t state) {
    digitalWrite(led, state);
}

void MultiShield::setLedSegment(uint16_t value, ledSegmentType_t type) {
    noInterrupts();
    ledSegmentValue = value;
    ledSegmentType = type;
    interrupts();
}

void MultiShield::updateLedSegment() {
    uint16_t tmp = ledSegmentValue;
    ledSegmentPos = (ledSegmentPos+1) & 0x03;

    switch(ledSegmentPos) {
    case 3:
	tmp = tmp / 1000;
	break;
    case 2:
	tmp = tmp / 100;
	break;
    case 1:
	tmp = tmp / 10;
	break;
    default:
	break;
    }
    
    tmp = tmp % 10;
    
    digitalWrite(LEDSEG_LATCH, LOW); 
    shiftOut(LEDSEG_DATA, LEDSEG_CLK, MSBFIRST, ledSegValToSegment[tmp]); 
    shiftOut(LEDSEG_DATA, LEDSEG_CLK, MSBFIRST, ledSegSelector[ledSegmentPos] );
    digitalWrite(LEDSEG_LATCH, HIGH);
}
