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
#include <stddef.h>

/*******************************************************************************
  宏定义
*******************************************************************************/

#define __CLK_LRADC2K_FREQ 2000      /** \brief clk_lradc2k 频率定义 */
#define __CLK_32K_FREQ     32000     /** \brief clk_32k 频率定义 */
#define __XTAL_FREQ        24000000  /** \brief 外部晶振频率定义 */
#define __REF_PLL_FREQ     480000000 /** \brief ref_pll 频率定义 */
#define __REF_ENET_FREQ    50000000  /** \brief ref_enet 频率定义 */
#define __RMII_CLK_IN_FREQ 50000000  /** \brief enet_rmii_clk_in 频率定义 */

/*******************************************************************************
  外部函数定义
*******************************************************************************/

/**
 * \brief 从分频值计算分频的分子和分母
 */
uint16_t __fraction_get (uint16_t bit, uint8_t bit_len, uint16_t *p_denominator)
{
    uint8_t               i;
    uint16_t              molecule = 0; /* 分子 */
    uint16_t              denominator;  /* 分母 */
    const static uint16_t temp[15] = {2, 4, 8, 16, 32, 64, 128, 256,
                                      512, 1024, 2048, 4096, 8192, 16384, 32768};

    denominator = temp[bit_len - 1];

    for (i = 0; i < bit_len; i++) {
        if (bit & (1 < i)) {
            molecule += denominator / temp[bit_len - 1 - i];
        }
    }

    if (p_denominator != NULL) {
        *p_denominator = denominator;
    }

    return molecule;
}

/**
 * \brief 获取 ref_xxx 频率
 */
static uint32_t __clk_ref_get (uint8_t clk_id)
{
    struct imx28_clkctrl_regs *p_regs  = (struct imx28_clkctrl_regs *)IMX28_CLKCTRL_BASE;
    uint8_t                    reg_idx = clk_id / 4;
    uint8_t                    sub_idx = clk_id % 4;
    uint8_t                    frac;

    if (clk_id > CLK_REF_XTAL) {
        return 0;
    }

    if (CLK_REF_ENET == clk_id) {

        /* 判断 PLL2 是否使能 */
        if (!(readl(&p_regs->pll2ctrl0.reg) & CLKCTRL_PLL2CTRL0_POWER) ||
            (readl(&p_regs->pll2ctrl0.reg) & CLKCTRL_PLL2CTRL0_CLKGATE)) {
            return 0;
        }

        return __REF_ENET_FREQ;
    }

    if (CLK_REF_XTAL == clk_id) {
        return __XTAL_FREQ;
    }

    /* 判断 PLL0 是否使能 */
    if (!(readl(&p_regs->pll0ctrl1.reg) & CLKCTRL_PLL0CTRL1_LOCK)) {
        return 0;
    }

    if (CLK_REF_PLL == clk_id) {
        return __REF_PLL_FREQ;
    }

    /* 判断门控时钟是否打开 */
    if (readl(&p_regs->frac[reg_idx].reg) & (1 << (7 + 8 * sub_idx))) {
        return 0;
    }

    frac = (readl(&p_regs->frac[reg_idx].reg) & (0x3f << (8 * sub_idx))) >> (8 * sub_idx);

    return (uint32_t)((uint64_t)480000000 * 18 / frac);
}

/**
 * \brief 获取 clk_emi 频率
 */
