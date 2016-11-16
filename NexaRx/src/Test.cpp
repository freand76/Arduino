#include "Arduino.h"
#include "HomeEasy.h"

HomeEasy homeEasy;

void printResult(char* result);

/**
 * Set up the serial interface and the HomeEasy class.
 */
void setup() {
  pinMode(13, OUTPUT);      
  
  Serial.begin(115200);
  Serial.println("Hello1");
  
  homeEasy = HomeEasy();
  Serial.println("Hello2");
  
  Serial.println("Hello3");
  homeEasy.init();
  Serial.println("Hello4");
}

/**
 * No processing is required in the loop for receiving.
 */
void loop() {
  if(homeEasy.getDone() == 1) {
    char* message = homeEasy.getMessage();
    int length = strlen(message);
    Serial.print(length);
    Serial.print(" ");
    
    int idx = 0;
      
    long data = 0;
    do {
      if ((idx % 2) == 1) {
	data = data << 1;
	if (message[idx-1] == '0' && message[idx] == '1') {
	} else if (message[idx-1] == '1' && message[idx] == '0') {
	  data = data | 0x01;
	} else {
	  Serial.println("Error");
	  idx = length+1;
	}
      }
      idx++;
      if (idx == length) {
	Serial.println(data, HEX);
	Serial.println( (0xffffffc0 & data) >> 6, HEX);
	Serial.println( (0x20 & data) >> 5, HEX);
	Serial.println( (0x10 & data) >> 4, HEX);
	Serial.println( (0x0f & data) >> 0, HEX);
      }
   } while(idx < length);
    
    homeEasy.release();
  }
}
