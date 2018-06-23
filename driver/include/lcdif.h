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

#include "imx28_reg.h"
#include <stdint.h>

/** \brief 定义 LCDIF 寄存器结构体 */
struct imx28_lcdif_regs {
    struct imx28_reg_32  ctrl;           /**< \brief 控制寄存器 */
    struct imx28_reg_32  ctrl1;          /**< \brief 控制寄存器 1 */
    struct imx28_reg_32  ctrl2;          /**< \brief 控制寄存器 2 */
    struct normal_reg_32 transfer_count; /**< \brief 有效数据计数器 */
    struct normal_reg_32 cur_buf;        /**< \brief 当前帧地址 */
    struct normal_reg_32 next_buf;       /**< \brief 下一帧地址 */
    struct normal_reg_32 timing;         /**< \brief LCD 时序寄存器 */
    struct imx28_reg_32  vdctrl0;        /**< \brief VSYNC/Dotclk 模式控制寄存器 0 */
    struct normal_reg_32 vdctrl1;        /**< \brief VSYNC/Dotclk 模式控制寄存器 1 */
    struct normal_reg_32 vdctrl2;        /**< \brief VSYNC/Dotclk 模式控制寄存器 2 */
    struct normal_reg_32 vdctrl3;        /**< \brief VSYNC/Dotclk 模式控制寄存器 3 */
    struct normal_reg_32 vdctrl4;        /**< \brief VSYNC/Dotclk 模式控制寄存器 4 */
    struct normal_reg_32 dvictrl0;       /**< \brief 0x0c0 */
    struct normal_reg_32 dvictrl1;       /**< \brief 0x0d0 */
    struct normal_reg_32 dvictrl2;       /**< \brief 0x0e0 */
    struct normal_reg_32 dvictrl3;       /**< \brief 0x0f0 */
    struct normal_reg_32 dvictrl4;       /**< \brief 0x100 */
    struct normal_reg_32 csc_coeff0;     /**< \brief 0x110 */
    struct normal_reg_32 csc_coeff1;     /**< \brief 0x120 */
    struct normal_reg_32 csc_coeff2;     /**< \brief 0x130 */
    struct normal_reg_32 csc_coeff3;     /**< \brief 0x140 */
    struct normal_reg_32 csc_coeff4;     /**< \brief 0x150 */
    struct normal_reg_32 csc_offset;     /**< \brief 0x160 */
    struct normal_reg_32 csc_limit;      /**< \brief 0x170 */
    struct normal_reg_32 data;           /**< \brief 0x180 */
    struct normal_reg_32 bm_error_stat;  /**< \brief 0x190 */
    struct normal_reg_32 crc_stat;       /**< \brief 0x1a0 */
    struct normal_reg_32 stat;           /**< \brief 0x1b0 */
    struct normal_reg_32 version;        /**< \brief 0x1c0 */
    struct normal_reg_32 debug0;         /**< \brief 0x1d0 */
    struct normal_reg_32 debug1;         /**< \brief 0x1e0 */
    struct normal_reg_32 debug2;         /**< \brief 0x1f0 */
};

