# TouchScreenAdapter

TouchScreenAdapter is a library providing a unified touchscreen API. Included are implementations for typical touchscreen types: 4-wire resistive and capacitive.

The library provides a consistent TouchPoint structure, containing X & Y translated to display coordinates (even for resistive screens) and Z for touch/pressure where 0 = 'not touched' and >0 = 'touched'. Where available, pressure will be reported in the range 1-255.

Its primary use is for the [EventTouchScreen](https://github.com/Stutchbury/EventTouchScreen) library (that will become part of [InputEvents](https://github.com/Stutchbury/InputEvents)) but is also very effective as a source for an [LVGL](https://lvgl.io/) input device: [lv_indev_t](https://docs.lvgl.io/master/details/main-components/indev.html).

## The Adapter Class

The abstract TouchScreenAdapter class specifies the following methods:

- <code>void begin()</code> - required for Arduino compatibility.
- <code>TouchScreenAdapter::TouchPoint getTouchPoint()</code> - returns a TouchScreenAdapter::TouchPoint with X & Y display coordinates with Z where 0 is not touched and >= 1 is touched.
- <code>TouchScreenAdapter::TouchPoint getTouchPointRaw()</code> - returns a TouchScreenAdapter::TouchPoint with raw Y, Y and Z coordinates.
- <code>void setDisplayWidth(uint16_t width)</code> Set the display width. Required for resistive touch or if reversing X and/or Y is necessary.
- <code>void setDisplayHeight(uint16_t height)</code> Set the display height. Required for resistive touch or if reversing X and/or Y is necessary.
- <code>void setRotation(uint8_t rotation)</code> Rotate the touch panel as required.

## Concrete Classes

Concrete implementations provide a constructor suitable for the underlying library and any additional methods required to configure the touch panel.

Adapter classes are fairly trivial to create but must be 'header only' to allow Arduino IDE to compile without error when the underlying libraries are not installed. Pull requests are most welcome.

The concrete classes currently provided are:

- <code>AdafruitResistiveTouchScreen</code> - resistive, using Adafruit's [TouchScreen](https://github.com/adafruit/Adafruit_TouchScreen) library.
- <code>AdafruitFT6206TouchScreen</code> - I2C, using Adafruit's [FT6206](https://github.com/adafruit/Adafruit_FT6206_Library) library.

## Source Code

The TouchScreenAdapter Library is available via the [Arduino](https://docs.arduino.cc/software/ide/) IDE or [PlatformIO](https://platformio.org/) Library Managers by searching for 'TouchScreenAdapter'.

Please see [full Doxygen generated API documentation](https://stutchbury.github.io/TouchScreenAdapter/api/index.html) for more information.

## Dependencies

Although this library lists no dependencies, it will be dependent on whichever underlying touch screen library that the chosen adapter uses.

## Support

Feedback, pull requests and [bug reports](https://github.com/Stutchbury/TouchScreenAdapter/issues) are welcome or chat on the InputEvents [Discord](https://discord.gg/GDcEcWPKKm) channel if you have any questions.


