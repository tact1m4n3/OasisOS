[org 0x7c00]
[bits 16]
    jmp 0x0:entry
entry:
    xor ax, ax
    mov ds, ax
    mov es, ax

    mov sp, 0x7c00
    mov bp, sp

    mov ah, 0x0
    mov al, 0x3
    int 0x10

    mov bx, hello_msg
    call print

    jmp $

print:
    mov ah, 0x0e
.loop:
    mov al, [bx]
    cmp al, 0
    je .end
    
    int 0x10
    add bx, 1
    jmp .loop
.end:
    ret

hello_msg: db "hello world from usb", 10, 13, 0

times 510 - ($ - $$) db 0
dw 0xaa55
