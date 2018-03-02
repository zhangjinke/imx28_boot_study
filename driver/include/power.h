/**
 * \file
 * \brief 电源管理驱动
 *
 * \internal
 * \par Modification history
 * - 1.00 18-03-02  pea, first implementation
 * \endinternal
 */

#ifndef __POWER_H
#define __POWER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * \brief 初始化电源
 *
 * \param 无
 *
 * \return 无
 */
void power_init (void);

#ifdef __cplusplus
}
#endif

#endif /* __POWER_H */

/* end of file */
