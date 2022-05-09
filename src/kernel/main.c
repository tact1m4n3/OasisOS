#include <types.h>
#include <string.h>
#include <ports.h>
#include <vga.h>
#include <debug.h>
#include <multiboot.h>
#include <pic.h>
#include <cpu.h>
#include <pit.h>
#include <memory.h>

extern void* end;

void kernel_main(uint64_t mboot_magic, void* mboot_info) {
    clear_screen();

    INFO("kernel booted\n");

    multiboot_init(mboot_magic, mboot_info);
    pmm_init();
    idt_init();
    pit_init();
    asm("sti");

    INFO("core kernel inited\n");
    INFO("%x", pmm_alloc_frame());

    for (;;);
}