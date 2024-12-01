#pragma once

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

#define sm_vsync_640x480_wrap_target 1
#define sm_vsync_640x480_wrap 13

static const uint16_t sm_vsync_640x480_program_instructions[] = {
    0x80a0, //  0: pull   block                      
            //     .wrap_target
    0xa027, //  1: mov    x, osr                     
    0x20c0, //  2: wait   1 irq, 0                   
    0xc001, //  3: irq    nowait 1                   
    0x0042, //  4: jmp    x--, 2                     
    0xe049, //  5: set    y, 9                       
    0x20c0, //  6: wait   1 irq, 0                   
    0x0086, //  7: jmp    y--, 6                     
    0xe000, //  8: set    pins, 0                    
    0x20c0, //  9: wait   1 irq, 0                   
    0x20c0, // 10: wait   1 irq, 0                   
    0xe05f, // 11: set    y, 31                      
    0x38c0, // 12: wait   1 irq, 0        side 1     
    0x008c, // 13: jmp    y--, 12                    
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program sm_vsync_640x480_program = {
    .instructions = sm_vsync_640x480_program_instructions,
    .length = 14,
    .origin = -1,
};

static inline pio_sm_config sm_vsync_640x480_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + sm_vsync_640x480_wrap_target, offset + sm_vsync_640x480_wrap);
    sm_config_set_sideset(&c, 2, true, false);
    return c;
}

static inline void sm_vsync_640x480_program_init(PIO pio, uint sm, uint offset, uint pin, uint clkdiv) {
    pio_sm_config c = sm_vsync_640x480_program_get_default_config(offset);
    sm_config_set_set_pins(&c, pin, 1);
    sm_config_set_sideset_pins(&c, pin);
    sm_config_set_clkdiv(&c, clkdiv) ;
    pio_gpio_init(pio, pin);
    pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);
    pio_sm_init(pio, sm, offset, &c);
}
#endif
