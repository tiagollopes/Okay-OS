/* --- Funções de comunicação com o Hardware --- */

// Escreve um byte em uma porta (necessário para o cursor)
void outb(unsigned short port, unsigned char data) {
    __asm__ volatile("outb %0, %1" : : "a"(data), "Nd"(port));
}

// Lê um byte de uma porta (necessário para o teclado)
unsigned char inb(unsigned short port) {
    unsigned char result;
    __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

// Move o cursor piscante (_) para a posição atual de escrita
void update_cursor(int pos) {
    unsigned short position = pos / 2; // O hardware conta caracteres, não bytes
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(position & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((position >> 8) & 0xFF));
}

/* --- Função Principal do Kernel --- */

void kernel_main() {
    char* video = (char*) 0xB8000;

    // 1. Limpa a tela
    for (int n = 0; n < 80 * 25 * 2; n += 2) {
        video[n] = ' ';
        video[n+1] = 0x07;
    }

    // 2. Escreve Linha 1: Título
    const char* msg1 = "Okay-OS Ativo!";
    int i = 0;
    while (msg1[i]) {
        video[i*2] = msg1[i];
        video[i*2 + 1] = 0x0B; // Azul claro
        i++;
    }

    // 3. Escreve Linha 2: Prompt (Endereço 160 é o início da 2ª linha)
    const char* msg2 = "Digite algo: ";
    int linha2_start = 160;
    int j = 0;
    while (msg2[j]) {
        video[linha2_start + (j*2)] = msg2[j];
        video[linha2_start + (j*2) + 1] = 0x07; // Cinza
        j++;
    }

    // A posição de digitação começa logo após o "Digite algo: "
    int pos = linha2_start + (j * 2);
    update_cursor(pos);

    // 4. Loop do Teclado
    while (1) {
        if (inb(0x64) & 0x01) {
            unsigned char scancode = inb(0x60);

            if (scancode < 0x80) {
                char letra = 0;

                switch(scancode) {
                    case 0x1E: letra = 'A'; break; case 0x30: letra = 'B'; break;
                    case 0x2E: letra = 'C'; break; case 0x20: letra = 'D'; break;
                    case 0x12: letra = 'E'; break; case 0x21: letra = 'F'; break;
                    case 0x22: letra = 'G'; break; case 0x23: letra = 'H'; break;
                    case 0x17: letra = 'I'; break; case 0x24: letra = 'J'; break;
                    case 0x25: letra = 'K'; break; case 0x26: letra = 'L'; break;
                    case 0x32: letra = 'M'; break; case 0x31: letra = 'N'; break;
                    case 0x18: letra = 'O'; break; case 0x19: letra = 'P'; break;
                    case 0x10: letra = 'Q'; break; case 0x13: letra = 'R'; break;
                    case 0x1F: letra = 'S'; break; case 0x14: letra = 'T'; break;
                    case 0x16: letra = 'U'; break; case 0x2F: letra = 'V'; break;
                    case 0x11: letra = 'W'; break; case 0x2D: letra = 'X'; break;
                    case 0x15: letra = 'Y'; break; case 0x2C: letra = 'Z'; break;
                    case 0x39: letra = ' '; break;
                    case 0x0E: // BACKSPACE
                        if (pos > linha2_start + (j * 2)) {
                            pos -= 2;
                            video[pos] = ' ';
                            video[pos + 1] = 0x07;
                            update_cursor(pos);
                        }
                        continue;
                }

                if (letra != 0) {
                    video[pos] = letra;
                    video[pos + 1] = 0x0E; // Letras em Amarelo
                    pos += 2;
                    update_cursor(pos);
                }
            }
        }
    }
}
