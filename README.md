#  Okay-OS

O **Okay-OS** é um sistema operacional educacional escrito do zero para a arquitetura x86. O objetivo deste projeto é aprender, na prática, como o hardware e o software se comunicam nos níveis mais baixos (Ring 0).

---

##  Fase Atual: Sistema de Arquivos e Tags (v1.0.2)

O sistema evoluiu drasticamente! Além de rodar em **Modo Protegido (32 bits)**, ele agora possui um sistema de busca de dados em memória e um interpretador de comandos básico.

###  O que o sistema já faz:

* **Bootloader (`boot.asm`)**: Gerencia o salto do Modo Real para o Modo Protegido, configura a GDT e carrega múltiplos setores do disco para a RAM.
* **Kernel em C**: Gerencia a lógica do sistema com drivers de vídeo VGA (0xB8000) e teclado (Porta 0x60).
* **SFS (Simple File System)**: Implementação de um sistema de arquivos baseado em tags no arquivo `init.ok`.
* **Interpretador de Comandos**:
    * `--HELP`: Busca e exibe instruções do arquivo de dados.
    * `--VERSION`: Exibe a versão atual do sistema carregada do disco.
    * `--INFO`: Exibe informações atual do sistema carregado do disco.
    * `CLEAR`: Limpa o buffer de vídeo e reseta a posição do cursor.
* **Gerenciamento de Cursor**: Controle via portas de hardware (0x3D4/0x3D5) para acompanhar a digitação.

---

## 🛠️ Como Compilar e Executar

Este projeto foi desenvolvido e testado em ambiente Linux (**Lubuntu/Ubuntu**).

### 1. Pré-requisitos
Instale as ferramentas necessárias:
```bash
sudo apt update
sudo apt install nasm gcc-multilib build-essential qemu-system-x86
```

### 2. Compilar (Build)
O sistema utiliza um script inteligente que garante o alinhamento de 4KB para o Kernel e injeta o sistema de arquivos:

```Bash
chmod +x build.sh
./build.sh
```

### 3. Executar no QEMU
```Bash
qemu-system-i386 -drive format=raw,file=build/os.img
```
***Estrutura do Projeto***

boot/boot.asm: Inicialização, GDT e carregamento de setores do disco.

kernel/kernel.c: Core do SO, drivers e lógica de busca por tags ASCII.

init.ok: Nosso "disco rígido" virtual contendo as strings e dados do sistema.

linker.ld: Organiza o binário para que o código comece exatamente em 0x1000.

build.sh: Automação do pipeline (GCC -> NASM -> LD -> TRUNCATE -> CAT).

🛡️ Licença

Projeto para fins educacionais. Sinta-se à vontade para explorar e modificar!

Feito por Tiago LLopes

Santos/SP - Brasil
