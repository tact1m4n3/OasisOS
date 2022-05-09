#include <cpu.h>

#include <string.h>
#include <debug.h>
#include <pic.h>

typedef struct idt_entry {
    uint16_t base_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t flags;
    uint16_t base_mid;
    uint32_t base_high;
    uint32_t zero;
} __attribute__((packed)) idt_entry_t;

typedef struct idt_ptr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) idt_ptr_t;

extern void load_idt(void*);

#include "int.h"

static idt_entry_t idt[256];
static idt_ptr_t idt_ptr;
static void* int_handlers[256];

static void set_idt_entry(uint8_t id, uint64_t base, uint16_t selector, uint16_t flags) {
    idt[id].base_low = base & 0xFFFF;
    idt[id].selector = selector;
    idt[id].ist = 0;
    idt[id].flags = flags;
    idt[id].base_mid = (base >> 16) & 0xFFFF;
    idt[id].base_high = (base >> 32) & 0xFFFFFFFF;
    idt[id].zero = 0;
}

void idt_init() {
    idt_ptr.limit = 256 * sizeof(idt_entry_t) - 1;
    idt_ptr.base = (uint64_t)&idt;

    set_idt_entry(0, (uint64_t)&_isr0, 0x08, 0x8E);
    set_idt_entry(1, (uint64_t)&_isr1, 0x08, 0x8E);
    set_idt_entry(2, (uint64_t)&_isr2, 0x08, 0x8E);
    set_idt_entry(3, (uint64_t)&_isr3, 0x08, 0x8E);
    set_idt_entry(4, (uint64_t)&_isr4, 0x08, 0x8E);
    set_idt_entry(5, (uint64_t)&_isr5, 0x08, 0x8E);
    set_idt_entry(6, (uint64_t)&_isr6, 0x08, 0x8E);
    set_idt_entry(7, (uint64_t)&_isr7, 0x08, 0x8E);
    set_idt_entry(8, (uint64_t)&_isr8, 0x08, 0x8E);
    set_idt_entry(9, (uint64_t)&_isr9, 0x08, 0x8E);
    set_idt_entry(10, (uint64_t)&_isr10, 0x08, 0x8E);
    set_idt_entry(11, (uint64_t)&_isr11, 0x08, 0x8E);
    set_idt_entry(12, (uint64_t)&_isr12, 0x08, 0x8E);
    set_idt_entry(13, (uint64_t)&_isr13, 0x08, 0x8E);
    set_idt_entry(14, (uint64_t)&_isr14, 0x08, 0x8E);
    set_idt_entry(15, (uint64_t)&_isr15, 0x08, 0x8E);
    set_idt_entry(16, (uint64_t)&_isr16, 0x08, 0x8E);
    set_idt_entry(17, (uint64_t)&_isr17, 0x08, 0x8E);
    set_idt_entry(18, (uint64_t)&_isr18, 0x08, 0x8E);
    set_idt_entry(19, (uint64_t)&_isr19, 0x08, 0x8E);
    set_idt_entry(20, (uint64_t)&_isr20, 0x08, 0x8E);
    set_idt_entry(21, (uint64_t)&_isr21, 0x08, 0x8E);
    set_idt_entry(22, (uint64_t)&_isr22, 0x08, 0x8E);
    set_idt_entry(23, (uint64_t)&_isr23, 0x08, 0x8E);
    set_idt_entry(24, (uint64_t)&_isr24, 0x08, 0x8E);
    set_idt_entry(25, (uint64_t)&_isr25, 0x08, 0x8E);
    set_idt_entry(26, (uint64_t)&_isr26, 0x08, 0x8E);
    set_idt_entry(27, (uint64_t)&_isr27, 0x08, 0x8E);
    set_idt_entry(28, (uint64_t)&_isr28, 0x08, 0x8E);
    set_idt_entry(29, (uint64_t)&_isr29, 0x08, 0x8E);
    set_idt_entry(30, (uint64_t)&_isr30, 0x08, 0x8E);
    set_idt_entry(31, (uint64_t)&_isr31, 0x08, 0x8E);
    set_idt_entry(128, (uint64_t)&_isr128, 0x08, 0x8E | 0x60);
    
    set_idt_entry(32 + 0, (uint64_t)&_irq0, 0x08, 0x8E);
    set_idt_entry(32 + 1, (uint64_t)&_irq1, 0x08, 0x8E);
    set_idt_entry(32 + 2, (uint64_t)&_irq2, 0x08, 0x8E);
    set_idt_entry(32 + 3, (uint64_t)&_irq3, 0x08, 0x8E);
    set_idt_entry(32 + 4, (uint64_t)&_irq4, 0x08, 0x8E);
    set_idt_entry(32 + 5, (uint64_t)&_irq5, 0x08, 0x8E);
    set_idt_entry(32 + 6, (uint64_t)&_irq6, 0x08, 0x8E);
    set_idt_entry(32 + 7, (uint64_t)&_irq7, 0x08, 0x8E);
    set_idt_entry(32 + 8, (uint64_t)&_irq8, 0x08, 0x8E);
    set_idt_entry(32 + 9, (uint64_t)&_irq9, 0x08, 0x8E);
    set_idt_entry(32 + 10, (uint64_t)&_irq10, 0x08, 0x8E);
    set_idt_entry(32 + 11, (uint64_t)&_irq11, 0x08, 0x8E);
    set_idt_entry(32 + 12, (uint64_t)&_irq12, 0x08, 0x8E);
    set_idt_entry(32 + 13, (uint64_t)&_irq13, 0x08, 0x8E);
    set_idt_entry(32 + 14, (uint64_t)&_irq14, 0x08, 0x8E);
    set_idt_entry(32 + 15, (uint64_t)&_irq15, 0x08, 0x8E);

    pic_remap();

    asm volatile("lidt %0" : : "m"(idt_ptr));
}

void set_int_handler(uint8_t id, void* handler) {
    int_handlers[id] = handler;
}

regs_t* _int_handler(regs_t* r) {
    regs_t* (*handler)(regs_t* r);
    handler = int_handlers[r->int_no];
    if (handler) {
        r = handler(r);
    } else {
        FATAL("unhandled interrupt %x\n", r->int_no);
        for (;;);
    }

    if (r->int_no >= 32 && r->int_no < 48) {
        pic_send_eoi(r->int_no);
    }

    return r;
}
