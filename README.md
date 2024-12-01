# RGBargy

__VGA display library for Arduino and RP2* microcontrollers__

This is an Arduino library providing a VGA display-driver for the RP2* family of microcontrollers. The goal of this library is to provide an easy way to drive a relatively low-res vga screen directly from the Arduino framework, and not having to install compilers or the pico development environment.

```
#include <RGBargy.h>

RGBargy rgbg(RGBG_MODE_640x480, 125);

void setup() {
  randomSeed(analogRead(0));
}

void loop() {
    rgbg.line(r(640), r(480), r(640), r(480), r(8));
    delay(1);
}
int r(int r) {return random(r);}
```

## Hardware connections / pinout

| RP2040 GPIO pin | VGA connector pin |
|-----------------|-------------------|
| GPIO 4          | Horizontal sync   |
| GPIO 5          | Vertical sync     |
| GPIO 6          | Red   (w/330R)    |
| GPIO 7          | Green (w/330R)    |
| GPIO 8          | Blue  (w/330R)    |
| GND             | GND               |


## Supported resolutions

For this library to work, the CPU frequency has to be selected manually in the Arduino IDE to match the resolution used in the code. The CPU frequency must be an integer multiple of the pixel frequencies below. See "CPU freq" column.

Please note that in the current version you actually need to pass the selected cpu freq in the constructor, because it's tricky to get an correct result from the pico api right now.

| Resolution  | Refresh rate | Pixel freq | set CPU freq | Clock divider | Dev status   |
|-------------|-------------:|-----------:|-------------:|--------------:|--------------|
| __640x480__ |         60Hz | 25.175 MHz |      125 MHz |            5x | __working__  |
|   640x480   |         60Hz | 25.175 MHz |      150 MHz |            8x | devel        |
|   640x480   |         60Hz | 25.175 MHz |      175 MHz |            8x | devel        |
|   640x480   |         60Hz | 25.175 MHz |      200 MHz |            8x | maybe later  |
| __800x600__ |         60Hz | 40.000 MHz |      120 MHz |            3x | __working__  |
|   800x600   |         60Hz | 40.000 MHz |      200 MHz |            5x | maybe later  |
|   800x600   |         85Hz | 56.250 MHz |      225 MHz |            4x | __testing__  |
|  1024x768   |         70Hz | 75.000 MHz |      225 MHz |            3x | devel        |


## VGA timings

The table below shows sync polarity, front porch, sync pulse and back porch lengths for both horizontal and vertical sync lines. For the 640x480 resolution we're actually using a round 25MHz as the pixel clock, instead of 25.175MHz specified in the standard. All monitors we tested seem to tolarate this so far.

| Resolution | Refresh rate | Horizontal |       |      |      | Vertical |       |      |      |
|------------|-------------:|-----------:|------:|-----:|-----:|:--------:|------:|-----:|-----:|
|            |              | polarity   | front | sync | back | polarity | front | sync | back |
|   640x480  |         60Hz |    ---     |    16 |   96 |   48 |   ---    |    10 |    2 |   33 |
|   800x600  |         60Hz |    +++     |    40 |  128 |   88 |   +++    |     1 |    4 |   23 |
|   800x600  |         85Hz |    +++     |    32 |   64 |  152 |   +++    |     1 |    3 |   27 |
|  1024x768  |         70Hz |    ---     |    24 |  136 |  144 |   ---    |     3 |    6 |   29 |


## References

### Algos
- [Midpoint circle algorithm](https://en.wikipedia.org/wiki/Midpoint_circle_algorithm)
- [Bresenham's line algorithm](https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm)

### Tools / libraries used
- [Arduino Pico Core ](https://github.com/earlephilhower/arduino-pico)
- [Online PIO Assembler](https://wokwi.com/tools/pioasm)

### Docs
https://www.raspberrypi.com/documentation/microcontrollers/pico-series.html


## License
- [MIT license](https://mit-license.org/)
