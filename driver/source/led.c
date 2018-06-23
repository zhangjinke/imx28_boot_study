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
#include "led.h"
#include "pinctrl.h"
#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
  宏定义
*******************************************************************************/

#define __LED_RUN_PIN  PIN_ID(1, 22) /**< \brief LED_RUN 引脚号 */
#define __LED_ERR_PIN  PIN_ID(1, 23) /**< \brief LED_ERR 引脚号 */

/*******************************************************************************
  外部函数定义
*******************************************************************************/

/**
 * \brief 点亮指定 LED
 */
void led_on (uint32_t id)
{
    if (LED_RUN == id) {
        pinctrl_dout_set(__LED_RUN_PIN, 0);
    } else {
        pinctrl_dout_set(__LED_ERR_PIN, 0);
    }
}

/**
 * \brief 熄灭指定 LED
 */
void led_off (uint32_t id)
{
    if (LED_RUN == id) {
        pinctrl_dout_set(__LED_RUN_PIN, 1);
    } else {
        pinctrl_dout_set(__LED_ERR_PIN, 1);
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
    pinctrl_muxsel_set(__LED_RUN_PIN, 3);
    pinctrl_muxsel_set(__LED_ERR_PIN, 3);

    /* 使能 GPIO1_22 和 GPIO1_23 */
    pinctrl_doe_set(__LED_RUN_PIN, 1);
    pinctrl_doe_set(__LED_ERR_PIN, 1);
}

/* end of file */
