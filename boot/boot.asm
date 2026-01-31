org 0x7C00
bits 16

start:
    ; Aponta ES para a memória de vídeo
    mov ax, 0xB800
    mov es, ax

    ; Limpa a tela inteira (80x25 = 2000 caracteres)
    xor di, di
    mov cx, 2000

clear_screen:
    mov byte [es:di], ' '     ; espaço
    mov byte [es:di+1], 0x0F  ; branco no preto
    add di, 2
    loop clear_screen

    ; Escreve a mensagem
    xor di, di
    mov si, mensagem

print_loop:
    lodsb
    cmp al, 0
    je done

    mov [es:di], al
    mov byte [es:di+1], 0x0F
    add di, 2
    jmp print_loop

done:
    hlt

mensagem:
    db 'Meu SO escreve direto no video!', 0

times 510 - ($ - $$) db 0
dw 0xAA55
