#include "Arduino.h"
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/dma.h"

#include "RGBargy.h"
#include "vgahsync_640x480.pio.h"
#include "vgavsync_640x480.pio.h"
#include "vgacolor_640x480.pio.h"

#define HSYNC_PIN  4
#define VSYNC_PIN  5
#define COLOR_PINS 6

#define SWAP(a, b) { short t = a; a = b; b = t; }
#define TOPMASK    0b00001111
#define BOTTOMMASK 0b11110000

#define TXCOUNT 153600
unsigned char vga_framebuffer[TXCOUNT];
unsigned char * fb_pointer = &vga_framebuffer[0];

short mode_width, mode_height;
short mode_hfrontporch, mode_hsync, mode_hbackporch;
short mode_vfrontporch, mode_vsync, mode_vbackporch;

RGBargy::RGBargy(byte mode) {
    // handling resolution-dependent parameters
    short hsync_active, vsync_active, color_active;
    switch(mode) {
        case RGBG_MODE_640x480:
            mode_width       = 640;
            mode_height      = 480;
            mode_hfrontporch = 16;
            mode_hsync       = 96;
            mode_hbackporch  = 48;
            mode_vfrontporch = 11;
            mode_vsync       = 2;
            mode_vbackporch  = 31;
            break;
        case RGBG_MODE_800x600:
            mode_width       = 800;
            mode_height      = 600;
            mode_hfrontporch = 56;
            mode_hsync       = 120;
            mode_hbackporch  = 64;
            mode_vfrontporch = 37;
            mode_vsync       = 6;
            mode_vbackporch  = 23;
            break;
        case RGBG_MODE_1024x768:
            break;
    }
    hsync_active = mode_width + mode_hfrontporch - 1;
    vsync_active = mode_height - 1;
    color_active = mode_width / 2 - 1;

    // setting up pio manchines
    PIO pio = pio0;
    uint hsync_sm = 0;
    uint vsync_sm = 1;
    uint color_sm = 2;
    uint hsync_offset = pio_add_program(pio, &vgahsync_program);
    uint vsync_offset = pio_add_program(pio, &vgavsync_program);
    uint color_offset = pio_add_program(pio, &vgacolor_program);
    vgahsync_program_init(pio, hsync_sm, hsync_offset, HSYNC_PIN);
    vgavsync_program_init(pio, vsync_sm, vsync_offset, VSYNC_PIN);
    vgacolor_program_init(pio, color_sm, color_offset, COLOR_PINS);
    pio_sm_put_blocking(pio, hsync_sm, hsync_active);
    pio_sm_put_blocking(pio, vsync_sm, vsync_active);
    pio_sm_put_blocking(pio, color_sm, color_active);

    // setting up dma channels
    int rgb_chan_0 = 0;
    int rgb_chan_1 = 1;
    dma_channel_config c0 = dma_channel_get_default_config(rgb_chan_0);
    channel_config_set_transfer_data_size(&c0, DMA_SIZE_8);
    channel_config_set_read_increment(&c0, true);
    channel_config_set_write_increment(&c0, false);
    channel_config_set_dreq(&c0, DREQ_PIO0_TX2) ; 
    channel_config_set_chain_to(&c0, rgb_chan_1);
    dma_channel_configure(rgb_chan_0, &c0, &pio->txf[color_sm], &vga_framebuffer, TXCOUNT, false);
    dma_channel_config c1 = dma_channel_get_default_config(rgb_chan_1);
    channel_config_set_transfer_data_size(&c1, DMA_SIZE_32);
    channel_config_set_read_increment(&c1, false);
    channel_config_set_write_increment(&c1, false);
    channel_config_set_chain_to(&c1, rgb_chan_0);
    dma_channel_configure(rgb_chan_1, &c1, &dma_hw->ch[rgb_chan_0].read_addr, &fb_pointer, 1, false);

    // starting pio machines and dma channels in sync
    pio_enable_sm_mask_in_sync(pio, ((1u << hsync_sm) | (1u << vsync_sm) | (1u << color_sm)));
    dma_start_channel_mask((1u << rgb_chan_0)) ;
}

void RGBargy::pixel(short x, short y, byte color) {
    int pixel = ((640 * y) + x);
    if (pixel & 1) {
        vga_framebuffer[pixel>>1] = (vga_framebuffer[pixel>>1] & TOPMASK) | (color << 4) ;
    } else {
        vga_framebuffer[pixel>>1] = (vga_framebuffer[pixel>>1] & BOTTOMMASK) | (color) ;
    }
}

int RGBargy::get_mode_width() {
    return 640;
};
int RGBargy::get_mode_height() {
    return 480;
};
byte RGBargy::get_mode_bitdepth() {
    return 3;
};

void RGBargy::line(short x0, short y0, short x1, short y1, byte color) {
    short dx, dy, ystep, err;
    short steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        SWAP(x0, y0);
        SWAP(x1, y1);
    }
    if (x0 > x1) {
        SWAP(x0, x1);
        SWAP(y0, y1);
    }
    dx = x1 - x0;
    dy = abs(y1 - y0);
    err = dx / 2;
    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }
    for (; x0<=x1; x0++) {
        if (steep) {
            pixel(y0, x0, color);
        } else {
            pixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}
