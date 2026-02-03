# Okay-OS

O **Okay-OS** é um sistema operacional educacional escrito do zero para arquitetura x86.

O objetivo deste projeto é aprender, na prática, como o hardware e o software se comunicam nos níveis mais baixos.

## Fase Atual: Interface e Entrada de Dados (32 Bits)

O sistema evoluiu! Além de rodar em **Modo Protegido (32 bits)**, ele agora possui drivers básicos para interação com o usuário.

### O que o sistema já faz:

1.  **Bootloader (`boot.asm`)**: Faz o salto do Modo Real para o Modo Protegido e configura a GDT.
2.  **Driver de Vídeo VGA**: Escreve diretamente na memória `0xB8000`, com suporte a cores e controle do cursor de hardware.
3.  **Driver de Teclado**: Lê *scancodes* via porta `0x60` e traduz para caracteres ASCII, permitindo digitação em tempo real.
4.  **Gerenciamento de Cursor**: O cursor piscante (`_`) acompanha a digitação e reage ao Backspace.

---

## 🛠️ Como Compilar e Executar

Este projeto foi desenvolvido em ambiente Linux (Lubuntu/Ubuntu).

### 1. Pré-requisitos

Instale as ferramentas de compilação e o emulador:

`sudo apt update`

`sudo apt install nasm gcc-multilib build-essential qemu-system-x86`

### 2. Compilar (Build)

Use o script de automação para gerar a imagem do sistema:


`chmod +x build.sh`

`./build.sh`

Isso gerará o arquivo build/os.img

### 3. Executar no QEMU

Para rodar o SO com o mapa de teclado correto:

`qemu-system-i386 -drive format=raw,file=build/os.img -k pt-br`

*(Dica: Clique dentro da janela do QEMU para o sistema capturar seu teclado. Use Ctrl+Alt ou Ctrl+Alt+G para sair).*


**Estrutura de Arquivos**

boot/boot.asm: *Inicialização, GDT e salto para 32-bit.*

kernel/entry.asm: *Ponte entre o Assembly e o Kernel em C.*

kernel/kernel.c: *Coração do sistema (Drivers de vídeo, cursor e teclado).*

linker.ld: *Define a organização do código na memória RAM.*

build.sh: *Script que automatiza o GCC, NASM e LD.*
