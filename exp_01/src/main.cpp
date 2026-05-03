#include <Arduino.h>

// on the nodemcu internal LED is usually on pin 2 (D4)
const int LED_PIN = 2;

void setup() {
    // set runs once when the board pwers once
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    // loops runs forever
    digitalWrite(LED_PIN, LOW);     // trun LED on 
    delay(500);                     // wait 500 ms
    digitalWrite(LED_PIN, HIGH);    // turn LED off
    delay(500);                     // wait 500 ms
}
