#include "HardwareSerial.h"
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT    64
#define OLED_RESET       -1

// creating display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
    Serial.begin(115200);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 Allocation failed!"));
        for(;;); // don't proceed, loop for ever
    }

    display.clearDisplay();
    display.setTextSize(1);                 // normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);    // set text color
    display.setCursor(0,0);                 // start at top left corner

    display.println(F("Hello,"));
    display.setTextSize(2);                 // increase the text size
    display.println(F("Thiwanka!"));
    
    display.display();
}

void loop() {
    // only need to print once, so the loop stays empty for now
}
