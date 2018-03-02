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

/*******************************************************************************
  外部变量声明
*******************************************************************************/

extern uint32_t _stext;
extern uint32_t _sbss;
extern uint32_t _ebss;

/*******************************************************************************
  外部函数定义
*******************************************************************************/

/**
 * \brief 代码重定位
 */
void relocate (void)
{
    uint32_t *p_src_addr          = (uint32_t *)0;
    uint32_t *p_dest_start_addr   = (uint32_t *)&_stext;
    uint32_t *p_dest_end_addr     = (uint32_t *)&_sbss;
    uint32_t *p_runtime_bss_end_addr = (uint32_t *)&_ebss;

    /* 重定位 .data .rodata 段 */
    while (p_dest_start_addr <= p_dest_end_addr) {
        *p_dest_start_addr++ = *p_src_addr++;
    }

    /* 清零 .bss 段 */
    while (p_dest_end_addr <= p_runtime_bss_end_addr) {
        *p_dest_end_addr++ = 0x00000000;
    }
}

/* end of file */
