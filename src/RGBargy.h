#ifndef RGBargy_h
#define RGBargy_h

#define VGA_MODE_640x480  1
#define VGA_MODE_800x600  2
#define VGA_MODE_1024x768 3

#define VGA_PORT_0 0
#define VGA_PORT_1 1
#define VGA_PORT_2 2

#define VGA_COLOR_4bit_iiii      1
#define VGA_COLOR_4bit_rgbi      2
#define VGA_COLOR_4bit_rggb      3
#define VGA_COLOR_8bit_rrrgggbb  4
#define VGA_COLOR_8bit_rrggbbii  5

#define VGA_LARGE_OFF   0
#define VGA_LARGE_4     4
#define VGA_LARGE_5     5
#define VGA_LARGE_8     8
#define VGA_LARGE_10   10
#define VGA_LARGE_12   12
#define VGA_LARGE_16   16

#define VGA_PORT0_COLOR_PINS 0
#define VGA_PORT0_HSYNC_PIN  5
#define VGA_PORT0_VSYNC_PIN  4
#define VGA_PORT1_COLOR_PINS 6
#define VGA_PORT1_HSYNC_PIN  11
#define VGA_PORT1_VSYNC_PIN  10
#define VGA_PORT2_HSYNC_PIN  12
#define VGA_PORT2_VSYNC_PIN  21
#define VGA_PORT2_COLOR_PINS 20

#define SWAP(a, b) { short t = a; a = b; b = t; }

class RGBargy {
  public:
    RGBargy(byte mode_, byte port_=VGA_PORT_0);
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

    int fb_size;
    unsigned char * fb_pointer0;

    short hsync_active, vsync_active, color_active;

    PIO  pio;
    byte pio_hsync_sm = 0;
    byte pio_hsync_pin;
    byte pio_hsync_offset;
    byte pio_vsync_sm = 1;
    byte pio_vsync_pin;
    byte pio_vsync_offset;
    byte pio_color_sm = 2;
    byte pio_color_pins;
    byte pio_color_offset;

    byte dma_color_chan_0;
    byte dma_color_chan_1;

    void symm8_plot(short xc, short yc, short x, short y, char c);
    void symm4_plot(short xc, short yc, short x, short y, char c);
    void symm4_fill(short xc, short yc, short x, short y, char c);
    void symm2_fill(short xc, short yc, short x, short y, char c);
};

#endif
