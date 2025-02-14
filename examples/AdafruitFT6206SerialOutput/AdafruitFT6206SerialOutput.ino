/**
 * A very simple example that prints actual and raw X & Y values to Serial.
 * 
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2025 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */
#include <Arduino.h>

// First include the underlying touch library
#include <Adafruit_FT6206.h>
// Then include the addapter for it.
#include "AdafruitFT6206TouchScreen.h"

const uint16_t MY_DISPLAY_X = 240;
const uint16_t MY_DISPLAY_Y = 320;

// We're going to rate limit the touch, partly so we don't flood Serial but
// also to allow other things to happen in loop().
const uint16_t rateLimitMs = 10;
uint32_t rateLimitTimer = 0;

// Create adapter with default arguments
AdafruitFT6206TouchScreen touchPanel;
// Optional constructor args are as per Adafruit library:
// AdafruitFT6206TouchScreen(uint8_t thresh = FT62XX_DEFAULT_THRESHOLD, TwoWire *theWire = &Wire, uint8_t i2c_addr = FT62XX_DEFAULT_ADDR)

void setup() {
    Serial.begin(9600);
    delay(1000);

    touchPanel.begin();

    // If not 240x320 (note: native is portrait)
    //touchPanel.setDisplayWidth(MY_DISPLAY_X);
    //touchPanel.setDisplayHeight(MY_DISPLAY_Y);

    // Set the touch panel rotation to suit the display if required
    // 0 = native
    // 1 = 90deg CW
    // 2 = 180deg CW
    // 3 = 270deg CW
    //touchPanel.setRotation(0);

    Serial.println("Starting Adafruit FT6206 TouchScreenAdapter Serial Example...");
}

void loop() {
    if ( millis() > rateLimitTimer + rateLimitMs ) {
        rateLimitTimer = millis();
        //Get a TouchScreenAdapter::TouchPoint with the current values
        TouchScreenAdapter::TouchPoint tp = touchPanel.getTouchPoint();
        // If the panel is touched, get the raw point values and print both raw and translated value.
        if (tp.z > 0 ) {
            // Tidier output if your framework supports printf...
            //Serial.printf("Display X: %3i, Y: %3i, Z: %3i \n", tp.x, tp.y, tp.z);
            Serial.print(", Display X: ");
            Serial.print(tp.x);
            Serial.print(", Y: ");
            Serial.print(tp.y);
            Serial.print(", Z: ");
            Serial.print(tp.z);
            Serial.println();
        }
    }
}

