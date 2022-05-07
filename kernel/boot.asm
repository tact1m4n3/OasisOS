MBOOT2_MAGIC equ 0xe85250d6
MBOOT2_ARCH equ 0
MBOOT2_LENGTH equ mboot2_header.end - mboot2_header
MBOOT2_CHECKSUM equ -(MBOOT2_MAGIC + MBOOT2_ARCH + MBOOT2_LENGTH)

[section .multiboot]
align 8
mboot2_header:
    dd MBOOT2_MAGIC
    dd MBOOT2_ARCH
    dd MBOOT2_LENGTH
    dd MBOOT2_CHECKSUM

align 8
.end_tag:
    dw 0
    dw 0
    dd 8
.end:

[section .text]
[global start]
[bits 32]
start:
    cli

    mov esp, stack.top

    mov edi, eax
    mov esi, ebx

    mov eax, PDPT
    or eax, 0b11
    mov dword [PML4], eax

    mov eax, PDT
    or eax, 0b11
    mov dword [PDPT], eax

    mov ecx, 0
.loop:
    mov eax, 0x200000
    mul ecx
    or eax, 0b10000011
    mov dword [PDT + ecx * 8], eax
    inc ecx
    cmp ecx, 512
    jne .loop

    mov eax, PML4
    mov cr3, eax

    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    lgdt [gdt64.ptr]

    jmp 0x08:realm64

error:
    mov dword [0xb8000], 0x4f524f45
    mov dword [0xb8004], 0x00004f52
    hlt

[bits 64]
[extern kernel_main]
realm64:
    mov ax, 0x0
    mov ss, ax
    mov ds, ax
    mov es, ax
    
    mov rax, 0xFFFFFFFF
    and rdi, rax
    and rsi, rax

    call kernel_main

    jmp $

[section .bss]
align 4096
PML4:
    resb 4096
PDPT:
    resb 4096
PDT:
    resb 4096
stack:
    resb 4096
.top:

[section .rodata]
align 8
gdt64:
    dq 0
.code:
    dq (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53)
.ptr:
    dw $ - gdt64 - 1
    dq gdt64
