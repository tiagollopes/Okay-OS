# Okay-OS

Sistema operacional educacional criado do zero, com foco em baixo nível.

## Fase atual
Bootloader em Assembly que:
- Inicializa em 16 bits (Real Mode)
- Configura GDT
- Entra em 32 bits (Protected Mode)
- Transfere execução para um kernel 32 bits

## Status
- Protected Mode 32 bits funcionando
- Escrita direta na memória de vídeo (VGA)
- Executando corretamente no QEMU
- Base pronta para entrada de kernel em C
