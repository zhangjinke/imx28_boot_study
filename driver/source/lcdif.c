/**
 * \file
 * \brief LCD 驱动
 *
 * \internal
 * \par Modification history
 * - 1.00 18-03-03  pea, first implementation
 * \endinternal
 */

/*******************************************************************************
  头文件包含
*******************************************************************************/
#include "lcdif.h"
#include "io.h"
#include "pinctrl.h"
#include "regs_base.h"
#include <stdint.h>

/*******************************************************************************
  宏定义
*******************************************************************************/

#define __LED_PIN  PIN_ID(3, 28) /**< \brief 液晶背光引脚号 */

/*******************************************************************************
  本地函数定义
*******************************************************************************/

/**
 * \brief 初始化 IO 口
 */
static void __gpio_init (void)
{
    int32_t i;

    /* 将 __LED_PIN 选择为 GPIO */
    pinctrl_muxsel_set(__LED_PIN, 3);

    /* 使能 __LED_PIN */
    pinctrl_doe_set(__LED_PIN, 1);

    /* 初始化 LCD 引脚 */
    for (i = 0; i < 16; i++) {
        pinctrl_muxsel_set(PIN_ID(1, i), 0);
        pinctrl_doe_set(PIN_ID(1, i), 1);
    }

    for (i = 24; i < 28; i++) {
        pinctrl_muxsel_set(PIN_ID(1, i), 1);
        pinctrl_doe_set(PIN_ID(1, i), 1);
    }
}

/*******************************************************************************
  外部函数定义
*******************************************************************************/

/**
 * \brief 使能 LCD
 */
void lcd_enable (void)
{

    /* 打开背光 */
    pinctrl_dout_set(__LED_PIN, 1);
}

/**
 * \brief 失能 LCD
 */
void lcd_disable (void)
{

    /* 关闭背光 */
    pinctrl_dout_set(__LED_PIN, 0);
}

/**
 * \brief 初始化 LCD
 */
void lcdif_init (void)
{
//    struct imx28_lcdif_regs *p_regs = (struct imx28_lcdif_regs *)IMX28_LCDIF_BASE;

    __gpio_init();

}

/* end of file */
