/**
 * \file
 * \brief CLKCTRL 驱动
 *
 * \internal
 * \par Modification history
 * - 1.00 18-03-01  pea, first implementation
 * \endinternal
 */

#ifndef __CLKCTRL_H
#define __CLKCTRL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "imx28_reg.h"
#include <stdint.h>

struct imx28_clkctrl_regs {
    struct imx28_reg_32  pll0ctrl0;      /* 0x00 */
    struct normal_reg_32 pll0ctrl1;      /* 0x10 */
    struct imx28_reg_32  pll1ctrl0;      /* 0x20 */
    struct normal_reg_32 pll1ctrl1;      /* 0x30 */
    struct imx28_reg_32  pll2ctrl0;      /* 0x40 */
    struct imx28_reg_32  cpu;            /* 0x50 */
    struct imx28_reg_32  hbus;           /* 0x60 */
    struct normal_reg_32 xbus;           /* 0x70 */
    struct imx28_reg_32  xtal;           /* 0x80 */
    struct normal_reg_32 ssp[4];         /* 0x90 */
    struct normal_reg_32 gpmi;           /* 0xd0 */
    struct normal_reg_32 spdif;          /* 0xe0 */
    struct normal_reg_32 emi;            /* 0xf0 */
    struct normal_reg_32 saif[2];        /* 0x100 */
    struct normal_reg_32 lcdif;          /* 0x120 */
    struct normal_reg_32 etm;            /* 0x130 */
    struct normal_reg_32 enet;           /* 0x140 */
    struct normal_reg_32 hsadc;          /* 0x150 */
    struct normal_reg_32 flexcan;        /* 0x160 */
    uint32_t             reserved0[16];
    struct imx28_reg_32  frac[2];        /* 0x1b0 */
    struct imx28_reg_32  clkseq;         /* 0x1d0 */
    struct normal_reg_32 reset;          /* 0x1e0 */
    struct normal_reg_32 status;         /* 0x1f0 */
    struct normal_reg_32 version;        /* 0x200 */
};

#define CLKCTRL_PLL0CTRL0_LFR_SEL_MASK       (3 << 28)
#define CLKCTRL_PLL0CTRL0_LFR_SEL_OFFSET     28
#define CLKCTRL_PLL0CTRL0_LFR_SEL_DEFAULT    (0 << 28)
#define CLKCTRL_PLL0CTRL0_LFR_SEL_TIMES_2    (1 << 28)
#define CLKCTRL_PLL0CTRL0_LFR_SEL_TIMES_05   (2 << 28)
#define CLKCTRL_PLL0CTRL0_LFR_SEL_UNDEFINED  (3 << 28)
#define CLKCTRL_PLL0CTRL0_CP_SEL_MASK        (3 << 24)
#define CLKCTRL_PLL0CTRL0_CP_SEL_OFFSET      24
#define CLKCTRL_PLL0CTRL0_CP_SEL_DEFAULT     (0 << 24)
#define CLKCTRL_PLL0CTRL0_CP_SEL_TIMES_2     (1 << 24)
#define CLKCTRL_PLL0CTRL0_CP_SEL_TIMES_05    (2 << 24)
#define CLKCTRL_PLL0CTRL0_CP_SEL_UNDEFINED   (3 << 24)
#define CLKCTRL_PLL0CTRL0_DIV_SEL_MASK       (3 << 20)
#define CLKCTRL_PLL0CTRL0_DIV_SEL_OFFSET     20
#define CLKCTRL_PLL0CTRL0_DIV_SEL_DEFAULT    (0 << 20)
#define CLKCTRL_PLL0CTRL0_DIV_SEL_LOWER      (1 << 20)
#define CLKCTRL_PLL0CTRL0_DIV_SEL_LOWEST     (2 << 20)
#define CLKCTRL_PLL0CTRL0_DIV_SEL_UNDEFINED  (3 << 20)
#define CLKCTRL_PLL0CTRL0_EN_USB_CLKS        (1 << 18)
#define CLKCTRL_PLL0CTRL0_POWER              (1 << 17)