static uint32_t __clk_emi_get (void)
{
    struct imx28_clkctrl_regs *p_regs  = (struct imx28_clkctrl_regs *)IMX28_CLKCTRL_BASE;
    uint32_t                   reg_emi = readl(&p_regs->emi.reg);
    uint32_t                   freq    = 0;
    uint32_t                   sync_mode_en;
    uint32_t                   div;
    uint32_t                   bypass_emi;

    bypass_emi = readl(&p_regs->clkseq.reg) & CLKCTRL_CLKSEQ_BYPASS_CPU;

    if (bypass_emi) { /* ref_xtal */
        div = (reg_emi & CLKCTRL_EMI_DIV_XTAL_MASK) >> CLKCTRL_EMI_DIV_XTAL_OFFSET;
        freq = __clk_ref_get(CLK_REF_XTAL) / div;
    } else { /* ref_cpu 或 ref_emi */
        div = (reg_emi & CLKCTRL_EMI_DIV_EMI_MASK) >> CLKCTRL_EMI_DIV_EMI_OFFSET;
        sync_mode_en = reg_emi & CLKCTRL_EMI_SYNC_MODE_EN;
        if (sync_mode_en) { /* clk_h 和 clk_emi 同步，时钟源为 ref_cpu */
            freq = __clk_ref_get(CLK_REF_CPU) / div;
        } else { /* clk_h 和 clk_emi 异步，时钟源为 ref_emi */
            freq = __clk_ref_get(CLK_REF_EMI) / div;
        }
    }

    return freq;
}

/**
 * \brief 获取 clk_p 频率
 */
static uint32_t __clk_p_get (void)
{
    struct imx28_clkctrl_regs *p_regs  = (struct imx28_clkctrl_regs *)IMX28_CLKCTRL_BASE;
    uint16_t                   molecule;    /* 分子 */
    uint16_t                   denominator; /* 分母 */
    uint32_t                   reg_cpu = readl(&p_regs->cpu.reg);
    uint32_t                   freq    = 0;
    uint32_t                   div;
    uint32_t                   bypass_cpu;

    bypass_cpu = readl(&p_regs->clkseq.reg) & CLKCTRL_CLKSEQ_BYPASS_CPU;
    if (bypass_cpu) { /* ref_xtal */
        div = (reg_cpu & CLKCTRL_CPU_DIV_XTAL_MASK) >> CLKCTRL_CPU_DIV_XTAL_OFFSET;
        if ((reg_cpu & CLKCTRL_CPU_DIV_XTAL_FRAC_EN) && !(div & (1 << 9))) { /* 小数分频 */
            molecule = __fraction_get(div, 10, &denominator);
            freq = __clk_ref_get(CLK_REF_XTAL) * molecule / denominator;
        } else if ((reg_cpu & CLKCTRL_CPU_DIV_XTAL_FRAC_EN) && (div & (1 << 9))) { /* 门控分频 */
            molecule = __fraction_get(div, 10, &denominator);
            freq = __clk_ref_get(CLK_REF_XTAL) * molecule / denominator;
        } else { /* 整数分频 */
            freq = __clk_ref_get(CLK_REF_XTAL) / div;
        }
    } else { /* ref_cpu */
        div = (reg_cpu & CLKCTRL_CPU_DIV_CPU_MASK) >> CLKCTRL_CPU_DIV_CPU_OFFSET;
        freq = __clk_ref_get(CLK_REF_CPU) / div;
    }

    return freq;
}

/**
 * \brief 获取 clk_h 频率
 */
static uint32_t __clk_h_get (void)
{
    struct imx28_clkctrl_regs *p_regs   = (struct imx28_clkctrl_regs *)IMX28_CLKCTRL_BASE;
    uint16_t                   molecule;    /* 分子 */
    uint16_t                   denominator; /* 分母 */
    uint32_t                   reg_hbus = readl(&p_regs->hbus.reg);
    uint32_t                   freq     = 0;
    uint32_t                   sync_mode_en;
    uint32_t                   div;

    sync_mode_en = readl(&p_regs->emi.reg) & CLKCTRL_EMI_SYNC_MODE_EN;
    if (sync_mode_en) { /* clk_h 和 clk_emi 同步 */
        freq = __clk_emi_get();
    } else { /* clk_h 和 clk_emi 异步 */
        div = (reg_hbus & CLKCTRL_HBUS_DIV_MASK) >> CLKCTRL_HBUS_DIV_OFFSET;
        if ((reg_hbus & CLKCTRL_HBUS_DIV_FRAC_EN) && !(div & (1 << 4))) { /* 小数分频 */
            molecule = __fraction_get(div, 5, &denominator);
            freq = __clk_p_get() * molecule / denominator;
        } else if ((reg_hbus & CLKCTRL_HBUS_DIV_FRAC_EN) && (div & (1 << 4))) { /* 门控分频 */
            molecule = __fraction_get(div, 5, &denominator);
            freq = __clk_p_get() * molecule / denominator;
        } else { /* 整数分频 */
            freq = __clk_p_get() / div;
        }
    }

    return freq;
}

