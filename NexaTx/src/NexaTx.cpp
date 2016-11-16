#include "Arduino.h"
#include "NexaTx.h"

#define NEXA_TX_PIN        5

#define NEXA_WORDLENGTH    32
#define NEXA_TX_COUNT      3
#define NEXA_ON_BITPOS     4
#define NEXA_GROUP_BITPOS  5
#define NEXA_REMOTE_BITPOS 6

#define NEXA_HIGH_PULSE_US 250
#define NEXA_LOW_START_US  2600
#define NEXA_LOW_ZERO_US   280
#define NEXA_LOW_ONE_US    1150
#define NEXA_LOW_END_MS    10

NexaTx::NexaTx() {
}

void NexaTx::init() {
    pinMode(NEXA_TX_PIN, OUTPUT);
    digitalWrite(NEXA_TX_PIN, LOW);
}  

void NexaTx::transmitStart() {
    digitalWrite(NEXA_TX_PIN, HIGH);
    delayMicroseconds(NEXA_HIGH_PULSE_US);
    digitalWrite(NEXA_TX_PIN, LOW);
    delayMicroseconds(NEXA_LOW_START_US);
}

void NexaTx::transmitZero() {
    digitalWrite(NEXA_TX_PIN, HIGH);
    delayMicroseconds(NEXA_HIGH_PULSE_US);
    digitalWrite(NEXA_TX_PIN, LOW);
    delayMicroseconds(NEXA_LOW_ZERO_US);
    digitalWrite(NEXA_TX_PIN, HIGH);
    delayMicroseconds(NEXA_HIGH_PULSE_US);
    digitalWrite(NEXA_TX_PIN, LOW);
    delayMicroseconds(NEXA_LOW_ONE_US);
}

void NexaTx::transmitOne() {
    digitalWrite(NEXA_TX_PIN, HIGH);
    delayMicroseconds(NEXA_HIGH_PULSE_US);
    digitalWrite(NEXA_TX_PIN, LOW);
    delayMicroseconds(NEXA_LOW_ONE_US);
    digitalWrite(NEXA_TX_PIN, HIGH);
    delayMicroseconds(NEXA_HIGH_PULSE_US);
    digitalWrite(NEXA_TX_PIN, LOW);
    delayMicroseconds(NEXA_LOW_ZERO_US);
}

void NexaTx::transmitStop() {
    digitalWrite(NEXA_TX_PIN, HIGH);
    delayMicroseconds(NEXA_HIGH_PULSE_US);
    digitalWrite(NEXA_TX_PIN, LOW);
    delay(NEXA_LOW_END_MS);
}

void NexaTx::transmitWord(unsigned long data, int txCount) {
    for (int txCounter = 0; txCounter < txCount; txCounter++) {
	transmitStart();
	for (int idx = 0; idx < NEXA_WORDLENGTH; idx++) {
	    if ((data >> (NEXA_WORDLENGTH - 1 - idx)) & 0x01) {
		transmitOne();
	    } else {
		transmitZero();
	    }
	} 
	transmitStop();
    }
}

void NexaTx::transmit(unsigned long remoteId, boolean group, boolean on, int switchId, int txCount) {
    unsigned long data = remoteId << NEXA_REMOTE_BITPOS;
    if (group) data = data | (1 << NEXA_GROUP_BITPOS);
    if (on) data = data | (1 << NEXA_ON_BITPOS);
    data = data | (switchId & 0x0f);
    transmitWord(data, txCount);
} 

void NexaTx::transmit(unsigned long remoteId, boolean group, boolean on, int switchId) {
    transmit(remoteId, group, on, switchId, NEXA_TX_COUNT); 
}
