/**
 * \file
 * \brief CLKCTRL 驱动
 *
 * \internal
 * \par Modification history
 * - 1.00 18-03-01  pea, first implementation
 * \endinternal
 */

#ifndef __CLKCTRL_H
#define __CLKCTRL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * \brief 打印 CLKCTRL 的寄存器值
 *
 * \param 无
 *
 * \return 无
 */
void clkctrl_reg_print (void);

/**
 * \brief 将系统时钟恢复为默认设置
 *
 * \param 无
 *
 * \return 无
 */
void clkctrl_deinit (void);

/**
 * \brief 初始化系统时钟
 *
 * \param 无
 *
 * \return 无
 */
void clkctrl_init (void);

#ifdef __cplusplus
}
#endif

#endif /* __CLKCTRL_H */

/* end of file */
