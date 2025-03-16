# RGBargy

__VGA display library for Arduino and RP2* microcontrollers__

<img align="right" width="320" src="docs/assets/images/helloworld.gif">This is an Arduino library providing a VGA display-driver for the RP2* family of microcontrollers. The goal of this library is to provide an easy way to drive a relatively low-res vga screen directly from the Arduino framework, and not having to install compilers or the pico development environment.

It can drive up to 2 screens simultaneously on an RP2040 microcontroller, and 3 screens on RP2350 microcontrollers (this latter one is a work in progress).

## Hardware connections / pinout

...

## Supported resolutions

For this library to work, the CPU frequency has to be selected manually in the Arduino IDE to match the resolution used in the code. The CPU frequency must be an integer multiple of the pixel frequencies below. See "Pixel freq" and "CPU freq" column. (note: for 25.175Mhz pixel frequency, the round 25MHz cpu frequency is apparently good enough)

| Resolution   | Refresh rate | Pixel freq |  CPU freq | Divider | Dev status  |
|--------------|-------------:|-----------:|----------:|--------:|-------------|
|  __640x480__ |         60Hz | 25.175 MHz |   100 MHz |      4x | __working__ |
|  __640x480__ |         60Hz | 25.175 MHz |   125 MHz |      5x | __working__ |
|  __640x480__ |         60Hz | 25.175 MHz |   150 MHz |      6x | __working__ |
|    640x480   |         60Hz | 25.175 MHz |   175 MHz |      7x | testing     |
|  __640x480__ |         60Hz | 25.175 MHz |   200 MHz |      8x | __working__ |
|  __640x480__ |         60Hz | 25.175 MHz |   225 MHz |      9x | testing     |
|  __640x480__ |         60Hz | 25.175 MHz |   250 MHz |     10x | testing     |
|  __800x600__ |         60Hz | 40.000 MHz |   120 MHz |      3x | __working__ |
|  __800x600__ |         60Hz | 40.000 MHz |   200 MHz |      5x | __working__ |
|    800x600   |         85Hz | 56.250 MHz |   225 MHz |      4x | devel       |
|    800x600   |         60Hz | 40.000 MHz |   240 MHz |      6x | __working__ |
| __1024x768__ |         70Hz | 75.000 MHz |   225 MHz |      3x | __working__ |


## VGA timings

The table below shows sync polarity, front porch, sync pulse and back porch lengths for both horizontal and vertical sync lines.

| Resolution | Refresh | Horizontal |       |      |      | Vertical |       |      |      |
|------------|--------:|-----------:|------:|-----:|-----:|:--------:|------:|-----:|-----:|
|            |         | polarity   | front | sync | back | polarity | front | sync | back |
| __640x480__|    60Hz |    ---     |    16 |   96 |   48 |   ---    |    10 |    2 |   33 |
| __800x600__|    60Hz |    +++     |    40 |  128 |   88 |   +++    |     1 |    4 |   23 |
|   800x600  |    85Hz |    +++     |    32 |   64 |  152 |   +++    |     1 |    3 |   27 |
|__1024x768__|    70Hz |    ---     |    24 |  136 |  144 |   ---    |     3 |    6 |   29 |

For the 640x480 resolution a round 25MHz is used as the pixel clock, instead of 25.175MHz specified in the standard. For the 1024x768 resolution the vertical front porch of 2 rows is used instead of 3 rows, because of PIO state-machine space-constraints. All monitors tested seem to tolerate these so far.


## References

### Algos
- [Midpoint circle algorithm](https://en.wikipedia.org/wiki/Midpoint_circle_algorithm)
- [Bresenham's line algorithm](https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm)

### Tools / libraries used
- [Arduino Pico Core ](https://github.com/earlephilhower/arduino-pico)
- [Online PIO Assembler](https://wokwi.com/tools/pioasm)

### Docs
- [Pico Series](https://www.raspberrypi.com/documentation/microcontrollers/pico-series.html)
- [Arduino-Pico](https://arduino-pico.readthedocs.io/en/latest/)

## Licensing and Credits
- __RGBargy__ licensed under [MIT license](https://mit-license.org/)
- The __Arduino IDE__ and __ArduinoCore-API__ by the Arduino team. IDE is licensed under [GPL](https://www.gnu.org/licenses/gpl-3.0.html).
- The __RP2040 GCC-based toolchain__ is licensed under the [GPL](https://www.gnu.org/licenses/gpl-3.0.html).
- The __Pico-SDK__ is by Raspberry Pi (Trading) Ltd and licensed under the [BSD 3-Clause license](https://opensource.org/license/bsd-3-clause).
- __Arduino-Pico__ core files are licensed under the [LGPL](https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html).