#define CLKCTRL_PLL0CTRL1_LOCK               (1 << 31)
#define CLKCTRL_PLL0CTRL1_FORCE_LOCK         (1 << 30)
#define CLKCTRL_PLL0CTRL1_LOCK_COUNT_MASK    0xffff
#define CLKCTRL_PLL0CTRL1_LOCK_COUNT_OFFSET  0

#define CLKCTRL_PLL1CTRL0_CLKGATEEMI         (1 << 31)
#define CLKCTRL_PLL1CTRL0_LFR_SEL_MASK       (3 << 28)
#define CLKCTRL_PLL1CTRL0_LFR_SEL_OFFSET     28
#define CLKCTRL_PLL1CTRL0_LFR_SEL_DEFAULT    (0 << 28)
#define CLKCTRL_PLL1CTRL0_LFR_SEL_TIMES_2    (1 << 28)
#define CLKCTRL_PLL1CTRL0_LFR_SEL_TIMES_05   (2 << 28)
#define CLKCTRL_PLL1CTRL0_LFR_SEL_UNDEFINED  (3 << 28)
#define CLKCTRL_PLL1CTRL0_CP_SEL_MASK        (3 << 24)
#define CLKCTRL_PLL1CTRL0_CP_SEL_OFFSET      24
#define CLKCTRL_PLL1CTRL0_CP_SEL_DEFAULT     (0 << 24)
#define CLKCTRL_PLL1CTRL0_CP_SEL_TIMES_2     (1 << 24)
#define CLKCTRL_PLL1CTRL0_CP_SEL_TIMES_05    (2 << 24)
#define CLKCTRL_PLL1CTRL0_CP_SEL_UNDEFINED   (3 << 24)
#define CLKCTRL_PLL1CTRL0_DIV_SEL_MASK       (3 << 20)
#define CLKCTRL_PLL1CTRL0_DIV_SEL_OFFSET     20
#define CLKCTRL_PLL1CTRL0_DIV_SEL_DEFAULT    (0 << 20)
#define CLKCTRL_PLL1CTRL0_DIV_SEL_LOWER      (1 << 20)
#define CLKCTRL_PLL1CTRL0_DIV_SEL_LOWEST     (2 << 20)
#define CLKCTRL_PLL1CTRL0_DIV_SEL_UNDEFINED  (3 << 20)
#define CLKCTRL_PLL1CTRL0_EN_USB_CLKS        (1 << 18)
#define CLKCTRL_PLL1CTRL0_POWER              (1 << 17)

#define CLKCTRL_PLL1CTRL1_LOCK               (1 << 31)
#define CLKCTRL_PLL1CTRL1_FORCE_LOCK         (1 << 30)
#define CLKCTRL_PLL1CTRL1_LOCK_COUNT_MASK    0xffff
#define CLKCTRL_PLL1CTRL1_LOCK_COUNT_OFFSET  0

#define CLKCTRL_PLL2CTRL0_CLKGATE          (1 << 31)
#define CLKCTRL_PLL2CTRL0_LFR_SEL_MASK     (3 << 28)
#define CLKCTRL_PLL2CTRL0_LFR_SEL_OFFSET   28
#define CLKCTRL_PLL2CTRL0_HOLD_RING_OFF_B  (1 << 26)
#define CLKCTRL_PLL2CTRL0_CP_SEL_MASK      (3 << 24)
#define CLKCTRL_PLL2CTRL0_CP_SEL_OFFSET    24
#define CLKCTRL_PLL2CTRL0_POWER            (1 << 23)

#define CLKCTRL_CPU_BUSY_REF_XTAL     (1 << 29)
#define CLKCTRL_CPU_BUSY_REF_CPU      (1 << 28)
#define CLKCTRL_CPU_DIV_XTAL_FRAC_EN  (1 << 26)
#define CLKCTRL_CPU_DIV_XTAL_MASK     (0x3ff << 16)
#define CLKCTRL_CPU_DIV_XTAL_OFFSET   16
#define CLKCTRL_CPU_INTERRUPT_WAIT    (1 << 12)
#define CLKCTRL_CPU_DIV_CPU_FRAC_EN   (1 << 10)
#define CLKCTRL_CPU_DIV_CPU_MASK      0x3f
#define CLKCTRL_CPU_DIV_CPU_OFFSET    0

