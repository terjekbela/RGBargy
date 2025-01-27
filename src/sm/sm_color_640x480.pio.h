#pragma once

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

#define sm_color_640x480_wrap_target 2
#define sm_color_640x480_wrap 8

static const uint16_t sm_color_640x480x100_program_instructions[] = {
    0x80a0, 0xa047, 0xe000, 0xa022, 0x22c1, 0x80a0, 0x6304, 0x6104, 0x0045
};
static const uint16_t sm_color_640x480x125_program_instructions[] = {
    0x80a0, 0xa047, 0xe000, 0xa022, 0x23c1, 0x80a0, 0x6404, 0x6204, 0x0045
};
static const uint16_t sm_color_640x480x150_program_instructions[] = {
    0x80a0, 0xa047, 0xe000, 0xa022, 0x24c1, 0x80a0, 0x6504, 0x6304, 0x0045
};
static const uint16_t sm_color_640x480x175_program_instructions[] = {
    0x80a0, 0xa047, 0xe000, 0xa022, 0x25c1, 0x80a0, 0x6604, 0x6404, 0x0045
};
static const uint16_t sm_color_640x480x200_program_instructions[] = {
    0x80a0, 0xa047, 0xe000, 0xa022, 0x26c1, 0x80a0, 0x6704, 0x6504, 0x0045
};
static const uint16_t sm_color_640x480x225_program_instructions[] = {
    0x80a0, 0xa047, 0xe000, 0xa022, 0x27c1, 0x80a0, 0x6804, 0x6604, 0x0045
};
static const uint16_t sm_color_640x480x250_program_instructions[] = {
    0x80a0, 0xa047, 0xe000, 0xa022, 0x28c1, 0x80a0, 0x6904, 0x6704, 0x0045
};

#if !PICO_NO_HARDWARE
static const struct pio_program sm_color_640x480x100_program = {
    .instructions = sm_color_640x480x100_program_instructions,
    .length = 9,
    .origin = -1,
};

static const struct pio_program sm_color_640x480x125_program = {
    .instructions = sm_color_640x480x125_program_instructions,
    .length = 9,
    .origin = -1,
};

static const struct pio_program sm_color_640x480x150_program = {
    .instructions = sm_color_640x480x150_program_instructions,
    .length = 9,
    .origin = -1,
};

static const struct pio_program sm_color_640x480x175_program = {
    .instructions = sm_color_640x480x175_program_instructions,
    .length = 9,
    .origin = -1,
};

static const struct pio_program sm_color_640x480x200_program = {
    .instructions = sm_color_640x480x200_program_instructions,
    .length = 9,
    .origin = -1,
};

static const struct pio_program sm_color_640x480x225_program = {
    .instructions = sm_color_640x480x225_program_instructions,
    .length = 9,
    .origin = -1,
};

static const struct pio_program sm_color_640x480x250_program = {
    .instructions = sm_color_640x480x250_program_instructions,
    .length = 9,
    .origin = -1,
};

static inline pio_sm_config sm_color_640x480_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + sm_color_640x480_wrap_target, offset + sm_color_640x480_wrap);
    return c;
}

static inline void sm_color_640x480_program_init(PIO pio, uint sm, uint offset, uint pin) {
    pio_sm_config c = sm_color_640x480_program_get_default_config(offset);
    sm_config_set_set_pins(&c, pin, 4);
    sm_config_set_out_pins(&c, pin, 4);
    pio_gpio_init(pio, pin);
    pio_gpio_init(pio, pin+1);
    pio_gpio_init(pio, pin+2);
    pio_gpio_init(pio, pin+3);
    pio_sm_set_consecutive_pindirs(pio, sm, pin, 4, true);
    pio_sm_init(pio, sm, offset, &c);
}
#endif
