/**
 * \file
 * \brief DUART 驱动
 *
 * \internal
 * \par Modification history
 * - 1.00 18-02-27  pea, first implementation
 * \endinternal
 */

#ifndef __DUART_H
#define __DUART_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * \brief 输出一个字符
 *
 * \param[in] c 待输出的字符
 *
 * \return 输出成功返回输出的字符，否则返回 EOF
 */
int putchar (int c);

/**
 * \brief 输出一个字符串
 *
 * \param[in] p_str 待输出的字符串
 *
 * \return 输出成功返回输出的字符数，否则返回 EOF
 */
int puts (const char *p_str);

/**
 * \brief 输入一个字符
 *
 * \param[in] c 待输入的字符
 *
 * \return 输入成功返回输入的字符，否则返回 EOF
 */
int getchar (void);

int printf (const char *fmt, ...);

/**
 * \brief 初始化 DUART
 *
 * \param[in] baudrate 需要初始化的波特率
 *
 * \retval  0 初始化成功
 * \retval -1 波特率不支持
 */
int duart_init (uint32_t baudrate);

#ifdef __cplusplus
}
#endif

#endif /* __DUART_H */

/* end of file */