#define CLKCTRL_HBUS_ASM_BUSY               (1 << 31)
#define CLKCTRL_HBUS_DCP_AS_ENABLE          (1 << 30)
#define CLKCTRL_HBUS_PXP_AS_ENABLE          (1 << 29)
#define CLKCTRL_HBUS_ASM_EMIPORT_AS_ENABLE  (1 << 27)
#define CLKCTRL_HBUS_APBHDMA_AS_ENABLE      (1 << 26)
#define CLKCTRL_HBUS_APBXDMA_AS_ENABLE      (1 << 25)
#define CLKCTRL_HBUS_TRAFFIC_JAM_AS_ENABLE  (1 << 24)
#define CLKCTRL_HBUS_TRAFFIC_AS_ENABLE      (1 << 23)
#define CLKCTRL_HBUS_CPU_DATA_AS_ENABLE     (1 << 22)
#define CLKCTRL_HBUS_CPU_INSTR_AS_ENABLE    (1 << 21)
#define CLKCTRL_HBUS_ASM_ENABLE             (1 << 20)
#define CLKCTRL_HBUS_AUTO_CLEAR_DIV_ENABLE  (1 << 19)
#define CLKCTRL_HBUS_SLOW_DIV_MASK          (7 << 16)
#define CLKCTRL_HBUS_SLOW_DIV_OFFSET        16
#define CLKCTRL_HBUS_SLOW_DIV_BY1           (0 << 16)
#define CLKCTRL_HBUS_SLOW_DIV_BY2           (1 << 16)
#define CLKCTRL_HBUS_SLOW_DIV_BY4           (2 << 16)
#define CLKCTRL_HBUS_SLOW_DIV_BY8           (3 << 16)
#define CLKCTRL_HBUS_SLOW_DIV_BY16          (4 << 16)
#define CLKCTRL_HBUS_SLOW_DIV_BY32          (5 << 16)
#define CLKCTRL_HBUS_DIV_FRAC_EN            (1 << 5)
#define CLKCTRL_HBUS_DIV_MASK               0x1f
#define CLKCTRL_HBUS_DIV_OFFSET             0

#define CLKCTRL_XBUS_BUSY                   (1 << 31)
#define CLKCTRL_XBUS_AUTO_CLEAR_DIV_ENABLE  (1 << 11)
#define CLKCTRL_XBUS_DIV_FRAC_EN            (1 << 10)
#define CLKCTRL_XBUS_DIV_MASK               0x3ff
#define CLKCTRL_XBUS_DIV_OFFSET             0

#define CLKCTRL_XTAL_UART_CLK_GATE       (1 << 31)
#define CLKCTRL_XTAL_PWM_CLK24M_GATE     (1 << 29)
#define CLKCTRL_XTAL_TIMROT_CLK32K_GATE  (1 << 26)
#define CLKCTRL_XTAL_DIV_UART_MASK       3
#define CLKCTRL_XTAL_DIV_UART_OFFSET     0

#define CLKCTRL_SSP_CLKGATE      (1 << 31)
#define CLKCTRL_SSP_BUSY         (1 << 29)
#define CLKCTRL_SSP_DIV_FRAC_EN  (1 << 9)
#define CLKCTRL_SSP_DIV_MASK     0x1ff
#define CLKCTRL_SSP_DIV_OFFSET   0

#define CLKCTRL_GPMI_CLKGATE      (1 << 31)
#define CLKCTRL_GPMI_BUSY         (1 << 29)
#define CLKCTRL_GPMI_DIV_FRAC_EN  (1 << 10)
#define CLKCTRL_GPMI_DIV_MASK     0x3ff
#define CLKCTRL_GPMI_DIV_OFFSET   0

