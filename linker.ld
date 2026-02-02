ENTRY(_start)
SECTIONS
{
    /* O kernel será carregado no endereço 0x1000 pelo bootloader */
    . = 0x1000;

    .text : {
        build/entry.o(.text)
        *(.text)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss)
    }
}
