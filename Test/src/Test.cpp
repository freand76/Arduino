#include "Arduino.h"

int ledState = LOW;             // ledState used to set the LED
const int ledPin =  13;      // the number of the LED pin

void setup() 
{ 
  Serial.begin(115200); 
  // prints title with ending line break 
  pinMode(ledPin, OUTPUT);      
  Serial.println("ASCII Table ~ Character Map"); 
} 

void loop() 
{ 
  delay(1000);
  if (ledState == LOW) {
      ledState = HIGH;
Serial.println("HIGH");
}   else {
      ledState = LOW;
Serial.println("LOW");
}

  // set the LED with the ledState of the variable:
  digitalWrite(ledPin, ledState);
} 
