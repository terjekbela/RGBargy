#include "Arduino.h"
#include "hardware/vreg.h"
#include "hardware/clocks.h"

#include "RGBargy.h"
#include "sm/sm_hsync_640x480.pio.h"
#include "sm/sm_vsync_640x480.pio.h"
#include "sm/sm_color_640x480x100.pio.h"
#include "sm/sm_color_640x480x125.pio.h"
#include "sm/sm_color_640x480x150.pio.h"
#include "sm/sm_color_640x480x175.pio.h"
#include "sm/sm_color_640x480x200.pio.h"
#include "sm/sm_color_640x480x225.pio.h"
#include "sm/sm_hsync_800x600.pio.h"
#include "sm/sm_vsync_800x600.pio.h"
#include "sm/sm_color_800x600x120.pio.h"
#include "sm/sm_color_800x600x200.pio.h"
//#include "sm/sm_color_800x600x240.pio.h"

short mode, mode_width, mode_height, mode_hfrontporch;
int hsync_active, vsync_active, color_active;

RGBargy::RGBargy(short mode_) {
    // core voltage and mhz settings
    // vreg_set_voltage(VREG_VOLTAGE_1_20);
    // delay(10);
    // set_sys_clock_khz(175000, true);
    // delay(10);

    // storing mode
    mode = mode_;

    // setting up resolution-dependent parameters
    switch(mode) {
        case RGBG_MODE_640x480:
            mode_width       = 640;
            mode_height      = 480;
            mode_hfrontporch = 16;
            break;
        case RGBG_MODE_800x600:
            mode_width       = 800;
            mode_height      = 600;
            mode_hfrontporch = 40;
            break;
        case RGBG_MODE_1024x768:
            mode_width       = 1024;
            mode_height      = 768;
            mode_hfrontporch = 24;
            break;
    }
    hsync_active = mode_width + mode_hfrontporch - 1;
    vsync_active = mode_height - 1;
    color_active = mode_width / 2 - 1;

    // allocating framebuffer based on res and color depth
    fb_size     = mode_width * mode_height / 2; // (/2 == 4-bit)
    fb_pointer0 = (unsigned char *)malloc(fb_size);
    memset(fb_pointer0, 0, fb_size);    
}

RGBargy::~RGBargy() {
    // releasing framebuffer memory
    free(fb_pointer0);
}

void RGBargy::begin() {
    // getting cpu frequency for clock division
    int cpu_mhz = get_cpu_mhz();

    // setting up pio machines
    PIO pio = pio0;
    uint hsync_sm = 0;
    uint vsync_sm = 1;
    uint color_sm = 2;
    uint hsync_offset, vsync_offset, color_offset;
    switch(mode) {
        case RGBG_MODE_640x480:
            hsync_offset = pio_add_program(pio, &sm_hsync_640x480_program);
            vsync_offset = pio_add_program(pio, &sm_vsync_640x480_program);
            switch(cpu_mhz) {
                case 100:
                    color_offset = pio_add_program(pio, &sm_color_640x480x100_program);
                    sm_color_640x480x100_program_init(pio, color_sm, color_offset, RGBG_COLOR_PINS);
                    break;
                case 125:
                    color_offset = pio_add_program(pio, &sm_color_640x480x125_program);
                    sm_color_640x480x125_program_init(pio, color_sm, color_offset, RGBG_COLOR_PINS);
                    break;
                case 150:
                    color_offset = pio_add_program(pio, &sm_color_640x480x150_program);
                    sm_color_640x480x150_program_init(pio, color_sm, color_offset, RGBG_COLOR_PINS);
                    break;
                case 175:
                    color_offset = pio_add_program(pio, &sm_color_640x480x175_program);
                    sm_color_640x480x175_program_init(pio, color_sm, color_offset, RGBG_COLOR_PINS);
                    break;
                case 200:
                    color_offset = pio_add_program(pio, &sm_color_640x480x200_program);
                    sm_color_640x480x200_program_init(pio, color_sm, color_offset, RGBG_COLOR_PINS);
                    break;
                case 225:
                    color_offset = pio_add_program(pio, &sm_color_640x480x225_program);
                    sm_color_640x480x225_program_init(pio, color_sm, color_offset, RGBG_COLOR_PINS);
                    break;
            }
            sm_hsync_640x480_program_init(pio, hsync_sm, hsync_offset, RGBG_HSYNC_PIN, cpu_mhz / 25);
            sm_vsync_640x480_program_init(pio, vsync_sm, vsync_offset, RGBG_VSYNC_PIN, cpu_mhz / 25);
            break;
        case RGBG_MODE_800x600:
            hsync_offset = pio_add_program(pio, &sm_hsync_800x600_program);
            vsync_offset = pio_add_program(pio, &sm_vsync_800x600_program);
            switch(cpu_mhz) {
                case 120:
                    color_offset = pio_add_program(pio, &sm_color_800x600x120_program);
                    sm_color_800x600x120_program_init(pio, color_sm, color_offset, RGBG_COLOR_PINS);
                    break;
                case 200:
                    color_offset = pio_add_program(pio, &sm_color_800x600x200_program);
                    sm_color_800x600x200_program_init(pio, color_sm, color_offset, RGBG_COLOR_PINS);
                    break;
                case 240:
                    color_offset = pio_add_program(pio, &sm_color_800x600x240_program);
                    sm_color_800x600x240_program_init(pio, color_sm, color_offset, RGBG_COLOR_PINS);
                    break;
            }
            sm_hsync_800x600_program_init(pio, hsync_sm, hsync_offset, RGBG_HSYNC_PIN, cpu_mhz / 40);
            sm_vsync_800x600_program_init(pio, vsync_sm, vsync_offset, RGBG_VSYNC_PIN, cpu_mhz / 40);
            break;
    }
    pio_sm_put_blocking(pio, hsync_sm, hsync_active);
    pio_sm_put_blocking(pio, vsync_sm, vsync_active);
    pio_sm_put_blocking(pio, color_sm, color_active);

    // setting up dma channels
    int color_chan_0 = 0;
    int color_chan_1 = 1;
    dma_channel_config c0 = dma_channel_get_default_config(color_chan_0);
    channel_config_set_transfer_data_size(&c0, DMA_SIZE_8);
    channel_config_set_read_increment(&c0, true);
    channel_config_set_write_increment(&c0, false);
    channel_config_set_dreq(&c0, DREQ_PIO0_TX2) ; 
    channel_config_set_chain_to(&c0, color_chan_1);
    dma_channel_configure(color_chan_0, &c0, &pio->txf[color_sm], &*fb_pointer0, fb_size, false);
    dma_channel_config c1 = dma_channel_get_default_config(color_chan_1);
    channel_config_set_transfer_data_size(&c1, DMA_SIZE_32);
    channel_config_set_read_increment(&c1, false);
    channel_config_set_write_increment(&c1, false);
    channel_config_set_chain_to(&c1, color_chan_0);
    dma_channel_configure(color_chan_1, &c1, &dma_hw->ch[color_chan_0].read_addr, &fb_pointer0, 1, false);

    // starting pio machines and dma channels in sync
    pio_enable_sm_mask_in_sync(pio, ((1u << hsync_sm) | (1u << vsync_sm) | (1u << color_sm)));
    dma_start_channel_mask((1u << color_chan_0)) ;
}

