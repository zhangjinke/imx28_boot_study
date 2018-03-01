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
//    clkctrl_deinit();
    led_init();
    duart_init(115200);
    ddr2_init();
    //clkctrl_init();

    mdelay(100);

    led_on(LED_RUN);

    printf("hello world\r\n");

    if (ddr2_test(0x40000000, 1024) != 0) {
        printf("ddr2_test failed\r\n");
    } else {
        printf("ddr2_test success\r\n");
    }

    printf("ocram write speed is %d KB/s\r\n", 
           ddr2_write_speed_test(30 * 1024, 50 * 1024 / 4));

    printf("ddr2 write speed is %d KB/s\r\n", 
           ddr2_write_speed_test(0x40000000, 1024 * 1024 / 4));

    printf("ddr2 read speed is %d KB/s\r\n", 
           ddr2_read_speed_test(0x40000000, 1024 * 1024 / 4));

    while (1) {
        led_on(LED_RUN);
        delay_test(1000);

        led_off(LED_RUN);
        delay_test(1000);
    }
}

/* end of file */
