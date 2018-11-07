/**
 * \file
 * \brief 电源管理驱动
 *
 * \internal
 * \par Modification history
 * - 1.00 18-03-02  pea, first implementation
 * \endinternal
 */

/*******************************************************************************
  头文件包含
*******************************************************************************/
#include "power.h"
#include "io.h"
#include "regs_base.h"
#include <stdint.h>

/*******************************************************************************
  宏定义
*******************************************************************************/

/*******************************************************************************
  外部函数定义
*******************************************************************************/

/**
 * \brief 初始化电源
 */
void power_init (void)
{
    //HW_POWER_VDDDCTRL，HW_POWER_VDDACTRL，HW_POWER_VDDIOCTRL 和 HW_POWER_VDDMEMCTRL 寄存器中的 TRG
    //BO_OFFSET 通常设置为低于目标电压75-100 mV
    //如果电压降至掉电检测器的电平，则可选择触发FIQ。然后，CPU可以缓解问题和/或优雅地关闭系统。
    //配置完成，禁用 PWDN_5VBRNOUT、PWDN_BATTBRNOUT
    //HW_POWER_LOOPCTRL 获得最大效率和最小电源纹波：TOGGLE_DIF，EN_CM_HYST和EN_RCSCALE = 0x1。
    //另外，设置HW_POWER_BATTMONITOR_EN_BATADJ
    //检测5V去除的推荐方法需要在HW_POWER_5VCTRL中设置VBUSVALID_5VDETECT并将检测阈值VBUSVALID_TRSH编程为0x1
    //最小化线性稳压和DC-DC的相互作用，必须在HW_POWER_VDDIOCTRL，HW_POWER_VDDACTRL和HW_POWER_VDDDCTRL寄存器中设置LINREG_OFFSET = 0b2。
    //最后，在HW_POWER_5VCTRL寄存器中设置DCDC_XFER并清除PWDN_5VBRNOUT。

    struct imx28_power_regs *p_regs  = (struct imx28_power_regs *)IMX28_POWER_BASE;
    uint32_t                 temp;

    /* 设置 vddio 为 3300mV，vddio 掉电检测电压为 3150mV */
    temp = readl(&p_regs->vddioctrl.reg);
    temp &= ~(POWER_VDDIOCTRL_TRG_MASK | POWER_VDDIOCTRL_BO_OFFSET_MASK);
    temp |= (((3300 - 2800) / 50) << POWER_VDDIOCTRL_TRG_OFFSET) & POWER_VDDIOCTRL_TRG_MASK;
    temp |= (((3300 - 3150) / 50) << POWER_VDDIOCTRL_BO_OFFSET_OFFSET) & POWER_VDDIOCTRL_BO_OFFSET_MASK;
    writel(temp, &p_regs->vddioctrl.reg);

    /* 设置 vdda 为 2200mV，vdda 掉电检测电压为 2050mV */
    temp = readl(&p_regs->vddactrl.reg);
    temp &= ~(POWER_VDDACTRL_TRG_MASK | POWER_VDDACTRL_BO_OFFSET_MASK);
    temp |= (((2200 - 1500) / 25) << POWER_VDDACTRL_TRG_OFFSET) & POWER_VDDACTRL_TRG_MASK;
    temp |= (((2200 - 2050) / 25) << POWER_VDDACTRL_BO_OFFSET_OFFSET) & POWER_VDDACTRL_BO_OFFSET_MASK;
    writel(temp, &p_regs->vddactrl.reg);

    /* 设置 vddmem 为 1800mV，vddmem 掉电检测电压为 1650mV */
    temp = readl(&p_regs->vddmemctrl.reg);
    temp &= ~(POWER_VDDMEMCTRL_TRG_MASK | POWER_VDDMEMCTRL_BO_OFFSET_MASK);
    temp |= (((1800 - 1100) / 25) << POWER_VDDMEMCTRL_TRG_OFFSET) & POWER_VDDMEMCTRL_TRG_MASK;
    temp |= (((1800 - 1650) / 25) << POWER_VDDMEMCTRL_BO_OFFSET_OFFSET) & POWER_VDDMEMCTRL_BO_OFFSET_MASK;
    writel(temp, &p_regs->vddmemctrl.reg);

    /* 设置 vddd 为 1550mV，vddd 掉电检测电压为 1450mV */
    temp = readl(&p_regs->vdddctrl.reg);
    temp &= ~(POWER_VDDDCTRL_TRG_MASK | POWER_VDDDCTRL_BO_OFFSET_MASK);
    temp |= (((1550 - 800) / 25) << POWER_VDDDCTRL_TRG_OFFSET) & POWER_VDDDCTRL_TRG_MASK;
    temp |= (((1550 - 1450) / 25) << POWER_VDDDCTRL_BO_OFFSET_OFFSET) & POWER_VDDDCTRL_BO_OFFSET_MASK;
    writel(temp, &p_regs->vdddctrl.reg);
}

/* end of file */
