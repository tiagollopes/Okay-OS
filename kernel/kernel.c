/* --- Funções de comunicação com o Hardware --- */

void outb(unsigned short port, unsigned char data) {
    __asm__ volatile("outb %0, %1" : : "a"(data), "Nd"(port));
}

unsigned char inb(unsigned short port) {
    unsigned char result;
    __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

void update_cursor(int pos) {
    unsigned short position = pos / 2;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(position & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((position >> 8) & 0xFF));
}

/* --- Funções Utilitárias --- */

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

// Verifica se uma string começa com outra (mais seguro para comandos)
int starts_with(const char* str, const char* prefix) {
    while (*prefix) {
        if (*prefix++ != *str++) return 0;
    }
    return 1;
}

void print(const char* str, int* pos, char cor) {
    char* video = (char*) 0xB8000;
    int i = 0;
    while (str[i]) {
        video[*pos] = str[i];
        video[*pos + 1] = cor;
        *pos += 2;
        i++;
    }
}

/* --- Sistema de Arquivos (Busca por Tags) --- */
void buscar_e_imprimir_tag(const char* tag, int* pos) {
    char* video = (char*) 0xB8000;

    // MUDANÇA: Começa a busca em 0x2000 para não ler o próprio Kernel
    char* busca = (char*) 0x2000;
    int achou_algo = 0;

    for (int m = 0; m < 40000; m++) {
        int i = 0;
        // Compara a tag
        while (tag[i] != '\0' && busca[m + i] == tag[i]) {
            i++;
        }

        if (tag[i] == '\0') {
            achou_algo = 1;
            *pos = ((*pos / 160) + 1) * 160;
            print(" >> ", pos, 0x0E);

            char* texto = &busca[m + i];
            for (int k = 0; k < 60; k++) {
                if (texto[k] == '[' || texto[k] == '\r' || texto[k] == '\n') break;

                if (texto[k] >= 32 && texto[k] <= 126) {
                    video[*pos] = texto[k];
                    video[*pos + 1] = 0x0F;
                    *pos += 2;
                }
            }
            *pos = ((*pos / 160) + 1) * 160;
            break;
        }
    }

    if (!achou_algo) {
        *pos = ((*pos / 160) + 1) * 160;
        print(" ERRO: TAG NAO ENCONTRADA NO DISCO", pos, 0x0C);
        *pos = ((*pos / 160) + 1) * 160;
    }
}

void kernel_main() {
    char* video = (char*) 0xB8000;
    char buffer[80];
    int buf_idx = 0;
    int pos = 0;

    // Estados do Teclado
    int caps_lock_ativo = 0;
    int shift_pressionado = 0;

    void limpar_tela() {
        for (int n = 0; n < 80 * 25 * 2; n += 2) {
            video[n] = ' ';
            video[n+1] = 0x07;
        }
    }

    limpar_tela();
    print("OKAY-OS v1.0.3", &pos, 0x0B);
    pos = 160;
    print("> ", &pos, 0x0A);
    update_cursor(pos);

    while (1) {
        if (inb(0x64) & 0x01) {
            unsigned char scancode = inb(0x60);

            // 1. Detectar SHIFT (Pressionado e Solto)
            if (scancode == 0x2A || scancode == 0x36) {
                shift_pressionado = 1;
            }
            else if (scancode == 0xAA || scancode == 0xB6) {
                shift_pressionado = 0;
            }

            // 2. Detectar CAPS LOCK (Toggle)
            else if (scancode == 0x3A) {
                caps_lock_ativo = !caps_lock_ativo;
            }

            // 3. Processar teclas quando pressionadas (scancode < 0x80)
            if (scancode < 0x80) {
                char letra = 0;
                switch(scancode) {
                    // Letras
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

                    // Números e Símbolos Superiores (Usa Operador Ternário para decidir)
                    case 0x02: letra = shift_pressionado ? '!' : '1'; break;
                    case 0x03: letra = shift_pressionado ? '@' : '2'; break;
                    case 0x04: letra = shift_pressionado ? '#' : '3'; break;
                    case 0x05: letra = shift_pressionado ? '$' : '4'; break;
                    case 0x06: letra = shift_pressionado ? '%' : '5'; break;
                    case 0x07: letra = shift_pressionado ? '^' : '6'; break;
                    case 0x08: letra = shift_pressionado ? '&' : '7'; break;
                    case 0x09: letra = shift_pressionado ? '*' : '8'; break;
                    case 0x0A: letra = shift_pressionado ? '(' : '9'; break;
                    case 0x0B: letra = shift_pressionado ? ')' : '0'; break;

                    // Pontuação e Símbolos
                    case 0x34: letra = shift_pressionado ? '>' : '.'; break;
                    case 0x33: letra = shift_pressionado ? '<' : ','; break;
                    case 0x27: letra = shift_pressionado ? ':' : ';'; break;
                    case 0x0C: letra = shift_pressionado ? '_' : '-'; break;
                    case 0x0D: letra = shift_pressionado ? '+' : '='; break;
                    case 0x35: letra = shift_pressionado ? '?' : '/'; break;
                    case 0x1A: letra = shift_pressionado ? '{' : '['; break;
                    case 0x1B: letra = shift_pressionado ? '}' : ']'; break;
                    case 0x39: letra = ' '; break;

                    case 0x1C: // ENTER
                        buffer[buf_idx] = '\0';
                        if (starts_with(buffer, "--HELP") || starts_with(buffer, "--help")) {
                            buscar_e_imprimir_tag("[HELP]", &pos);
                        }
                        else if (starts_with(buffer, "--VERSION") || starts_with(buffer, "--version")) {
                            buscar_e_imprimir_tag("[VERS]", &pos);
                        }
                        else if (starts_with(buffer, "--INFO") || starts_with(buffer, "--info")) {
                            buscar_e_imprimir_tag("[INFO]", &pos);
                        }
                        else if (starts_with(buffer, "CLEAR") || starts_with(buffer, "clear")) {
                            limpar_tela();
                            pos = 0;
                        }
                        else if (buf_idx > 0) {
                            pos = ((pos / 160) + 1) * 160;
                            print("COMANDO DESCONHECIDO", &pos, 0x0C);
                            pos = ((pos / 160) + 1) * 160;
                        }
                        print("> ", &pos, 0x0A);
                        buf_idx = 0;
                        update_cursor(pos);
                        continue;

                    case 0x0E: // BACKSPACE
                        if (buf_idx > 0) {
                            buf_idx--;
                            pos -= 2;
                            video[pos] = ' ';
                            update_cursor(pos);
                        }
                        continue;
                }

                // 4. Lógica de Caixa Alta/Baixa (XOR entre Caps e Shift)
                if (letra >= 'A' && letra <= 'Z') {
                    // Se Caps=1 e Shift=0 -> Maiúscula (Original)
                    // Se Caps=1 e Shift=1 -> Minúscula (Inverte)
                    // Se Caps=0 e Shift=1 -> Maiúscula
                    // Se Caps=0 e Shift=0 -> Minúscula
                    int deve_ser_maiuscula = caps_lock_ativo ^ shift_pressionado;
                    if (!deve_ser_maiuscula) {
                        letra = letra + 32;
                    }
                }

                if (letra != 0 && buf_idx < 79) {
                    buffer[buf_idx++] = letra;
                    video[pos] = letra;
                    video[pos + 1] = 0x0E; // Amarelo
                    pos += 2;
                    update_cursor(pos);
                }
            }
        }
    }
}