/**
 * \brief 获取 clk_etm 频率
 */
static uint32_t __clk_etm_get (void)
{
    struct imx28_clkctrl_regs *p_regs  = (struct imx28_clkctrl_regs *)IMX28_CLKCTRL_BASE;
    uint16_t                   molecule;    /* 分子 */
    uint16_t                   denominator; /* 分母 */
    uint32_t                   reg_etm = readl(&p_regs->etm.reg);
    uint32_t                   freq    = 0;
    uint32_t                   div;
    uint32_t                   bypass_etm;

    bypass_etm = readl(&p_regs->clkseq.reg) & CLKCTRL_CLKSEQ_BYPASS_ETM;
    if (bypass_etm) { /* ref_xtal */
        div = (reg_etm & CLKCTRL_ETM_DIV_MASK) >> CLKCTRL_ETM_DIV_OFFSET;
        if ((reg_etm & CLKCTRL_ETM_DIV_FRAC_EN) && !(div & (1 << 6))) { /* 小数分频 */
            molecule = __fraction_get(div, 7, &denominator);
            freq = __clk_ref_get(CLK_REF_XTAL) * molecule / denominator;
        } else if ((reg_etm & CLKCTRL_ETM_DIV_FRAC_EN) && (div & (1 << 6))) { /* 门控分频 */
            molecule = __fraction_get(div, 7, &denominator);
            freq = __clk_ref_get(CLK_REF_XTAL) * molecule / denominator;
        } else { /* 整数分频 */
            freq = __clk_ref_get(CLK_REF_XTAL) / div;
        }
    } else { /* ref_cpu */
        div = (reg_etm & CLKCTRL_ETM_DIV_MASK) >> CLKCTRL_ETM_DIV_OFFSET;
        freq = __clk_ref_get(CLK_REF_CPU) / div;
    }

    return freq;
}

/**
 * \brief 获取 clk_ssp 频率
 */
static uint32_t __clk_ssp_get (uint8_t ssp_id)
{
    struct imx28_clkctrl_regs *p_regs  = (struct imx28_clkctrl_regs *)IMX28_CLKCTRL_BASE;
    uint16_t                   molecule;    /* 分子 */
    uint16_t                   denominator; /* 分母 */
    uint32_t                   reg_ssp = readl(&p_regs->ssp[ssp_id].reg);
    uint32_t                   freq    = 0;
    uint32_t                   div;
    uint32_t                   bypass_ssp;

    bypass_ssp = readl(&p_regs->clkseq.reg) & (1 << (3 + ssp_id));
    if (bypass_ssp) { /* ref_xtal */
        div = (reg_ssp & CLKCTRL_SSP_DIV_MASK) >> CLKCTRL_SSP_DIV_OFFSET;
        if ((reg_ssp & CLKCTRL_SSP_DIV_FRAC_EN) && !(div & (1 << 8))) { /* 小数分频 */
            molecule = __fraction_get(div, 9, &denominator);
            freq = __clk_ref_get(CLK_REF_XTAL) * molecule / denominator;
        } else if ((reg_ssp & CLKCTRL_SSP_DIV_FRAC_EN) && (div & (1 << 8))) { /* 门控分频 */
            molecule = __fraction_get(div, 9, &denominator);
            freq = __clk_ref_get(CLK_REF_XTAL) * molecule / denominator;
        } else { /* 整数分频 */
            freq = __clk_ref_get(CLK_REF_XTAL) / div;
        }
    } else { /* ref_iox */
        div = (reg_ssp & CLKCTRL_SSP_DIV_MASK) >> CLKCTRL_SSP_DIV_OFFSET;

        if (ssp_id <= 1) {
            freq = __clk_ref_get(CLK_REF_IO0) / div;
        } else {
            freq = __clk_ref_get(CLK_REF_IO1) / div;
        }
    }

    return freq;
}

