#include <vga.h>

#include <string.h>
#include <ports.h>

#define DEFAULT_COLOR 0x0f

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

#define VGA_ADDR_PORT 0x3D4
#define VGA_DATA_PORT 0x3D5

static uint16_t* vga_buffer = (uint16_t*)0xb8000;
static uint16_t pos_x, pos_y;

static void update_vga_cursor() {
    uint16_t final_pos = pos_x + pos_y * SCREEN_WIDTH;
    
    outb(VGA_ADDR_PORT, 0xF);
    outb(VGA_DATA_PORT, final_pos & 0xFF);
    outb(VGA_ADDR_PORT, 0xE);
    outb(VGA_DATA_PORT, (final_pos >> 8) & 0xFF);
}

void vga_print_char(char c) {
    switch (c) {
        case '\n':
            pos_y++;
            pos_x = 0;
            break;
        case '\t':
            pos_x += 4;
            break;
        default:
            vga_buffer[pos_x + pos_y * SCREEN_WIDTH] = c | (DEFAULT_COLOR << 8);
            pos_x++;
            break;
    }

    if (pos_x >= SCREEN_WIDTH) {
        pos_y += pos_x / SCREEN_WIDTH;
        pos_x %= SCREEN_WIDTH;
    }

    update_vga_cursor();
}

void vga_print_str(char* s) {
    while (*s) {
        vga_print_char(*s++);
    }
}

void vga_clear_buffer() {
    pos_x = 0, pos_y = 0;
    
    for (uint64_t i = 0; i < SCREEN_WIDTH; i++) {
        for (uint64_t j = 0; j < SCREEN_HEIGHT; j++) {
            vga_buffer[j + i * SCREEN_WIDTH] = ' ' | (DEFAULT_COLOR << 8);
        }
    }

    update_vga_cursor();
}
