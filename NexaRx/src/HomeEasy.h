#ifndef HomeEasy_h
#define HomeEasy_h

#include "Arduino.h"

/**
 * A class for sending and receiving messages for HomeEasy devices.
 * 
 * This class uses digital pin 8 as the input from the receiver and pin 13 as the output to the
 * transmitter.  Timer 1 is used for interrupts.
 */
class HomeEasy
{
 public:
  
  /**
   * Constructor
   */
  HomeEasy();
  
  
  /**
   * Initialise the system.  This makes the receiver start listening.
   */
  void init();
  int getDone();
  char* getMessage();
  void release();

 private:
};

#endif
