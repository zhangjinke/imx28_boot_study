/**
 * \file
 * \brief 代码重定位
 *
 * \internal
 * \par Modification history
 * - 1.00 18-03-02  pea, first implementation
 * \endinternal
 */

/*******************************************************************************
  头文件包含
*******************************************************************************/
#include <stdint.h>
#include "relocate.h"
#include "io.h"

/*******************************************************************************
  外部变量声明
*******************************************************************************/

extern uint32_t _stext;
extern uint32_t _sbss;
extern uint32_t _ebss;

/*******************************************************************************
  外部函数定义
*******************************************************************************/

void bss_init (void)
{
    volatile uint32_t *p_dest_end   = (uint32_t *)&_sbss;
    volatile uint32_t *p_bss_end    = (uint32_t *)&_ebss;

    /* 清零 .bss 段 */
    while (p_dest_end < p_bss_end) {
        *p_dest_end++ = 0x00000000;
    }
}

/**
 * \brief 代码重定位
 */
void relocate (volatile uint32_t *p_src_start)
{
    volatile uint32_t *p_dest_start = (uint32_t *)&_stext;
    volatile uint32_t *p_dest_end   = (uint32_t *)&_sbss;

    /* 重定位 .data .rodata 段 */
    while (p_dest_start < p_dest_end) {
        *p_dest_start++ = *p_src_start++;
    }

    bss_init();
}

/* end of file */
