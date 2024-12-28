#ifndef RGBargy_h
#define RGBargy_h

#define RGBG_MODE_640x480  1
#define RGBG_MODE_800x600  2
#define RGBG_MODE_1024x768 3

#define RGBG_PORT_0 0
#define RGBG_PORT_1 1
#define RGBG_PORT_2 2

#define RGBG_COLORS_4bit_iiii      1
#define RGBG_COLORS_4bit_rgbi      2
#define RGBG_COLORS_4bit_rggb      3
#define RGBG_COLORS_8bit_rrrgggbb  4
#define RGBG_COLORS_8bit_rrggbbii  5

#define RGBG_LARGE_OFF   0
#define RGBG_LARGE_4     4
#define RGBG_LARGE_5     5
#define RGBG_LARGE_8     8
#define RGBG_LARGE_10   10
#define RGBG_LARGE_12   12
#define RGBG_LARGE_16   16

#define RGBG_PORT0_HSYNC_PIN  21
#define RGBG_PORT0_VSYNC_PIN  20
#define RGBG_PORT0_COLOR_PINS 16
#define RGBG_PORT1_HSYNC_PIN  14
#define RGBG_PORT1_VSYNC_PIN  15
#define RGBG_PORT1_COLOR_PINS 10
#define RGBG_PORT2_HSYNC_PIN  8
#define RGBG_PORT2_VSYNC_PIN  9
#define RGBG_PORT2_COLOR_PINS 4

#define SWAP(a, b) { short t = a; a = b; b = t; }

class RGBargy {
  public:
    RGBargy(byte mode_, byte port_=RGBG_PORT_0);
    ~RGBargy();

    void begin(short large_=0);
    void clear();
    void grid(char c);
    void pixel(short x, short y, char c);
    void hline(short x, short y, short l, char c);
    void vline(short x, short y, short l, char c);
    void line(short x0, short y0, short x1, short y1, char c);
    void rect(short x0, short y0, short x1, short y1, char c, bool f=false);
    void circle(short xc, short yc, short r, char c, bool f=false);
    void ellipse(short xc, short yc, short rx, short ry, char c, bool f=false);
    void superellipse(short xc, short yc, short rx, short ry, float deg, char c);

    int get_mode_width();
    int get_mode_height();
    int get_mode_bitdepth();
    int get_cpu_mhz();

  private:
    byte mode, port, large;
    short mode_width, mode_height, mode_hfrontporch;
    short hsync_active, vsync_active, color_active;
    int fb_size;
    unsigned char * fb_pointer0;
    void symm8_plot(short xc, short yc, short x, short y, char c);
    void symm4_plot(short xc, short yc, short x, short y, char c);
    void symm4_fill(short xc, short yc, short x, short y, char c);
    void symm2_fill(short xc, short yc, short x, short y, char c);
};

#endif
