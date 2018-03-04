/**
 * \file
 * \brief LCD 驱动
 *
 * \internal
 * \par Modification history
 * - 1.00 18-03-03  pea, first implementation
 * \endinternal
 */

#ifndef __LCDIF_H
#define __LCDIF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * \brief 使能 LCD
 *
 * \param 无
 *
 * \return 无
 */
void lcd_enable (void);
/**
 * \brief 初始化 LCD
 *
 * \param 无
 *
 * \return 无
 */
void lcdif_init (void);

#ifdef __cplusplus
}
#endif

#endif /* __LCDIF_H */

/* end of file */
