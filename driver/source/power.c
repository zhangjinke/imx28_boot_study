/**
 * \file
 * \brief 电源管理驱动
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
#include "power.h"

/*******************************************************************************
  宏定义
*******************************************************************************/

/* \brief 定义 POWER 相关寄存器 */
#define HW_POWER_CTRL         (*(volatile uint32_t *)0x80044000)
#define HW_POWER_5VCTRL       (*(volatile uint32_t *)0x80044010)
#define HW_POWER_MINPWR       (*(volatile uint32_t *)0x80044020)
#define HW_POWER_CHARGE       (*(volatile uint32_t *)0x80044030)
#define HW_POWER_VDDDCTRL     (*(volatile uint32_t *)0x80044040)
#define HW_POWER_VDDACTRL     (*(volatile uint32_t *)0x80044050)
#define HW_POWER_VDDIOCTRL    (*(volatile uint32_t *)0x80044060)
#define HW_POWER_VDDMEMCTRL   (*(volatile uint32_t *)0x80044070)
#define HW_POWER_DCDC4P2      (*(volatile uint32_t *)0x80044080)
#define HW_POWER_MISC         (*(volatile uint32_t *)0x80044090)
#define HW_POWER_DCLIMITS     (*(volatile uint32_t *)0x800440a0)
#define HW_POWER_LOOPCTRL     (*(volatile uint32_t *)0x800440b0)
#define HW_POWER_STS          (*(volatile uint32_t *)0x800440c0)
#define HW_POWER_SPEED        (*(volatile uint32_t *)0x800440d0)
#define HW_POWER_BATTMONITOR  (*(volatile uint32_t *)0x800440e0)
#define HW_POWER_RESET        (*(volatile uint32_t *)0x80044100)
#define HW_POWER_DEBUG        (*(volatile uint32_t *)0x80044110)
#define HW_POWER_THERMAL      (*(volatile uint32_t *)0x80044120)
#define HW_POWER_USB1CTRL     (*(volatile uint32_t *)0x80044130)
#define HW_POWER_SPECIAL      (*(volatile uint32_t *)0x80044140)
#define HW_POWER_VERSION      (*(volatile uint32_t *)0x80044150)
#define HW_POWER_ANACLKCTRL   (*(volatile uint32_t *)0x80044160)
#define HW_POWER_REFCTRL      (*(volatile uint32_t *)0x80044170)

/*******************************************************************************
  外部函数定义
*******************************************************************************/

/**
 * \brief 初始化电源
 */
void power_init (void)
{

}

/* end of file */
