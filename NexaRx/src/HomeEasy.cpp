/**
* HomeEasy Library
*
* Usage notes : 
*     By default the library is hooked up to a fixed set of pins (for the benefit of the interrupts).  On your Arduino, you should connect the transmitter data to pin 13 and the receiver data to pin 8 
*     There is a degree of configurability for the transmission pin.  We use PORTB, so you must choose a pin on that PORT (PB*) from http://www.arduino.cc/en/Hacking/PinMapping168
*     To configure the transmitter data pin, define HETXPIN to the pin of choice from PB*
*/
#include "HomeEasy.h"

// for backwards compatibility
#ifndef PINB0
  #define PINB0 PB0
#endif
#ifndef PINB5
  #define PINB5 PB5
#endif

#ifndef HETXPIN
        #define HETXPIN PINB5
#endif
#ifndef HERXPIN
        #define HERXPIN PINB0
#endif

// variables used for receiving the messages
unsigned int pulseWidth = 0;

/**
 * Constructor
 */
HomeEasy::HomeEasy()
{
}

        
/**
 * Initialise the system.
 * 
 * Enables the receiving of messages.
 */
void HomeEasy::init()
{
        // ensure the receiver pin is set for input
        DDRB &= ~_BV(HERXPIN);
        
        // disable PWM (default)
        TCCR1A = 0x00;
        
        // set prescaler to 1/8.  TCNT1 increments every 0.5 micro seconds
        // falling edge used as trigger
        TCCR1B = 0x02;
        
        // enable input capture interrupt for timer 1
        TIMSK1 = _BV(ICIE1);
}


/**
 * The input interrupt handler.
 * 
 * This is where the message is received and decoded.
 */

int index;
char msg[128];
int highPulse;
typedef enum {
  WAIT_FOR_START,
  RX_DATA,
  RX_DONE,
} rx_state_t; 

rx_state_t rx_state = WAIT_FOR_START;

int HomeEasy::getDone() {
  return (rx_state == RX_DONE);
}

char* HomeEasy::getMessage() {
  return msg;
}

void HomeEasy::release() {
  rx_state = WAIT_FOR_START;
}

ISR(TIMER1_CAPT_vect)
{
        // reset counter
        TCNT1 = 0;
        
        // get value of input compare register, divide by two to get microseconds
        pulseWidth = (ICR1 / 2);
     
        if (bit_is_clear(TCCR1B, ICES1)) { 
	  // falling edge was detected, HIGH pulse end
	  highPulse = pulseWidth;
	    // advanced protocol latch
	  digitalWrite(13,1);
	} else { 
	  // raising edge was detected, LOW pulse end
	  if (highPulse > 230 && highPulse < 280) {
	    switch(rx_state) {
	    case WAIT_FOR_START :
	      if (pulseWidth > 2550 && pulseWidth < 2750) {
		rx_state = RX_DATA;
		index = 0;
	      }
	      break;
	    case RX_DATA :
	      if (pulseWidth > 200 && pulseWidth < 365) {
		msg[index] = '0';
		index++;
	      } else if (pulseWidth > 1000 && pulseWidth < 1360) {
		msg[index] = '1';
		index++;	  
	      } else if (pulseWidth > 9480 && pulseWidth < 11500) {
		// pause between messages
		msg[index] = '\0';
		rx_state = RX_DONE;
	      } else {
		sprintf(msg, "E1: %d %d", highPulse, pulseWidth);
		rx_state = RX_DONE;
	      }
	      break;
	    case RX_DONE :
	      break;
	    }
	  } else {
	    if (rx_state != RX_DONE) {
	      rx_state = WAIT_FOR_START;
	    }
	  }
        }
        
        // toggle bit value to trigger on the other edge
        TCCR1B ^= _BV(ICES1);
}