/**
 * \brief 获取 clk_gpmi 频率
 */
static uint32_t __clk_gpmi_get (void)
{
    struct imx28_clkctrl_regs *p_regs   = (struct imx28_clkctrl_regs *)IMX28_CLKCTRL_BASE;
    uint16_t                   molecule;    /* 分子 */
    uint16_t                   denominator; /* 分母 */
    uint32_t                   reg_gpmi = readl(&p_regs->gpmi.reg);
    uint32_t                   freq     = 0;
    uint32_t                   div;
    uint32_t                   bypass_gpmi;

    bypass_gpmi = readl(&p_regs->clkseq.reg) & CLKCTRL_CLKSEQ_BYPASS_GPMI;
    if (bypass_gpmi) { /* ref_xtal */
        div = (reg_gpmi & CLKCTRL_GPMI_DIV_MASK) >> CLKCTRL_GPMI_DIV_OFFSET;
        if ((reg_gpmi & CLKCTRL_GPMI_DIV_FRAC_EN) && !(div & (1 << 9))) { /* 小数分频 */
            molecule = __fraction_get(div, 10, &denominator);
            freq = __clk_ref_get(CLK_REF_XTAL) * molecule / denominator;
        } else if ((reg_gpmi & CLKCTRL_GPMI_DIV_FRAC_EN) && (div & (1 << 9))) { /* 门控分频 */
            molecule = __fraction_get(div, 10, &denominator);
            freq = __clk_ref_get(CLK_REF_XTAL) * molecule / denominator;
        } else { /* 整数分频 */
            freq = __clk_ref_get(CLK_REF_XTAL) / div;
        }
    } else { /* ref_gpmi */
        div = (reg_gpmi & CLKCTRL_GPMI_DIV_MASK) >> CLKCTRL_GPMI_DIV_OFFSET;
        freq = __clk_ref_get(CLK_REF_GPMI) / div;
    }

    return freq;
}

/**
 * \brief 获取 clk_saif 频率
 */
static uint32_t __clk_saif_get (uint8_t saif_id)
{
    struct imx28_clkctrl_regs *p_regs   = (struct imx28_clkctrl_regs *)IMX28_CLKCTRL_BASE;
    uint16_t                   molecule;    /* 分子 */
    uint16_t                   denominator; /* 分母 */
    uint32_t                   reg_saif = readl(&p_regs->saif[saif_id].reg);
    uint32_t                   freq     = 0;
    uint32_t                   div;
    uint32_t                   bypass_saif;

    bypass_saif = readl(&p_regs->clkseq.reg) & (1 << saif_id);
    if (bypass_saif) { /* ref_xtal */
        div = (reg_saif & CLKCTRL_SAIF_DIV_MASK) >> CLKCTRL_SAIF_DIV_OFFSET;
        if ((reg_saif & CLKCTRL_SAIF_DIV_FRAC_EN) && !(div & (1 << 8))) { /* 小数分频 */
            molecule = __fraction_get(div, 9, &denominator);
            freq = __clk_ref_get(CLK_REF_XTAL) * molecule / denominator;
        } else if ((reg_saif & CLKCTRL_SAIF_DIV_FRAC_EN) && (div & (1 << 8))) { /* 门控分频 */
            molecule = __fraction_get(div, 9, &denominator);
            freq = __clk_ref_get(CLK_REF_XTAL) * molecule / denominator;
        } else { /* 整数分频 */
            freq = __clk_ref_get(CLK_REF_XTAL) / div;
        }
    } else { /* ref_pll */
        div = (reg_saif & CLKCTRL_SAIF_DIV_MASK) >> CLKCTRL_SAIF_DIV_OFFSET;
        freq = __clk_ref_get(CLK_REF_PLL) / div;
    }

    return freq;
}

