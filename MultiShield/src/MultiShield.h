#include "Arduino.h"
#include <stdint.h>

#define BUZZER_PIN 3

typedef enum {
    LED_1 = 0,
    LED_2,
    LED_3,
    LED_4,
    NOF_LEDS,
} ledpin_t;

typedef enum {
    BUTTON_1,
    BUTTON_2,
    BUTTON_3,
    NOF_BUTTONS,
} buttonpin_t;

typedef enum {
    LED_ON,
    LED_OFF,
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
    LEDSEG_CLEAR,
} ledSegmentType_t;

/* 0x01 - top */
/* 0x02 - top-right */
/* 0x04 - bottom-right */
/* 0x08 - bottom */
/* 0x10 - bottom-left */
/* 0x20 - top-left */
/* 0x40 - middle */
/* 0x80 - dot */

const uint8_t ledSegValToSegment[] = {
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, /* 0-5 */
    0x92, 0x82, 0xF8, 0x80, 0x90, /* 5-9 */
    0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e, /* A-F */
    0xaf, /* r */
    0xff, /* empty */
    0xbf, /* - */
};

typedef enum {
    SEG_0,
    SEG_1,
    SEG_2,
    SEG_3,
    SEG_4,
    SEG_5,
    SEG_6,
    SEG_7,
    SEG_8,
    SEG_9,
    SEG_a,
    SEG_b,
    SEG_c,
    SEG_d,
    SEG_e,
    SEG_f,
    SEG_r,
    SEG_EMPTY,
    SEG_DASH,
} segsymbol_t;

#define NOF_SEG_SYMBOLS 4

const uint8_t ledSegSelector[NOF_SEG_SYMBOLS] = { 0xF8, 0xF4, 0xF2, 0xF1 };
const uint8_t ledPinSelector[NOF_LEDS] = { 13, 12, 11, 10 };
const uint8_t buttonPinSelector[NOF_BUTTONS] = { A1, A2, A3 };

class MultiShield {
public:
    void init();
    /* LED */
    void ledState(ledpin_t led, ledState_t state, uint8_t intensity = 0xff);
    /* LED Segment Display */
    void setLedSegment(int32_t value, ledSegmentType_t type = LEDSEG_DEC);
    void setLedSegmentError();
    void clearLedSegment();
    void updateLedSegment();
    /* Buttons */
    bool isButtonPressed(buttonpin_t button);

    /* Time */
    void waitMillis(int millis);
    uint32_t getCurrentMillis();

    volatile int waitCounter;
    volatile uint32_t millisCounter;
    volatile uint8_t ledIntensity[NOF_LEDS];
    volatile uint8_t ledCounter[NOF_LEDS];
private:
    void setLedSegmentDec(int32_t value);
    void setLedSegmentHex(int32_t value);
    volatile int8_t ledSegmentValue[NOF_SEG_SYMBOLS];
};

extern MultiShield Multi;
