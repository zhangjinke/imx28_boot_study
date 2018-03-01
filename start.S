/**
 * \file
 * \brief 启动文件
 *
 * \internal
 * \par modification history
 * - 1.00 18-02-27  pea, first implementation
 * \endinternal
 */

    .global reset
reset:

    /* 设置栈指针 */
    ldr sp, =0x1fffc

    bl main

    /* 设置 CPU 为 SVC32 模式，并关闭 FIQ 和 IRQ */
    mrs r0, cpsr
    bic r0, r0, #0x1f
    orr r0, r0, #0xd3
    msr cpsr, r0

    /* 关闭 D cache 前先 flush */
    mov r0, #0
flush_dcache:
    mrc p15, 0, r15, c7, c10, 3
    bne flush_dcache

    mcr p15, 0, r0, c8, c7, 0 /* 失能 TLB */
    mcr p15, 0, r0, c7, c5, 0 /* 失能 I Cache */

    /* 关闭 MMU 和 D cache */
    mrc p15, 0, r0, c1, c0, 0
    bic r0, r0, #0x00000300 /* clear bits 9:8 (---- --RS) */
    bic r0, r0, #0x00000087 /* clear bits 7, 2:0 (B--- -CAM) */
    bic r0, r0, #0x00002000 /* clear bit 13 (--V- ----) */
    orr r0, r0, #0x00000002 /* set bit 1 (A) Align */
    mcr p15, 0, r0, c1, c0, 0

halt:
    b halt

/* end of file */