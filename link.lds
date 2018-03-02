
MEMORY
{
    OCRAM (rwx) : ORIGIN = 1024, LENGTH = 128K
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
        *(.bss) 
    } > OCRAM
}