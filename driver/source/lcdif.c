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
#include "clkctrl.h"
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

/**
 * \brief 软件复位 LCDIF
 */
static void __soft_reset (void)
{
    struct imx28_lcdif_regs   *p_regs = (struct imx28_lcdif_regs *)IMX28_LCDIF_BASE;

    /* 清除软件复位位 */
    writel(LCDIF_CTRL_SFTRST, &p_regs->ctrl[0].clr);
    while (readl(&p_regs->ctrl[0].reg) & LCDIF_CTRL_SFTRST);

    /* 使能时钟 */
    writel(LCDIF_CTRL_CLKGATE, &p_regs->ctrl[0].clr);

    /* 开始复位 */
    writel(LCDIF_CTRL_SFTRST, &p_regs->ctrl[0].set);
    while (!(readl(&p_regs->ctrl[0].reg) & LCDIF_CTRL_SFTRST));

    /* 退出复位 */
    writel(LCDIF_CTRL_SFTRST, &p_regs->ctrl[0].clr);
    while (readl(&p_regs->ctrl[0].reg) & LCDIF_CTRL_SFTRST);

    /* 使能时钟 */
    writel(LCDIF_CTRL_CLKGATE, &p_regs->ctrl[0].clr);
    while (readl(&p_regs->ctrl[0].reg) & LCDIF_CTRL_CLKGATE);
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
    struct imx28_lcdif_regs   *p_regs         = (struct imx28_lcdif_regs *)IMX28_LCDIF_BASE;
    struct imx28_clkctrl_regs *p_clkctrl_regs = (struct imx28_clkctrl_regs *)IMX28_CLKCTRL_BASE;
    uint32_t                   temp;
    uint8_t                    pixfrac;

    __gpio_init();

    /* 使能并配置 ref_pix 为 454MHz */
    pixfrac = (480 * 18 / 454) & CLKCTRL_FRAC_FRAC_MASK; /* pixfrac = 480 * 18 / ref_cpu */
    temp = readl(&p_clkctrl_regs->frac[1].reg);
    temp &= ~CLKCTRL_FRAC_FRAC_MASK;
    temp |= pixfrac << CLKCTRL_FRAC_FRAC_OFFSET;
    temp &= ~CLKCTRL_FRAC_CLKGATE;
    writel(temp, &p_clkctrl_regs->frac[1].reg);

    /* 软件复位 LCDIF */
    __soft_reset();
    writel(LCDIF_CTRL_READ_WRITEB, &p_regs->ctrl[0].clr);
    writel(LCDIF_CTRL_LCDIF_MASTER, &p_regs->ctrl[0].set);
}

/* end of file */
