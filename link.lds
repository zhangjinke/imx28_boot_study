
MEMORY
{
    OCRAM (rwx) : ORIGIN = 0x00000000, LENGTH = 128K
    DDR2  (rwx) : ORIGIN = 0x40000000, LENGTH = 64M
}

SECTIONS
{
    . = 0x00000000;

    . = ALIGN(4);
    .text_start :
    {
        _stext = .;
        start.o (.text)
    } > OCRAM

    . = ALIGN(4);
    .text :
    {
        *(.text)
    } > DDR2

    . = ALIGN(4);
    .rodata :
    {
        *(.rodata)
    } > DDR2

    . = ALIGN(4);
    .data :
    {
        *(.data)
    } > DDR2

    . = ALIGN(4);
    .bss :
    {
        _sbss = .;
        *(.bss)
        *(.COMMON)
        _ebss = .;
    } > DDR2
}