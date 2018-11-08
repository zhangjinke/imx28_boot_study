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
#include "my_malloc.h"

/**
 * \brief 主函数
 */
int main (void)
{

    /* 堆初始化 */
    my_mem_init(MEM_DDR);

    /* 初始化中断控制器 */
    icoll_init();

    printf("\r\n\r\n");

    printf("clk_p: %dHz\r\n", clk_get(CLK_P));
    printf("clk_h: %dHz\r\n", clk_get(CLK_H));

    printf("hello world\r\n");

    mdelay(100);

    while (1) {
        led_on(LED_ERR);
        mdelay(1000);

        led_off(LED_ERR);
        mdelay(1000);
    }
}

/* end of file */