/**
 * \brief 获取 clk_lcdif 频率
 */
static uint32_t __clk_lcdif_get (void)
{
    struct imx28_clkctrl_regs *p_regs    = (struct imx28_clkctrl_regs *)IMX28_CLKCTRL_BASE;
    uint16_t                   molecule;    /* 分子 */
    uint16_t                   denominator; /* 分母 */
    uint32_t                   reg_lcdif = readl(&p_regs->lcdif.reg);
    uint32_t                   freq      = 0;
    uint32_t                   div;
    uint32_t                   bypass_lcdif;

    bypass_lcdif = readl(&p_regs->clkseq.reg) & CLKCTRL_CLKSEQ_BYPASS_LCDIF;
    if (bypass_lcdif) { /* ref_xtal */
        div = (reg_lcdif & CLKCTRL_LCDIF_DIV_MASK) >> CLKCTRL_LCDIF_DIV_OFFSET;
        if ((reg_lcdif & CLKCTRL_LCDIF_DIV_FRAC_EN) && !(div & (1 << 12))) { /* 小数分频 */
            molecule = __fraction_get(div, 13, &denominator);
            freq = __clk_ref_get(CLK_REF_XTAL) * molecule / denominator;
        } else if ((reg_lcdif & CLKCTRL_LCDIF_DIV_FRAC_EN) && (div & (1 << 12))) { /* 门控分频 */
            molecule = __fraction_get(div, 13, &denominator);
            freq = __clk_ref_get(CLK_REF_XTAL) * molecule / denominator;
        } else { /* 整数分频 */
            freq = __clk_ref_get(CLK_REF_XTAL) / div;
        }
    } else { /* ref_pix */
        div = (reg_lcdif & CLKCTRL_LCDIF_DIV_MASK) >> CLKCTRL_LCDIF_DIV_OFFSET;
        freq = __clk_ref_get(CLK_REF_PIX) / div;
    }

    return freq;
}

/**
 * \brief 获取 clk_hsadc 频率
 */
static uint32_t __clk_hsadc_get (void)
{
    struct imx28_clkctrl_regs *p_regs     = (struct imx28_clkctrl_regs *)IMX28_CLKCTRL_BASE;
    uint8_t                    div;
    uint8_t                    div_list[] = {9, 18, 36, 72};
    uint32_t                   reg_hsadc  = readl(&p_regs->hsadc.reg);
    uint32_t                   freq       = 0;

    div = (reg_hsadc & CLKCTRL_HSADC_FREQDIV_MASK) >> CLKCTRL_HSADC_FREQDIV_OFFSET;
    freq = __clk_ref_get(CLK_REF_PIX) / div_list[div];

    return freq;
}

/**
 * \brief 获取 clk_enet_time 频率
 */
