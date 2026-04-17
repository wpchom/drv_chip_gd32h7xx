/*!
    \file    dsi_test.h
    \brief   the header file of DSI
    
    \version 2025-09-29, V0.2.0, firmware for GD32H77x
*/

/*
    Copyright (c) 2025, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#ifndef LCM_DSI_H
#define LCM_DSI_H

#include <stdint.h>
#include <stdio.h>
#include "dsi_lcd_cfg.h"
#include "gd32h77x.h"

#define BACKLIGHT_PIN                           GPIO_PIN_11
#define BACKLIGHT_GPIO_PORT                     GPIOD
#define BACKLIGHT_GPIO_CLK                      RCU_GPIOD

#define LCD_RST_PIN                             GPIO_PIN_7
#define LCD_RST_GPIO_PORT                       GPIOG
#define LCD_RST_GPIO_CLK                        RCU_GPIOG

#define DSI_TE_PIN                              GPIO_PIN_15
#define DSI_TE_GPIO_PORT                        GPIOA
#define DSI_TE_GPIO_CLK                         RCU_GPIOA
#define DSI_TE_AFIO                             GPIO_AF_4

/* MIPI DCS pixel formats */                
#define MIPI_DCS_PIXEL_FMT_24BIT                7
#define MIPI_DCS_PIXEL_FMT_18BIT                6
#define MIPI_DCS_PIXEL_FMT_16BIT                5
#define MIPI_DCS_PIXEL_FMT_12BIT                3
#define MIPI_DCS_PIXEL_FMT_8BIT                 2
#define MIPI_DCS_PIXEL_FMT_3BIT                 1

/* MIPI DSI Processor-to-Peripheral transaction types */
enum{
    MIPI_DSI_V_SYNC_START                       = 0x01,         /* Sync Event, V Sync Start */
    MIPI_DSI_V_SYNC_END                         = 0x11,         /* Sync Event, V Sync End */
    MIPI_DSI_H_SYNC_START                       = 0x21,         /* Sync Event, H Sync Start */
    MIPI_DSI_H_SYNC_END                         = 0x31,         /* Sync Event, H Sync End */

    MIPI_DSI_COLOR_MODE_OFF                     = 0x02,         /* Color Mode (CM) Off Command */
    MIPI_DSI_COLOR_MODE_ON                      = 0x12,         /* Color Mode (CM) On Command */
    MIPI_DSI_SHUTDOWN_PERIPHERAL                = 0x22,         /* Shut Down Peripheral Command */
    MIPI_DSI_TURN_ON_PERIPHERAL                 = 0x32,         /* Turn On Peripheral Command */

    MIPI_DSI_GENERIC_SHORT_WRITE_0_PARAM        = 0x03,         /* Generic Short WRITE, no parameters */
    MIPI_DSI_GENERIC_SHORT_WRITE_1_PARAM        = 0x13,         /* Generic Short WRITE, 1 parameter */
    MIPI_DSI_GENERIC_SHORT_WRITE_2_PARAM        = 0x23,         /* Generic Short WRITE, 2 parameters */

    MIPI_DSI_GENERIC_READ_REQUEST_0_PARAM       = 0x04,         /* Generic READ, no parameters */
    MIPI_DSI_GENERIC_READ_REQUEST_1_PARAM       = 0x14,         /* Generic READ, 1 parameter */
    MIPI_DSI_GENERIC_READ_REQUEST_2_PARAM       = 0x24,         /* Generic READ, 2 parameters */

    MIPI_DSI_DCS_SHORT_WRITE                    = 0x05,         /* DCS Short WRITE, no parameters */
    MIPI_DSI_DCS_SHORT_WRITE_PARAM              = 0x15,         /* DCS Short WRITE, 1 parameter */

    MIPI_DSI_DCS_READ                           = 0x06,         /* DCS READ, no parameter */

    MIPI_DSI_DCS_COMPRESSION_MODE               = 0x07,         /* Compression Mode Command */
    MIPI_DSI_PPS_LONG_WRITE                     = 0x0A,         /* Picture Parameter Set */

    MIPI_DSI_SET_MAXIMUM_RETURN_PACKET_SIZE     = 0x37,         /* Set Maximum Return Packet Size */

    MIPI_DSI_END_OF_TRANSMISSION                = 0x08,         /* End of Transmission packet (EoT) */

    MIPI_DSI_NULL_PACKET                        = 0x09,         /* Null Packet, no data */
    MIPI_DSI_BLANKING_PACKET                    = 0x19,         /* Blanking Packet, no data */
    MIPI_DSI_GENERIC_LONG_WRITE                 = 0x29,         /* Generic Long Write */
    MIPI_DSI_DCS_LONG_WRITE                     = 0x39,         /* DCS long write */

