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
#include "led.h"
#include "delay.h"

/**
 * \brief 主函数
 */
int main (void)
{
    led_init();

    while (1) {
        led_on(LED_RUN);
        led_off(LED_ERR);
        mdelay(500);

        led_off(LED_RUN);
        led_on(LED_ERR);
        mdelay(500);
    }
}

/* end of file */
