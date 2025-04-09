#pragma once
#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

// --- //
// rgb15 – 15-bit output program
// --- //

// Define wrap targets for the new program.
#define rgb15_wrap_target 2
#define rgb15_wrap        6

static const uint16_t rgb15_program_instructions[] = {
    0x80a0, // 0: pull   block                      
    0xa047, // 1: mov    x, osr
             // .wrap_target (index 2)
    0xe000, // 2: set    pins, 0                    
    0xa022, // 3: mov    x, osr                     
    0x23c1, // 4: wait   1 irq, 1             [3]
    0x80a0, // 5: pull   block                      
    0x640f, // 6: out    pins, 15            [4]
    0x0045, // 7: jmp    x--, 5                     
             // .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program rgb15_program = {
    .instructions = rgb15_program_instructions,
    .length = 8,
    .origin = -1,
};

static inline pio_sm_config rgb15_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + rgb15_wrap_target, offset + rgb15_wrap);
    // Configure auto-pull: shift to right, auto-pull enabled with threshold = 16 bits.
    sm_config_set_out_shift(&c, true, false, 16);
    return c;
}

static inline void rgb15_program_init(PIO pio, uint sm, uint offset, uint pin) {
    pio_sm_config c = rgb15_program_get_default_config(offset);
    // Map both the SET and OUT pin groups to 15 consecutive pins starting at 'pin'
    sm_config_set_set_pins(&c, pin, 15);
    sm_config_set_out_pins(&c, pin, 15);
    // Initialize the 15 GPIOs for output.
    for (int i = 0; i < 15; i++) {
        pio_gpio_init(pio, pin + i);
        gpio_init(pin + i);
        gpio_set_dir(pin + i, GPIO_OUT);
    }
    pio_sm_set_consecutive_pindirs(pio, sm, pin, 15, true);
    pio_sm_init(pio, sm, offset, &c);
}
#endif