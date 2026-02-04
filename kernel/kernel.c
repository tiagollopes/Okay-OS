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
/* --- Função Principal --- */

void kernel_main() {
    char* video = (char*) 0xB8000;
    char buffer[80];
    int buf_idx = 0;
    int pos = 0;

    void limpar_tela() {
        for (int n = 0; n < 80 * 25 * 2; n += 2) {
            video[n] = ' ';
            video[n+1] = 0x07;
        }
    }

    limpar_tela();
    print("OKAY-OS", &pos, 0x0B);
    pos = 160; // Pula para a segunda linha
    print("> ", &pos, 0x0A);
    update_cursor(pos);

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
                    case 0x0C: letra = '-'; break;
                    case 0x39: letra = ' '; break;
                    case 0x1C: // TECLA ENTER
                        buffer[buf_idx] = '\0'; // Finaliza a string

                        // DEBUG: Vamos ver o que o Kernel recebeu
                        // Pula linha e mostra o que foi digitado entre aspas
                       // pos = ((pos / 160) + 1) * 160;
                        // print("DEBUG: VOCE DIGITOU [", &pos, 0x07);
                        // print(buffer, &pos, 0x0F);
                        // print("]", &pos, 0x07);
                        // pos = ((pos / 160) + 1) * 160;

                        // Comparações
                        if (starts_with(buffer, "--HELP")) {
                            buscar_e_imprimir_tag("[HELP]", &pos);
                        }
                        else if (starts_with(buffer, "--VERSION")) {
                            buscar_e_imprimir_tag("[VERS]", &pos);
                        }
                        else if (starts_with(buffer, "--INFO")) {
                            buscar_e_imprimir_tag("[INFO]", &pos);
                        }
                        else if (starts_with(buffer, "CLEAR")) {
                            limpar_tela();
                            pos = 0;
                        }
                        else {
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
                if (letra != 0 && buf_idx < 79) {
                    buffer[buf_idx++] = letra;
                    video[pos] = letra;
                    video[pos + 1] = 0x0E;
                    pos += 2;
                    update_cursor(pos);
                }
            }
        }
    }
}
