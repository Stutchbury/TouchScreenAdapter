/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2025 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#ifndef ADAFRUIT_RESISTIVE_TOUCH_SCREEN_ADAPTER_H
#define ADAFRUIT_RESISTIVE_TOUCH_SCREEN_ADAPTER_H

//Only create adapapter if Adafruit's TouchScreen is already included.
#ifdef _ADAFRUIT_TOUCHSCREEN_H_

#include <Arduino.h>

#include "TouchScreenAdapter.h"


/**
 * @brief Concrete implementation of TouchScreenAdapter for Adafruit's resistive TouchScreen class.
 * 
 * @details Provides a number of class methods to 'tune-in' the mapping of the raw resistance values in X and Y 
 * mapped to the display's X and Y pixel position. This will need to be done for each screen/MCU combination.

 * The Z pressure can also be mapped but do not recommend this as the reported pressures are not very repeatable.
 * 
 * Resistive screens will never be as consistent as capacitive ones but Some screen and board combinations work better than other - I suspect this is due to better pullup resistors.
 * 
 */
class AdafruitResistiveTouchScreen : public TouchScreenAdapter {

public:

    /**
     * @brief Construct a new Resistive Touch Screen object
     * 
     * @param pinXPos can be a digital pin
     * @param pinYPos Must be an analog pin, use "An" notation!
     * @param pinXNeg must be an analog pin, use "An" notation!
     * @param pinYNeg can be a digital pin
     * @param ohms Measurement of ohms between pinXPos and PinXNeg (default 300). Can improve touch accuracy.
     * @param displayWidth Native width of display with no rotation set (default 240)
     * @param displayHeight Native height of display with no rotation set (default 320)
     */
    AdafruitResistiveTouchScreen(uint8_t pinXPos, uint8_t pinYPos, uint8_t pinXNeg,uint8_t pinYNeg, uint16_t ohms=300, uint16_t displayWidth=240, uint16_t displayHeight=320 ) 
    {   
        nativeDisplayWidth = displayWidth;
        nativeDisplayHeight = displayHeight;
        this->displayHeight = nativeDisplayHeight;
        this->displayWidth = nativeDisplayWidth;
        touchscreen = new TouchScreen(pinXPos, pinYPos, pinXNeg, pinYNeg, ohms);
    }

    /**
     * @brief Destroy the AdafruitResistiveTouchScreen object
     * 
     */
    ~AdafruitResistiveTouchScreen() {
        delete touchscreen; //Safe to delete even if null;
    }


    /**
     * @brief Get the TouchPoint struct
     * 
     * @details The TouchPoint is populated with X and Y mapped from the raw resistance values to display pixel position.
     * 
     * @return TouchScreenAdapter::TouchPoint 
     */
    TouchScreenAdapter::TouchPoint getTouchPoint(void) { 
        TSPoint tsPoint = touchscreen->getPoint();
        //Serial.printf("TSPoint X: %3i, Y: %3i, Z: %3i \n", tsPoint.x, tsPoint.y, tsPoint.x);
        TouchScreenAdapter::TouchPoint touchpoint;
        // The resistive touch panel will irregularly report touch when at rest
        // I suspect it is the flexible membrane 'relaxing' or a read error in the TouchScreen lib,
        // so we have to chck if Z, X and Y are within the expected resistance range.
        if ( tsPoint.z > (int16_t)minRawZ && tsPoint.z < (int16_t)maxRawZ
        && tsPoint.x > (int16_t)minRawX && tsPoint.x < (int16_t)maxRawX
        && tsPoint.y > (int16_t)minRawY && tsPoint.y < (int16_t)maxRawY ) {
            touchpoint.z = constrain(map(tsPoint.z, maxRawZ, minRawZ, 1, 255), 1, 255);
            uint16_t x = rotation % 2 ? tsPoint.y : tsPoint.x;
            uint16_t y = rotation % 2 ? tsPoint.x : tsPoint.y;
            if ( rotation == 2 || rotation == 3 ) {
                touchpoint.x = constrain(map(x, maxRawX, minRawX, 0, displayWidth), 0, displayWidth-1);
            } else {
                touchpoint.x = constrain(map(x, minRawX, maxRawX, 0, displayWidth), 0, displayWidth-1);
            }
            if ( rotation == 1 || rotation == 2 ) {
                touchpoint.y = constrain(map(y, maxRawY, minRawY, 0, displayHeight), 0, displayHeight-1);
            } else {
                touchpoint.y = constrain(map(y, minRawY, maxRawY, 0, displayHeight), 0, displayHeight-1);
            }
        }
        return touchpoint;
    }


    /**
     * @brief Get the raw TouchPoint struct
     * 
     * @details The TouchPoint is populated with the raw resistance values from the Adafruit TouchScreen library.
     * 
     * Use these values to measure and set the raw min/max X and Y values.
     * 
     * @return TouchScreenAdapter::TouchPoint 
     */
    TouchScreenAdapter::TouchPoint getTouchPointRaw(void) {
        TSPoint tsPoint = touchscreen->getPoint();
        TouchScreenAdapter::TouchPoint touchpoint;
        touchpoint.x = tsPoint.x;
        touchpoint.y = tsPoint.y;
        touchpoint.z = tsPoint.z;
        return touchpoint;
    }

    /**
     * @brief Empty begin() implementation
     * 
     * @return true Always.
     * @return false Never.
     */
    bool begin(void) { return true; }

    // Not implemented because the Adfruit TouchScreen library doesn't use the pressueThreshold (or pressureThreshhold :-)
    // void setPressureThreshold(uint8_t pressure) {
    //     touchscreen->pressureThreshhold = pressure;
    // }

    /**
     * @brief Set the minimun raw Y value
     * 
     * @param limit The minimum Y value reported by getTouchPointRaw()
     */
    void setMinRawY(uint16_t limit) { minRawY = limit; }

    /**
     * @brief Set the minimun raw X value
     * 
     * @param limit The minimum X value reported by getTouchPointRaw()
     */
    void setMinRawX(uint16_t limit) { minRawX = limit; }

    /**
     * @brief Set the maximum raw X value
     * 
     * @param limit The maximum X value reported by getTouchPointRaw()
     */
    void setMaxRawX(uint16_t limit) { maxRawX = limit; }

    /**
     * @brief Set the maximum raw Y value
     * 
     * @param limit The maximum Y value reported by getTouchPointRaw()
     */
    void setMaxRawY(uint16_t limit) { maxRawY = limit; }

    /**
     * @brief Set the maximum raw Z value
     * 
     * @param limit The maximum Z value reported by getTouchPointRaw()
     */
    void setMinRawZ(uint16_t limit) { minRawZ = limit; }

    /**
     * @brief Set the maximum raw Z value
     * 
     * @param limit The maximum Z value reported by getTouchPointRaw()
     */
    void setMaxRawZ(uint16_t limit) { maxRawZ = limit; }

protected:

private:

    //Have to use heap or Teensy on PlatformIO will fail to link. :-/
    //https://forum.pjrc.com/index.php?threads/multiple-definition-of-error-when-using-std-function-with-argument-teensy41-platformio.74505/
    TouchScreen *touchscreen = nullptr;

    uint16_t minRawY = 100;
    uint16_t maxRawY = 900;
    uint16_t minRawX = 100;
    uint16_t maxRawX = 900;
    uint16_t minRawZ = 1;
    uint16_t maxRawZ = 1023;
    uint8_t zThreshold = 10;
};


#endif
#endif
