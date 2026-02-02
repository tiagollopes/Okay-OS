#!/bin/bash
set -e
mkdir -p build

echo "1. Compilando Kernel em C..."
# Trocamos i386-elf-gcc por gcc -m32
gcc -m32 -ffreestanding -fno-pic -O2 -c kernel/kernel.c -o build/kernel_c.o

echo "2. Montando entry.asm (32 bits)..."
nasm -f elf32 kernel/entry.asm -o build/entry.o

echo "3. Linkando tudo em kernel.bin..."
# Trocamos i386-elf-gcc-ld por ld -m elf_i386
ld -m elf_i386 -T linker.ld -o build/kernel.bin build/entry.o build/kernel_c.o --oformat binary

echo "4. Montando bootloader..."
nasm -f bin boot/boot.asm -o build/boot.bin

echo "5. Criando imagem final..."
cat build/boot.bin build/kernel.bin > build/os.img

echo "Pronto! Imagem gerada em build/os.img"
