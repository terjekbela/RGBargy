#pragma once

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

#define sm_color_640x480x8bit_wrap_target 2
#define sm_color_640x480x8bit_wrap 7

static const uint16_t sm_color_640x480x150x8bit_program_instructions[] = {
    0x80a0, 0xa047, 0xe000, 0xa022, 0x24c1, 0x80a0, 0x6308, 0x0045
};
static const uint16_t sm_color_640x480x200x8bit_program_instructions[] = {
    0x80a0, 0xa047, 0xe000, 0xa022, 0x26c1, 0x80a0, 0x6508, 0x0045
};

#if !PICO_NO_HARDWARE
static const struct pio_program sm_color_640x480x150x8bit_program = {
    .instructions = sm_color_640x480x150x8bit_program_instructions,
    .length = 8,
    .origin = -1,
};
static const struct pio_program sm_color_640x480x200x8bit_program = {
    .instructions = sm_color_640x480x200x8bit_program_instructions,
    .length = 8,
    .origin = -1,
};

static inline pio_sm_config sm_color_640x480x8bit_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + sm_color_640x480x8bit_wrap_target, offset + sm_color_640x480x8bit_wrap);
    return c;
}

static inline void sm_color_640x480x8bit_program_init(PIO pio, uint sm, uint offset, uint pin) {
    pio_sm_config c = sm_color_640x480x8bit_program_get_default_config(offset);
    sm_config_set_set_pins(&c, pin, 8);
    sm_config_set_out_pins(&c, pin, 8);
    pio_gpio_init(pio, pin);
    pio_gpio_init(pio, pin+1);
    pio_gpio_init(pio, pin+2);
    pio_gpio_init(pio, pin+3);
    pio_gpio_init(pio, pin+4);
    pio_gpio_init(pio, pin+5);
    pio_gpio_init(pio, pin+6);
    pio_gpio_init(pio, pin+7);
    pio_sm_set_consecutive_pindirs(pio, sm, pin, 8, true);
    pio_sm_init(pio, sm, offset, &c);
}
#endif
