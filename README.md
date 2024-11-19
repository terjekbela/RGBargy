# VGAcontrol library for arduino / rp2040 / rp2350

This is an arduino library providing a display-driver using the VGA protocol for the RP2* family of microcontrollers. The goal of this library is to provide an easy way to drive a relatively low-res vga screen from directly the arduino framework, and not having to install the pico development environment.

## VGA timings for popular resolutions and refresh rates
| Resolution | Refresh rate | Pixel freq |  Horizontal     |                |                | Vertical        |                |                |
|------------|--------------|-----------:|----------------:|---------------:|---------------:|----------------:|---------------:|---------------:|
|            |              |            | front<br/>porch | sync<br/>pulse | back<br/>porch | front<br/>porch | sync<br/>pulse | back<br/>porch |
|   640x480  |        60 Hz | 25.175 MHz |              16 |             96 |             48 |              11 |              2 |             31 |
|   640x480  |        72 Hz | 31.500 MHz |              24 |             40 |            128 |               9 |              3 |             28 |
|   640x480  |        75 Hz | 31.050 MHz |              16 |             96 |             48 |              11 |              2 |             32 |
|   640x480  |        75 Hz | 31.050 MHz |              32 |             48 |            112 |               1 |              3 |             25 |
|   800x600  |        56 Hz | 38.810 MHz |              32 |            128 |            128 |               1 |              4 |             14 |
|   800x600  |        60 Hz | 40.000 MHz |              40 |            128 |             88 |               1 |              4 |             23 |
|   800x600  |        72 Hz | 50.000 MHz |              56 |            120 |             64 |              37 |              6 |             23 |
|   800x600  |        75 Hz | 49.500 MHz |              16 |             80 |            160 |               1 |              2 |             21 |
|   800x600  |        85 Hz | 56.250 MHz |              32 |             64 |            152 |               1 |              3 |             27 |
|  1024x768  |        60 Hz | 65.000 MHz |              24 |            136 |            160 |               3 |              6 |             29 |
|  1024x768  |        70 Hz | 75.000 MHz |              24 |            136 |            144 |               3 |              6 |             29 |
|  1024x768  |        75 Hz | 78.750 MHz |              16 |             96 |            176 |               1 |              3 |             28 |
|  1024x768  |        85 Hz | 94.500 MHz |              48 |             96 |            208 |               1 |              3 |             36 |
