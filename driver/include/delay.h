/**
 * \file
 * \brief 延时
 *
 * \internal
 * \par Modification history
 * - 1.00 18-02-27  pea, first implementation
 * \endinternal
 */

#ifndef __DELAY_H
#define __DELAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * \brief 延时指定微秒
 *
 * param[in] us 延时的微秒数
 *
 * \return 无
 */
void udelay (uint32_t us);

/**
 * \brief 延时指定毫秒
 *
 * param[in] ms 延时的毫秒数
 *
 * \return 无
 */
void mdelay (uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif /* __DELAY_H */

/* end of file */
