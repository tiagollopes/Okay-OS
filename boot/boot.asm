[org 0x7C00]
bits 16

start:
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Limpa a tela
    mov ax, 0x0003
    int 0x10

    ; Carrega o kernel do disco (Lendo 15 setores para garantir)
    mov ah, 0x02
    mov al, 15
    mov ch, 0
    mov cl, 2           ; Setor 2 (onde começa o kernel)
    mov dh, 0
    mov dl, 0x80        ; Primeiro disco rígido
    mov bx, 0x1000      ; Endereço onde o kernel será carregado
    int 0x13

    cli                 ; Desativa interrupções
    lgdt [gdt_descriptor] ; Carrega a tabela de segmentos (GDT)

    ; Ativa o Modo Protegido no registro CR0
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax

    ; Faz um "Far Jump" para limpar o pipeline e entrar em 32 bits
    jmp 0x08:init_pm

bits 32
init_pm:
    ; Configura os registros de dados para o novo segmento de 32 bits
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Pula para o endereço onde carregamos o kernel (0x1000)
    jmp 0x1000

; Tabela GDT necessária para 32 bits
gdt_start:
    dq 0x0
gdt_code:
    dw 0xFFFF, 0x0, 0x9A00, 0x00CF
gdt_data:
    dw 0xFFFF, 0x0, 0x9200, 0x00CF
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

times 510-($-$$) db 0
dw 0xAA55