#define CLKCTRL_SPDIF_CLKGATE  (1 << 31)

#define CLKCTRL_EMI_CLKGATE            (1 << 31)
#define CLKCTRL_EMI_SYNC_MODE_EN       (1 << 30)
#define CLKCTRL_EMI_BUSY_REF_XTAL      (1 << 29)
#define CLKCTRL_EMI_BUSY_REF_EMI       (1 << 28)
#define CLKCTRL_EMI_BUSY_REF_CPU       (1 << 27)
#define CLKCTRL_EMI_BUSY_SYNC_MODE     (1 << 26)
#define CLKCTRL_EMI_BUSY_DCC_RESYNC    (1 << 17)
#define CLKCTRL_EMI_DCC_RESYNC_ENABLE  (1 << 16)
#define CLKCTRL_EMI_DIV_XTAL_MASK      (0xf << 8)
#define CLKCTRL_EMI_DIV_XTAL_OFFSET    8
#define CLKCTRL_EMI_DIV_EMI_MASK       0x3f
#define CLKCTRL_EMI_DIV_EMI_OFFSET     0

#define CLKCTRL_SAIF0_CLKGATE      (1 << 31)
#define CLKCTRL_SAIF0_BUSY         (1 << 29)
#define CLKCTRL_SAIF0_DIV_FRAC_EN  (1 << 16)
#define CLKCTRL_SAIF0_DIV_MASK     0xffff
#define CLKCTRL_SAIF0_DIV_OFFSET   0

#define CLKCTRL_SAIF1_CLKGATE      (1 << 31)
#define CLKCTRL_SAIF1_BUSY         (1 << 29)
#define CLKCTRL_SAIF1_DIV_FRAC_EN  (1 << 16)
#define CLKCTRL_SAIF1_DIV_MASK     0xffff
#define CLKCTRL_SAIF1_DIV_OFFSET   0

#define CLKCTRL_DIS_LCDIF_CLKGATE      (1 << 31)
#define CLKCTRL_DIS_LCDIF_BUSY         (1 << 29)
#define CLKCTRL_DIS_LCDIF_DIV_FRAC_EN  (1 << 13)
#define CLKCTRL_DIS_LCDIF_DIV_MASK     0x1fff
#define CLKCTRL_DIS_LCDIF_DIV_OFFSET   0

#define CLKCTRL_ETM_CLKGATE      (1 << 31)
#define CLKCTRL_ETM_BUSY         (1 << 29)
#define CLKCTRL_ETM_DIV_FRAC_EN  (1 << 7)
#define CLKCTRL_ETM_DIV_MASK     0x7f
#define CLKCTRL_ETM_DIV_OFFSET   0

#define CLKCTRL_ENET_SLEEP               (1 << 31)
#define CLKCTRL_ENET_DISABLE             (1 << 30)
#define CLKCTRL_ENET_STATUS              (1 << 29)
#define CLKCTRL_ENET_BUSY_TIME           (1 << 27)
#define CLKCTRL_ENET_DIV_TIME_MASK       (0x3f << 21)
#define CLKCTRL_ENET_DIV_TIME_OFFSET     21
#define CLKCTRL_ENET_TIME_SEL_MASK       (3 << 19)
#define CLKCTRL_ENET_TIME_SEL_OFFSET     19
#define CLKCTRL_ENET_TIME_SEL_XTAL       (0 << 19)
#define CLKCTRL_ENET_TIME_SEL_PLL        (1 << 19)
#define CLKCTRL_ENET_TIME_SEL_RMII_CLK   (2 << 19)
#define CLKCTRL_ENET_TIME_SEL_UNDEFINED  (3 << 19)
#define CLKCTRL_ENET_CLK_OUT_EN          (1 << 18)
#define CLKCTRL_ENET_RESET_BY_SW_CHIP    (1 << 17)
#define CLKCTRL_ENET_RESET_BY_SW         (1 << 16)

