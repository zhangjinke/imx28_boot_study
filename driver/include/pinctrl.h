/**
 * \file
 * \brief PINCTRL 驱动
 *
 * \internal
 * \par Modification history
 * - 1.00 18-06-23  pea, first implementation
 * \endinternal
 */

#ifndef __PINCTRL_H
#define __PINCTRL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "imx28_reg.h"
#include <stdint.h>

/** \brief 定义 PINCTRL 寄存器结构体 */
struct imx28_pinctrl_regs {
    struct imx28_reg_32 ctrl;          /**< \brief 控制寄存器 */
    uint32_t            reserved0[60];
    struct imx28_reg_32 muxsel[14];    /**< \brief 引脚功能选择寄存器 */
    uint32_t            reserved1[72];
    struct imx28_reg_32 drive[20];     /**< \brief 引脚驱动选择寄存器 */
    uint32_t            reserved2[112];
    struct imx28_reg_32 pull[7];       /**< \brief 上拉电阻使能寄存器 */
    uint32_t            reserved3[36];
    struct imx28_reg_32 dout[5];       /**< \brief 输出状态寄存器 */
    uint32_t            reserved4[108];
    struct imx28_reg_32 din[5];        /**< \brief 输入状态寄存器 */
    uint32_t            reserved5[108];
    struct imx28_reg_32 doe[5];        /**< \brief 输出使能寄存器 */
    uint32_t            reserved6[300];
    struct imx28_reg_32 pin2irq[5];    /**< \brief 中断选择寄存器 */
    uint32_t            reserved7[44];
    struct imx28_reg_32 irqen[5];      /**< \brief 中断使能寄存器 */
    uint32_t            reserved8[44];
    struct imx28_reg_32 irqlevel[5];   /**< \brief 中断类型寄存器 */
    uint32_t            reserved9[44];
    struct imx28_reg_32 irqpol[5];     /**< \brief 中断极性寄存器 */
    uint32_t            reserved10[44];
    struct imx28_reg_32 irqstat[5];    /**< \brief 中断状态寄存器 */
    uint32_t            reserved11[380];
    struct imx28_reg_32 emi_odt_ctrl;  /**< \brief EMI 终端电阻寄存器 */
    uint32_t            reserved12[76];
    struct imx28_reg_32 emi_ds_ctrl;   /**< \brief EMI Slice 寄存器 */
};

/** \brief 引脚 ID 定义 */
#define PIN_ID(bank, num)      ((bank << 5) | (num & 0x1f))
#define PIN_BANK_GET(id)       (id >> 5)
#define PIN_NUM_GET(id)        (id & 0x1f)

#define PINCTRL_CTRL_SFTRST    (1 << 31)
#define PINCTRL_CTRL_CLKGATE   (1 << 30)
#define PINCTRL_CTRL_PRESENT4  (1 << 24)
#define PINCTRL_CTRL_PRESENT3  (1 << 23)
#define PINCTRL_CTRL_PRESENT2  (1 << 22)
#define PINCTRL_CTRL_PRESENT1  (1 << 21)
#define PINCTRL_CTRL_PRESENT0  (1 << 20)
#define PINCTRL_CTRL_IRQOUT4   (1 << 4)
#define PINCTRL_CTRL_IRQOUT3   (1 << 3)
#define PINCTRL_CTRL_IRQOUT2   (1 << 2)
#define PINCTRL_CTRL_IRQOUT1   (1 << 1)
#define PINCTRL_CTRL_IRQOUT0   (1 << 0)

/**
 * \brief 引脚功能配置
 *
 * \param[in] pin_id    引脚 ID
 * \param[in] mux       选择的功能
 *
 * \return 无
 */
void pinctrl_muxsel_set (uint32_t pin_id, uint8_t mux);

/**
 * \brief 引脚电平与驱动能力配置
 *
 * \param[in] pin_id    引脚 ID
 * \param[in] voltage   引脚电平，0: 1.8V 1: 3.3V
 * \param[in] drive     驱动能力，0: 弱 1: 中 2: 强
 *
 * \return 无
 */
void pinctrl_drive_set (uint32_t pin_id, uint8_t  voltage, uint8_t  drive);

/**
 * \brief 上拉电阻配置
 *
 * \param[in] pin_id    引脚 ID
 * \param[in] enable    是否使能上拉电阻
 *
 * \return 无
 */
void pinctrl_pullup_set (uint32_t pin_id, uint8_t enable);

/**
 * \brief 输出电平配置
 *
 * \param[in] pin_id    引脚 ID
 * \param[in] level     输出电平
 *
 * \return 无
 */
void pinctrl_dout_set (uint32_t pin_id, uint8_t level);

/**
 * \brief 输入电平获取
 *
 * \param[in] pin_id    引脚 ID
 *
 * \return 读取到的引脚电平
 */
int8_t pinctrl_din_get (uint32_t pin_id);

/**
 * \brief 输出使能配置
 *
 * \param[in] pin_id    引脚 ID
 * \param[in] enable    是否使能输出
 *
 * \return 无
 */
void pinctrl_doe_set (uint32_t pin_id, uint8_t enable);

#ifdef __cplusplus
}
#endif

#endif /* __PINCTRL_H */

/* end of file */
