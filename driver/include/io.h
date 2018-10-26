/**
 * \file
 * \brief IO 操作
 *
 * \internal
 * \par Modification history
 * - 1.00 18-06-22  pea, first implementation
 * \endinternal
 */

#ifndef __IO_H
#define __IO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* \brief 定义优化屏障 */
#define DMB asm volatile ("" : : : "memory")

#define dmb()   DMB

#define __arch_getb(a)          (*(volatile uint8_t *)(a))
#define __arch_getw(a)          (*(volatile uint16_t *)(a))
#define __arch_getl(a)          (*(volatile uint32_t *)(a))
#define __arch_getq(a)          (*(volatile uint64_t *)(a))

#define __arch_putb(v,a)        (*(volatile uint8_t *)(a) = (v))
#define __arch_putw(v,a)        (*(volatile uint16_t *)(a) = (v))
#define __arch_putl(v,a)        (*(volatile uint32_t *)(a) = (v))
#define __arch_putq(v,a)        (*(volatile uint64_t *)(a) = (v))

#define __iormb()   dmb()
#define __iowmb()   dmb()

#define writeb(v,c) ({ uint8_t  __v = v; __iowmb(); __arch_putb(__v,c); __v; })
#define writew(v,c) ({ uint16_t __v = v; __iowmb(); __arch_putw(__v,c); __v; })
#define writel(v,c) ({ uint32_t __v = v; __iowmb(); __arch_putl(__v,c); __v; })
#define writeq(v,c) ({ uint64_t __v = v; __iowmb(); __arch_putq(__v,c); __v; })

#define readb(c)    ({ uint8_t  __v = __arch_getb(c); __iormb(); __v; })
#define readw(c)    ({ uint16_t __v = __arch_getw(c); __iormb(); __v; })
#define readl(c)    ({ uint32_t __v = __arch_getl(c); __iormb(); __v; })
#define readq(c)    ({ uint64_t __v = __arch_getq(c); __iormb(); __v; })

#ifdef __cplusplus
}
#endif

#endif /* __IO_H */

/* end of file */
