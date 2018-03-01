/**
 * \file
 * \brief DDR2 驱动
 *
 * \internal
 * \par Modification history
 * - 1.00 18-03-01  pea, first implementation
 * \endinternal
 */

#ifndef __DDR2_H
#define __DDR2_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * \brief 测试 DDR2 写入速度
 *
 * \param[in] addr  测试的地址
 * \param[in] lenth 测试的长度
 *
 * \return 写入速度，单位为 KB/s
 */
uint32_t ddr2_write_speed_test (uint32_t addr, uint32_t lenth);

/**
 * \brief 测试 DDR2 读取速度
 *
 * \param[in] addr  测试的地址
 * \param[in] lenth 测试的长度
 *
 * \return 读取速度，单位为 KB/s
 */
uint32_t ddr2_read_speed_test (uint32_t addr, uint32_t lenth);

/**
 * \brief 测试 DDR2
 *
 * \param[in] addr  测试的地址
 * \param[in] lenth 测试的长度
 *
 * \retval  0 测试成功
 * \retval -1 测试失败
 */
int ddr2_test (uint32_t addr, uint32_t lenth);

/**
 * \brief 初始化 DDR2
 *
 * \param 无
 *
 * \retval  0 初始化成功
 * \retval -1 初始化失败
 */
int ddr2_init (void);

#ifdef __cplusplus
}
#endif

#endif /* __DDR2_H */

/* end of file */
