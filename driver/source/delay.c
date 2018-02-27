/**
 * \file
 * \brief 延时
 *
 * \internal
 * \par Modification history
 * - 1.00 18-02-27  pea, first implementation
 * \endinternal
 */

/*******************************************************************************
  头文件包含
*******************************************************************************/
#include <stdint.h>
#include "delay.h"

/*******************************************************************************
  宏定义
*******************************************************************************/

/* \brief 定义 Microseconds Counter 寄存器 */
#define HW_DIGCTL_MICROSECONDS (*(volatile uint32_t *)0x8001c0c0)

/*******************************************************************************
  外部函数定义
*******************************************************************************/

/**
 * \brief 延时指定微秒
 */
void udelay (uint32_t us)
{
    uint32_t start, cur;
    
    start = cur = HW_DIGCTL_MICROSECONDS;

    while (cur < start + us) {
        cur = HW_DIGCTL_MICROSECONDS;
    }
}

/**
 * \brief 延时指定毫秒
 */
void mdelay (uint32_t ms)
{
    udelay(1000 * ms);
}

/* end of file */
