#pragma once

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

#define vgavsync_800x600_wrap_target 1
#define vgavsync_800x600_wrap 12

static const uint16_t vgavsync_800x600_program_instructions[] = {
    0x80a0, //  0: pull   block                      
            //     .wrap_target
    0xa027, //  1: mov    x, osr                     
    0x20c0, //  2: wait   1 irq, 0                   
    0xc001, //  3: irq    nowait 1                   
    0x0042, //  4: jmp    x--, 2                     
    0x20c0, //  5: wait   1 irq, 0                   
    0xe001, //  6: set    pins, 1                    
    0x20c0, //  7: wait   1 irq, 0                   
    0x20c0, //  8: wait   1 irq, 0                   
    0x20c0, //  9: wait   1 irq, 0                   
    0xe059, // 10: set    y, 25                      
    0x30c0, // 11: wait   1 irq, 0        side 0     
    0x008b, // 12: jmp    y--, 11                    
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program vgavsync_800x600_program = {
    .instructions = vgavsync_800x600_program_instructions,
    .length = 13,
    .origin = -1,
};

static inline pio_sm_config vgavsync_800x600_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + vgavsync_800x600_wrap_target, offset + vgavsync_800x600_wrap);
    sm_config_set_sideset(&c, 2, true, false);
    return c;
}

static inline void vgavsync_800x600_program_init(PIO pio, uint sm, uint offset, uint pin) {
    pio_sm_config c = vgavsync_800x600_program_get_default_config(offset);
    sm_config_set_set_pins(&c, pin, 1);
    sm_config_set_sideset_pins(&c, pin);
    sm_config_set_clkdiv(&c, 4);
    pio_gpio_init(pio, pin);
    pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);
    pio_sm_init(pio, sm, offset, &c);
}
#endif
