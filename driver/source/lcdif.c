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
#include <stdint.h>
#include "lcdif.h"

/*******************************************************************************
  宏定义
*******************************************************************************/

/* \brief 定义 GPIO 相关寄存器 */
#define HW_PINCTRL_MUXSEL7 (*(volatile uint32_t *)0x80018170)
#define HW_PINCTRL_DOE3    (*(volatile uint32_t *)0x80018B30)
#define HW_PINCTRL_DOUT3   (*(volatile uint32_t *)0x80018730)

/* \brief 定义 LCDIF 相关寄存器 */
#define HW_LCDIF_CTRL           (*(volatile uint32_t *)0x80030000)
#define HW_LCDIF_CTRL1          (*(volatile uint32_t *)0x80030010)
#define HW_LCDIF_CTRL2          (*(volatile uint32_t *)0x80030020)
#define HW_LCDIF_TRANSFER_COUNT (*(volatile uint32_t *)0x80030030)
#define HW_LCDIF_CUR_BUF        (*(volatile uint32_t *)0x80030040)
#define HW_LCDIF_NEXT_BUF       (*(volatile uint32_t *)0x80030050)
#define HW_LCDIF_TIMING         (*(volatile uint32_t *)0x80030060)
#define HW_LCDIF_VDCTRL0        (*(volatile uint32_t *)0x80030070)
#define HW_LCDIF_VDCTRL1        (*(volatile uint32_t *)0x80030080)
#define HW_LCDIF_VDCTRL2        (*(volatile uint32_t *)0x80030090)
#define HW_LCDIF_VDCTRL3        (*(volatile uint32_t *)0x800300a0)
#define HW_LCDIF_VDCTRL4        (*(volatile uint32_t *)0x800300b0)
#define HW_LCDIF_DVICTRL0       (*(volatile uint32_t *)0x800300c0)
#define HW_LCDIF_DVICTRL1       (*(volatile uint32_t *)0x800300d0)
#define HW_LCDIF_DVICTRL2       (*(volatile uint32_t *)0x800300e0)
#define HW_LCDIF_DVICTRL3       (*(volatile uint32_t *)0x800300f0)
#define HW_LCDIF_DVICTRL4       (*(volatile uint32_t *)0x80030100)
#define HW_LCDIF_CSC_COEFF0     (*(volatile uint32_t *)0x80030110)
#define HW_LCDIF_CSC_COEFF1     (*(volatile uint32_t *)0x80030120)
#define HW_LCDIF_CSC_COEFF2     (*(volatile uint32_t *)0x80030130)
#define HW_LCDIF_CSC_COEFF3     (*(volatile uint32_t *)0x80030140)
#define HW_LCDIF_CSC_COEFF4     (*(volatile uint32_t *)0x80030150)
#define HW_LCDIF_CSC_OFFSET     (*(volatile uint32_t *)0x80030160)
#define HW_LCDIF_CSC_LIMIT      (*(volatile uint32_t *)0x80030170)
#define HW_LCDIF_DATA           (*(volatile uint32_t *)0x80030180)
#define HW_LCDIF_BM_ERROR_STAT  (*(volatile uint32_t *)0x80030190)
#define HW_LCDIF_CRC_STAT       (*(volatile uint32_t *)0x800301a0)
#define HW_LCDIF_STAT           (*(volatile uint32_t *)0x800301b0)
#define HW_LCDIF_VERSION        (*(volatile uint32_t *)0x800301c0)
#define HW_LCDIF_DEBUG0         (*(volatile uint32_t *)0x800301d0)
#define HW_LCDIF_DEBUG1         (*(volatile uint32_t *)0x800301e0)
#define HW_LCDIF_DEBUG2         (*(volatile uint32_t *)0x800301f0)

/*******************************************************************************
  外部函数定义
*******************************************************************************/

void gpio_init (void)
{

    /* 将 GPIO3_28 选择为 GPIO */
    HW_PINCTRL_MUXSEL7 |= (0x03 << 24);
    
    /* 使能 GPIO1_22 和 GPIO1_23 */
    HW_PINCTRL_DOE3 |= (1 << 28);
}

/**
 * \brief 使能 LCD
 */
void lcd_enable (void)
{
    /* 打开背光 GPIO3_28 */
    HW_PINCTRL_DOUT3 |= (1 << 28);

}

/**
 * \brief 失能 LCD
 */
void lcd_disable (void)
{
    /* 打开背光 GPIO3_28 */
    HW_PINCTRL_DOUT3 &= ~(1 << 28);

}

/**
 * \brief 初始化 LCD
 */
void lcdif_init (void)
{
    gpio_init();
}

/* end of file */
