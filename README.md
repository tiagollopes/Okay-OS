# Okay-OS

Sistema operacional educacional criado do zero, com foco em baixo nível.

## Fase atual
Bootloader em Assembly que:
- Inicializa em 16 bits (Real Mode)
- Configura GDT
- Entra em 32 bits (Protected Mode)
- Transfere execução para um kernel 32 bits em C
- Escrita direta na memória de vídeo (VGA)

## Status
- Protected Mode 32 bits funcionando
- Kernel em C executando corretamente no QEMU
- Base pronta para evolução (C, drivers, multitarefa etc.)

## Como compilar e rodar
Para compilar e rodar o sistema, siga os passos abaixo:

1 - Criar a pasta de build:

mkdir -p build

2 - Compilar o kernel e gerar o binário:

nasm -f elf32 kernel/entry.asm -o build/entry.o

nasm -f elf32 kernel/kernel.asm -o build/kernel.o

i386-elf-gcc -ffreestanding -c kernel/kernel.c -o build/kernel_c.o

ld -m elf_i386 -T linker.ld -o build/kernel.bin build/entry.o build/kernel.o build/kernel_c.o --oformat binary

3 - Compilar o bootloader e montar a imagem final:

nasm -f bin boot/boot.asm -o build/boot.bin

cat build/boot.bin build/kernel.bin > build/os.img

4 - Rodar no QEMU:

qemu-system-i386 -drive format=raw,file=build/os.img



