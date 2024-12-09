#ifndef RGBargy_h
#define RGBargy_h

#define RGBG_MODE_640x480  1
#define RGBG_MODE_800x600  2
#define RGBG_MODE_1024x768 3

#define RGBG_COLORS_1bit_i         1
#define RGBG_COLORS_4bit_rgbi      2
#define RGBG_COLORS_4bit_rggb      3
#define RGBG_COLORS_8bit_rrrgggbb  4
#define RGBG_COLORS_8bit_rrggbbii  5

// #define RGBG_HSYNC_PIN  4
// #define RGBG_VSYNC_PIN  5
// #define RGBG_COLOR_PINS 6

#define RGBG_HSYNC_PIN  21
#define RGBG_VSYNC_PIN  20
#define RGBG_COLOR_PINS 16

#define SWAP(a, b) { short t = a; a = b; b = t; }

class RGBargy {
  public:
    RGBargy(short mode_);
    ~RGBargy();

    void begin();
    void clear();
    void pixel(short x, short y, char color);
    void hline(short x, short y, short l, char color);
    void vline(short x, short y, short l, char color);
    void line(short x0, short y0, short x1, short y1, char color);
    void rect(short x0, short y0, short x1, short y1, char color, bool fill=false);
    void circle(short xc, short yc, short r, char color);

    int get_mode_width();
    int get_mode_height();
    int get_mode_bitdepth();
    int get_cpu_mhz();

  private:
    short mode, mode_width, mode_height, mode_hfrontporch;
    int fb_size;
    unsigned char * fb_pointer0;
    void symm8_plot(short xc, short yc, short x, short y, char c);
};

#endif