static uint32_t __clk_enet_time_get (void)
{
    struct imx28_clkctrl_regs *p_regs   = (struct imx28_clkctrl_regs *)IMX28_CLKCTRL_BASE;
    uint32_t                   reg_enet = readl(&p_regs->enet.reg);
    uint32_t                   freq     = 0;
    uint32_t                   div;
    uint32_t                   time_sel;

    div = (reg_enet & CLKCTRL_ENET_DIV_TIME_MASK) >> CLKCTRL_ENET_DIV_TIME_OFFSET;
    time_sel = reg_enet & CLKCTRL_ENET_TIME_SEL_MASK;
    if (CLKCTRL_ENET_TIME_SEL_XTAL == time_sel) { /* ref_xtal */
        freq = __clk_ref_get(CLK_REF_XTAL) / div;
    } else if (CLKCTRL_ENET_TIME_SEL_PLL == time_sel) { /* ref_pll */
        freq = __clk_ref_get(CLK_REF_PLL) / div;
    } else if (CLKCTRL_ENET_TIME_SEL_RMII_CLK == time_sel) { /* enet_rmii_clk_in */
        freq = __RMII_CLK_IN_FREQ / div;
    }

    return freq;
}

/**
 * \brief 获取 clk_x 频率
 */
static uint32_t __clk_x_get (void)
{
    struct imx28_clkctrl_regs *p_regs    = (struct imx28_clkctrl_regs *)IMX28_CLKCTRL_BASE;
    uint16_t                   molecule;    /* 分子 */
    uint16_t                   denominator; /* 分母 */
    uint32_t                   reg_xbus = readl(&p_regs->xbus.reg);
    uint32_t                   freq      = 0;
    uint32_t                   div;

    /* todo 没找到时钟门控在哪使能 */

    div = (reg_xbus & CLKCTRL_XBUS_DIV_MASK) >> CLKCTRL_XBUS_DIV_OFFSET;
    if ((reg_xbus & CLKCTRL_XBUS_DIV_FRAC_EN) && !(div & (1 << 9))) { /* 小数分频 */
        molecule = __fraction_get(div, 10, &denominator);
        freq = __clk_ref_get(CLK_REF_XTAL) * molecule / denominator;
    } else if ((reg_xbus & CLKCTRL_XBUS_DIV_FRAC_EN) && (div & (1 << 9))) { /* 门控分频 */
        molecule = __fraction_get(div, 10, &denominator);
        freq = __clk_ref_get(CLK_REF_XTAL) * molecule / denominator;
    } else { /* 整数分频 */
        freq = __clk_ref_get(CLK_REF_XTAL) / div;
    }

    return freq;
}

/**
 * \brief 时钟频率获取
 *
 * \param[in] clk_id 时钟号
 *
 * \return 时钟频率
 */
