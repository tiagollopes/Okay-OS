org 0x7C00
bits 16

start:
    cli
    xor ax, ax
    mov ds, ax
    mov ss, ax
    mov sp, 0x7C00

    ; carregar GDT
    lgdt [gdt_descriptor]

    ; habilitar protected mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; salto FAR para 32 bits
    jmp CODE_SEG:pm_start

; =========================
; GDT
; =========================
gdt_start:
    dq 0x0000000000000000
gdt_code:
    dq 0x00CF9A000000FFFF
gdt_data:
    dq 0x00CF92000000FFFF
gdt_end:

; descritor da GDT
gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

; seletores (ISSENCIAL estarem aqui)
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

; =========================
; 32 BITS
; =========================
bits 32
pm_start:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    ; -----------------
    ; Limpa a tela
    ; -----------------
    mov edi, 0xB8000
    mov ecx, 2000
    mov eax, 0x07200720

.clear:
    mov [edi], eax
    add edi, 4
    loop .clear

    ; -----------------
    ; Escreve mensagem
    ; -----------------
    mov edi, 0xB8000
    mov esi, message

.print:
    lodsb
    test al, al
    jz .hang
    mov [edi], al
    mov byte [edi+1], 0x0F
    add edi, 2
    jmp .print

.hang:
    cli
    hlt

message:
    db 'Kernel 32 bits ATIVO!', 0

times 510 - ($ - $$) db 0
dw 0xAA55
