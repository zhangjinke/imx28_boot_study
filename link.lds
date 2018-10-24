
MEMORY
{
    OCRAM (rwx) : ORIGIN = 0x00000000, LENGTH = 128K
    DDR2  (rwx) : ORIGIN = 0x40000000, LENGTH = 64M
}

SECTIONS
{
    . = 0x00000000;

    . = ALIGN(4);
    .text :
    {
        _stext = .;
        start.o (.text)
        *(.text)
    } > OCRAM

    . = ALIGN(4);
    .rodata :
    {
        *(.rodata)
    } > OCRAM

    . = ALIGN(4);
    .data :
    {
        *(.data)
    } > OCRAM

    . = ALIGN(4);
    .bss :
    {
        _sbss = .;
        *(.bss)
        *(.COMMON)
        _ebss = .;
    } > OCRAM
}