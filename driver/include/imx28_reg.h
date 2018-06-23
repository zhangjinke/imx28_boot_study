/**
 * \file
 * \brief IMX28 寄存器定义
 *
 * \internal
 * \par Modification history
 * - 1.00 18-06-23  pea, first implementation
 * \endinternal
 */

#ifndef __IMX28_REG_H
#define __IMX28_REG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/** \brief imx28 特殊寄存器定义 */
struct imx28_reg_32 {
    uint32_t reg;
    uint32_t set;
    uint32_t clr;
    uint32_t tog;
};

/** \brief imx28 普通寄存器定义 */
struct normal_reg_32 {
    uint32_t reg;
    uint32_t reserved[3];
};

#ifdef __cplusplus
}
#endif

#endif /* __IMX28_REG_H */

/* end of file */
