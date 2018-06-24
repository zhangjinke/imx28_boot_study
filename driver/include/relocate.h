/**
 * \file
 * \brief 代码重定位
 *
 * \internal
 * \par Modification history
 * - 1.00 18-03-02  pea, first implementation
 * \endinternal
 */

#ifndef __RELOCATE_H
#define __RELOCATE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * \brief 代码重定位
 *
 * \param 无
 *
 * \return 无
 */
void relocate (volatile uint32_t *p_src_start);

#ifdef __cplusplus
}
#endif

#endif /* __RELOCATE_H */

/* end of file */
