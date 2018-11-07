/**
 * \file
 * \brief i.MX28 裸机编程学习
 *
 * \internal
 * \par Modification history
 * - 1.00 18-02-27  pea, first implementation
 * \endinternal
 */

#include <stdint.h>
#include "delay.h"
#include "led.h"
#include "duart.h"
#include "ddr2.h"
#include "clkctrl.h"
#include "lcdif.h"
#include "icoll.h"
//#include "pic.h"

///** \brief LCD 帧缓冲区 */
//static uint16_t g_lcd_fb[272][480];
//
///** \brief LCD 参数结构定义 */
//static struct lcd_params lcd_params = {
//    {
//        .vclk  = NORMAL,
//        .vden  = NORMAL,
//        .hsync = NORMAL,
//        .vsync = NORMAL,
//    },
//    {
//        .vsw = 1,
//        .vbp = 8,
//        .vfp = 8,
//        .hsw = 1,
//        .hbp = 40,
//        .hfp = 5,
//        .vclk = 9000000,
//    },
//    .xres = 480,
//    .yres = 272,
//    .bpp  = 16,
//    .p_fb = &g_lcd_fb[0][0],
//};

void delay_test (volatile uint32_t ms)
{
    ms *= 7000;
    while (ms--);
}

/**
 * \brief 主函数
 */
int main (void)
{
//    uint32_t i;
//    uint32_t j;

    /* 初始化中断控制器 */
    icoll_init();

    printf("\r\n\r\n");

    printf("clk_p: %dHz\r\n", clk_get(CLK_P));
    printf("clk_h: %dHz\r\n", clk_get(CLK_H));
    printf("clk_lcdif: %dHz\r\n", clk_get(CLK_LCDIF));
//    lcdif_init(&lcd_params);
//    printf("clk_lcdif: %dHz\r\n", clk_get(CLK_LCDIF));

//    lcd_enable();
    led_on(LED_ERR);

    printf("hello world\r\n");

    mdelay(100);

#if 0
    while (1) {
        for (i = 0; i < lcd_params.yres; i++) {
            for (j = 0; j < lcd_params.xres; j++) {
                if (lcd_params.xres * 1 / 8 > j) {
                    g_lcd_fb[i][j] = (0 << 12) | (0 << 6) | (0 << 0);
                } else if (lcd_params.xres * 2 / 8 > j) {
                    g_lcd_fb[i][j] = (0 << 12) | (0 << 6) | (255 << 0);
                } else if (lcd_params.xres * 3 / 8 > j) {
                    g_lcd_fb[i][j] = (0 << 12) | (255 << 6) | (0 << 0);
                } else if (lcd_params.xres * 4 / 8 > j) {
                    g_lcd_fb[i][j] = (0 << 12) | (255 << 6) | (255 << 0);
                } else if (lcd_params.xres * 5 / 8 > j) {
                    g_lcd_fb[i][j] = (uint16_t)(255 << 12) | (0 << 6) | (0 << 0);
                } else if (lcd_params.xres * 6 / 8 > j) {
                    g_lcd_fb[i][j] = (uint16_t)(255 << 12) | (0 << 6) | (255 << 0);
                } else if (lcd_params.xres * 7 / 8 > j) {
                    g_lcd_fb[i][j] = (uint16_t)(255 << 12) | (255 << 6) | (0 << 0);
                } else if (lcd_params.xres * 8 / 8 > j) {
                    g_lcd_fb[i][j] = (uint16_t)(255 << 12) | (255 << 6) | (255 << 0);
                }

                //if (((0 == j) && (0 == i)) ||
                //    ((479 == j) && (0 == i)) ||
                //    ((479 == j) && (271 == i)) ||
                //    ((0 == j) && (271 == i))) {
                //    g_lcd_fb[i][j] = ~g_lcd_fb[i][j];
                //    printf("x:%d\ty:%d\r\n", j, i);
                //}

                udelay(10);
            }
        }
        mdelay(1000);

        for (i = 0; i < lcd_params.yres; i++) {
            for (j = 0; j < lcd_params.xres; j++) {
                g_lcd_fb[i][j] = 0;
                udelay(10);
            }
        }
        mdelay(1000);
    }
#endif

#if 0
    if (ddr2_test(0x43f00000, 1024) != 0) {
        printf("ddr2_test failed\r\n");
    } else {
        printf("ddr2_test success\r\n");
    }

    printf("ocram write speed is %d KB/s\r\n",
           ddr2_write_speed_test(30 * 1024, 50 * 1024 / 4));

    printf("ddr2 write speed is %d KB/s\r\n",
           ddr2_write_speed_test(0x43f00000, 1024 * 1024 / 4));

    printf("ddr2 read speed is %d KB/s\r\n",
           ddr2_read_speed_test(0x43f00000, 1024 * 1024 / 4));
#endif

    while (1) {
//        for (i = 0; i < sizeof(g_lcd_fb) / 4; i++) {
//            *((uint32_t *)g_lcd_fb + i) = *((uint32_t *)pic0 + i);
//        }
        led_on(LED_ERR);
        mdelay(1000);

//        for (i = 0; i < sizeof(g_lcd_fb) / 4; i++) {
//            *((uint32_t *)g_lcd_fb + i) = *((uint32_t *)pic1 + i);
//        }
        led_off(LED_ERR);
        mdelay(1000);
    }
}

/* end of file */
