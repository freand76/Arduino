#include "Arduino.h"
#include <stdint.h>

typedef enum {
    LED_0 = 10,
    LED_1 = 11,
    LED_2 = 12,
    LED_3 = 13,
} ledpin_t;

typedef enum {
    LED_ON = LOW,
    LED_OFF = HIGH,
} ledState_t;

typedef enum {
    LEDSEG_LATCH = 4,
    LEDSEG_CLK = 7,
    LEDSEG_DATA = 8,
} ledSegmentPin_t;

typedef enum {
    LEDSEG_DEC,
    LEDSEG_HEX,
    LEDSEG_FLOAT,
} ledSegmentType_t;

const uint8_t ledSegValToSegment[] = { 0xC0, 0xF9, 0xA4, 0xB0, 0x99,
                                       0x92, 0x82, 0xF8, 0X80, 0X90 };
const uint8_t ledSegSelector[] = { 0xF8, 0xF4, 0xF2, 0xF1 };

class MultiShield {
public:
    void init();
    void ledState(ledpin_t led, ledState_t state);
    void setLedSegment(uint16_t value, ledSegmentType_t type = LEDSEG_DEC);    
    void updateLedSegment();
    void waitMillis(int millis);

    volatile int waitCounter;
private:
    volatile uint16_t ledSegmentValue;
    volatile uint16_t ledSegmentPos;
    ledSegmentType_t ledSegmentType;
};

extern MultiShield Multi;