#define LCDIF_CTRL_SFTRST                     (1 << 31)
#define LCDIF_CTRL_CLKGATE                    (1 << 30)
#define LCDIF_CTRL_YCBCR422_INPUT             (1 << 29)
#define LCDIF_CTRL_READ_WRITEB                (1 << 28)
#define LCDIF_CTRL_WAIT_FOR_VSYNC_EDGE        (1 << 27)
#define LCDIF_CTRL_DATA_SHIFT_DIR             (1 << 26)
#define LCDIF_CTRL_SHIFT_NUM_BITS_MASK        (0x1f << 21)
#define LCDIF_CTRL_SHIFT_NUM_BITS_OFFSET      21
#define LCDIF_CTRL_DVI_MODE                   (1 << 20)
#define LCDIF_CTRL_BYPASS_COUNT               (1 << 19)
#define LCDIF_CTRL_VSYNC_MODE                 (1 << 18)
#define LCDIF_CTRL_DOTCLK_MODE                (1 << 17)
#define LCDIF_CTRL_DATA_SELECT                (1 << 16)
#define LCDIF_CTRL_INPUT_DATA_SWIZZLE_MASK    (3 << 14)
#define LCDIF_CTRL_INPUT_DATA_SWIZZLE_OFFSET  14
#define LCDIF_CTRL_CSC_DATA_SWIZZLE_MASK      (3 << 12)
#define LCDIF_CTRL_CSC_DATA_SWIZZLE_OFFSET    12
#define LCDIF_CTRL_LCD_DATABUS_WIDTH_MASK     (3 << 10)
#define LCDIF_CTRL_LCD_DATABUS_WIDTH_OFFSET   10
#define LCDIF_CTRL_LCD_DATABUS_WIDTH_16BIT    (0 << 10)
#define LCDIF_CTRL_LCD_DATABUS_WIDTH_8BIT     (1 << 10)
#define LCDIF_CTRL_LCD_DATABUS_WIDTH_18BIT    (2 << 10)
#define LCDIF_CTRL_LCD_DATABUS_WIDTH_24BIT    (3 << 10)
#define LCDIF_CTRL_WORD_LENGTH_MASK           (3 << 8)
#define LCDIF_CTRL_WORD_LENGTH_OFFSET         8
#define LCDIF_CTRL_WORD_LENGTH_16BIT          (0 << 8)
#define LCDIF_CTRL_WORD_LENGTH_8BIT           (1 << 8)
#define LCDIF_CTRL_WORD_LENGTH_18BIT          (2 << 8)
#define LCDIF_CTRL_WORD_LENGTH_24BIT          (3 << 8)
#define LCDIF_CTRL_RGB_TO_YCBCR422_CSC        (1 << 7)
#define LCDIF_CTRL_LCDIF_MASTER               (1 << 5)
#define LCDIF_CTRL_DATA_FORMAT_16_BIT         (1 << 3)
#define LCDIF_CTRL_DATA_FORMAT_18_BIT         (1 << 2)
#define LCDIF_CTRL_DATA_FORMAT_24_BIT         (1 << 1)
#define LCDIF_CTRL_RUN                        (1 << 0)

#define LCDIF_CTRL1_COMBINE_MPU_WR_STRB                (1 << 27)
#define LCDIF_CTRL1_BM_ERROR_IRQ_EN                    (1 << 26)
#define LCDIF_CTRL1_BM_ERROR_IRQ                       (1 << 25)
#define LCDIF_CTRL1_RECOVER_ON_UNDERFLOW               (1 << 24)
#define LCDIF_CTRL1_INTERLACE_FIELDS                   (1 << 23)
#define LCDIF_CTRL1_START_INTERLACE_FROM_SECOND_FIELD  (1 << 22)
#define LCDIF_CTRL1_FIFO_CLEAR                         (1 << 21)
#define LCDIF_CTRL1_IRQ_ON_ALTERNATE_FIELDS            (1 << 20)
#define LCDIF_CTRL1_BYTE_PACKING_FORMAT_MASK           (0xf << 16)
#define LCDIF_CTRL1_BYTE_PACKING_FORMAT_OFFSET         16
#define LCDIF_CTRL1_OVERFLOW_IRQ_EN                    (1 << 15)
#define LCDIF_CTRL1_UNDERFLOW_IRQ_EN                   (1 << 14)
#define LCDIF_CTRL1_CUR_FRAME_DONE_IRQ_EN              (1 << 13)
#define LCDIF_CTRL1_VSYNC_EDGE_IRQ_EN                  (1 << 12)
#define LCDIF_CTRL1_OVERFLOW_IRQ                       (1 << 11)
#define LCDIF_CTRL1_UNDERFLOW_IRQ                      (1 << 10)
#define LCDIF_CTRL1_CUR_FRAME_DONE_IRQ                 (1 << 9)
#define LCDIF_CTRL1_VSYNC_EDGE_IRQ                     (1 << 8)
#define LCDIF_CTRL1_BUSY_ENABLE                        (1 << 2)
#define LCDIF_CTRL1_MODE86                             (1 << 1)
#define LCDIF_CTRL1_RESET                              (1 << 0)