    MIPI_DSI_LOOSELY_PACKED_PIXEL_STREAM_YCBCR20= 0x0c,         /* Loosely Packed Pixel Stream, 20-bit YCbCr 4:2:2 Format */
    MIPI_DSI_PACKED_PIXEL_STREAM_YCBCR24        = 0x1c,         /* Packed Pixel Stream, 24-bit YCbCr 4:2:2 Format */
    MIPI_DSI_PACKED_PIXEL_STREAM_YCBCR16        = 0x2c,         /* Packed Pixel Stream, 16-bit YCbCr 4:2:2 Format */

    MIPI_DSI_PACKED_PIXEL_STREAM_30             = 0x0d,         /* Packed Pixel Stream, 30-bit Format, Long Packet */
    MIPI_DSI_PACKED_PIXEL_STREAM_36             = 0x1d,         /* Packed Pixel Stream, 36-bit Format, Long Packet */
    MIPI_DSI_PACKED_PIXEL_STREAM_YCBCR12        = 0x3d,         /* Packed Pixel Stream, 12-bit YCbCr 4:2:0 Format */

    MIPI_DSI_PACKED_PIXEL_STREAM_16             = 0x0e,         /* Packed Pixel Stream, 16-bit Format, Long Packet */
    MIPI_DSI_PACKED_PIXEL_STREAM_18             = 0x1e,         /* Packed Pixel Stream, 18-bit Format, Long Packet */
    MIPI_DSI_PIXEL_STREAM_3BYTE_18              = 0x2e,         /* Pixel Stream, 18-bit Format in Three Bytes, Long Packet */
    MIPI_DSI_PACKED_PIXEL_STREAM_24             = 0x3e,         /* Packed Pixel Stream, 24-bit Format, Long Packet */
};

/* MIPI DSI Peripheral-to-Processor transaction types (MIPI DSI Specification) */
enum {
    MIPI_DSI_RX_ACKNOWLEDGE_AND_ERROR_REPORT     = 0x02,        /* Acknowledge and Error Report */
    MIPI_DSI_RX_END_OF_TRANSMISSION              = 0x08,        /* End of Transmission packet (EoTp) */
    MIPI_DSI_RX_GENERIC_SHORT_READ_RESPONSE_1BYTE= 0x11,        /* Generic Short READ Response, 1 byte returned */
    MIPI_DSI_RX_GENERIC_SHORT_READ_RESPONSE_2BYTE= 0x12,        /* Generic Short READ Response, 2 bytes returned */
    MIPI_DSI_RX_GENERIC_LONG_READ_RESPONSE       = 0x1a,        /* Generic Long READ Response */
    MIPI_DSI_RX_DCS_LONG_READ_RESPONSE           = 0x1c,        /* DCS Long READ Response */
    MIPI_DSI_RX_DCS_SHORT_READ_RESPONSE_1BYTE    = 0x21,        /* DCS Short READ Response, 1 byte returned */
    MIPI_DSI_RX_DCS_SHORT_READ_RESPONSE_2BYTE    = 0x22,        /* DCS Short READ Response, 2 bytes returned */
};

