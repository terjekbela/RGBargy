#pragma once

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

#define sm_hsync_640x480_wrap_target 1
#define sm_hsync_640x480_wrap 8

static const uint16_t sm_hsync_640x480_program_instructions[] = {
    0x80a0, //  0: pull   block                      
            //     .wrap_target
    0xa027, //  1: mov    x, osr                     
    0x0042, //  2: jmp    x--, 2                     
    0xff00, //  3: set    pins, 0                [31]
    0xff00, //  4: set    pins, 0                [31]
    0xff00, //  5: set    pins, 0                [31]
    0xff01, //  6: set    pins, 1                [31]
    0xec01, //  7: set    pins, 1                [12]
    0xc100, //  8: irq    nowait 0               [1] 
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program sm_hsync_640x480_program = {
    .instructions = sm_hsync_640x480_program_instructions,
    .length = 9,
    .origin = -1,
};

static inline pio_sm_config sm_hsync_640x480_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + sm_hsync_640x480_wrap_target, offset + sm_hsync_640x480_wrap);
    return c;
}

static inline void sm_hsync_640x480_program_init(PIO pio, uint sm, uint offset, uint pin, uint clkdiv) {
    pio_sm_config c = sm_hsync_640x480_program_get_default_config(offset);
    sm_config_set_set_pins(&c, pin, 1);
    sm_config_set_clkdiv(&c, clkdiv);
    pio_gpio_init(pio, pin);
    pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);
    pio_sm_init(pio, sm, offset, &c);
}
#endif