#define LCDIF_CTRL2_OUTSTANDING_REQS_MASK                 (7 << 21)
#define LCDIF_CTRL2_OUTSTANDING_REQS_OFFSET               21
#define LCDIF_CTRL2_OUTSTANDING_REQS_REQ_1                (0 << 21)
#define LCDIF_CTRL2_OUTSTANDING_REQS_REQ_2                (1 << 21)
#define LCDIF_CTRL2_OUTSTANDING_REQS_REQ_4                (2 << 21)
#define LCDIF_CTRL2_OUTSTANDING_REQS_REQ_8                (3 << 21)
#define LCDIF_CTRL2_OUTSTANDING_REQS_REQ_16               (4 << 21)
#define LCDIF_CTRL2_BURST_LEN_8                           (1 << 20)
#define LCDIF_CTRL2_ODD_LINE_PATTERN_MASK                 (7 << 16)
#define LCDIF_CTRL2_ODD_LINE_PATTERN_OFFSET               16
#define LCDIF_CTRL2_ODD_LINE_PATTERN_RGB                  (0 << 16)
#define LCDIF_CTRL2_ODD_LINE_PATTERN_RBG                  (1 << 16)
#define LCDIF_CTRL2_ODD_LINE_PATTERN_GBR                  (2 << 16)
#define LCDIF_CTRL2_ODD_LINE_PATTERN_GRB                  (3 << 16)
#define LCDIF_CTRL2_ODD_LINE_PATTERN_BRG                  (4 << 16)
#define LCDIF_CTRL2_ODD_LINE_PATTERN_BGR                  (5 << 16)
#define LCDIF_CTRL2_EVEN_LINE_PATTERN_MASK                (7 << 12)
#define LCDIF_CTRL2_EVEN_LINE_PATTERN_OFFSET              12
#define LCDIF_CTRL2_EVEN_LINE_PATTERN_RGB                 (0 << 12)
#define LCDIF_CTRL2_EVEN_LINE_PATTERN_RBG                 (1 << 12)
#define LCDIF_CTRL2_EVEN_LINE_PATTERN_GBR                 (2 << 12)
#define LCDIF_CTRL2_EVEN_LINE_PATTERN_GRB                 (3 << 12)
#define LCDIF_CTRL2_EVEN_LINE_PATTERN_BRG                 (4 << 12)
#define LCDIF_CTRL2_EVEN_LINE_PATTERN_BGR                 (5 << 12)
#define LCDIF_CTRL2_READ_PACK_DIR                         (1 << 10)
#define LCDIF_CTRL2_READ_MODE_OUTPUT_IN_RGB_FORMAT        (1 << 9)
#define LCDIF_CTRL2_READ_MODE_6_BIT_INPUT                 (1 << 8)
#define LCDIF_CTRL2_READ_MODE_NUM_PACKED_SUBWORDS_MASK    (7 << 4)
#define LCDIF_CTRL2_READ_MODE_NUM_PACKED_SUBWORDS_OFFSET  4
#define LCDIF_CTRL2_INITIAL_DUMMY_READ_MASK               (7 << 1)
#define LCDIF_CTRL2_INITIAL_DUMMY_READ_OFFSET             1

#define LCDIF_TRANSFER_COUNT_V_COUNT_MASK    (0xffff << 16)
#define LCDIF_TRANSFER_COUNT_V_COUNT_OFFSET  16
#define LCDIF_TRANSFER_COUNT_H_COUNT_MASK    (0xffff << 0)
#define LCDIF_TRANSFER_COUNT_H_COUNT_OFFSET  0

