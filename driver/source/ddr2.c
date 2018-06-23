/**
 * \file
 * \brief DDR2 驱动
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
#include <stdarg.h>
#include "ddr2.h"
#include "delay.h"

/*******************************************************************************
  宏定义
*******************************************************************************/

/* \brief 定义 POWER 相关寄存器 */
#define HW_POWER_VDDACTRL    (*(volatile uint32_t *)0x80044050)

/* \brief 定义 CLKCTRL 相关寄存器 */
#define HW_CLKCTRL_EMI       (*(volatile uint32_t *)0x800400f0)
#define HW_CLKCTRL_FRAC0     (*(volatile uint32_t *)0x800401b0)
#define HW_CLKCTRL_CLKSEQ    (*(volatile uint32_t *)0x800401d0)
#define HW_CLKCTRL_EMI       (*(volatile uint32_t *)0x800400f0)

/* \brief 定义 GPIO 相关寄存器 */
#define HW_PINCTRL_MUXSEL10       (*(volatile uint32_t *)0x800181a0)
#define HW_PINCTRL_MUXSEL11       (*(volatile uint32_t *)0x800181b0)
#define HW_PINCTRL_MUXSEL12       (*(volatile uint32_t *)0x800181c0)
#define HW_PINCTRL_MUXSEL13       (*(volatile uint32_t *)0x800181d0)
#define HW_PINCTRL_CTRL           (*(volatile uint32_t *)0x80018000)
#define HW_PINCTRL_EMI_DS_CTRL    (*(volatile uint32_t *)0x80019b80)

/* \brief 定义 DDR2 相关寄存器 */
#define HW_DRAM_CTL00    (*(volatile uint32_t *)0x800e0000)


/*******************************************************************************
  外部函数定义
*******************************************************************************/

/**
 * \brief 测试 DDR2 写入速度
 */
uint32_t ddr2_write_speed_test (uint32_t addr, uint32_t lenth)
{
    uint32_t           i;
    uint32_t           start_us;
    uint32_t           end_us;
    volatile uint32_t *p_addr   = (uint32_t *)addr;

    start_us = systick_get();
    for (i = 0; i < lenth; i++) {
        *p_addr++ = i;
    }
    end_us = systick_get();

    return (uint32_t)((((uint64_t)lenth * 4) * 1000000 / (end_us - start_us)) / 1024);
}

/**
 * \brief 测试 DDR2 写入速度
 */
uint32_t ddr2_read_speed_test (uint32_t addr, uint32_t lenth)
{
    uint32_t           i;
    uint32_t           start_us;
    uint32_t           end_us;
    volatile uint32_t *p_addr   = (uint32_t *)addr;

    start_us = systick_get();
    for (i = 0; i < lenth; i++) {
        *p_addr++;
    }

    end_us = systick_get();

    return (uint32_t)((((uint64_t)lenth * 4) * 1000000 / (end_us - start_us)) / 1024);
}

/**
 * \brief 测试 DDR2
 */
int ddr2_test (uint32_t addr, uint32_t lenth)
{
    int                err = 0;
    uint32_t           i;
    volatile uint32_t *p_addr = (uint32_t *)addr;

    for (i = 0; i < lenth; i++) {
        *p_addr++ = i;
    }

    p_addr = (uint32_t *)addr;
    for (i = 0; i < lenth; i++) {
        if (*p_addr++ != i) {
            err = -1;
            break;
        }
    }

    return err;
}

/**
 * \brief 初始化 DDR2
 */
