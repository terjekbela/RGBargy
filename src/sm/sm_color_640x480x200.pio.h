#pragma once

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

#define sm_color_640x480x200_wrap_target 2
#define sm_color_640x480x200_wrap 8

static const uint16_t sm_color_640x480x200_program_instructions[] = {
    0x80a0, //  0: pull   block                      
    0xa047, //  1: mov    y, osr                     
            //     .wrap_target
    0xe000, //  2: set    pins, 0                    
    0xa022, //  3: mov    x, y                       
    0x26c1, //  4: wait   1 irq, 1               [6] 
    0x80a0, //  5: pull   block                      
    0x6704, //  6: out    pins, 4                [7] 
    0x6504, //  7: out    pins, 4                [5] 
    0x0045, //  8: jmp    x--, 5                     
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program sm_color_640x480x200_program = {
    .instructions = sm_color_640x480x200_program_instructions,
    .length = 9,
    .origin = -1,
};

#endif