/* MIPI DCS commands (Specification for display command set DCS, Command List) */
enum {
    MIPI_DCS_NOP                                = 0x00,
    MIPI_DCS_SOFT_RESET                         = 0x01,
    MIPI_DCS_GET_DISPLAY_ID                     = 0x04,
    MIPI_DCS_GET_RED_CHANNEL                    = 0x06,
    MIPI_DCS_GET_GREEN_CHANNEL                  = 0x07,
    MIPI_DCS_GET_BLUE_CHANNEL                   = 0x08,
    MIPI_DCS_GET_DISPLAY_STATUS                 = 0x09,
    MIPI_DCS_GET_POWER_MODE                     = 0x0A,
    MIPI_DCS_GET_ADDRESS_MODE                   = 0x0B,
    MIPI_DCS_GET_PIXEL_FORMAT                   = 0x0C,
    MIPI_DCS_GET_DISPLAY_MODE                   = 0x0D,
    MIPI_DCS_GET_SIGNAL_MODE                    = 0x0E,
    MIPI_DCS_GET_DIAGNOSTIC_RESULT              = 0x0F,
    MIPI_DCS_ENTER_SLEEP_MODE                   = 0x10,
    MIPI_DCS_EXIT_SLEEP_MODE                    = 0x11,
    MIPI_DCS_ENTER_PARTIAL_MODE                 = 0x12,
    MIPI_DCS_ENTER_NORMAL_MODE                  = 0x13,
    MIPI_DCS_EXIT_INVERT_MODE                   = 0x20,
    MIPI_DCS_ENTER_INVERT_MODE                  = 0x21,
    MIPI_DCS_SET_GAMMA_CURVE                    = 0x26,
    MIPI_DCS_SET_DISPLAY_OFF                    = 0x28,
    MIPI_DCS_SET_DISPLAY_ON                     = 0x29,
    MIPI_DCS_SET_COLUMN_ADDRESS                 = 0x2A,
    MIPI_DCS_SET_PAGE_ADDRESS                   = 0x2B,
    MIPI_DCS_WRITE_MEMORY_START                 = 0x2C,
    MIPI_DCS_WRITE_LUT                          = 0x2D,
    MIPI_DCS_READ_MEMORY_START                  = 0x2E,
    MIPI_DCS_SET_PARTIAL_AREA                   = 0x30,
    MIPI_DCS_SET_SCROLL_AREA                    = 0x33,
    MIPI_DCS_SET_TEAR_OFF                       = 0x34,
    MIPI_DCS_SET_TEAR_ON                        = 0x35,
    MIPI_DCS_SET_ADDRESS_MODE                   = 0x36,
    MIPI_DCS_SET_SCROLL_START                   = 0x37,
    MIPI_DCS_EXIT_IDLE_MODE                     = 0x38,
    MIPI_DCS_ENTER_IDLE_MODE                    = 0x39,
    MIPI_DCS_SET_PIXEL_FORMAT                   = 0x3A,
    MIPI_DCS_WRITE_MEMORY_CONTINUE              = 0x3C,
    MIPI_DCS_READ_MEMORY_CONTINUE               = 0x3E,
    MIPI_DCS_SET_TEAR_SCANLINE                  = 0x44,
    MIPI_DCS_GET_SCANLINE                       = 0x45,
    MIPI_DCS_SET_DISPLAY_BRIGHTNESS             = 0x51,    /* MIPI DCS 1.3 */
    MIPI_DCS_GET_DISPLAY_BRIGHTNESS             = 0x52,    /* MIPI DCS 1.3 */
    MIPI_DCS_WRITE_CONTROL_DISPLAY              = 0x53,    /* MIPI DCS 1.3 */
    MIPI_DCS_GET_CONTROL_DISPLAY                = 0x54,    /* MIPI DCS 1.3 */
    MIPI_DCS_WRITE_POWER_SAVE                   = 0x55,    /* MIPI DCS 1.3 */
    MIPI_DCS_GET_POWER_SAVE                     = 0x56,    /* MIPI DCS 1.3 */
    MIPI_DCS_SET_CABC_MIN_BRIGHTNESS            = 0x5E,    /* MIPI DCS 1.3 */
    MIPI_DCS_GET_CABC_MIN_BRIGHTNESS            = 0x5F,    /* MIPI DCS 1.3 */
    MIPI_DCS_READ_DDB_START                     = 0xA1,
    MIPI_DCS_READ_DDB_CONTINUE                  = 0xA8,
};

/* DSI parameter struct definitions */
typedef enum {
    HIGH_SPEED                      = 0x0U,
    LOW_POWER                       = 0x1U,
}cmd_tx_mode_enum;

typedef enum {
    MIPI_DSI_FMT_RGB565             = DSI_VIDEO_MODE_FMT_RGB565,
    MIPI_DSI_FMT_RGB666             = DSI_VIDEO_MODE_FMT_RGB666,
    MIPI_DSI_FMT_RGB666_PACKED      = DSI_VIDEO_MODE_FMT_RGB666LP,
    MIPI_DSI_FMT_RGB888             = DSI_VIDEO_MODE_FMT_RGB888,
}pixel_format_enum;

typedef enum {
    VIDEO_NON_BURST_SYNC_PULSES     = DSI_NON_BURST_MODE_SYNC_PULSE,
    VIDEO_NON_BURST_SYNC_EVENTS     = DSI_NON_BURST_MODE_SYNC_EVENTS,
    VIDEO_BURST_MODE                = DSI_BURST_MODE,
    COMMAND_MODE                    = DSI_VIDEO_MODE_FORMAT_NONE,
}dsi_cmd_video_enum;

typedef enum {
    ONE_DATA_LANE                   = 0x1U,
    TWO_DATA_LANE                   = 0x2U,
}data_lane_num_enum;

#define _INIT_CMD(cmd, is_lpdt, delay, ...) {  \
    .type   = cmd,                             \
    .is_lp   = is_lpdt,                        \
    .mdelay = delay,                           \
    .len    = sizeof((char[]){__VA_ARGS__}),   \
    .data   = (char[]){__VA_ARGS__} }

typedef struct {
    uint8_t type;
    cmd_tx_mode_enum is_lp;
    uint16_t mdelay;
    uint32_t len;
    const char *data;
} __attribute__((packed)) lcm_init_cmd_struct;

