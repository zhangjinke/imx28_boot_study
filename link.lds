
MEMORY
{
    OCRAM (rwx) : ORIGIN = 0x00000000, LENGTH = 128K
}

SECTIONS
{
    . = 0x00000000;
    . = ALIGN(4);

    .text : 
    {
        start.o (.text)
        *(.text) 
    } > OCRAM

    .rodata : 
    {
        *(.rodata) 
    } > OCRAM

    .data : 
    {
        *(.data) 
    } > OCRAM

    .bss : 
    {
        *(.bss) 
    } > OCRAM
}