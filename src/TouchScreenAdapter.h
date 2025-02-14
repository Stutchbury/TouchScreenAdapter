/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2025 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */


#ifndef TOUCH_SCREEN_ADAPTER_H
#define TOUCH_SCREEN_ADAPTER_H

#include <Arduino.h>

/**
 * @brief A lightweight abstract Abstract class for touch screen panels.
 * 
 */
class TouchScreenAdapter {

public:

    /**
     * @brief A consistent struct containing points X & Y translated to display coordinates plus Z as pressure.
     * 
     */
    struct TouchPoint {

        /**
         * @brief Construct a new Touch Point object
         * 
         */
        TouchPoint(void)  { x = y = z = 0; }

        /**
         * @brief Construct a new Touch Point object
         * 
         * @param _x 
         * @param _y 
         * @param _z 
         */
        TouchPoint(int16_t _x, int16_t _y, int16_t _z) {
            x = _x;
            y = _y;
            z = _z;
        }
        uint16_t x = 0; ///< Mapped 0 to screen width -1 
        uint16_t y = 0; ///< Mapped 0 to screen height -1
        uint16_t z = 0; ///< Mapped 0-255 representing pressure (0 = not touched). is uint16_t because raw is odten > 255

        /**
         * @brief Assignment operator.
         * 
         * @param tp A TouchPoint
         * @return TouchPoint& 
         */
        TouchPoint& operator=(const TouchPoint& tp) {
            // Check for self-assignment
            if (this != &tp) {
                x = tp.x;
                y = tp.y;
                z = tp.z;
            }
            return *this;
        }
    };

    /**
     * @brief Set the native (non-rotated) display width (X) in pixels
     * 
     * @details Is set to 240 by default. For some adapters (resistive) this *must* be set to the native width of your display.
     * For adapters that report pixel position directly, this is only reqired if X and/or Y need to be reversed. (Ahem, FT6206).
     * 
     * @param widthPx Width in pixels
     */
    void setDisplayWidth(uint16_t widthPx) { 
        nativeDisplayWidth = widthPx; 
        displayWidth = nativeDisplayWidth;
        setRotation(rotation);
    }

    /**
     * @brief Set the native (non-rotated) display height (Y) in pixels
     * 
     * @details Is set to 320 by default. For some adapters (resistive) this *must* be set to the native height of your display.
     * For adapters that report pixel position directly, this is only used to reverse X or Y.
     * 
     * @param heightPx Width in pixels
     */
    void setDisplayHeight(uint16_t heightPx) {
        nativeDisplayHeight = heightPx; 
        displayHeight = nativeDisplayHeight;
        setRotation(rotation);
    }

    /**
     * @brief Get the TouchPoint object 
     * 
     * @details Returns a TouchPoint. Display are x & y populated (ie non-zero) only if touched.
     * 
     * @return TouchPoint 
     */
    virtual TouchScreenAdapter::TouchPoint getTouchPoint(void) = 0;

    /**
     * @brief Get a TouchPoint struct containing raw values from the underlying library. 
     * 
     * @details Returns a TouchPoint with raw x, y & z populated from the underlying library. For most panels this will usually return the same values as getTouchPoint() but can be useful for tuning resistive panels.
     * Note: This will return a populated TouchPoint irrespective of the Z value. Generally used for setup or debugging.
     * 
     * @return TouchPoint 
     */
    virtual TouchScreenAdapter::TouchPoint getTouchPointRaw(void) = 0;

    /**
     * @brief For compatibility with the Arduino library convention.
     * 
     * @details Must be implemented (but can be empty) as some touch libraries require it. Can return a boolean to idicate if initialisation has failed.
     * 
     * @return true The initialisation was successful.
     * @return false The initialisation was failed.
     */
    virtual bool begin(void) = 0;

    /**
     * @brief Set the rotation touch screen.
     * 
     * @details This should be called at the same time the display rotation is set.
     * 
     * @param r Follows the convention of:
     *  - 0 = native orientation
     *  - 1 = 90degrees from native
     *  - 2 = 180degrees from native
     *  - 3 = 270degrees from native
     */
    void setRotation(uint8_t r ) {
        rotation = (r & 3);
        switch (rotation) {
        case 0:
        case 2:
            displayWidth = nativeDisplayWidth;
            displayHeight = nativeDisplayHeight;
            break;
        case 1:
        case 3:
            displayWidth = nativeDisplayHeight;
            displayHeight = nativeDisplayWidth;
            break;
        }
    }

protected:

    /**
     * @brief The current rotation of the touch panel. Default is 0 (native orientation).
     * 
     */
    uint8_t rotation = 0;

    uint16_t nativeDisplayWidth = 240; ///< The non-rotated width of the display in pixels.
    uint16_t nativeDisplayHeight = 320; ///< The non-rotated height of the display in pixels.

    uint16_t displayWidth = nativeDisplayWidth; ///< The (optionally rotated) width of the display in pixels.
    uint16_t displayHeight = nativeDisplayHeight; ///< The (optionally rotated) height of the display in pixels.

};


#endif