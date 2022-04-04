#include <Arduino.h>
#include <HardwareSerial.h>

//transmit sketch
void setup() {
  Serial2.begin(9600);
  Serial.begin(9600);
}

void loop() {
  int n = random(1,300);
  Serial2.write(n);
  Serial.println(n);
  delay(10000);
}