int ddr2_init (void)
{
    uint8_t            emifrac;
    uint32_t           temp;
    volatile uint32_t *HW_DRAM_CTL = (uint32_t *)0x800e0000;

    HW_CLKCTRL_EMI         = 0x80000002;   /* EMI 时钟源配置为 ref_xtal，并配置为 2 分频 */

    /* 使能并配置 ref_emi 为 392MHz */
    emifrac = 480 * 18 / 392; /* emifrac = 480 * 18 / ref_emi */
    temp = HW_CLKCTRL_FRAC0 & ~(0x3f << 8);
    HW_CLKCTRL_FRAC0 = temp | ((emifrac << 8) & (0x3f << 8));
    HW_CLKCTRL_FRAC0 &= ~(1 << 15);

    HW_CLKCTRL_CLKSEQ      &= ~(1 << 7); /* EMI 时钟源配置为 ref_emi */
    HW_POWER_VDDACTRL      = 0x0000270C;   /* 配置 VDDA 为 1.8V，配置 BO_OFFSET 为 1.575V，配置 Linear 电压比 DC-DC 低 25Mv */
    HW_PINCTRL_CTRL        &= ~0xC0000000; /* 使能 PINCTRL 的时钟，取消 PINCTRL 的复位 */
    HW_PINCTRL_EMI_DS_CTRL = 0x00032AAA;   /* 配置引脚为 DDR2 模式，驱动能力为 20Ma */
    HW_PINCTRL_MUXSEL10    = 0x00000000;   /* 使能 EMI 引脚 */
    HW_PINCTRL_MUXSEL11    = 0x00000000;   /* 使能 EMI 引脚 */
    HW_PINCTRL_MUXSEL12    = 0x00000000;   /* 使能 EMI 引脚 */
    HW_PINCTRL_MUXSEL13    = 0x00000000;   /* 使能 EMI 引脚 */
    HW_CLKCTRL_EMI         = 0x00000002;   /* EMI 时钟源配置为 ref_emi，且为 2 分频(196.35MHz) */

    HW_DRAM_CTL[0] = 0x00000000;
    HW_DRAM_CTL[1] = 0x00000000;
    HW_DRAM_CTL[2] = 0x00000000;
    HW_DRAM_CTL[3] = 0x00000000;
    HW_DRAM_CTL[4] = 0x00000000;
    HW_DRAM_CTL[5] = 0x00000000;
    HW_DRAM_CTL[6] = 0x00000000;
    HW_DRAM_CTL[7] = 0x00000000;
    HW_DRAM_CTL[8] = 0x00000000;
    HW_DRAM_CTL[9] = 0x00000000;
    HW_DRAM_CTL[10] = 0x00000000;
    HW_DRAM_CTL[11] = 0x00000000;
    HW_DRAM_CTL[12] = 0x00000000;
    HW_DRAM_CTL[13] = 0x00000000;
    HW_DRAM_CTL[14] = 0x00000000;
    HW_DRAM_CTL[15] = 0x00000000;
    HW_DRAM_CTL[16] = 0x00000000;
    HW_DRAM_CTL[17] = 0x00000100;
    HW_DRAM_CTL[18] = 0x00000000;
    HW_DRAM_CTL[19] = 0x00000000;
    HW_DRAM_CTL[20] = 0x00000000;
    HW_DRAM_CTL[21] = 0x00000000;
    HW_DRAM_CTL[22] = 0x00000000;
    HW_DRAM_CTL[23] = 0x00000000;
    HW_DRAM_CTL[24] = 0x00000000;
    HW_DRAM_CTL[25] = 0x00000000;
    HW_DRAM_CTL[26] = 0x00010101;
    HW_DRAM_CTL[27] = 0x01010101;
    HW_DRAM_CTL[28] = 0x000f0f01;
    HW_DRAM_CTL[29] = 0x0102010a;
    HW_DRAM_CTL[30] = 0x00000000;
    HW_DRAM_CTL[31] = 0x00000101;
    HW_DRAM_CTL[32] = 0x00000100;
    HW_DRAM_CTL[33] = 0x00000100;
    HW_DRAM_CTL[34] = 0x00000000;
    HW_DRAM_CTL[35] = 0x00000002;
    HW_DRAM_CTL[36] = 0x01010000;
    HW_DRAM_CTL[37] = 0x07080403;
    HW_DRAM_CTL[38] = 0x0900a003;
    HW_DRAM_CTL[39] = 0x140000c8;
    HW_DRAM_CTL[40] = 0x02013880;
    HW_DRAM_CTL[41] = 0x00020303;
    HW_DRAM_CTL[42] = 0x006d6012;
    HW_DRAM_CTL[43] = 0x03330c2a;
    HW_DRAM_CTL[44] = 0x03060304;
    HW_DRAM_CTL[45] = 0x00c80037;
    HW_DRAM_CTL[46] = 0x00000000;
    HW_DRAM_CTL[47] = 0x00000000;
    HW_DRAM_CTL[48] = 0x00012100;
    HW_DRAM_CTL[49] = 0xffff0303;
    HW_DRAM_CTL[50] = 0x00012100;
    HW_DRAM_CTL[51] = 0xffff0303;
    HW_DRAM_CTL[52] = 0x00012100;
    HW_DRAM_CTL[53] = 0xffff0303;
    HW_DRAM_CTL[54] = 0x00012100;
    HW_DRAM_CTL[55] = 0xffff0303;
    HW_DRAM_CTL[56] = 0x00000003;
    HW_DRAM_CTL[57] = 0x00000000;
    HW_DRAM_CTL[58] = 0x00000000;
    HW_DRAM_CTL[59] = 0x00000000;
    HW_DRAM_CTL[60] = 0x00000000;
    HW_DRAM_CTL[61] = 0x00000000;
    HW_DRAM_CTL[62] = 0x00000000;
    HW_DRAM_CTL[63] = 0x00000000;
    HW_DRAM_CTL[64] = 0x00000000;
    HW_DRAM_CTL[65] = 0x00000000;
    HW_DRAM_CTL[66] = 0x00000612;
    HW_DRAM_CTL[67] = 0x01000f02;
    HW_DRAM_CTL[68] = 0x06120612;
    HW_DRAM_CTL[69] = 0x00000200;
    HW_DRAM_CTL[70] = 0x00020007;
    HW_DRAM_CTL[71] = 0xf4004a27;
    HW_DRAM_CTL[72] = 0xf4004a27;
    HW_DRAM_CTL[73] = 0xf4004a27;
    HW_DRAM_CTL[74] = 0xf4004a27;
    HW_DRAM_CTL[75] = 0x07000300;
    HW_DRAM_CTL[76] = 0x07000300;
    HW_DRAM_CTL[77] = 0x07400300;
    HW_DRAM_CTL[78] = 0x07400300;
    HW_DRAM_CTL[79] = 0x00000005;
    HW_DRAM_CTL[80] = 0x00000000;
    HW_DRAM_CTL[81] = 0x00000000;
    HW_DRAM_CTL[82] = 0x01000000;
    HW_DRAM_CTL[83] = 0x01020408;
    HW_DRAM_CTL[84] = 0x08040201;
    HW_DRAM_CTL[85] = 0x000f1133;
    HW_DRAM_CTL[86] = 0x00000000;
    HW_DRAM_CTL[87] = 0x00001f04;
    HW_DRAM_CTL[88] = 0x00001f04;
    HW_DRAM_CTL[89] = 0x00001f04;
    HW_DRAM_CTL[90] = 0x00001f04;
    HW_DRAM_CTL[91] = 0x00001f04;
    HW_DRAM_CTL[92] = 0x00001f04;
    HW_DRAM_CTL[93] = 0x00001f04;
    HW_DRAM_CTL[94] = 0x00001f04;
    HW_DRAM_CTL[95] = 0x00000000;
    HW_DRAM_CTL[96] = 0x00000000;
    HW_DRAM_CTL[97] = 0x00000000;
    HW_DRAM_CTL[98] = 0x00000000;
    HW_DRAM_CTL[99] = 0x00000000;
    HW_DRAM_CTL[100] = 0x00000000;
    HW_DRAM_CTL[101] = 0x00000000;
    HW_DRAM_CTL[102] = 0x00000000;
    HW_DRAM_CTL[103] = 0x00000000;
    HW_DRAM_CTL[104] = 0x00000000;
    HW_DRAM_CTL[105] = 0x00000000;
    HW_DRAM_CTL[106] = 0x00000000;
    HW_DRAM_CTL[107] = 0x00000000;
    HW_DRAM_CTL[108] = 0x00000000;
    HW_DRAM_CTL[109] = 0x00000000;
    HW_DRAM_CTL[110] = 0x00000000;
    HW_DRAM_CTL[111] = 0x00000000;
    HW_DRAM_CTL[112] = 0x00000000;
    HW_DRAM_CTL[113] = 0x00000000;
    HW_DRAM_CTL[114] = 0x00000000;
    HW_DRAM_CTL[115] = 0x00000000;
    HW_DRAM_CTL[116] = 0x00000000;
    HW_DRAM_CTL[117] = 0x00000000;
    HW_DRAM_CTL[118] = 0x00000000;
    HW_DRAM_CTL[119] = 0x00000000;
    HW_DRAM_CTL[120] = 0x00000000;
    HW_DRAM_CTL[121] = 0x00000000;
    HW_DRAM_CTL[122] = 0x00000000;
    HW_DRAM_CTL[123] = 0x00000000;
    HW_DRAM_CTL[124] = 0x00000000;
    HW_DRAM_CTL[125] = 0x00000000;
    HW_DRAM_CTL[126] = 0x00000000;
    HW_DRAM_CTL[127] = 0x00000000;
    HW_DRAM_CTL[128] = 0x00000000;
    HW_DRAM_CTL[129] = 0x00000000;
    HW_DRAM_CTL[130] = 0x00000000;
    HW_DRAM_CTL[131] = 0x00000000;
    HW_DRAM_CTL[132] = 0x00000000;
    HW_DRAM_CTL[133] = 0x00000000;
    HW_DRAM_CTL[134] = 0x00000000;
    HW_DRAM_CTL[135] = 0x00000000;
    HW_DRAM_CTL[136] = 0x00000000;
    HW_DRAM_CTL[137] = 0x00000000;
    HW_DRAM_CTL[138] = 0x00000000;
    HW_DRAM_CTL[139] = 0x00000000;
    HW_DRAM_CTL[140] = 0x00000000;
    HW_DRAM_CTL[141] = 0x00000000;
    HW_DRAM_CTL[142] = 0x00000000;
    HW_DRAM_CTL[143] = 0x00000000;
    HW_DRAM_CTL[144] = 0x00000000;
    HW_DRAM_CTL[145] = 0x00000000;
    HW_DRAM_CTL[146] = 0x00000000;
    HW_DRAM_CTL[147] = 0x00000000;
    HW_DRAM_CTL[148] = 0x00000000;
    HW_DRAM_CTL[149] = 0x00000000;
    HW_DRAM_CTL[150] = 0x00000000;
    HW_DRAM_CTL[151] = 0x00000000;
    HW_DRAM_CTL[152] = 0x00000000;
    HW_DRAM_CTL[153] = 0x00000000;
    HW_DRAM_CTL[154] = 0x00000000;
    HW_DRAM_CTL[155] = 0x00000000;
    HW_DRAM_CTL[156] = 0x00000000;
    HW_DRAM_CTL[157] = 0x00000000;
    HW_DRAM_CTL[158] = 0x00000000;
    HW_DRAM_CTL[159] = 0x00000000;
    HW_DRAM_CTL[160] = 0x00000000;
    HW_DRAM_CTL[161] = 0x00000000;
    HW_DRAM_CTL[162] = 0x00010000;
    HW_DRAM_CTL[163] = 0x00030404;
    HW_DRAM_CTL[164] = 0x00000005;
    HW_DRAM_CTL[165] = 0x00000000;
    HW_DRAM_CTL[166] = 0x00000000;
    HW_DRAM_CTL[167] = 0x00000000;
    HW_DRAM_CTL[168] = 0x00000000;
    HW_DRAM_CTL[169] = 0x00000000;
    HW_DRAM_CTL[170] = 0x00000000;
    HW_DRAM_CTL[171] = 0x01010000;
    HW_DRAM_CTL[172] = 0x01000000;
    HW_DRAM_CTL[173] = 0x03030000;
    HW_DRAM_CTL[174] = 0x00010303;
    HW_DRAM_CTL[175] = 0x01020202;
    HW_DRAM_CTL[176] = 0x00000000;
    HW_DRAM_CTL[177] = 0x02060303;
    HW_DRAM_CTL[178] = 0x21002103;
    HW_DRAM_CTL[179] = 0x00061200;
    HW_DRAM_CTL[180] = 0x06120612;
    HW_DRAM_CTL[181] = 0x04420442;
    HW_DRAM_CTL[182] = 0x04420442;
    HW_DRAM_CTL[183] = 0x00040004;
    HW_DRAM_CTL[184] = 0x00040004;
    HW_DRAM_CTL[185] = 0x00000000;
    HW_DRAM_CTL[186] = 0x00000000;
    HW_DRAM_CTL[187] = 0x00000000;
    HW_DRAM_CTL[188] = 0x00000000;
    HW_DRAM_CTL[189] = 0xffffffff;

    HW_DRAM_CTL[16] = 0x00000001;

    mdelay(1);

    return 0;
}

/* end of file */
