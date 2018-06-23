/**
 * \file
 * \brief DUART 驱动
 *
 * \internal
 * \par Modification history
 * - 1.00 18-02-27  pea, first implementation
 * \endinternal
 */

/*******************************************************************************
  头文件包含
*******************************************************************************/
#include <stdint.h>
#include <stdarg.h>
#include "duart.h"

/*******************************************************************************
  宏定义
*******************************************************************************/

/* \brief 定义 GPIO 相关寄存器 */
#define HW_PINCTRL_CTRL    (*(volatile uint32_t *)0x80018000)
#define HW_PINCTRL_MUXSEL6 (*(volatile uint32_t *)0x80018160)

/* \brief 定义 DUART 相关寄存器 */
#define HW_UARTDBG_DR      (*(volatile uint32_t *)0x80074000)
#define HW_UARTDBG_FR      (*(volatile uint32_t *)0x80074018)
#define HW_UARTDBG_IBRD    (*(volatile uint32_t *)0x80074024)
#define HW_UARTDBG_FBRD    (*(volatile uint32_t *)0x80074028)
#define HW_UARTDBG_H       (*(volatile uint32_t *)0x8007402c)
#define HW_UARTDBG_CR      (*(volatile uint32_t *)0x80074030)

/*******************************************************************************
  外部函数定义
*******************************************************************************/

/**
 * \brief 输出一个字符
 */
int putchar (int c)
{
    while (!(HW_UARTDBG_FR & (0x01 << 7)));
    HW_UARTDBG_DR = c & 0xff;

    return c;
}

/**
 * \brief 输出一个字符串
 */
int puts (const char *p_str)
{
    char *p_str_temp = (char *)p_str;
    int   i          = 0;

    while (*p_str_temp) {
        putchar((int)(*p_str_temp++));
        i++;
    }

    return i;
}

/**
 * \brief 输入一个字符
 */
int getchar (void)
{
    while (!(HW_UARTDBG_FR & (0x01 << 6)));
    return HW_UARTDBG_DR & 0xff;
}

int my_vfprintf (const char *fmt,
                 va_list     args)
{
    unsigned char   c, f;
    int             r;
    unsigned long   val;
    char            s[16];
    int             i, w, res, cc;


    for (cc = res = 0; cc >= 0; res += cc >= 0 ? cc : 0) {
        if ((c = *fmt++) == 0) {
            break;                      /* End of string */
        } else if (c != '%') {          /* Non escape cahracter */
            cc = putchar(c);
            continue;
        }
        w = f = 0;
        c = *fmt++;
        if (c == '0') {                 /* Flag: '0' padding */
            f = 1;
            c = *fmt++;
        }
        while (c >= '0' && c <= '9') {  /* Precision */
            w = w * 10 + (c - '0');
            c = *fmt++;
        }
        if (c == 'l') {                 /* Prefix: Size is long int */
            f |= 2;
            c  = *fmt++;
        }
        if (c == 's') {                 /* Type is string */
            cc = puts(va_arg(args, char *));
            continue;
        }
        if (c == 'c') {                 /* Type is character */
            cc = putchar(va_arg(args, int));
            continue;
        }
        if (c == '%') {                 /* Char is '%' */
            cc = putchar('%');
            continue;
        }
        r = 0;
        if (c == 'd') {
            r = 10;             /* Type is signed decimal */
        } else if (c == 'u') {
            r = 10;             /* Type is unsigned decimal */
        } else if (c == 'X') {
            r = 16;             /* Type is unsigned hexdecimal, upper case */
        } else if (c == 'x') {
            r = 0x100 + 16;     /* Type is unsigned hexdecimal, lower case */
        }else {
            break;              /* Unknown type */
        }
        if (f & 2) {            /* Get the value */
            val = (unsigned long)va_arg(args, long);
        } else {
            val = (c == 'd')
                ? (unsigned long)(long)va_arg(args, int)
                : (unsigned long)va_arg(args, unsigned int);
        }
        /* Put numeral string */
        if ((c == 'd') && (val & 0x80000000)) {
            val = 0 - val;
            f  |= 4;
        }
        i    = sizeof(s) - 1;
        s[i] = 0;
        do {
            c = (unsigned char)(val % (r & 0xff) + '0');
            if (c > '9') {
                if (r & 0x100) {
                    c += 39;
                } else {
                    c += 7;
                }
            }
            s[--i] = c;
            val   /= (r & 0xff);
        } while (i && val);
        if (i && (f & 4)) {
            s[--i] = '-';
        }
        w = sizeof(s) - 1 - w;
        while (i && i > w) {
            s[--i] = (f & 1) ? '0' : ' ';
        }
        cc = puts(&s[i]);
    }

    return res;
}

int printf (const char *fmt, ...)
{
    int     len;
    va_list args;

    va_start(args, fmt);
    
    len = my_vfprintf(fmt, args);
    va_end(args);

    return len;
}

/**
 * \brief 初始化 DUART
 */
int duart_init (uint32_t baudrate)
{
    uint32_t divisor = (24000000 * 4) / baudrate;

    if ((divisor < 0x40) | (divisor > 0x3FFFC0)) {
        return -1;
    }

    /* 将 GPIO3_2 和 GPIO3_3 配置为 DUART 功能 */
    HW_PINCTRL_MUXSEL6 &= ~(0x0f << 4);
    HW_PINCTRL_MUXSEL6 |= (0x0a << 4);

    /* 配置波特率 */
    HW_UARTDBG_FBRD = divisor & 0x3f;
    HW_UARTDBG_IBRD = (divisor >> 6) & 0xffff;

    /* 模式配置为 8-N-1 */
    HW_UARTDBG_H = (0x03 << 5);

    /* 使能 TX RX 和 DUART */
    HW_UARTDBG_CR = (0x03 << 8) | (0x01 << 0);

    //printf("duart init done\r\n");
    putchar('d');
    putchar('u');
    putchar('a');
    putchar('r');
    putchar('t');
    putchar(' ');
    putchar('i');
    putchar('n');
    putchar('i');
    putchar('t');
    putchar(' ');
    putchar('d');
    putchar('o');
    putchar('n');
    putchar('e');
    putchar('\r');
    putchar('\n');

    return 0;
}

/* end of file */
