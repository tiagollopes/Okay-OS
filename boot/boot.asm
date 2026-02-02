; boot.asm
[org 0x7C00]
bits 16

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    sti

    ; Limpa tela
    mov ax, 0x0003
    int 0x10

    ; ---- Ler kernel do disco ----
    mov ah, 0x02        ; INT 13h - read sectors
    mov al, 1           ; número de setores
    mov ch, 0           ; cilindro
    mov cl, 2           ; setor 2 (logo após boot)
    mov dh, 0           ; head
    mov dl, 0x80        ; disco (HD)
    mov bx, 0x1000      ; destino
    int 0x13

    jc disk_error

    ; ---- Pular para kernel ----
    jmp 0x0000:0x1000

disk_error:
    mov si, msg
.print:
    lodsb
    or al, al
    jz $
    mov ah, 0x0E
    int 0x10
    jmp .print

msg db "DISK ERROR", 0

times 510-($-$$) db 0
dw 0xAA55
