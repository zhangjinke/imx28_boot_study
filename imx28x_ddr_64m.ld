/**
 * \file    imx28x_ram.ld
 * \copy    COPYRIGHT (C) 2014, orz
 * \brief   IMX283 linker script file
 *
 * \internal
 * \par History
 * - 140504 orz, first implementation.
 * \endinternal
 */

/*****************************************************************************
                   debug 配置内存分配图

        +---------------------------------------+
        | 0x00000000    | MMP | 0x00000000      |   向量表
        +---------------------------------------+
                       ........
        +---------------------------------------+
        | 0x30000000    |  =  | 0x30000000      |   代码段 + ctor/dtor 表
        +---------------------------------------+
        | 0x30600000    |  =  | ??????????      |   数据段
        +---------------------------------------+
        | ??????????    |  =  | ??????????      |   清零段
        +---------------------------------------+

                       ........
        +---------------------------------------+
        | ??????????    |  =  | ??????????      |   堆栈
        +---------------------------------------+
                       ........
        +---------------------------------------+
        | 0x33ffff00    |  =  | 0x33ffff00      |   ramvector
        +---------------------------------------+
                       ........
        +---------------------------------------+
        | 0x40000000    |  =  | 0x40000000      |   reserve
        +---------------------------------------+
                       ........
*****************************************************************************/

/*****************************************************************************
    链接配置
*****************************************************************************/
STACK_SIZE = 0x2000;             /*  stack size config           */
OUTPUT_FORMAT(elf32-littlearm)   /*  binary file format          */
OUTPUT_ARCH(ARM)                 /*  target select               */

DMA_SAFE_MEM_SIZE = 8M;          /*  dma-safe memory size        */

/*****************************************************************************
    内存布局定义
*****************************************************************************/
MEMORY
{
/*  ROM (rx) : ORIGIN = 0x00000000, LENGTH = 64K */ /* On-Chip RAM */
    RAM (rw) : ORIGIN = 0x40000000, LENGTH = 64M    /* DDR2 */
    IRAM (rw) : ORIGIN = 0x00000100, LENGTH = 128K  /* On-Chip RAM */
}

