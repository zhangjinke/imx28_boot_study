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

//static uint16_t g_lcd_fb[272][480];

void delay_test (volatile uint32_t ms)
{
    ms *= 7000;
    while (ms--);
}

//struct lcd_params lcd_params = {
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
//    .xres    = 480,
//    .yres    = 272,
//    .bpp     = 16,
//    .fb_base = (uint32_t)&g_lcd_fb[0],
//};

/**
 * \brief 主函数
 */
int main (void)
{
    //lcdif_init(&lcd_params);

//    lcd_enable();
    led_on(LED_ERR);

    printf("hello world\r\n");

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
        led_on(LED_ERR);
        mdelay(1000);

        led_off(LED_ERR);
        mdelay(1000);
    }
}

/* end of file */
