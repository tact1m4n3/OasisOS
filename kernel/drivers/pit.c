#include <pit.h>

#include <ports.h>
#include <debug.h>
#include <cpu.h>

#define PIT_DATA 0x40
#define PIT_CMD 0x43

static regs_t* pit_callback(regs_t* r) {
    return r;
}

void pit_init() {
    pit_set_freq(100);
    set_int_handler(32, &pit_callback);
}

void pit_set_freq(uint16_t freq) {
    freq = 1193180 / freq;
    outb(PIT_CMD, 0x34);
    outb(PIT_DATA, freq);
    outb(PIT_DATA, (freq >> 8) & 0xFF);
}
