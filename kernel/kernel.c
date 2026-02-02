// kernel/kernel.c
void kernel_main() {
    char* video = (char*) 0xB8000;

    const char* msg = "Kernel 32 bits ativo!";
    while (*msg) {
        *video++ = *msg++;
        *video++ = 0x0F; // branco sobre preto
    }

    while (1) {}  // loop infinito
}
