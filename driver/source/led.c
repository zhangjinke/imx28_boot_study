/**
 * \file
 * \brief LED 驱动
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
#include <stdbool.h>
#include "led.h"

/*******************************************************************************
  宏定义
*******************************************************************************/

/* \brief 定义 GPIO 相关寄存器 */
#define HW_PINCTRL_MUXSEL3 (*(volatile uint32_t *)0x80018130)
#define HW_PINCTRL_DOE1    (*(volatile uint32_t *)0x80018B10)
#define HW_PINCTRL_DOUT1   (*(volatile uint32_t *)0x80018710)

/*******************************************************************************
  外部函数定义
*******************************************************************************/

/**
 * \brief 点亮指定 LED
 */
void led_on (uint32_t id)
{
    if (LED_RUN == id) {
        HW_PINCTRL_DOUT1 &= ~(1 << 22);
    } else {
        HW_PINCTRL_DOUT1 &= ~(1 << 23);
    }
}

/**
 * \brief 熄灭指定 LED
 */
void led_off (uint32_t id)
{
    if (LED_RUN == id) {
        HW_PINCTRL_DOUT1 |= (1 << 22);
    } else {
        HW_PINCTRL_DOUT1 |= (1 << 23);
    }
}

/**
 * \brief 初始化 LED
 */
void led_init (void)
{
    led_off(LED_RUN);
    led_off(LED_ERR);
    
    /* 将 GPIO1_22 和 GPIO1_23 选择为 GPIO */
    HW_PINCTRL_MUXSEL3 |= (0x03 << 12);
    HW_PINCTRL_MUXSEL3 |= (0x03 << 14);
    
    /* 使能 GPIO1_22 和 GPIO1_23 */
    HW_PINCTRL_DOE1 |= (1 << 22);
    HW_PINCTRL_DOE1 |= (1 << 23);
}

/* end of file */
