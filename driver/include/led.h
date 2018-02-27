/**
 * \file
 * \brief LED 驱动
 *
 * \internal
 * \par Modification history
 * - 1.00 18-02-27  pea, first implementation
 * \endinternal
 */

#ifndef __LED_H
#define __LED_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define  LED_RUN  0  /* \brief 定义 LED_RUN */
#define  LED_ERR  1  /* \brief 定义 LED_ERR */

/**
 * \brief 点亮指定 LED
 *
 * \param[in] id LED 号，可选为 LED_RUN 和 LED_ERR
 *
 * \return 无
 */
void led_on (uint32_t id);

/**
 * \brief 熄灭指定 LED
 *
 * \param[in] id LED 号，可选为 LED_RUN 和 LED_ERR
 *
 * \return 无
 */
void led_off (uint32_t id);

/**
 * \brief 初始化 LED
 *
 * \param 无
 *
 * \return 无
 */
void led_init (void);

#ifdef __cplusplus
}
#endif

#endif /* __LED_H */

/* end of file */
