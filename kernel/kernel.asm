; kernel.asm
[org 0x1000]
bits 16

start:
    mov ah, 0x0E
    mov al, 'O'
    int 0x10
    mov al, 'K'
    int 0x10

.hang:
    jmp .hang
