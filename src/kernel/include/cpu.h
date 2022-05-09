#pragma once

#include <types.h>

typedef struct regs {
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rsi, rdi, rbp, rdx, rcx, rbx, rax;
    uint64_t int_no, err_code;
    uint64_t rip, cs, rflags, rsp, ss;
} regs_t;

void idt_init();
void set_int_handler(uint8_t id, void* handler);
