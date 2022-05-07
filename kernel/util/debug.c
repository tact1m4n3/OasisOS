#include <debug.h>

#include <string.h>
#include <ports.h>
#include <vga.h>

static uint64_t num2str(char* buf, uint64_t val, uint64_t base) {
    if (val == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return 1;
    }

    uint64_t len = 0, copy = val;
    while (copy > 0) {
        len++;
        copy /= base;
    }
    
    char syms[] = "0123456789abcdef";
    uint64_t i = len;
    while (val > 0) {
        buf[--i] = syms[val % base];
        val /= base;
    }
    buf[len] = '\0';
    
    return len;
}

static void print_with_args(char* fmt, va_list args) {
    while (*fmt) {
        if (*fmt != '%') {
            vga_print_char(*fmt++);
            continue;
        }

        char* s;
        uint64_t base = 0;
        switch (*++fmt) {
            case 'c':
                vga_print_char((char)va_arg(args, int));
                break;
            case 's':
                vga_print_str((char*)va_arg(args, char*));
                break;
            case 'd':
                base = 10;
                break;
            case 'x':
                base = 16;
                break;
            default:
                vga_print_char('%');
                fmt--;
        }

        if (base) {
            char buf[128];
            uint64_t val = (uint64_t)va_arg(args, int);
            num2str(buf, val, base);
            vga_print_str(buf);
        }
        
        fmt++;
    }
}

void print(char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    print_with_args(fmt, args);
    va_end(args);
}

void clear_screen() {
    vga_clear_buffer();
}
