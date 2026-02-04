#!/bin/bash
set -e
mkdir -p build

echo "1. Compilando Kernel em C..."
gcc -m32 -ffreestanding -fno-pic -O2 -c kernel/kernel.c -o build/kernel_c.o

echo "2. Montando entry.asm..."
nasm -f elf32 kernel/entry.asm -o build/entry.o

echo "3. Linkando tudo em kernel.bin..."
ld -m elf_i386 -T linker.ld -o build/kernel.bin build/entry.o build/kernel_c.o --oformat binary

echo "4. Montando o Bootloader..."
nasm -f bin boot/boot.asm -o build/boot.bin

echo "5. Gerando imagem final..."
# Garante que o bootloader tem 512 bytes
truncate -s 512 build/boot.bin
# Garante que o kernel tem exatamente 4096 bytes (4KB)
truncate -s 4096 build/kernel.bin

# Junta tudo: [Boot 512b] + [Kernel 4KB] + [Seu Texto]
cat build/boot.bin build/kernel.bin init.ok > build/os.img

# Aumenta o tamanho final para a BIOS ler tudo
truncate -s 20k build/os.img

echo "------------------------------------------------"
echo "Pronto! Imagem gerada em build/os.img"
echo "O init.ok foi fundido logo apos o Kernel."
