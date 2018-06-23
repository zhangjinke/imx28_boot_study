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
#include "clkctrl.h"
#include "io.h"
#include "regs_base.h"
#include "duart.h"
#include <stdint.h>

/*******************************************************************************
  宏定义
*******************************************************************************/

/*******************************************************************************
  外部函数定义
*******************************************************************************/

/**
 * \brief 将系统时钟恢复为默认设置
 */
void clkctrl_deinit (void)
{
    struct imx28_clkctrl_regs *p_regs = (struct imx28_clkctrl_regs *)IMX28_CLKCTRL_BASE;

    writel(0x004483FF, &p_regs->clkseq.reg);
    writel(0x00010001, &p_regs->cpu.reg);
    writel(0x00000100, &p_regs->xbus.reg);
    writel(0x00000001, &p_regs->hbus.reg);
    writel(0x92929292, &p_regs->frac[0].reg);
    writel(0x00000000, &p_regs->pll0ctrl0.reg);
}

/**
 * \brief 初始化系统时钟
 */
void clkctrl_init (void)
{
    struct imx28_clkctrl_regs *p_regs = (struct imx28_clkctrl_regs *)IMX28_CLKCTRL_BASE;
    uint8_t                    cpufrac;
    uint32_t                   temp;

    clkctrl_deinit();

    /* 使能 PLL0 并等待锁定完成 */
    writel(CLKCTRL_PLL0CTRL0_POWER, &p_regs->pll0ctrl0.set);
    while (!(readl(&p_regs->pll0ctrl1.reg) & CLKCTRL_PLL0CTRL1_LOCK));

    /* 使能并配置 ref_cpu 为 454MHz */
    cpufrac = (480 * 18 / 454) & CLKCTRL_FRAC_FRAC_MASK; /* cpufrac = 480 * 18 / ref_cpu */
    temp = readl(&p_regs->frac[0].reg);
    temp &= ~CLKCTRL_FRAC_FRAC_MASK;
    temp |= cpufrac << CLKCTRL_FRAC_FRAC_OFFSET;
    temp &= ~CLKCTRL_FRAC_CLKGATE;
    writel(temp, &p_regs->frac[0].reg);

    /* 配置 CLK_H 为 CLK_P 的 1 分频(227MHz) */
    temp = readl(&p_regs->hbus.reg);
    temp &= CLKCTRL_HBUS_DIV_MASK;
    temp |= 1 << CLKCTRL_HBUS_DIV_OFFSET;
    writel(temp, &p_regs->hbus.reg);

    /* 配置 CLK_P 为 ref_cpu 的 2 分频(227MHz) */
    temp = readl(&p_regs->cpu.reg);
    temp &= ~CLKCTRL_CPU_DIV_CPU_MASK;
    temp |= 2 << CLKCTRL_CPU_DIV_CPU_OFFSET;
    writel(temp, &p_regs->cpu.reg);

    /* 选择 CPU 时钟源为 ref_cpu */
    writel(CLKCTRL_CLKSEQ_BYPASS_CPU, &p_regs->clkseq.clr);
}

/* end of file */
