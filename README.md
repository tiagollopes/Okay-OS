# Okay-OS

O **Okay-OS** é um sistema operacional educacional escrito do zero para a arquitetura x86. O objetivo deste projeto é aprender, na prática, como o hardware e o software se comunicam nos níveis mais baixos (Ring 0).

---

## Fase Atual: Interface e Estabilidade (v1.0.4)

O sistema agora oferece uma experiência de digitação completa e um interpretador de comandos inteligente.

### Novidades desta versão:

* **Teclado Avançado**: Suporte para números (`0-9`) e símbolos (`! @ # $ % & * ( ) _ + { } [ ] : ; > < ? /`).
* **Lógica Shift & Caps Lock**: Implementação de inversão de caixa (XOR) para comportamento de teclado real.
* **Comandos Case-Insensitive**: O terminal aceita comandos em maiúsculas ou minúsculas (ex: `CLEAR` ou `clear`).
* **Estabilidade de Memória**: Busca de arquivos otimizada iniciando em `0x2000` para evitar conflitos com o Kernel.

### 🛠️ O que o sistema já faz:

* **Bootloader (`boot.asm`)**: Gerencia o salto para Modo Protegido (32 bits) e carrega o sistema.
* **Kernel em C**: Gerencia drivers de vídeo VGA e teclado (Porta 0x60).
* **SFS (Simple File System)**: Sistema de arquivos baseado em tags no arquivo `init.ok`.
* **Interpretador de Comandos**: `--help`, `--version`, `--info` e `clear`.

---

## 💻 Como Compilar e Executar

1. **Compilar**: `./build.sh`
2. **Executar**: `qemu-system-i386 -drive format=raw,file=build/os.img`

---

## 📁 Estrutura do Projeto

* **`boot/boot.asm`**: Inicialização e GDT.
* **`kernel/kernel.c`**: Core do SO e Drivers de teclado/vídeo.
* **`init.ok`**: Dados do sistema (Tags de texto).
* **`linker.ld`**: Organização da memória RAM.
* **`build.sh`**: Automação da compilação e fusão de binários.

**Feito por Tiago LLopes** Santos/SP - Brasil 🇧🇷