/*****************************************************************************
    段定义
*****************************************************************************/
SECTIONS
{
    /* 代码段  LMA == VMA == 0x30000000 */
    .text :
    {
        __sys_mem_addr_start__ = .;
        KEEP(*(.vectors))       /* 使用KEEP使.vector段不被移除 */
        aw_backtrace_text_start = .;
        *(.text)
        *(.text.*)              /*  cpp namespace function      */
        *(.romrun)              /*  rom中必须的函数             */
        aw_backtrace_text_end = .;
        *(.rodata)              /*  read-only data (constants)  */
        *(.rodata*)
        *(.glue_7)
        *(.glue_7t)
    } > RAM

    /* section information for simple shell symbols */
    .text :
    {
        . = ALIGN(4);
        __shellsym_tab_start = .;
        KEEP(*(.shellsymbol))
        __shellsym_tab_end = .;
    } > RAM

    .ARM.extab : 
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > RAM

    /* .ARM.exidx is sorted, so has to go in its own output section */
    . = ALIGN(4);
     __exidx_start = .;
     PROVIDE(__exidx_start = __exidx_start);
    .ARM.exidx :
    {
        /* __exidx_start = .; */
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        /* __exidx_end = .;   */
    } > RAM
    . = ALIGN(4);
     __exidx_end = .;
     PROVIDE(__exidx_end = __exidx_end);

    /*
     * C++ 全局对象构造与析构函数表
     * 这里放在 .text 和 .ARM.exidx 之后, .data 之前,
     * 这里的  LMA 和 VMA 相同, 如果放在 .data 之后, LMA 与 VMA 不同,
     * 则需要启动程序从装载区搬运到运行区
     */

    . = ALIGN(4);
    .ctors :
    {
        KEEP (*cppRtBegin*.o(.ctors))
        KEEP (*(.preinit_array))
        KEEP (*(.init_array))
        KEEP (*(SORT(.ctors.*)))
        KEEP (*(.ctors))
        KEEP (*cppRtEnd*.o(.ctors))
    } > RAM

    .dtors :
    {
        KEEP (*cppRtBegin*.o(.dtors))
        KEEP (*(.fini_array))
        KEEP (*(SORT(.dtors.*)))
        KEEP (*(.dtors))
        KEEP (*cppRtEnd*.o(.dtors))
    } > RAM

    /* .data 段数据初始化内容放在这里 */
    . = ALIGN(16);
    _etext = . ;
    PROVIDE (etext = .);

    /*
     * 数据段
     */
    .data _etext : AT (_etext)
    {
        . = ALIGN(4);
         _data = . ;
         *(.data)
         *(.data.*)
    } > RAM

    _edata = ADDR(.data) + SIZEOF(.data);
    PROVIDE (edata = _edata);

    /*
     * 清零段
     * NOLOAD 表示不装载, 启动程序只需要借助 __bss_start 和 __bss_end 两个符号
     * 指定的起始地址和结束地址 将 .bss 区域清零即可
     */
    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        PROVIDE (__bss_start = .);
        *(.bss)
        *(.bss.*)
        *(COMMON)
        . = ALIGN(4);
        PROVIDE (__bss_end = .);
    } > RAM

    /* 系统堆 */
    . = ALIGN(4);
    PROVIDE (__heap_start__ = .);
    .heap (NOLOAD) :
    {

    } > RAM
    . = ORIGIN(RAM) + LENGTH(RAM) - STACK_SIZE - DMA_SAFE_MEM_SIZE;
    . = ALIGN(4);
    PROVIDE (__heap_end__ = .);

    /* 栈 */
    . = ALIGN(4);
    .stack (NOLOAD) :
    {

    } > RAM
    . += STACK_SIZE;
    PROVIDE (_stack = . - 4);

    /* dma-safe 堆 */
    . = ALIGN(4);
    PROVIDE (__heap_dma_start__ = .);
    .heap_dma (NOLOAD) :
    {

    } > RAM
    . += DMA_SAFE_MEM_SIZE;
    . = ALIGN(4);
    PROVIDE (__heap_dma_end__ = .);


    /* 页表段 */
    .page_table (NOLOAD) : AT (_edata)
    {
        *(.page_table)
        *(.page_table.*)
    } > IRAM

    /* Stabs debugging sections */
    .stab               0 : {*(.stab) }
    .stabstr            0 : {*(.stabstr) }
    .stab.excl          0 : {*(.stab.excl) }
    .stab.exclstr       0 : {*(.stab.exclstr) }
    .stab.index         0 : {*(.stab.index) }
    .stab.indexstr      0 : {*(.stab.indexstr) }
    .comment            0 : {*(.comment) }

    /*
     * Symbols in the DWARF debugging sections are relative to the beginning
     * of the section so we begin them at 0.
     */

    /* DWARF 1 */
    .debug              0 : {*(.debug) }
    .line               0 : {*(.line) }

    /* GNU DWARF 1 extensions */
    .debug_srcinfo      0 : {*(.debug_srcinfo) }
    .debug_sfnames      0 : {*(.debug_sfnames) }

    /* DWARF 1.1 and DWARF 2 */
    .debug_aranges      0 : {*(.debug_aranges) }
    .debug_pubnames     0 : {*(.debug_pubnames) }

    /* DWARF 2 */
    .debug_info         0 : {*(.debug_info .gnu.linkonce.wi.*) }
    .debug_abbrev       0 : {*(.debug_abbrev) }
    .debug_line         0 : {*(.debug_line) }
    .debug_frame        0 : {*(.debug_frame) }
    .debug_str          0 : {*(.debug_str) }
    .debug_loc          0 : {*(.debug_loc) }
    .debug_macinfo      0 : {*(.debug_macinfo) }

    /* SGI/MIPS DWARF 2 extensions */
    .debug_weaknames    0 : {*(.debug_weaknames) }
    .debug_funcnames    0 : {*(.debug_funcnames) }
    .debug_typenames    0 : {*(.debug_typenames) }
    .debug_varnames     0 : {*(.debug_varnames) }
}

ASSERT((__heap_end__ > __heap_start__), "NO Memory For HEAP!");

/* end of file */
