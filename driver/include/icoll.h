/**
 * \file
 * \brief ICOLL 驱动
 *
 * \internal
 * \par Modification history
 * - 1.00 18-11-06  pea, first implementation
 * \endinternal
 */

#ifndef __ICOLL_H
#define __ICOLL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "imx28_reg.h"
#include <stdint.h>

struct imx28_icoll_regs {
    struct imx28_reg_32  vector;         /* 0x00 */
    struct normal_reg_32 levelack;       /* 0x10 */
    struct imx28_reg_32  ctrl;           /* 0x20 */
    uint32_t             reserved0[4];
    struct imx28_reg_32  vbase;          /* 0x40 */
    uint32_t             reserved1[8];
    struct normal_reg_32 stat;           /* 0x70 */
    uint32_t             reserved2[8];
    struct imx28_reg_32  raw[4];         /* 0xa0 */
    uint32_t             reserved3[16];
    struct imx28_reg_32  interrupt[128]; /* 0x120 */
    uint32_t             reserved4[512];
    struct imx28_reg_32  debug;          /* 0x1120 */
    struct imx28_reg_32  dbgread[2];     /* 0x1130 */
    struct imx28_reg_32  dbgflag;        /* 0x1150 */
    struct imx28_reg_32  dbgrequest[4];  /* 0x1160 */
    uint32_t             reserved5[16];
    struct normal_reg_32 version;        /* 0x11e0 */
};

#define ICOLL_LEVELACK_IRQLEVELACK_MASK   (0x0f << 0)
#define ICOLL_LEVELACK_IRQLEVELACK_OFFSET 0

#define ICOLL_CTRL_SFTRST  (1 << 31)
#define ICOLL_CTRL_CLKGATE (1 << 30)

#define ICOLL_STAT_VECTOR_NUMBER_MASK   (0x7f << 0)
#define ICOLL_STAT_VECTOR_NUMBER_OFFSET 0

#define ICOLL_INTERRUPT_ENFIQ           (1 << 4)
#define ICOLL_INTERRUPT_SOFTIRQ         (1 << 3)
#define ICOLL_INTERRUPT_ENABLE          (1 << 2)
#define ICOLL_INTERRUPT_PRIORITY_MASK   (3 << 0)
#define ICOLL_INTERRUPT_PRIORITY_OFFSET 0

