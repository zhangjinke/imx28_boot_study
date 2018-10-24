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
#include "lcdif.h"
#include "io.h"
#include "pinctrl.h"
#include "clkctrl.h"
#include "regs_base.h"
#include <stdint.h>

/*******************************************************************************
  宏定义
*******************************************************************************/

#define __LED_PIN  PIN_ID(3, 28) /**< \brief 液晶背光引脚号 */

/*******************************************************************************
  本地函数定义
*******************************************************************************/

/**
 * \brief 初始化 IO 口
 */
static void __gpio_init (void)
{
    int32_t i;

    /* 将 __LED_PIN 选择为 GPIO */
    pinctrl_muxsel_set(__LED_PIN, 3);

    /* 使能 __LED_PIN */
    pinctrl_doe_set(__LED_PIN, 1);

    /* 初始化 LCD 引脚 */
    for (i = 0; i < 16; i++) {
        pinctrl_muxsel_set(PIN_ID(1, i), 0);
        pinctrl_doe_set(PIN_ID(1, i), 1);
    }

    for (i = 24; i < 28; i++) {
        pinctrl_muxsel_set(PIN_ID(1, i), 1);
        pinctrl_doe_set(PIN_ID(1, i), 1);
    }
}

/**
 * \brief 软件复位 LCDIF
 */
static void __soft_reset (void)
{
    struct imx28_lcdif_regs   *p_regs = (struct imx28_lcdif_regs *)IMX28_LCDIF_BASE;

    /* 清除软件复位位 */
    writel(LCDIF_CTRL_SFTRST, &p_regs->ctrl[0].clr);
    while (readl(&p_regs->ctrl[0].reg) & LCDIF_CTRL_SFTRST);

    /* 使能时钟 */
    writel(LCDIF_CTRL_CLKGATE, &p_regs->ctrl[0].clr);

    /* 开始复位 */
    writel(LCDIF_CTRL_SFTRST, &p_regs->ctrl[0].set);
    while (!(readl(&p_regs->ctrl[0].reg) & LCDIF_CTRL_SFTRST));

    /* 退出复位 */
    writel(LCDIF_CTRL_SFTRST, &p_regs->ctrl[0].clr);
    while (readl(&p_regs->ctrl[0].reg) & LCDIF_CTRL_SFTRST);

    /* 使能时钟 */
    writel(LCDIF_CTRL_CLKGATE, &p_regs->ctrl[0].clr);
    while (readl(&p_regs->ctrl[0].reg) & LCDIF_CTRL_CLKGATE);
}

/*******************************************************************************
  外部函数定义
*******************************************************************************/

/**
 * \brief 使能 LCD
 */
void lcd_enable (void)
{

    /* 打开背光 */
    pinctrl_dout_set(__LED_PIN, 1);
}

/**
 * \brief 失能 LCD
 */
void lcd_disable (void)
{

    /* 关闭背光 */
    pinctrl_dout_set(__LED_PIN, 0);
}

struct ctfb_res_modes {
    int xres;       /* visible resolution       */
    int yres;
    int refresh;        /* vertical refresh rate in hz  */
    /* Timing: All values in pixclocks, except pixclock (of course) */
    int pixclock;       /* pixel clock in ps (pico seconds) */
    int pixclock_khz;   /* pixel clock in kHz           */
    int left_margin;    /* time from sync to picture    */
    int right_margin;   /* time from picture to sync    */
    int upper_margin;   /* time from sync to picture    */
    int lower_margin;
    int hsync_len;      /* length of horizontal sync    */
    int vsync_len;      /* length of vertical sync  */
    int sync;       /* see FB_SYNC_*        */
    int vmode;      /* see FB_VMODE_*       */
};

#define __XRES  480
#define __YRES  272

static uint16_t buf[__YRES][__XRES];

struct ctfb_res_modes mode;

/**
 * \brief 初始化 LCD
 */