// clear the framebuffer with zero color
void RGBargy::clear() {
    memset(fb_pointer0, 0, fb_size);
}

// draw a pixel at x, y with color
void RGBargy::pixel(short x, short y, char color) {
    if (x < 0 || x > mode_width - 1)  return;
    if (y < 0 || y > mode_height - 1) return;
    int pixel = ((mode_width * y) + x);
    if (pixel & 1) {
        fb_pointer0[pixel>>1] = (fb_pointer0[pixel>>1] & 0b00001111) | (color << 4) ;
    } else {
        fb_pointer0[pixel>>1] = (fb_pointer0[pixel>>1] & 0b11110000) | (color) ;
    }
}

// draw horizontal line from x0, y0 with l in length
void RGBargy::hline(short x, short y, short l, char color) {
    for (int i = 0; i < l; i++) pixel(x + i, y, color);
}

// draw vertical line from x0, y0 with l in length
void RGBargy::vline(short x, short y, short l, char color) {
    for (int i = 0; i < l; i++) pixel(x, y+i, color);
}

// draw a line with Bresenham's algorithm
void RGBargy::line(short x0, short y0, short x1, short y1, char color) {
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
};

// draw a rectangle, filled or not
void RGBargy::rect(short x0, short y0, short x1, short y1, char color, bool fill) {
    if(!fill) {
        hline(x0, y0, x1 - x0, color);
        hline(x0, y1, x1 - x0, color);
        vline(x0, y0, y1 - y0, color);
        vline(x1, y0, y1 - y0, color);
    } else {
        for (int i = y0; i <= y1; i++) {
            hline(x0, i, x1 - x0, color);
        }
    }
}

// 8-way symmetric helper function for circle drawing
void RGBargy::symm8_plot(short xc, short yc, short x, short y, char color) {  
    pixel( x+xc,  y+yc, color);
    pixel( x+xc, -y+yc, color);
    pixel(-x+xc, -y+yc, color);
    pixel(-x+xc,  y+yc, color);
    pixel( y+xc,  x+yc, color);
    pixel( y+xc, -x+yc, color);
    pixel(-y+xc, -x+yc, color);
    pixel(-y+xc,  x+yc, color);
}

// draw a circle with the midpoint circle algorithm
void RGBargy::circle(short xc, short yc, short r, char color) {
    int x=0, y=r, d=3-(2*r);  
    symm8_plot(xc, yc, x, y, color);  
    while(x<=y) {  
        if(d<=0) {  
            d=d+(4*x)+6;  
        } else {  
            d=d+(4*x)-(4*y)+10;  
            y=y-1;  
        }  
        x=x+1;  
        symm8_plot(xc, yc, x, y, color);
    }  
}  



// return mode width
int RGBargy::get_mode_width() {
    return mode_width;
};

// return mode height
int RGBargy::get_mode_height() {
    return mode_height;
};

// return mode bitdepth
int RGBargy::get_mode_bitdepth() {
    return 3;
};

// return cpu frequency in MHz
int RGBargy::get_cpu_mhz() {
    //return frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS) / 1000;
    return clock_get_hz(clk_sys) / 1000000;
};
