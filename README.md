# Okay-OS

O **Okay-OS** é um sistema operacional educacional escrito do zero para arquitetura x86. O objetivo deste projeto é aprender, na prática, como o hardware e o software se comunicam nos níveis mais baixos.

## Fase Atual: O Salto para 32 Bits (Modo Protegido)

Atualmente, o sistema já consegue sair do "Modo Real" (16 bits) e entrar no "Modo Protegido" (32 bits). Isso é fundamental para que possamos escrever o Kernel em linguagens de alto nível, como **C**, em vez de apenas Assembly.

### Como o sistema inicia:
1.  **Bootloader (`boot.asm`)**: O PC liga em 16 bits. O bootloader prepara a GDT (Global Descriptor Table), ativa o Modo Protegido e carrega o kernel do disco para a memória.
2.  **Porteiro (`entry.asm`)**: Um pequeno código em Assembly de 32 bits que serve de ponte, chamando a função principal do Kernel em C.
3.  **Kernel (`kernel.c`)**: O "cérebro" do sistema. Nesta fase, ele escreve diretamente na memória de vídeo VGA (endereço `0xB8000`) para exibir mensagens na tela.

---

## 🛠️ Como Compilar e Executar

Este projeto foi desenvolvido em ambiente Linux (Lubuntu/Ubuntu).

### Pré-requisitos
1. Abra o seu terminal e instale as ferramentas necessárias:

sudo apt update
sudo apt install nasm gcc-multilib build-essential qemu-system-x86

2. Compilar (Build)
O projeto possui um script que faz todo o trabalho pesado de compilação e montagem da imagem do disco:


chmod +x build.sh
./build.sh
Isso gerará o arquivo build/os.img.

3. Executar no QEMU
Para ver o sistema funcionando em um emulador:


qemu-system-i386 -drive format=raw,file=build/os.img

Estrutura de Arquivos
boot/boot.asm: Código de inicialização (16-bit).

kernel/entry.asm: Ponto de entrada para o modo 32-bit.

kernel/kernel.c: Lógica principal em C.

linker.ld: Mapa que organiza onde cada pedaço de código fica na memória.

build.sh: Script de automação do processo de compilação.
