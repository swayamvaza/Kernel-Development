ENTRY(_start)

SECTIONS {
    . = 1M;

    .text : {
        /* Force retention of the header so the bootloader finds it */
        KEEP(*(.multiboot))
        *(.text)
    }

    .rodata : { *(.rodata) }
    .data   : { *(.data) }
    .bss    : { *(.bss)  }
}