typedef struct {
    uint8_t channel;                          /* virtual channel */
    uint8_t fps;                              /* frame per second */
    dsi_cmd_video_enum mode;                  /* command mode, video mode(burst, non-burst...) */
    data_lane_num_enum lane_num;              /* DPHY lane number */
    pixel_format_enum format;                 /* lcm pixel format */
    uint16_t hact;                            /* horizontal active width */
    uint16_t hsw;                             /* horizontal sync width */
    uint16_t hbp;                             /* horizontal sync back porch */
    uint16_t hfp;                             /* horizontal sync front porch */
    uint16_t vact;                            /* vertical active width */
    uint16_t vsw;                             /* vertical sync width */
    uint16_t vbp;                             /* vertical sync back porch */
    uint16_t vfp;                             /* vertical sync front porch */
} lcm_info_struct;

typedef struct {
    uint16_t hact;                            /* resolution horizontal counter */
    uint16_t vact;                            /* resolution vertical counter */
    uint16_t hswc;                            /* hsw count in term of tx_byte_clk */
    uint16_t hbpc;                            /* hbp count in term of tx_byte_clk */
    uint16_t hfpc;                            /* hfp count in term of tx_byte_clk */
    uint16_t hactc;                           /* hact count in term of tx_byte_clk */
    uint16_t vswc;                            /* vsw count in term of lines */
    uint16_t vbpc;                            /* vbp count in term of lines */
    uint16_t vfpc;                            /* vfp count in term of lines */
} dsi_timing_cfg_struct;

typedef struct {
    uint8_t freq_mode;                        /* 0-DDR freq < 200Mhz,1 DDR freq > 200Mhz */
    uint8_t esc_ck_sel;                       /* esc_clk > 20Mhz, need to divide by 2 for lptx_clk */
    uint8_t pll_divider;                      /* PLL divider from esc_clk for ddr */
    uint16_t dlcfg;                           /* hs2lp orlp2hs count in term of tx_byte_clk */
    uint16_t ck_hs2lp;                        /* clk hs2lp count in term of tx_byte_clk */
    uint16_t ck_lp2hs;                        /* clk lp2hs count in term of tx_byte_clk */
    uint16_t lp_clkc;                         /* txlp_clk count in term of tx_byte_clk */
    uint8_t dhs_prep;                         /* data lane Ths_prep */
    uint8_t dhs_zero;                         /* data lane Ths_zero */
    uint8_t dhs_trai;                         /* data lane Ths_trail */
    uint8_t dhs_exit;                         /* data lane Ths_exit */
    uint8_t ckhs_prep;                        /* clk lane Ths_prep */
    uint8_t ckhs_zero;                        /* clk lane Ths_zero */
    uint8_t ckhs_trai;                        /* clk lane Ths_trail */
    uint8_t ckhs_exit;                        /* clk lane Ths_exit */
} dphy_timing_cfg_struct;

typedef struct dsi_dphy_cfg {
    dsi_cmd_video_enum mode;                  /* command mode, video mode(burst, non-burst...) */
    pixel_format_enum format;                 /* dsi pixel format */
    data_lane_num_enum lane_num;              /* DPHY lane number */
    uint8_t channel;                          /* virtual channel */
    uint32_t total_pixel_byte;                /* bytes per frame */
    dsi_timing_cfg_struct dsi_timing;
    dphy_timing_cfg_struct dphy_timing;
}dsi_dphy_cfg_struct;

typedef struct {
    uint8_t channel;                          /* virtual channel id */
    uint8_t type;                             /* payload data type */
    uint8_t mode;                             /* low power mode or high speed mode */
    uint16_t mdelay;                          /* mdelay after this message transmission */

    uint32_t tx_len;                          /* length of tx_buf */
    const void *tx_buf;                       /* data to be written */

    uint32_t rx_len;                          /* length of rx_buf */
    void *rx_buf;                             /* data to be read, or NULL */
} __attribute__((aligned(4))) mipi_dsi_msg_struct;

extern __IO uint8_t ar_mode_bus_busy;
extern const uint8_t __attribute__((aligned(8))) logo[];

void cal_dphy_timing(uint32_t lpclk, uint32_t mipi_rate, dphy_timing_cfg_struct *cfg);
void mipi_dsi_device_transfer(mipi_dsi_msg_struct *msg);
void dsi_dphy_init(dsi_dphy_cfg_struct *dsi_cfg);
void dsi_clk_config(void);
void mipi_dsi_irq_handler(void);
void lcm_partial_upadte_set(uint16_t start_x, uint16_t end_x, uint16_t start_y, uint16_t end_y);
void dsi_dphy_lcm_test(void);

#endif /* DSI_LCD_H */
