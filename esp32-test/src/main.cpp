#include <Arduino.h>

void setup() {
  pinMode(2, OUTPUT);   // GPIO2 é o LED interno (em muitos modelos)
}

void loop() {
  digitalWrite(2, HIGH);
  delay(500);
  digitalWrite(2, LOW);
  delay(500);
}
