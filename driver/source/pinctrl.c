/**
 * \file
 * \brief PINCTRL 驱动
 *
 * \internal
 * \par Modification history
 * - 1.00 18-06-23  pea, first implementation
 * \endinternal
 */

/*******************************************************************************
  头文件包含
*******************************************************************************/
#include "pinctrl.h"
#include "io.h"
#include "regs_base.h"
#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
  外部函数定义
*******************************************************************************/

/**
 * \brief 引脚功能配置
 */
void pinctrl_muxsel_set (uint32_t pin_id, uint8_t mux)
{
    struct imx28_pinctrl_regs *p_regs = (struct imx28_pinctrl_regs *)IMX28_PINCTRL_BASE;
    uint8_t                    bank   = PIN_BANK_GET(pin_id);
    uint8_t                    num    = PIN_NUM_GET(pin_id);
    uint8_t                    i      = bank * 2 + num / 16;
    uint32_t                   temp;

    if (6 < bank) {
        return;
    }

    num -= (num / 16 * 16);

    temp = readl(&p_regs->muxsel[i].reg);
    temp &= ~(3 << (num * 2));
    temp |= ((mux & 3) << (num * 2));
    writel(temp, &p_regs->muxsel[i].reg);
}

/**
 * \brief 引脚电平与驱动能力配置
 */
void pinctrl_drive_set (uint32_t pin_id, uint8_t  voltage, uint8_t  drive)
{
    struct imx28_pinctrl_regs *p_regs = (struct imx28_pinctrl_regs *)IMX28_PINCTRL_BASE;
    uint8_t                    bank   = PIN_BANK_GET(pin_id);
    uint8_t                    num    = PIN_NUM_GET(pin_id);
    uint8_t                    i      = bank * 4 + num / 8;
    uint32_t                   temp;

    if (4 < bank) {
        return;
    }

    num -= (num / 8 * 8);

    temp = readl(&p_regs->drive[i].reg);
    temp &= ~(7 << (num * 4));
    temp |= ((voltage & 1) << (num * 4 + 2)) | ((drive & 3) << (num * 4));
    writel(temp, &p_regs->drive[i].reg);
}

/**
 * \brief 上拉电阻配置
 */
void pinctrl_pullup_set (uint32_t pin_id, uint8_t enable)
{
    struct imx28_pinctrl_regs *p_regs = (struct imx28_pinctrl_regs *)IMX28_PINCTRL_BASE;
    uint8_t                    bank   = PIN_BANK_GET(pin_id);
    uint8_t                    num    = PIN_NUM_GET(pin_id);

    if (6 < bank) {
        return;
    }

    if (enable) {
        writel((1 << num), &p_regs->pull[bank].set);
    } else {
        writel((1 << num), &p_regs->pull[bank].clr);
    }
}

/**
 * \brief 输出电平配置
 */
void pinctrl_dout_set (uint32_t pin_id, uint8_t level)
{
    struct imx28_pinctrl_regs *p_regs = (struct imx28_pinctrl_regs *)IMX28_PINCTRL_BASE;
    uint8_t                    bank   = PIN_BANK_GET(pin_id);
    uint8_t                    num    = PIN_NUM_GET(pin_id);

    if (4 < bank) {
        return;
    }

    if (level) {
        writel((1 << num), &p_regs->dout[bank].set);
    } else {
        writel((1 << num), &p_regs->dout[bank].clr);
    }
}

/**
 * \brief 输入电平获取
 */
int8_t pinctrl_din_get (uint32_t pin_id)
{
    struct imx28_pinctrl_regs *p_regs = (struct imx28_pinctrl_regs *)IMX28_PINCTRL_BASE;
    uint8_t                    bank   = PIN_BANK_GET(pin_id);
    uint8_t                    num    = PIN_NUM_GET(pin_id);

    if (4 < bank) {
        return -1;
    }

    return (readl(&p_regs->din[bank].reg) & (1 << num)) ? 1 : 0;
}

/**
 * \brief 输出使能配置
 */
void pinctrl_doe_set (uint32_t pin_id, uint8_t enable)
{
    struct imx28_pinctrl_regs *p_regs = (struct imx28_pinctrl_regs *)IMX28_PINCTRL_BASE;
    uint8_t                    bank   = PIN_BANK_GET(pin_id);
    uint8_t                    num    = PIN_NUM_GET(pin_id);

    if (4 < bank) {
        return;
    }

    if (enable) {
        writel((1 << num), &p_regs->doe[bank].set);
    } else {
        writel((1 << num), &p_regs->doe[bank].clr);
    }
}

/* end of file */