uint32_t clk_get (uint8_t clk_id)
{
    struct imx28_clkctrl_regs *p_regs  = (struct imx28_clkctrl_regs *)IMX28_CLKCTRL_BASE;
    uint32_t                   clk_freq;
    uint32_t                   reg;

    switch (clk_id) {

    case CLK_REF_CPU:
    case CLK_REF_EMI:
    case CLK_REF_IO1:
    case CLK_REF_IO0:
    case CLK_REF_PIX:
    case CLK_REF_HSADC:
    case CLK_REF_GPMI:
    case CLK_REF_PLL:
    case CLK_REF_XTAL:
        clk_freq = __clk_ref_get(clk_id);
        break;

    case CLK_P:
        clk_freq = __clk_p_get();
        break;

    case CLK_H:
    case CLK_OCROM: /* todo 没找到 OCROM 的门控时钟在哪使能 */
        clk_freq = __clk_h_get();
        break;

    case CLK_H_FLEXCAN0:
    case CLK_H_FLEXCAN0_IPG:
        reg = readl(&p_regs->flexcan.reg);
        if (reg & CLKCTRL_FLEXCAN_STOP_CAN0) {
            clk_freq = 0;
        } else {
            clk_freq = __clk_h_get();
        }
        break;

    case CLK_H_FLEXCAN1:
    case CLK_H_FLEXCAN1_IPG:
        reg = readl(&p_regs->flexcan.reg);
        if (reg & CLKCTRL_FLEXCAN_STOP_CAN1) {
            clk_freq = 0;
        } else {
            clk_freq = __clk_h_get();
        }
        break;

    case CLK_H_ENET_SWI:
    case CLK_H_MAC0:
    case CLK_H_MAC1:
        reg = readl(&p_regs->enet.reg);
        if (reg & CLKCTRL_ENET_STATUS) {
            clk_freq = 0;
        } else {
            clk_freq = __clk_h_get();
        }
        break;

    case CLK_ETM:
        clk_freq = __clk_etm_get();
        break;

    case CLK_EMI:
        clk_freq = __clk_emi_get();
        break;

    case CLK_SSP0:
    case CLK_SSP1:
    case CLK_SSP2:
    case CLK_SSP3:
        clk_freq = __clk_ssp_get(clk_id - CLK_SSP0);
        break;

    case CLK_GPMI:
        clk_freq = __clk_gpmi_get();
        break;

    case CLK_SPDIF:
    case CLK_PCMSPDIF:
        clk_freq = 0; /* todo 完成 SPDIF 驱动之后再实现 */
        break;

    case CLK_SAIF0:
    case CLK_SAIF1:
        clk_freq = __clk_saif_get(clk_id - CLK_SAIF0);
        break;

    case CLK_LCDIF:
        clk_freq = __clk_lcdif_get();
        break;

    case CLK_HSADC:
        clk_freq = __clk_hsadc_get();
        break;

    case CLK_ENET_TIME:
        clk_freq = __clk_enet_time_get();
        break;

    case CLK_X:
        clk_freq = __clk_x_get();
        break;

    case CLK_UART:
        reg = readl(&p_regs->xtal.reg);
        if (reg & CLKCTRL_XTAL_UART_CLK_GATE) {
            clk_freq = 0;
        } else {
            clk_freq = __clk_ref_get(CLK_REF_XTAL);
        }
        break;

    case CLK_XTAL24M:
        reg = readl(&p_regs->xtal.reg);
        if (reg & CLKCTRL_XTAL_PWM_CLK24M_GATE) {
            clk_freq = 0;
        } else {
            clk_freq = __clk_ref_get(CLK_REF_XTAL);
        }
        break;

    case CLK_32K:
        reg = readl(&p_regs->xtal.reg);
        if (reg & CLKCTRL_XTAL_TIMROT_CLK32K_GATE) {
            clk_freq = 0;
        } else {
            clk_freq = __CLK_32K_FREQ;
        }
        break;

    case CLK_FLEXCAN0_NOGATE:
    case CLK_FLEXCAN1_NOGATE:
        clk_freq = __clk_ref_get(CLK_REF_XTAL);
        break;

    case CLK_FLEXCAN0:
    case CLK_FLEXCAN1:
        clk_freq = 0; /* todo 完成 FLEXCAN 驱动之后再实现 */
        break;

    case CLK_LRADC2K:
        clk_freq = __CLK_LRADC2K_FREQ;
        break;

    default:
        clk_freq = 0;
    }

    return clk_freq;
}

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

//    clkctrl_deinit();

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

    /* 配置 CLK_P(内核时钟) 为 ref_cpu 的 2 分频(227MHz) */
    temp = readl(&p_regs->cpu.reg);
    temp &= ~CLKCTRL_CPU_DIV_CPU_MASK;
    temp |= 2 << CLKCTRL_CPU_DIV_CPU_OFFSET;
    writel(temp, &p_regs->cpu.reg);

    /* 配置 CLK_H(AHB/APBH 时钟) 为 CLK_P 的 1 分频(227MHz) */
    temp = readl(&p_regs->hbus.reg);
    temp &= ~CLKCTRL_HBUS_DIV_MASK;
    temp |= 1 << CLKCTRL_HBUS_DIV_OFFSET;
    writel(temp, &p_regs->hbus.reg);

    /* 选择 CPU 时钟源为 ref_cpu */
    writel(CLKCTRL_CLKSEQ_BYPASS_CPU, &p_regs->clkseq.clr);
}

/* end of file */
