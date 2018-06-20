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
    uint32_t *p_src_start  = (uint32_t *)0;
    uint32_t *p_dest_start = (uint32_t *)&_stext;
    uint32_t *p_dest_end   = (uint32_t *)&_sbss;
    uint32_t *p_bss_end    = (uint32_t *)&_ebss;

    p_dest_start += 0x10000000;
    p_dest_end += 0x10000000;
    p_bss_end += 0x10000000;

    printf("src_start: 0x%08x\r\ndest_start: 0x%08x\r\ndest_end: 0x%08x\r\nbss_end: 0x%08x\r\n",
           p_src_start,
           p_dest_start,
           p_dest_end,
           p_bss_end);

    /* 重定位 .data .rodata 段 */
    while (p_dest_start < p_dest_end) {
        *p_dest_start++ = *p_src_start++;
        //printf("0x%08x = 0x%08x\r\n", p_dest_start-1, *(p_dest_start-1));
    }

    /* 清零 .bss 段 */
    while (p_dest_end < p_bss_end) {
        printf("0x%08x = 0\r\n", p_dest_start);
        *p_dest_end++ = 0x00000000;
    }
}

/* end of file */
