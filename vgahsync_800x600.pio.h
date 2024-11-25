#pragma once

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

#define vgahsync_800x600_wrap_target 1
#define vgahsync_800x600_wrap 9

static const uint16_t vgahsync_800x600_program_instructions[] = {
    0x80a0, //  0: pull   block                      
            //     .wrap_target
    0xa027, //  1: mov    x, osr                     
    0x0042, //  2: jmp    x--, 2                     
    0xe023, //  3: set    x, 3                       
    0xff01, //  4: set    pins, 1                [31]
    0x0044, //  5: jmp    x--, 4                     
    0xff00, //  6: set    pins, 0                [31]
    0xff00, //  7: set    pins, 0                [31]
    0xf700, //  8: set    pins, 0                [23]
    0xc100, //  9: irq    nowait 0               [1] 
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program vgahsync_800x600_program = {
    .instructions = vgahsync_800x600_program_instructions,
    .length = 10,
    .origin = -1,
};

static inline pio_sm_config vgahsync_800x600_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + vgahsync_800x600_wrap_target, offset + vgahsync_800x600_wrap);
    return c;
}

static inline void vgahsync_800x600_program_init(PIO pio, uint sm, uint offset, uint pin) {
    pio_sm_config c = vgahsync_800x600_program_get_default_config(offset);
    sm_config_set_set_pins(&c, pin, 1);
    sm_config_set_clkdiv(&c, 3) ;
    pio_gpio_init(pio, pin);
    pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);
    pio_sm_init(pio, sm, offset, &c);
}
#endif
