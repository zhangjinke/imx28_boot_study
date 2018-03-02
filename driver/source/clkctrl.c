/**
 * \file
 * \brief CLKCTRL 驱动
 *
 * \internal
 * \par Modification history
 * - 1.00 18-03-01  pea, first implementation
 * \endinternal
 */

/*******************************************************************************
  头文件包含
*******************************************************************************/
#include <stdint.h>
#include "clkctrl.h"
#include "duart.h"

/*******************************************************************************
  宏定义
*******************************************************************************/

/* \brief 定义 CLKCTRL 寄存器 */
#define HW_CLKCTRL_PLL0CTRL0  (*(volatile uint32_t *)0x80040000)
#define HW_CLKCTRL_PLL0CTRL1  (*(volatile uint32_t *)0x80040010)
#define HW_CLKCTRL_PLL1CTRL0  (*(volatile uint32_t *)0x80040020)
#define HW_CLKCTRL_PLL1CTRL1  (*(volatile uint32_t *)0x80040030)
#define HW_CLKCTRL_PLL2CTRL0  (*(volatile uint32_t *)0x80040040)
#define HW_CLKCTRL_CPU        (*(volatile uint32_t *)0x80040050)
#define HW_CLKCTRL_HBUS       (*(volatile uint32_t *)0x80040060)
#define HW_CLKCTRL_XBUS       (*(volatile uint32_t *)0x80040070)
#define HW_CLKCTRL_XTAL       (*(volatile uint32_t *)0x80040080)
#define HW_CLKCTRL_SSP0       (*(volatile uint32_t *)0x80040090)
#define HW_CLKCTRL_SSP1       (*(volatile uint32_t *)0x800400a0)
#define HW_CLKCTRL_SSP2       (*(volatile uint32_t *)0x800400b0)
#define HW_CLKCTRL_SSP3       (*(volatile uint32_t *)0x800400c0)
#define HW_CLKCTRL_GPMI       (*(volatile uint32_t *)0x800400d0)
#define HW_CLKCTRL_SPDIF      (*(volatile uint32_t *)0x800400e0)
#define HW_CLKCTRL_EMI        (*(volatile uint32_t *)0x800400f0)
#define HW_CLKCTRL_SAIF0      (*(volatile uint32_t *)0x80040100)
#define HW_CLKCTRL_SAIF1      (*(volatile uint32_t *)0x80040110)
#define HW_CLKCTRL_DIS_LCDIF  (*(volatile uint32_t *)0x80040120)
#define HW_CLKCTRL_ETM        (*(volatile uint32_t *)0x80040130)
#define HW_CLKCTRL_ENET       (*(volatile uint32_t *)0x80040140)
#define HW_CLKCTRL_HSADC      (*(volatile uint32_t *)0x80040150)
#define HW_CLKCTRL_FLEXCAN    (*(volatile uint32_t *)0x80040160)
#define HW_CLKCTRL_FRAC0      (*(volatile uint32_t *)0x800401b0)
#define HW_CLKCTRL_FRAC1      (*(volatile uint32_t *)0x800401c0)
#define HW_CLKCTRL_CLKSEQ     (*(volatile uint32_t *)0x800401d0)
#define HW_CLKCTRL_RESET      (*(volatile uint32_t *)0x800401e0)
#define HW_CLKCTRL_STATUS     (*(volatile uint32_t *)0x800401f0)
#define HW_CLKCTRL_VERSION    (*(volatile uint32_t *)0x80040200)

/*******************************************************************************
  外部函数定义
*******************************************************************************/

/**
 * \brief 打印 CLKCTRL 的寄存器值
 */
void clkctrl_reg_print (void)
{
    printf("\r\n");
    printf("HW_CLKCTRL_PLL0CTRL0 : 0x%08x\r\n", HW_CLKCTRL_PLL0CTRL0);
    printf("HW_CLKCTRL_PLL0CTRL1 : 0x%08x\r\n", HW_CLKCTRL_PLL0CTRL1);
    printf("HW_CLKCTRL_CPU       : 0x%08x\r\n", HW_CLKCTRL_CPU);
    printf("HW_CLKCTRL_HBUS      : 0x%08x\r\n", HW_CLKCTRL_HBUS);
    printf("HW_CLKCTRL_XBUS      : 0x%08x\r\n", HW_CLKCTRL_XBUS);
    printf("HW_CLKCTRL_XTAL      : 0x%08x\r\n", HW_CLKCTRL_XTAL);
    printf("HW_CLKCTRL_EMI       : 0x%08x\r\n", HW_CLKCTRL_EMI);
    printf("HW_CLKCTRL_FRAC0     : 0x%08x\r\n", HW_CLKCTRL_FRAC0);
    printf("HW_CLKCTRL_FRAC1     : 0x%08x\r\n", HW_CLKCTRL_FRAC1);
    printf("HW_CLKCTRL_CLKSEQ    : 0x%08x\r\n", HW_CLKCTRL_CLKSEQ);
    printf("HW_CLKCTRL_RESET     : 0x%08x\r\n", HW_CLKCTRL_RESET);
    printf("HW_CLKCTRL_STATUS    : 0x%08x\r\n", HW_CLKCTRL_STATUS);
}

/**
 * \brief 将系统时钟恢复为默认设置
 */
void clkctrl_deinit (void)
{
    HW_CLKCTRL_CLKSEQ    = 0x004483FF;
    HW_CLKCTRL_CPU       = 0x00010001;
    HW_CLKCTRL_XBUS      = 0x00000100;
    HW_CLKCTRL_HBUS      = 0x00000001;
    HW_CLKCTRL_FRAC0     = 0x92929292;
    HW_CLKCTRL_PLL0CTRL0 = 0x00000000;
}

/**
 * \brief 初始化系统时钟
 */
void clkctrl_init (void)
{
    uint8_t  cpufrac;
    uint32_t temp;

    clkctrl_deinit();

    /* 使能 PLL0 并等待锁定完成 */
    HW_CLKCTRL_PLL0CTRL0 |= (1 << 17);
    while (!(HW_CLKCTRL_PLL0CTRL1 & (1 << 31)));

    /* 使能并配置 ref_cpu 为 454MHz */
    cpufrac = 480 * 18 / 454; /* cpufrac = 480 * 18 / ref_cpu */
    temp = HW_CLKCTRL_FRAC0 & ~(0x3f << 0);
    HW_CLKCTRL_FRAC0 = temp | ((cpufrac << 0) & (0x3f << 0));
    HW_CLKCTRL_FRAC0 &= ~(1 << 7);

    /* 配置 CLK_H 为 CLK_P 的 1 分频(227MHz) */
    temp = HW_CLKCTRL_HBUS & ~(0x1f << 0);
    HW_CLKCTRL_HBUS = temp | (1 << 0);

    /* 配置 CLK_P 为 ref_cpu 的 2 分频(227MHz) */
    temp = HW_CLKCTRL_CPU & ~(0x3f << 0);
    HW_CLKCTRL_CPU = temp | (2 << 0);

    /* 选择 CPU 时钟源为 ref_cpu */
    HW_CLKCTRL_CLKSEQ &= ~(1 << 18);
}

/* end of file */