#define LCDIF_CUR_BUF_ADDR_MASK    0xffffffff
#define LCDIF_CUR_BUF_ADDR_OFFSET  0

#define LCDIF_NEXT_BUF_ADDR_MASK    0xffffffff
#define LCDIF_NEXT_BUF_ADDR_OFFSET  0

#define LCDIF_TIMING_CMD_HOLD_MASK      (0xff << 24)
#define LCDIF_TIMING_CMD_HOLD_OFFSET    24
#define LCDIF_TIMING_CMD_SETUP_MASK     (0xff << 16)
#define LCDIF_TIMING_CMD_SETUP_OFFSET   16
#define LCDIF_TIMING_DATA_HOLD_MASK     (0xff << 8)
#define LCDIF_TIMING_DATA_HOLD_OFFSET   8
#define LCDIF_TIMING_DATA_SETUP_MASK    (0xff << 0)
#define LCDIF_TIMING_DATA_SETUP_OFFSET  0

#define LCDIF_VDCTRL0_VSYNC_OEB                 (1 << 29)
#define LCDIF_VDCTRL0_ENABLE_PRESENT            (1 << 28)
#define LCDIF_VDCTRL0_VSYNC_POL                 (1 << 27)
#define LCDIF_VDCTRL0_HSYNC_POL                 (1 << 26)
#define LCDIF_VDCTRL0_DOTCLK_POL                (1 << 25)
#define LCDIF_VDCTRL0_ENABLE_POL                (1 << 24)
#define LCDIF_VDCTRL0_VSYNC_PERIOD_UNIT         (1 << 21)
#define LCDIF_VDCTRL0_VSYNC_PULSE_WIDTH_UNIT    (1 << 20)
#define LCDIF_VDCTRL0_HALF_LINE                 (1 << 19)
#define LCDIF_VDCTRL0_HALF_LINE_MODE            (1 << 18)
#define LCDIF_VDCTRL0_VSYNC_PULSE_WIDTH_MASK    0x3ffff
#define LCDIF_VDCTRL0_VSYNC_PULSE_WIDTH_OFFSET  0

#define LCDIF_VDCTRL1_VSYNC_PERIOD_MASK    0xffffffff
#define LCDIF_VDCTRL1_VSYNC_PERIOD_OFFSET  0

#define LCDIF_VDCTRL2_HSYNC_PULSE_WIDTH_MASK    (0x3fff << 18)
#define LCDIF_VDCTRL2_HSYNC_PULSE_WIDTH_OFFSET  18
#define LCDIF_VDCTRL2_HSYNC_PERIOD_MASK         0x3ffff
#define LCDIF_VDCTRL2_HSYNC_PERIOD_OFFSET       0

#define LCDIF_VDCTRL3_MUX_SYNC_SIGNALS            (1 << 29)
#define LCDIF_VDCTRL3_VSYNC_ONLY                  (1 << 28)
#define LCDIF_VDCTRL3_HORIZONTAL_WAIT_CNT_MASK    (0xfff << 16)
#define LCDIF_VDCTRL3_HORIZONTAL_WAIT_CNT_OFFSET  16
#define LCDIF_VDCTRL3_VERTICAL_WAIT_CNT_MASK      (0xffff << 0)
#define LCDIF_VDCTRL3_VERTICAL_WAIT_CNT_OFFSET    0

#define LCDIF_VDCTRL4_DOTCLK_DLY_SEL_MASK             (7 << 29)
#define LCDIF_VDCTRL4_DOTCLK_DLY_SEL_OFFSET           29
#define LCDIF_VDCTRL4_SYNC_SIGNALS_ON                 (1 << 18)
#define LCDIF_VDCTRL4_DOTCLK_H_VALID_DATA_CNT_MASK    0x3ffff
#define LCDIF_VDCTRL4_DOTCLK_H_VALID_DATA_CNT_OFFSET  0

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
