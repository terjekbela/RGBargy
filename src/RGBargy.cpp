#include "Arduino.h"
#include "hardware/vreg.h"

#include "RGBargy.h"
#include "sm/sm_hsync_640x480.pio.h"
#include "sm/sm_vsync_640x480.pio.h"
#include "sm/sm_color_640x480x100.pio.h"
#include "sm/sm_color_640x480x125.pio.h"
#include "sm/sm_color_640x480x150.pio.h"
#include "sm/sm_color_640x480x175.pio.h"
#include "sm/sm_color_640x480x200.pio.h"
#include "sm/sm_color_640x480x225.pio.h"
#include "sm/sm_color_640x480x250.pio.h"
#include "sm/sm_hsync_800x600.pio.h"
#include "sm/sm_vsync_800x600.pio.h"
#include "sm/sm_color_800x600x120.pio.h"
#include "sm/sm_color_800x600x200.pio.h"
#include "sm/sm_color_800x600x240.pio.h"
#include "sm/sm_hsync_1024x768.pio.h"
#include "sm/sm_vsync_1024x768.pio.h"
#include "sm/sm_color_1024x768x225.pio.h"

short mode, mode_width, mode_height, mode_hfrontporch;
int hsync_active, vsync_active, color_active;

RGBargy::RGBargy(byte mode_, byte port_) {
    // storing mode and port parameters
    mode = mode_;
    port = port_;

    // setting up resolution-dependent parameters
    switch(mode) {
        case VGA_MODE_640x480:
            mode_width       = 640;
            mode_height      = 480;
            mode_hfrontporch = 16;
            break;
        case VGA_MODE_800x600:
            mode_width       = 800;
            mode_height      = 600;
            mode_hfrontporch = 40;
            break;
        case VGA_MODE_1024x768:
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
    // stopping dma channels
    dma_channel_abort(dma_color_chan_1);
    dma_channel_abort(dma_color_chan_0);

    // releasing framebuffer memory
    free(fb_pointer0);
}

void RGBargy::begin(short large_) {
    // storing large (pixels) parameter
    large = large_;
    
    // getting cpu frequency for state-machine clock division
    int cpu_mhz = get_cpu_mhz();

    // setting up pio machines
    switch(port) {
        case VGA_PORT_0:
            pio        = pio0;
            pio_hsync_pin  = VGA_PORT0_HSYNC_PIN;
            pio_vsync_pin  = VGA_PORT0_VSYNC_PIN;
            pio_color_pins = VGA_PORT0_COLOR_PINS;
            break;
        case VGA_PORT_1:
            pio        = pio1;
            pio_hsync_pin  = VGA_PORT1_HSYNC_PIN;
            pio_vsync_pin  = VGA_PORT1_VSYNC_PIN;
            pio_color_pins = VGA_PORT1_COLOR_PINS;
            break;
        case VGA_PORT_2:
            pio        = pio2;
            pio_hsync_pin  = VGA_PORT2_HSYNC_PIN;
            pio_vsync_pin  = VGA_PORT2_VSYNC_PIN;
            pio_color_pins = VGA_PORT2_COLOR_PINS;
            break;
    }
    switch(mode) {
        case VGA_MODE_640x480:
            pio_hsync_offset = pio_add_program(pio, &sm_hsync_640x480_program);
            pio_vsync_offset = pio_add_program(pio, &sm_vsync_640x480_program);
            switch(cpu_mhz) {
                case 100:
                    pio_color_offset = pio_add_program(pio, &sm_color_640x480x100_program);
                    sm_color_640x480x100_program_init(pio, pio_color_sm, pio_color_offset, pio_color_pins);
                    break;
                case 125:
                    pio_color_offset = pio_add_program(pio, &sm_color_640x480x125_program);
                    sm_color_640x480x125_program_init(pio, pio_color_sm, pio_color_offset, pio_color_pins);
                    break;
                case 150:
                    pio_color_offset = pio_add_program(pio, &sm_color_640x480x150_program);
                    sm_color_640x480x150_program_init(pio, pio_color_sm, pio_color_offset, pio_color_pins);
                    break;
                case 175:
                    pio_color_offset = pio_add_program(pio, &sm_color_640x480x175_program);
                    sm_color_640x480x175_program_init(pio, pio_color_sm, pio_color_offset, pio_color_pins);
                    break;
                case 200:
                    pio_color_offset = pio_add_program(pio, &sm_color_640x480x200_program);
                    sm_color_640x480x200_program_init(pio, pio_color_sm, pio_color_offset, pio_color_pins);
                    break;
                case 225:
                    pio_color_offset = pio_add_program(pio, &sm_color_640x480x225_program);
                    sm_color_640x480x225_program_init(pio, pio_color_sm, pio_color_offset, pio_color_pins);
                    break;
                case 250:
                    pio_color_offset = pio_add_program(pio, &sm_color_640x480x250_program);
                    sm_color_640x480x250_program_init(pio, pio_color_sm, pio_color_offset, pio_color_pins);
                    break;
            }
            sm_hsync_640x480_program_init(pio, pio_hsync_sm, pio_hsync_offset, pio_hsync_pin, cpu_mhz / 25);
            sm_vsync_640x480_program_init(pio, pio_vsync_sm, pio_vsync_offset, pio_vsync_pin, cpu_mhz / 25);
            break;
        case VGA_MODE_800x600:
            pio_hsync_offset = pio_add_program(pio, &sm_hsync_800x600_program);
            pio_vsync_offset = pio_add_program(pio, &sm_vsync_800x600_program);
            switch(cpu_mhz) {
                case 120:
                    pio_color_offset = pio_add_program(pio, &sm_color_800x600x120_program);
                    sm_color_800x600x120_program_init(pio, pio_color_sm, pio_color_offset, pio_color_pins);
                    break;
                case 200:
                    pio_color_offset = pio_add_program(pio, &sm_color_800x600x200_program);
                    sm_color_800x600x200_program_init(pio, pio_color_sm, pio_color_offset, pio_color_pins);
                    break;
                case 240:
                    pio_color_offset = pio_add_program(pio, &sm_color_800x600x240_program);
                    sm_color_800x600x240_program_init(pio, pio_color_sm, pio_color_offset, pio_color_pins);
                    break;
            }
            sm_hsync_800x600_program_init(pio, pio_hsync_sm, pio_hsync_offset, pio_hsync_pin, cpu_mhz / 40);
            sm_vsync_800x600_program_init(pio, pio_vsync_sm, pio_vsync_offset, pio_vsync_pin, cpu_mhz / 40);
            break;
        case VGA_MODE_1024x768:
            pio_hsync_offset = pio_add_program(pio, &sm_hsync_1024x768_program);
            pio_vsync_offset = pio_add_program(pio, &sm_vsync_1024x768_program);
            switch(cpu_mhz) {
                case 225:
                    pio_color_offset = pio_add_program(pio, &sm_color_1024x768x225_program);
                    sm_color_1024x768x225_program_init(pio, pio_color_sm, pio_color_offset, pio_color_pins);
                    break;
            }
            sm_hsync_1024x768_program_init(pio, pio_hsync_sm, pio_hsync_offset, pio_hsync_pin, cpu_mhz / 75);
            sm_vsync_1024x768_program_init(pio, pio_vsync_sm, pio_vsync_offset, pio_vsync_pin, cpu_mhz / 75);
            break;
    }
    pio_sm_put_blocking(pio, pio_hsync_sm, hsync_active);
    pio_sm_put_blocking(pio, pio_vsync_sm, vsync_active);
    pio_sm_put_blocking(pio, pio_color_sm, color_active);

    // setting up dma channels
    switch(port) {
        case VGA_PORT_0:
            dma_color_chan_0 = 0;
            dma_color_chan_1 = 1;
            break;
        case VGA_PORT_1:
            dma_color_chan_0 = 2;
            dma_color_chan_1 = 3;
            break;
        case VGA_PORT_2:
            dma_color_chan_0 = 4;
            dma_color_chan_1 = 5;
            break;
    }
    dma_channel_config c0 = dma_channel_get_default_config(dma_color_chan_0);
    channel_config_set_transfer_data_size(&c0, DMA_SIZE_8);
    channel_config_set_read_increment(&c0, true);
    channel_config_set_write_increment(&c0, false);
    switch(port) {
        case VGA_PORT_0:
            channel_config_set_dreq(&c0, DREQ_PIO0_TX2);
            break;
        case VGA_PORT_1:
            channel_config_set_dreq(&c0, DREQ_PIO1_TX2);
            break;
        case VGA_PORT_2:
            channel_config_set_dreq(&c0, DREQ_PIO2_TX2);
            break;
    }
    channel_config_set_chain_to(&c0, dma_color_chan_1);
    dma_channel_configure(dma_color_chan_0, &c0, &pio->txf[pio_color_sm], &*fb_pointer0, fb_size, false);
    dma_channel_config c1 = dma_channel_get_default_config(dma_color_chan_1);
    channel_config_set_transfer_data_size(&c1, DMA_SIZE_32);
    channel_config_set_read_increment(&c1, false);
    channel_config_set_write_increment(&c1, false);
    channel_config_set_chain_to(&c1, dma_color_chan_0);
    dma_channel_configure(dma_color_chan_1, &c1, &dma_hw->ch[dma_color_chan_0].read_addr, &fb_pointer0, 1, false);

    // starting pio machines and dma channels in sync
    pio_enable_sm_mask_in_sync(pio, ((1u << pio_hsync_sm) | (1u << pio_vsync_sm) | (1u << pio_color_sm)));
    dma_start_channel_mask((1u << dma_color_chan_0)) ;
}



// clear the framebuffer with zero color
void RGBargy::clear() {
    memset(fb_pointer0, 0, fb_size);
}

// draw a grid for large pixel mode with color c
void RGBargy::grid(char c) {
    short large_ = large;
    large = 0;
    clear();
    if (large_) {
        for (int x = 0; x < mode_width/large_; x++) {
            vline(x*large_,          0, mode_height, c);
            vline(x*large_+large_-1, 0, mode_height, c);
        }
        for (int y = 0; y < mode_height/large_; y++) {
            hline(0, y*large_,          mode_width, c);
            hline(0, y*large_+large_-1, mode_width, c);
        }
    }
    large = large_;
}

// draw a pixel at x, y with color
void RGBargy::pixel(short x, short y, char c) {
    int pixel;
    if (large) {
        if (x < 0 || x > mode_width/large  - 1) return;
        if (y < 0 || y > mode_height/large - 1) return;
        for (byte dy = 1; dy<large-1; dy++) {
            for (byte dx = 1; dx<large-1; dx++) {
                pixel = mode_width*y*large + mode_width*dy + x*large + dx;
                if (pixel & 1) fb_pointer0[pixel>>1] = (fb_pointer0[pixel>>1] & 0b00001111) | (c << 4);
                    else       fb_pointer0[pixel>>1] = (fb_pointer0[pixel>>1] & 0b11110000) | (c);
            }
        }
    } else {
        if (x < 0 || x > mode_width  - 1) return;
        if (y < 0 || y > mode_height - 1) return;
        int pixel = (mode_width * y) + x;
        if (pixel & 1) fb_pointer0[pixel>>1] = (fb_pointer0[pixel>>1] & 0b00001111) | (c << 4);
            else       fb_pointer0[pixel>>1] = (fb_pointer0[pixel>>1] & 0b11110000) | (c);
    }
}

// draw horizontal line from x0, y0 with l in length
void RGBargy::hline(short x, short y, short l, char c) {
    for (int i = 0; i < l; i++) pixel(x + i, y, c);
}

// draw vertical line from x0, y0 with l in length
void RGBargy::vline(short x, short y, short l, char c) {
    for (int i = 0; i < l; i++) pixel(x, y+i, c);
}

// draw a line with Bresenham's algorithm
void RGBargy::line(short x0, short y0, short x1, short y1, char c) {
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
            pixel(y0, x0, c);
        } else {
            pixel(x0, y0, c);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
};

// draw a rectangle, filled or not
void RGBargy::rect(short x0, short y0, short x1, short y1, char c, bool f) {
    if(!f) {
        hline(x0, y0, x1 - x0, c);
        hline(x0, y1, x1 - x0, c);
        vline(x0, y0, y1 - y0, c);
        vline(x1, y0, y1 - y0, c);
    } else {
        for (int i = y0; i <= y1; i++) {
            hline(x0, i, x1 - x0+1, c);
        }
    }
}

// draw a circle with the midpoint circle algorithm
void RGBargy::circle(short xc, short yc, short r, char c, bool f) {
    int x=0, y=r, d=3 - (2 * r);  
    if(f) symm4_fill(xc, yc, x, y, c); else symm8_plot(xc, yc, x, y, c);
    while(x<=y) {
        //delay(100);
        if(d<=0) {  
            d+= (4*x)+6;  
        } else {  
            d+=(4*x)-(4*y)+10;  
            y--;  
        }  
        x++;  
        if(f) symm4_fill(xc, yc, x, y, c); else symm8_plot(xc, yc, x, y, c);
    }
}

// draw an ellipse with the midpoint ellipse algorithm
void RGBargy::ellipse(short xc, short yc, short rx, short ry, char c, bool f) {
    float rx1, ry1, rx2, ry2;
    int p, x, y, px, py;
    ry1 = ry * ry;
    rx1 = rx * rx;
    ry2 =  2 * ry1;
    rx2 =  2 * rx1;
    x   = 0;
    y   = ry;
    symm4_plot(xc, yc, x, y, c);
    px = 0;
    py = rx2 * y;
    p  = (ry1 - rx1 * ry + (0.25 * rx1));
    while(px < py) {
        x=x+1;
        px=px+ry2;
        if(p >= 0) {
            y  =  y - 1;
            py = py - rx2;
        }
        if(p < 0) {
            p=p+ry1+px;
        } else {
            p=p+ry1+px-py;
        }
        symm4_plot(xc, yc, x, y, c);
    }
    p = (ry1 * (x + 0.5) * (x + 0.5) + rx1 * (y - 1) * (y - 1) - rx1 * ry1);
    while(y>0) {
        y=y-1;
        py=py-rx2;
        if(p <= 0){
            x  = x  + 1;
            px = px + ry2;
        }
        if(p > 0) {
            p = p + rx1 - py;
        } else {
            p = p + rx1 - py + px;
        }
        symm4_plot(xc, yc, x, y, c);
    }
}

// draw a superellipse 
void RGBargy::superellipse(short xc, short yc, short rx, short ry, float n, char c) {
    for (float t=0; t<PI/2; t+=0.002) {
        float co = cos(t);
        float si = sin(t);
        float x  = round(pow(abs(co), 2/n) * rx);
        float y  = round(pow(abs(si), 2/n) * ry);
        symm4_plot(xc, yc, x, y, c);
    }
}



// 8-way symmetric helper function for circle
void RGBargy::symm8_plot(short xc, short yc, short x, short y, char c) {  
    pixel(xc+x, yc+y, c);
    pixel(xc+x, yc-y, c);
    pixel(xc-x, yc+y, c);
    pixel(xc-x, yc-y, c);
    pixel(xc+y, yc+x, c);
    pixel(xc+y, yc-x, c);
    pixel(xc-y, yc+x, c);
    pixel(xc-y, yc-x, c);
}

// 4-way symmetric helper function for ellipse
void RGBargy::symm4_plot(short xc, short yc, short x, short y, char c) {  
    pixel(xc+x, yc+y, c);
    pixel(xc+x, yc-y, c);
    pixel(xc-x, yc+y, c);
    pixel(xc-x, yc-y, c);
}

// 4-way symmetric helper function for filled circle
void RGBargy::symm4_fill(short xc, short yc, short x, short y, char c) {  
    hline(xc-x, yc-y, x*2+1, c);
    hline(xc-x, yc+y, x*2+1, c);
    hline(xc-y, yc-x, y*2+1, c);
    hline(xc-y, yc+x, y*2+1, c);
}

// 2-way symmetric helper function for filled ellipse
void RGBargy::symm2_fill(short xc, short yc, short x, short y, char c) {  
    hline(xc-x, yc-y, x*2+1, c);
    hline(xc-x, yc+y, x*2+1, c);
}

// return mode width
int RGBargy::get_mode_width() {
    if (large) return mode_width/large;
    return mode_width;
};

// return mode height
int RGBargy::get_mode_height() {
    if (large) return mode_height/large;
    return mode_height;
};

// return mode bitdepth
int RGBargy::get_mode_bitdepth() {
    return 4;
};

// return cpu frequency in MHz
int RGBargy::get_cpu_mhz() {
    //return frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS) / 1000;
    return clock_get_hz(clk_sys) / 1000000;
};
