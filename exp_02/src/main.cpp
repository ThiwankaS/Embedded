#include "c_types.h"
#include "core_esp8266_features.h"
#include <Arduino.h>
#include <cstdint>

class Led {
    private:
        uint8_t pin;
    public:
        Led(uint8_t p) : pin(p) {}

        void begin() {
            pinMode(pin, OUTPUT);
        }

        void on() const {
            digitalWrite(pin, HIGH);
        }

        void off() const {
            digitalWrite(pin, LOW);
        }
};

Led externalLed(5);

void setup() {
    externalLed.begin();
}

void loop() {
    externalLed.on();
    delay(1500);
    externalLed.off();
    delay(1500);
}