void lcdif_init (struct lcd_params *p_lcd_params)
{
    struct imx28_lcdif_regs   *p_regs         = (struct imx28_lcdif_regs *)IMX28_LCDIF_BASE;
    struct imx28_clkctrl_regs *p_clkctrl_regs = (struct imx28_clkctrl_regs *)IMX28_CLKCTRL_BASE;
    int32_t                    i;
    int32_t                    j;
    uint16_t                   k;
    uint32_t                   temp;
    uint8_t                    pixfrac;

    mode.xres = __XRES;
    mode.yres = __YRES;
    mode.left_margin = 40;
    mode.right_margin = 5;
    mode.upper_margin = 8;
    mode.lower_margin = 8;
    mode.hsync_len = 1;
    mode.vsync_len = 1;

    for (i = 0; i < __YRES; i++) {
        for (j = 0; j < __XRES; j++) {
            buf[i][j] = 0;
        }
    }

    __gpio_init();

    writel(CLKCTRL_CLKSEQ_BYPASS_DIS_LCDIF, &p_clkctrl_regs->clkseq.clr);

    /* 使能并配置 ref_pix 为 270MHz */
    pixfrac = (480 * 18 / 270) & CLKCTRL_FRAC_FRAC_MASK;
    writel(CLKCTRL_FRAC_CLKGATE, &p_clkctrl_regs->frac[1].set);
    temp = readl(&p_clkctrl_regs->frac[1].reg);
    temp &= ~CLKCTRL_FRAC_FRAC_MASK;
    temp |= pixfrac << CLKCTRL_FRAC_FRAC_OFFSET;
    writel(temp, &p_clkctrl_regs->frac[1].reg);
    writel(CLKCTRL_FRAC_CLKGATE, &p_clkctrl_regs->frac[1].clr);

    /* 配置 LCDIF 时钟为 9MHz */
    temp = readl(&p_clkctrl_regs->lcdif.reg);
    temp &= ~CLKCTRL_DIS_LCDIF_CLKGATE;
    writel(temp, &p_clkctrl_regs->lcdif.reg);
    temp &= ~CLKCTRL_DIS_LCDIF_DIV_MASK;
    temp |= 30 << CLKCTRL_DIS_LCDIF_DIV_OFFSET;
    writel(temp, &p_clkctrl_regs->lcdif.reg);
    while (readl(&p_clkctrl_regs->lcdif.reg) & CLKCTRL_DIS_LCDIF_BUSY);

    __soft_reset();                                                    /* 软件复位 LCDIF */
    writel(LCDIF_CTRL_READ_WRITEB, &p_regs->ctrl[0].clr);              /* 写入模式 */
    writel(LCDIF_CTRL_LCDIF_MASTER, &p_regs->ctrl[0].set);             /* 主机模式 */
    writel(LCDIF_CTRL_INPUT_DATA_SWIZZLE_MASK, &p_regs->ctrl[0].clr);  /* 不进行字节交换 */
    writel(LCDIF_CTRL_DATA_SHIFT_DIR, &p_regs->ctrl[0].clr);           /* 数据左移 */
    writel(LCDIF_CTRL_SHIFT_NUM_BITS_MASK, &p_regs->ctrl[0].clr);      /* 移动 0 位 */
    writel(LCDIF_CTRL_LCD_DATABUS_WIDTH_MASK, &p_regs->ctrl[0].clr);   /* LCD 为 16 位宽 */
    writel(LCDIF_CTRL_WORD_LENGTH_MASK, &p_regs->ctrl[0].clr);         /* 输入数据为 16 位宽 */

    /* 低 16 位数据有效 */
    writel(LCDIF_CTRL1_BYTE_PACKING_FORMAT_MASK, &p_regs->ctrl[1].clr);
    writel(0xf << LCDIF_CTRL1_BYTE_PACKING_FORMAT_OFFSET, &p_regs->ctrl[1].set);

    writel(LCDIF_CTRL_DOTCLK_MODE, &p_regs->ctrl[0].set);    /* DOTCLK 模式 */
    writel(LCDIF_CTRL_BYPASS_COUNT, &p_regs->ctrl[0].set);   /* 连续运行 */

    writel(LCDIF_VDCTRL0_VSYNC_OEB, &p_regs->vdctrl0.clr);
    writel(LCDIF_VDCTRL0_VSYNC_POL, &p_regs->vdctrl0.clr);
    writel(LCDIF_VDCTRL0_HSYNC_POL, &p_regs->vdctrl0.clr);
    writel(LCDIF_VDCTRL0_DOTCLK_POL, &p_regs->vdctrl0.clr);
    writel(LCDIF_VDCTRL0_ENABLE_POL, &p_regs->vdctrl0.set);
    writel(LCDIF_VDCTRL0_ENABLE_PRESENT, &p_regs->vdctrl0.set);
    writel(LCDIF_VDCTRL0_VSYNC_PERIOD_UNIT, &p_regs->vdctrl0.set);
    writel(LCDIF_VDCTRL0_VSYNC_PULSE_WIDTH_UNIT, &p_regs->vdctrl0.set);

    writel(LCDIF_VDCTRL0_VSYNC_PULSE_WIDTH_MASK, &p_regs->vdctrl0.clr);
    writel(mode.vsync_len << LCDIF_VDCTRL0_VSYNC_PULSE_WIDTH_OFFSET, &p_regs->vdctrl0.set);

    writel(mode.upper_margin + mode.lower_margin + mode.vsync_len + mode.yres, 
          &p_regs->vdctrl1.reg);

    writel((mode.hsync_len << LCDIF_VDCTRL2_HSYNC_PULSE_WIDTH_OFFSET) |
           ((mode.left_margin + mode.right_margin + mode.hsync_len + mode.xres) << LCDIF_VDCTRL2_HSYNC_PERIOD_OFFSET),
          &p_regs->vdctrl2.reg);

    writel(((mode.left_margin + mode.hsync_len) << LCDIF_VDCTRL3_HORIZONTAL_WAIT_CNT_OFFSET) |
           ((mode.upper_margin + mode.vsync_len) << LCDIF_VDCTRL3_VERTICAL_WAIT_CNT_OFFSET),
          &p_regs->vdctrl3.reg);

    writel((LCDIF_VDCTRL4_SYNC_SIGNALS_ON) |
           (mode.xres << LCDIF_VDCTRL4_DOTCLK_H_VALID_DATA_CNT_OFFSET),
          &p_regs->vdctrl4.reg);

    writel((uint32_t)buf, &p_regs->cur_buf.reg);
    writel((uint32_t)buf, &p_regs->next_buf.reg);

    writel((mode.yres << LCDIF_TRANSFER_COUNT_V_COUNT_OFFSET) |
           (mode.xres << LCDIF_TRANSFER_COUNT_H_COUNT_OFFSET),
          &p_regs->transfer_count.reg);

    writel(LCDIF_CTRL_RUN, &p_regs->ctrl[0].set);

    lcd_enable();

    mdelay(100);

    while (1) {
        for (i = 0; i < __YRES; i++) {
            for (j = 0; j < __XRES; j++) {
                if (__XRES * 1 / 8 > j) {
                    buf[i][j] = (0 << 12) | (0 << 6) | (0 << 0);
                } else if (__XRES * 2 / 8 > j) {
                    buf[i][j] = (0 << 12) | (0 << 6) | (255 << 0);
                } else if (__XRES * 3 / 8 > j) {
                    buf[i][j] = (0 << 12) | (255 << 6) | (0 << 0);
                } else if (__XRES * 4 / 8 > j) {
                    buf[i][j] = (0 << 12) | (255 << 6) | (255 << 0);
                } else if (__XRES * 5 / 8 > j) {
                    buf[i][j] = (255 << 12) | (0 << 6) | (0 << 0);
                } else if (__XRES * 6 / 8 > j) {
                    buf[i][j] = (255 << 12) | (0 << 6) | (255 << 0);
                } else if (__XRES * 7 / 8 > j) {
                    buf[i][j] = (255 << 12) | (255 << 6) | (0 << 0);
                } else if (__XRES * 8 / 8 > j) {
                    buf[i][j] = (255 << 12) | (255 << 6) | (255 << 0);
                }

                //if (((0 == j) && (0 == i)) ||
                //    ((479 == j) && (0 == i)) ||
                //    ((479 == j) && (271 == i)) ||
                //    ((0 == j) && (271 == i))) {
                //    buf[i][j] = ~buf[i][j];
                //    printf("x:%d\ty:%d\r\n", j, i);
                //}

                udelay(10);
            }
        }
        mdelay(1000);

        for (i = 0; i < __YRES; i++) {
            for (j = 0; j < __XRES; j++) {
                buf[i][j] = 0;
                udelay(10);
            }
        }
        mdelay(1000);
    }


//    k = 0;
//    while (1) {
//        mdelay(100);
//        for (i = 0; i < __YRES; i++) {
//            for (j = 0; j < __XRES; j++) {
//                buf[i][j] = k;
//            }
//        }
//        k += 1;
//    }
}

/* end of file */
