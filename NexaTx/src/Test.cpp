#include "Arduino.h"
#include "NexaTx.h"

NexaTx nexaTx;

void setup() {
  Serial.begin(115200);
  Serial.println("Hello World!");
  nexaTx.init();
}

void loop() {
  delay(1000);
  Serial.println("On");  
  nexaTx.transmit(0x55d836, false, true, 2);
  delay(1000);
  Serial.println("Off");
  nexaTx.transmit(0x55d836, false, false, 2);
}
