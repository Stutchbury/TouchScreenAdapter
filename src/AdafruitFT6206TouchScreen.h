/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2025 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#ifndef ADAFRUIT_FT6206_TOUCH_SCREEN_ADAPTER_H
#define ADAFRUIT_FT6206_TOUCH_SCREEN_ADAPTER_H

//Only create adapter if Adafruit's FT6206 library is already included.
#ifdef ADAFRUIT_FT6206_LIBRARY

#include <Arduino.h>


#include "TouchScreenAdapter.h"
#include <Wire.h>      // this is needed for FT6206


/**
 * @brief Concrete implementation of TouchScreenAdapter for Adafruit's FT6206 TouchScreen class.
 * 
 * @details Although the FT6206 reports X and Y as pixel coordinates, both axis are reversed! (ie higher numbers are up or left)
 * So setDisplayWidth() and setDisplayHeight() are *required*  if they are not the default 240(W) and 320(H) in order to reverse X & Y.
 * 
 */
class AdafruitFT6206TouchScreen : public TouchScreenAdapter {

public:

    /**
     * @brief Construct a new AdafruitFT6206TouchScreen Adapter
     * 
     * @details When using this constructor you *must* setDisplayWidth() and setDisplayHeight() if they are not the default 240(W) and 320(H)
     * 
     * @param thresh The touch threshold, defaults to <code>FT62XX_DEFAULT_THRESHOLD</code>
     * @param theWire The required <code>Wire</code> library, defaults to the standard Wire.
     * @param i2c_addr The I2C address, defaults to FT62XX_DEFAULT_ADDR
     */
    AdafruitFT6206TouchScreen(uint8_t thresh = FT62XX_DEFAULT_THRESHOLD,
             TwoWire *theWire = &Wire, uint8_t i2c_addr = FT62XX_DEFAULT_ADDR) { 
        this->thresh = thresh;
        this->wire = theWire;
        this->i2c_addr = i2c_addr;
    }

    /**
     * @brief begin() Calls the FT6106 <code>begin()</code> method.
     * 
     * @return true Successfully initialised the Adafruit FT6206 driver
     * @return false Failed to initialise the Adafruit FT6206 driver
     */
    bool begin(void) {
        return ctp.begin(thresh, wire, i2c_addr);
    }


    /**
     * @brief Get the Touch Point object
     * 
     * @return TouchScreenAdapter::TouchPoint 
     */
    TouchScreenAdapter::TouchPoint getTouchPoint() {
        TS_Point tsPoint = ctp.getPoint();
        TouchScreenAdapter::TouchPoint touchPoint;
        uint16_t x = rotation % 2 ? tsPoint.y : tsPoint.x;
        uint16_t y = rotation % 2 ? tsPoint.x : tsPoint.y;
        // Would you believe it?
        // The FT6206 report both X and Y backwards!
        touchPoint.x = ( rotation == 2 || rotation == 3 ) ? x : displayWidth - x;
        touchPoint.y = ( rotation == 1 || rotation == 2 ) ? y : displayHeight - y;
        touchPoint.z = tsPoint.z;
        return touchPoint;
    }

    /**
     * @brief Get the raw TouchPoint struct containing X/Y values directly from the underlyig library.
     * 
     * @details For the FT6206 this will result in the X and Y values being reversed! 
     * ie low values are botom/right and high values are top/left. 
     * 
     * @return TouchScreenAdapter::TouchPoint 
     */
    TouchScreenAdapter::TouchPoint getTouchPointRaw() {
        TS_Point p = ctp.getPoint();
        return TouchScreenAdapter::TouchPoint(p.x, p.y, p.z);
    }



private:

    Adafruit_FT6206 ctp = Adafruit_FT6206();
    uint8_t thresh = FT62XX_DEFAULT_THRESHOLD;
    TwoWire *wire = &Wire;
    uint8_t i2c_addr = FT62XX_DEFAULT_ADDR;



};



#endif
#endif
