/**
 * \file
 * \brief ICOLL 驱动
 *
 * \internal
 * \par Modification history
 * - 1.00 18-11-06  pea, first implementation
 * \endinternal
 */

/*******************************************************************************
  头文件包含
*******************************************************************************/
#include "icoll.h"
#include "io.h"
#include "regs_base.h"
#include "duart.h"
#include <stdint.h>
#include <stddef.h>

/*******************************************************************************
  宏定义
*******************************************************************************/

static void (*__pfn_isr[MAX_IRQ_NUM + 1])(void *);
static uint32_t *__p_arg[MAX_IRQ_NUM + 1];

/*******************************************************************************
  外部函数定义
*******************************************************************************/

/**
 * \brief 软件复位 ICOLL
 */
static void __soft_reset (void)
{
    struct imx28_icoll_regs *p_regs = (struct imx28_icoll_regs *)IMX28_ICOLL_BASE;

    /* 清除软件复位位 */
    writel(ICOLL_CTRL_SFTRST, &p_regs->ctrl.clr);
    while (readl(&p_regs->ctrl.reg) & ICOLL_CTRL_SFTRST);

    /* 使能时钟 */
    writel(ICOLL_CTRL_CLKGATE, &p_regs->ctrl.clr);

    /* 开始复位 */
    writel(ICOLL_CTRL_SFTRST, &p_regs->ctrl.set);
    while (!(readl(&p_regs->ctrl.reg) & ICOLL_CTRL_SFTRST));

    /* 退出复位 */
    writel(ICOLL_CTRL_SFTRST, &p_regs->ctrl.clr);
    while (readl(&p_regs->ctrl.reg) & ICOLL_CTRL_SFTRST);

    /* 使能时钟 */
    writel(ICOLL_CTRL_CLKGATE, &p_regs->ctrl.clr);
    while (readl(&p_regs->ctrl.reg) & ICOLL_CTRL_CLKGATE);
}

void __irq_handler (void)
{
    struct imx28_icoll_regs *p_regs = (struct imx28_icoll_regs *)IMX28_ICOLL_BASE;
    uint8_t                  vector_number;
    uint8_t                  priority;

    /* 通知 ICOLL 已经进入中断服务函数 */
    writel(0, &p_regs->vector.reg);

    /* 读取中断号 */
    vector_number = readl(&p_regs->stat.reg) & ICOLL_STAT_VECTOR_NUMBER_MASK;

    /* 读取优先级 */
    priority = readl(&p_regs->interrupt[vector_number].reg) & ICOLL_INTERRUPT_PRIORITY_MASK;

    /* 调用用户注册的中断服务函数 */
    if (__pfn_isr[vector_number] != NULL) {
        __pfn_isr[vector_number](__p_arg[vector_number]);
    }

    /* 通知 ICOLL 退出中断服务函数 */
    writel(1 << priority, &p_regs->levelack.reg);
}

/**
 * \brief 使能中断
 */
void int_enable (uint32_t irq_num)
{
    struct imx28_icoll_regs *p_regs = (struct imx28_icoll_regs *)IMX28_ICOLL_BASE;

    writel(ICOLL_INTERRUPT_ENABLE, &p_regs->interrupt[irq_num].set);
}

/**
 * \brief 失能中断
 */
void int_disable (uint32_t irq_num)
{
    struct imx28_icoll_regs *p_regs = (struct imx28_icoll_regs *)IMX28_ICOLL_BASE;

    writel(ICOLL_INTERRUPT_ENABLE, &p_regs->interrupt[irq_num].clr);
}

/**
 * \brief 设置中断优先级
 */
void int_priority_set (uint32_t irq_num, uint8_t priority)
{
    struct imx28_icoll_regs *p_regs = (struct imx28_icoll_regs *)IMX28_ICOLL_BASE;
    uint8_t                  enable;

    /* 保存当前使能状态，并失能中断 */
    enable = readl(&p_regs->interrupt[irq_num].reg) & ICOLL_INTERRUPT_ENABLE;
    writel(ICOLL_INTERRUPT_ENABLE, &p_regs->interrupt[irq_num].clr);

    /* 设置中断优先级 */
    writel(ICOLL_INTERRUPT_PRIORITY_MASK, &p_regs->interrupt[irq_num].clr);
    writel(priority & ICOLL_INTERRUPT_PRIORITY_MASK, &p_regs->interrupt[irq_num].set);

    /* 恢复之前保存的中断使能状态 */
    writel(enable, &p_regs->interrupt[irq_num].set);
}

/**
 * \brief 连接用户中断服务函数
 */
void int_connect (uint32_t irq_num, void (*pfn_isr)(void *), void *p_arg)
{
    __pfn_isr[irq_num] = pfn_isr;
    __p_arg[irq_num] = p_arg;
}

/**
 * \brief 断开用户中断服务函数
 */
void int_disconnect (uint32_t irq_num)
{
    __pfn_isr[irq_num] = NULL;
    __p_arg[irq_num] = NULL;
}

/**
 * \brief 触发软件中断
 */
void int_soft_irq (uint32_t irq_num, uint8_t enable)
{
    struct imx28_icoll_regs *p_regs = (struct imx28_icoll_regs *)IMX28_ICOLL_BASE;

    if (enable) {
        writel(ICOLL_INTERRUPT_SOFTIRQ, &p_regs->interrupt[irq_num].set);
    } else {
        writel(ICOLL_INTERRUPT_SOFTIRQ, &p_regs->interrupt[irq_num].clr);
    }
}

/**
 * \brief 初始化中断控制器
 */
void icoll_init (void)
{
    uint32_t i;

    for (i = 0; i < MAX_IRQ_NUM + 1; i++) {
        __pfn_isr[i] = NULL;
        __p_arg[i] = NULL;
    }

    __soft_reset();

    irq_enable();
}

/* end of file */
