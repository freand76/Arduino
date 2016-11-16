#ifndef NEXATX_H
#define NEXATX_H

#include "Arduino.h"

#define NEXA_OFF false
#define NEXA_ON true
#define NEXA_GROUP_ON true
#define NEXA_GROUP_OFF false

class NexaTx
{
 public:
    NexaTx();
    void init();
    void transmit(unsigned long,boolean,boolean,int);
    void transmit(unsigned long,boolean,boolean,int,int);
 private:
    void transmitWord(unsigned long, int);
    void transmitStart();
    void transmitOne();
    void transmitZero();
    void transmitStop();
};

#endif