/** \brief 中断号定义 */
#define IRQ_BATT_BROWNOUT     0
#define IRQ_VDDD_BROWNOUT     1
#define IRQ_VDDIO_BROWNOUT    2
#define IRQ_VDDA_BROWNOUT     3
#define IRQ_VDD5V_DROOP       4
#define IRQ_DCDC4P2_BROWNOUT  5
#define IRQ_VDD5V             6
#define IRQ_CAN0              8
#define IRQ_CAN1              9
#define IRQ_LRADC_TOUCH       10
#define IRQ_HSADC             13
#define IRQ_LRADC_THRESH0     14
#define IRQ_LRADC_THRESH1     15
#define IRQ_LRADC_CH0         16
#define IRQ_LRADC_CH1         17
#define IRQ_LRADC_CH2         18
#define IRQ_LRADC_CH3         19
#define IRQ_LRADC_CH4         20
#define IRQ_LRADC_CH5         21
#define IRQ_LRADC_CH6         22
#define IRQ_LRADC_CH7         23
#define IRQ_LRADC_BUTTON0     24
#define IRQ_LRADC_BUTTON1     25
#define IRQ_PERFMON           27
#define IRQ_RTC_1MSEC         28
#define IRQ_RTC_ALARM         29
#define IRQ_COMMS             31
#define IRQ_EMI_ERROR         32
#define IRQ_LCDIF             38
#define IRQ_PXP               39
#define IRQ_BCH               41
#define IRQ_GPMI              42
#define IRQ_SPDIF_ERROR       45
#define IRQ_DUART             47
#define IRQ_TIMER0            48
#define IRQ_TIMER1            49
#define IRQ_TIMER2            50
#define IRQ_TIMER3            51
#define IRQ_DCP_VMI           52
#define IRQ_DCP               53
#define IRQ_DCP_SECURE        54
#define IRQ_SAIF1             58
#define IRQ_SAIF0             59
#define IRQ_SPDIF_DMA         66
#define IRQ_I2C0_DMA          68
#define IRQ_I2C1_DMA          69
#define IRQ_AUART0_RX_DMA     70
#define IRQ_AUART0_TX_DMA     71
#define IRQ_AUART1_RX_DMA     72
#define IRQ_AUART1_TX_DMA     73
#define IRQ_AUART2_RX_DMA     74
#define IRQ_AUART2_TX_DMA     75
#define IRQ_AUART3_RX_DMA     76
#define IRQ_AUART3_TX_DMA     77
#define IRQ_AUART4_RX_DMA     78
#define IRQ_AUART4_TX_DMA     79
#define IRQ_SAIF0_DMA         80
#define IRQ_SAIF1_DMA         81
#define IRQ_SSP0_DMA          82
#define IRQ_SSP1_DMA          83
#define IRQ_SSP2_DMA          84
#define IRQ_SSP3_DMA          85
#define IRQ_LCDIF_DMA         86
#define IRQ_HSADC_DMA         87
#define IRQ_GPMI_DMA          88
#define IRQ_DIGCTL_DEBUG_TRAP 89
#define IRQ_USB1              92
#define IRQ_USB0              93
#define IRQ_USB1_WAKEUP       94
#define IRQ_USB0_WAKEUP       95
#define IRQ_SSP0_ERROR        96
#define IRQ_SSP1_ERROR        97
#define IRQ_SSP2_ERROR        98
#define IRQ_SSP3_ERROR        99
#define IRQ_ENET_SWI          100
#define IRQ_ENET_MAC0         101
#define IRQ_ENET_MAC1         102
#define IRQ_ENET_MAC0_1588    103
#define IRQ_ENET_MAC1_1588    104
#define IRQ_I2C1_ERROR        110
#define IRQ_I2C0_ERROR        111
#define IRQ_AUART0            112
#define IRQ_AUART1            113
#define IRQ_AUART2            114
#define IRQ_AUART3            115
#define IRQ_AUART4            116
#define IRQ_PINCTRL5          122
#define IRQ_PINCTRL4          123
#define IRQ_PINCTRL3          124
#define IRQ_PINCTRL2          125
#define IRQ_PINCTRL1          126
#define IRQ_PINCTRL0          127

#define MAX_IRQ_NUM IRQ_PINCTRL0

/**
 * \brief 失能 irq，并返回当前 irq 状态
 */
uint32_t irq_lock (void);

/**
 * \brief 使能 irq，且根据传入参数决定是否使能
 */
void irq_unlock (uint32_t);

/**
 * \brief 使能 irq
 */
void irq_enable (void);

/**
 * \brief 失能 irq
 */
void irq_disable (void);

/**
 * \brief 使能中断
 *
 * \brief[in] irq_num 中断号
 */
void int_enable (uint32_t irq_num);

/**
 * \brief 失能中断
 *
 * \brief[in] irq_num 中断号
 */
void int_disable (uint32_t irq_num);

/**
 * \brief 设置中断优先级
 *
 * \brief[in] irq_num  中断号
 * \brief[in] priority 优先级，0~3，编号越大，优先级越高 todo 实现中断嵌套支持
 */
void int_priority_set (uint32_t irq_num, uint8_t priority);

/**
 * \brief 连接用户中断服务函数
 *
 * \brief[in] irq_num 中断号
 * \brief[in] pfn_isr 用户中断服务函数
 * \brief[in] p_arg   用户中断服务函数参数
 */
void int_connect (uint32_t irq_num, void (*pfn_isr)(void *), void *p_arg);

/**
 * \brief 断开用户中断服务函数
 *
 * \brief[in] irq_num 中断号
 */
void int_disconnect (uint32_t irq_num);

/**
 * \brief 触发软件中断
 *
 * \brief[in] irq_num 中断号
 * \brief[in] enable  是否触发
 */
void int_soft_irq (uint32_t irq_num, uint8_t enable);

/**
 * \brief 初始化中断控制器
 */
void icoll_init (void);

#ifdef __cplusplus
}
#endif

#endif /* __ICOLL_H */

/* end of file */
