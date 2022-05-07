%macro ISR_ERR 1
[global _isr%1]
_isr%1:
    push qword %1
    jmp int_common
%endmacro

%macro ISR_NO_ERR 1
[global _isr%1]
_isr%1:
    push qword 0
    push qword %1
    jmp int_common
%endmacro

%macro IRQ 1
[global _irq%1]
_irq%1:
    push qword 0
    push qword %1 + 0x20
    jmp int_common
%endmacro

%macro pushaq 0
    push rax
    push rbx
    push rcx
    push rdx
    push rbp
    push rdi
    push rsi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
%endmacro

%macro popaq 0
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rsi
    pop rdi
    pop rbp
    pop rdx
    pop rcx
    pop rbx
    pop rax
%endmacro

[section .text]
[extern _int_handler]
int_common:
    pushaq

    mov rdi, rsp
    call _int_handler
    mov rsp, rax

    popaq
    add esp, 16

    iretq

ISR_NO_ERR  0
ISR_NO_ERR  1
ISR_NO_ERR  2
ISR_NO_ERR  3
ISR_NO_ERR  4
ISR_NO_ERR  5
ISR_NO_ERR  6
ISR_NO_ERR  7
ISR_ERR     8
ISR_NO_ERR  9
ISR_ERR     10
ISR_ERR     11
ISR_ERR     12
ISR_ERR     13
ISR_ERR     14
ISR_NO_ERR  15
ISR_NO_ERR  16
ISR_ERR     17
ISR_NO_ERR  18
ISR_NO_ERR  19
ISR_NO_ERR  20
ISR_ERR     21
ISR_NO_ERR  22
ISR_NO_ERR  23
ISR_NO_ERR  24
ISR_NO_ERR  25
ISR_NO_ERR  26
ISR_NO_ERR  27
ISR_NO_ERR  28
ISR_ERR     29
ISR_ERR     30
ISR_NO_ERR  31
ISR_NO_ERR  128

IRQ  0
IRQ  1
IRQ  2
IRQ  3
IRQ  4
IRQ  5
IRQ  6
IRQ  7
IRQ  8
IRQ  9
IRQ  10
IRQ  11
IRQ  12
IRQ  13
IRQ  14
IRQ  15
