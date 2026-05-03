#include "core_esp8266_features.h"
#include <Arduino.h>
#include <cstdint>

// Simple abstraction for Pin State logic
enum class Light {
    // inverted for common Anode
    On = LOW,
    Off = HIGH
};

struct RGBState {
    Light r, g, b;
};

class RGBLed {
    private:
        uint8_t pinR, pinG, pinB;
    public:
        RGBLed(uint8_t r, uint8_t g, uint8_t b) : pinR(r), pinG(g), pinB(b) {}

        void begin() {
            pinMode(pinR, OUTPUT);
            pinMode(pinG, OUTPUT);
            pinMode(pinB, OUTPUT);
            display({ Light::Off, Light::Off, Light::Off });
        }

        void display(RGBState state) const {
            digitalWrite(pinR, static_cast<int>(state.r));
            digitalWrite(pinG, static_cast<int>(state.g));
            digitalWrite(pinB, static_cast<int>(state.b));
        }
};

// D1 = 5, D2 = 4, D3 = 0
RGBLed statusLed(5, 4, 0);

void setup() {
    statusLed.begin();
}

void loop() {
    statusLed.display({ Light::On, Light::Off, Light::Off }); // red
    delay(1000);
    statusLed.display({ Light::Off, Light::On, Light::Off }); // green
    delay(1000);
    statusLed.display({ Light::Off, Light::Off, Light::On }); // blue
    delay(1000);
}
