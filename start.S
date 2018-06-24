/**
 * \file
 * \brief 启动文件
 *

 * \internal
 * \par modification history
 * - 1.00 18-02-27  pea, first implementation
 * \endinternal
 */
.global _start

/*
 * ARM9 异常向量表
 */
_start:
    b   reset
    ldr pc, _undefined_instruction
    ldr pc, _software_interrupt
    ldr pc, _prefetch_abort
    ldr pc, _data_abort
    ldr pc, _not_used
    ldr pc, _irq
    ldr pc, _fiq

/*
 * 间接异常向量表
 */
    .globl  _undefined_instruction
    .globl  _software_interrupt
    .globl  _prefetch_abort
    .globl  _data_abort
    .globl  _not_used
    .globl  _irq
    .globl  _fiq

_undefined_instruction: .word undefined_instruction
_software_interrupt:    .word software_interrupt
_prefetch_abort:    .word prefetch_abort
_data_abort:        .word data_abort
_not_used:      .word not_used
_irq:           .word irq
_fiq:           .word fiq

    .balignl 16, 0xdeadbeef

/*
 * 中断处理函数
 */
.align  5
undefined_instruction:
software_interrupt:
prefetch_abort:
data_abort:
not_used:
irq:
fiq:
1:
    bl  1b          /* hang and never return */

.align  2
/*
 * 复位中断处理函数
 */
reset:

    /* 设置 CPU 为 SVC32 模式，并关闭 FIQ 和 IRQ */
    mrs r0, cpsr
    bic r0, r0, #0x1f
    orr r0, r0, #0xd3
    msr cpsr, r0

//    /* 关闭 D cache 前先 flush */
//    mov r0, #0
//flush_dcache:
//    mrc p15, 0, r15, c7, c10, 3
//    bne flush_dcache
//
//    mcr p15, 0, r0, c8, c7, 0 /* 失能 TLB */
//    mcr p15, 0, r0, c7, c5, 0 /* 失能 I Cache */
//
//    /* 关闭 MMU 和 D cache */
//    mrc p15, 0, r0, c1, c0, 0
//    bic r0, r0, #0x00000300 /* clear bits 9:8 (---- --RS) */
//    bic r0, r0, #0x00000087 /* clear bits 7, 2:0 (B--- -CAM) */
//    bic r0, r0, #0x00002000 /* clear bit 13 (--V- ----) */
//    orr r0, r0, #0x00000002 /* set bit 1 (A) Align */
//    mcr p15, 0, r0, c1, c0, 0

    /* 设置栈指针 */
    ldr sp, =0x1fffc

    /* 初始化 LED */
    bl led_init

    /* 点亮 LED RUN */
    ldr r0, =0
    bl led_on

    ldr r0, =50
    bl mdelay

    /* 初始化调试串口 */
    ldr r0, =115200
    bl duart_init

    /* 初始化时钟 */
    bl clkctrl_init

    /* 初始化 DDR */
    bl ddr2_init

    /* 重定位到 DDR */
    ldr r0, =0
    bl relocate

    //bl main
    ldr pc, =main

halt:
    b halt

/* end of file */