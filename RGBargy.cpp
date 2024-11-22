#include "Arduino.h"
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/dma.h"

#include "RGBargy.h"
#include "vgahsync.pio.h"
#include "vgavsync.pio.h"
#include "vgargb.pio.h"

#define H_ACTIVE   655
#define V_ACTIVE   479
#define RGB_ACTIVE 319
#define TXCOUNT 153600

unsigned char vga_data_array[TXCOUNT];
unsigned char * address_pointer = &vga_data_array[0] ;

#define HSYNC_PIN 4
#define VSYNC_PIN 5
#define RED_PIN   6
#define GREEN_PIN 7
#define BLUE_PIN  8

#define SWAP(a, b) { short t = a; a = b; b = t; }
#define TOPMASK 0b00001111
#define BOTTOMMASK 0b11110000

RGBargy::RGBargy() {

}

void RGBargy::init() {
    PIO pio = pio0;
    uint hsync_offset = pio_add_program(pio, &vgahsync_program);
    uint vsync_offset = pio_add_program(pio, &vgavsync_program);
    uint rgb_offset   = pio_add_program(pio, &vgargb_program);

    uint hsync_sm = 0;
    uint vsync_sm = 1;
    uint rgb_sm   = 2;

    vgahsync_program_init(pio, hsync_sm, hsync_offset, HSYNC_PIN);
    vgavsync_program_init(pio, vsync_sm, vsync_offset, VSYNC_PIN);
    vgargb_program_init(  pio, rgb_sm,   rgb_offset,   RED_PIN);

    int rgb_chan_0 = 0;
    int rgb_chan_1 = 1;

    dma_channel_config c0 = dma_channel_get_default_config(rgb_chan_0);
    channel_config_set_transfer_data_size(&c0, DMA_SIZE_8);
    channel_config_set_read_increment(&c0, true);
    channel_config_set_write_increment(&c0, false);
    channel_config_set_dreq(&c0, DREQ_PIO0_TX2) ; 
    channel_config_set_chain_to(&c0, rgb_chan_1);
    dma_channel_configure(rgb_chan_0, &c0, &pio->txf[rgb_sm], &vga_data_array, TXCOUNT, false);

    dma_channel_config c1 = dma_channel_get_default_config(rgb_chan_1);
    channel_config_set_transfer_data_size(&c1, DMA_SIZE_32);
    channel_config_set_read_increment(&c1, false);
    channel_config_set_write_increment(&c1, false);
    channel_config_set_chain_to(&c1, rgb_chan_0);
    dma_channel_configure(rgb_chan_1, &c1, &dma_hw->ch[rgb_chan_0].read_addr, &address_pointer, 1, false);

    pio_sm_put_blocking(pio, hsync_sm, H_ACTIVE);
    pio_sm_put_blocking(pio, vsync_sm, V_ACTIVE);
    pio_sm_put_blocking(pio, rgb_sm, RGB_ACTIVE);

    pio_enable_sm_mask_in_sync(pio, ((1u << hsync_sm) | (1u << vsync_sm) | (1u << rgb_sm)));
    dma_start_channel_mask((1u << rgb_chan_0)) ;
}

void RGBargy::pixel(short x, short y, byte color) {
    if (x > 639) x = 639;
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (y > 479) y = 479;
    int pixel = ((640 * y) + x);
    if (pixel & 1) {
        vga_data_array[pixel>>1] = (vga_data_array[pixel>>1] & TOPMASK) | (color << 4) ;
    }
    else {
        vga_data_array[pixel>>1] = (vga_data_array[pixel>>1] & BOTTOMMASK) | (color) ;
    }
}