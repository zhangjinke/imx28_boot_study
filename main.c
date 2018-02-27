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

/**
 * \brief 主函数
 */
int main (void)
{
    led_init();
    duart_init(115200);

    led_on(LED_RUN);

    printf("hello world\n");

    while (1) {
        mdelay(500);
    }
}

/* end of file */