#define CLKCTRL_HSADC_RESETB          (1 << 30)
#define CLKCTRL_HSADC_FREQDIV_MASK    (3 << 28)
#define CLKCTRL_HSADC_FREQDIV_OFFSET  28

#define CLKCTRL_FLEXCAN_STOP_CAN0    (1 << 30)
#define CLKCTRL_FLEXCAN_CAN0_STATUS  (1 << 29)
#define CLKCTRL_FLEXCAN_STOP_CAN1    (1 << 28)
#define CLKCTRL_FLEXCAN_CAN1_STATUS  (1 << 27)

#define CLKCTRL_FRAC_CLKGATE      (1 << 7)
#define CLKCTRL_FRAC_STABLE       (1 << 6)
#define CLKCTRL_FRAC_FRAC_MASK    0x3f
#define CLKCTRL_FRAC_FRAC_OFFSET  0
#define CLKCTRL_FRAC0_CPU         0
#define CLKCTRL_FRAC0_EMI         1
#define CLKCTRL_FRAC0_IO1         2
#define CLKCTRL_FRAC0_IO0         3
#define CLKCTRL_FRAC1_PIX         0
#define CLKCTRL_FRAC1_HSADC       1
#define CLKCTRL_FRAC1_GPMI        2

#define CLKCTRL_CLKSEQ_BYPASS_CPU               (1 << 18)
#define CLKCTRL_CLKSEQ_BYPASS_DIS_LCDIF         (1 << 14)
#define CLKCTRL_CLKSEQ_BYPASS_DIS_LCDIF_BYPASS  (1 << 14)
#define CLKCTRL_CLKSEQ_BYPASS_DIS_LCDIF_PFD     (0 << 14)
#define CLKCTRL_CLKSEQ_BYPASS_ETM               (1 << 8)
#define CLKCTRL_CLKSEQ_BYPASS_EMI               (1 << 7)
#define CLKCTRL_CLKSEQ_BYPASS_SSP3              (1 << 6)
#define CLKCTRL_CLKSEQ_BYPASS_SSP2              (1 << 5)
#define CLKCTRL_CLKSEQ_BYPASS_SSP1              (1 << 4)
#define CLKCTRL_CLKSEQ_BYPASS_SSP0              (1 << 3)
#define CLKCTRL_CLKSEQ_BYPASS_GPMI              (1 << 2)
#define CLKCTRL_CLKSEQ_BYPASS_SAIF1             (1 << 1)
#define CLKCTRL_CLKSEQ_BYPASS_SAIF0             (1 << 0)

#define CLKCTRL_RESET_WDOG_POR_DISABLE       (1 << 5)
#define CLKCTRL_RESET_EXTERNAL_RESET_ENABLE  (1 << 4)
#define CLKCTRL_RESET_THERMAL_RESET_ENABLE   (1 << 3)
#define CLKCTRL_RESET_THERMAL_RESET_DEFAULT  (1 << 2)
#define CLKCTRL_RESET_CHIP                   (1 << 1)
#define CLKCTRL_RESET_DIG                    (1 << 0)

#define CLKCTRL_STATUS_CPU_LIMIT_MASK    (3 << 30)
#define CLKCTRL_STATUS_CPU_LIMIT_OFFSET  30

#define CLKCTRL_VERSION_MAJOR_MASK    (0xff << 24)
#define CLKCTRL_VERSION_MAJOR_OFFSET  24
#define CLKCTRL_VERSION_MINOR_MASK    (0xff << 16)
#define CLKCTRL_VERSION_MINOR_OFFSET  16
#define CLKCTRL_VERSION_STEP_MASK     0xffff
#define CLKCTRL_VERSION_STEP_OFFSET   0

/**
 * \brief 将系统时钟恢复为默认设置
 *
 * \param 无
 *
 * \return 无
 */
void clkctrl_deinit (void);

/**
 * \brief 初始化系统时钟
 *
 * \param 无
 *
 * \return 无
 */
void clkctrl_init (void);

#ifdef __cplusplus
}
#endif

#endif /* __CLKCTRL_H */

/* end of file */
