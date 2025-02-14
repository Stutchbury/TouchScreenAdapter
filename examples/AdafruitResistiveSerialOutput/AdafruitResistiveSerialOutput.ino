/**
 * A very simple example that prints actual and raw X & Y values to Serial.
 * 
 * Useful for tuning the top, bottom, left & right edges.
 * 
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2025 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */
#include <Arduino.h>

// First include the underlying touch library
#include "TouchScreen.h"
// Then include the addapter for it.
#include "AdafruitResistiveTouchScreen.h"

//For the Adafruit TouchScreen.h
const uint8_t XP = A7;  // Y+ must be an analog pin, use "An" notation!
const uint8_t YP = A6;  // X- must be an analog pin, use "An" notation!
const uint8_t XM = A9;  // Y- can be a digital pin
const uint8_t YM = A8;  // X+ can be a digital pin


// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For mine, it's 317 ohms across the X plate
const uint16_t TOUCHSCREEN_OHMS = 317;

const uint16_t MY_DISPLAY_X = 240;
const uint16_t MY_DISPLAY_Y = 320;

// We're going to rate limit the touch, partly so we don't flood Serial but
// also to allow other things to happen in loop().
const uint16_t rateLimitMs = 10;
uint32_t rateLimitTimer = 0;

// Create adapter. If omitted, last two args will default to 320 and 240
AdafruitResistiveTouchScreen touchPanel(XP, YP, XM, YM, TOUCHSCREEN_OHMS);

void setup() {
    Serial.begin(9600);
    delay(1000);

    touchPanel.begin();

    // If not 240x320 (note: native is portrait)
    //touchPanel.setDisplayWidth(MY_DISPLAY_X);
    //touchPanel.setDisplayHeight(MY_DISPLAY_Y);

    //Adjust the edges of the touch panel. These are the min/max numbers from getTouchPointRaw()
    touchPanel.setMinRawX(100);
    touchPanel.setMaxRawX(905);
    touchPanel.setMinRawY(160);
    touchPanel.setMaxRawY(900);
    //And the min/max raw touch pressure
    touchPanel.setMinRawZ(48); //For resistive, low value is a heavy touch
    touchPanel.setMaxRawZ(750); //For resistive, high value is a light touch

    // Set the touch panel rotation to suit the display if required
    // 0 = native
    // 1 = 90deg CW
    // 2 = 180deg CW
    // 3 = 270deg CW
    //touchPanel.setRotation(0);

    Serial.println("Starting Adafruit Resistive TouchScreenAdapter Serial Example...");
}

void loop() {
    if ( millis() > rateLimitTimer + rateLimitMs ) {
        rateLimitTimer = millis();
        //Get a TouchScreenAdapter::TouchPoint with the current values
        TouchScreenAdapter::TouchPoint tp = touchPanel.getTouchPoint();
        // If the panel is touched, get the raw point values and print both raw and translated value.
        if (tp.z > 0 ) {
            //Get the raw TouchPoit
            TouchScreenAdapter::TouchPoint tpr = touchPanel.getTouchPointRaw();
            // Tidier output if your framework supports printf...
            //Serial.printf("Touched Raw X: %3i, Y: %3i, Z: %3i, Display X: %3i, Y: %3i, Z: %3i \n", tpr.x, tpr.y, tpr.z, tp.x, tp.y, tp.z);
            Serial.print("Touched Raw X: ");
            Serial.print(tpr.x);
            Serial.print(",\tY: ");
            Serial.print(tpr.y);
            Serial.print(",\tZ: ");
            Serial.print(tpr.z);
            Serial.print(",\tDisplay X: ");
            Serial.print(tp.x);
            Serial.print(",\tY: ");
            Serial.print(tp.y);
            Serial.print(",\tZ: ");
            Serial.print(tp.z);
            Serial.println();
        }
    }
}

