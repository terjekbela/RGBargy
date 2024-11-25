# RGBargy

__VGA display library for Arduino and RP2* microcontrollers__

This is an Arduino library providing a VGA display-driver for the RP2* family of microcontrollers. The goal of this library is to provide an easy way to drive a relatively low-res vga screen directly from the Arduino framework, and not having to install compilers or the pico development environment.

```
#include <RGBargy.h>

RGBargy rgbg(RGBG_MODE_640x480);

void setup() {
  randomSeed(analogRead(0));
}

void loop() {
    rgbg.line(
        random(640), random(480), random(640), random(480), random(7)+1
    );
    delay(1);
}
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


## VGA timings for supported resolutions

For this library to work, the CPU frequency has to be selected manually in the Arduino IDE to match the resolution used in the code. The CPU frequency must be an integer multiple of the pixel frequencies below. See "CPU freq" column.

| Resolution  | Refresh rate | Pixel freq | CPU frequency | Clock divider | Status       |
|-------------|-------------:|-----------:|--------------:|--------------:|--------------|
| __640x480__ |         60Hz | 25.175 MHz |  125 MHz      |            5x | working      |
|   800x600   |         72Hz | 50.000 MHz |  150 MHz      |            3x | in the works |
|   800x600   |         85Hz | 56.250 MHz |  225 MHz      |            4x | easier?!?    |
|  1024x768   |         70Hz | 75.000 MHz |  225 MHz      |            3x | in the works |

The table below shows front porch, sync pulse and back porch lengths for both horizontal and vertical sync lines. For the 640x480 resolution we're actually using a round 25MHz as the pixel clock, instead of 25.175MHz specified in the standard. All monitors we tested seem to tolarate this so far.

| Resolution | Horizontal sync |       |       |      | Vertical sync |       |      |      |
|------------|-----------------|------:|------:|-----:|---------------|------:|-----:|-----:|
|            | sync polarity   | front | sync  | back | sync polarity | front | sync | back |
|   640x480  | NEGATIVE ---    |    16 |    96 |   48 | NEGATIVE ---  |    10 |    2 |   33 |
|   800x600  | POSITIVE +++    |    56 |   120 |   64 | POSITIVE +++  |    36 |    6 |   23 |
|   800x600  | POSITIVE +++    |    56 |   120 |   64 | POSITIVE +++  |     1 |    3 |   27 |
|  1024x768  | NEGATIVE ---    |    24 |   136 |  144 | NEGATIVE ---  |     3 |    6 |   29 |


## References

### Algos
- [Midpoint circle algorithm](https://en.wikipedia.org/wiki/Midpoint_circle_algorithm)
- [Bresenham's line algorithm](https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm)

### Tools / libraries used
- [Arduino Pico Core ](https://github.com/earlephilhower/arduino-pico)
- [Online PIO Assembler](https://wokwi.com/tools/pioasm)

### Docs

## License
- MIT license
