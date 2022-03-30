OUTPUT_FORMAT("binary")

SECTIONS
{
    . = 0x7c00;

    .text :
    {
        *(.boot)
        *(.text)
    }

    .data :
    {
        *(.data)
    }

    .bss :
    {
        *(COMMON)
        *(.bss)
    }

    .rodata :
    {
        *(.rodata*)
    }

    end = .;
}
