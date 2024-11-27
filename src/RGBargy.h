#ifndef RGBargy_h
#define RGBargy_h

#define RGBG_MODE_640x480     1
#define RGBG_MODE_800x600     2
#define RGBG_MODE_1024x768    3

#define RGBG_COLORS_1bit      1
#define RGBG_COLORS_4bit_1111 2
#define RGBG_COLORS_4bit_121  3
#define RGBG_COLORS_8bit_332  4
#define RGBG_COLORS_8bit_2222 5
#define RGBG_COLORS_16bit_565 6

#define RGBG_HSYNC_PIN  4
#define RGBG_VSYNC_PIN  5
#define RGBG_COLOR_PINS 6

#define SWAP(a, b) { short t = a; a = b; b = t; }
#define TOPMASK    0b00001111
#define BOTTOMMASK 0b11110000

class RGBargy {
  public:
    RGBargy(byte mode);
    void pixel(short x, short y, byte color);
    void line(short x0, short y0, short x1, short y1, byte color);
    int get_mode_width();
    int get_mode_height();
    byte get_mode_bitdepth();
  private:
    int _pin;
    unsigned char * fb_pointer0;
}

#endif
