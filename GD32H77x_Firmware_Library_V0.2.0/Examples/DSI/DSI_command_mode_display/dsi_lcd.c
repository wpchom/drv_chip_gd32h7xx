/*!
    \file    dsi_lcd.c
    \brief   the DSI LCD configuration file

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

#include "gd32h77x.h"
#include "gd32h77x_dsi.h"
#include "dsi_lcd.h"
#include "systick.h"
#include <string.h>
#include <stdbool.h>

#define MDMA_MAX_BYTES  64
#define ceil(n, d) ((n)/(d) + (((n)%(d)) != 0 ? 1 : 0))
#define max(x, y)  ((x) > (y) ? (x) : (y))

__IO uint8_t lp_rd_avail = 0;
__IO uint8_t ar_mode_bus_busy = 0;

#ifdef LCM_TEST
/* DPHY low power clk hz */
uint32_t pixel_clk   = PIXEL_CLK;
/* DPHY low power clk hz */
uint32_t lpclk   = LP_CLK;
/* DPHY high speed rate bps */
uint32_t hs_rate = HS_RATE;
#endif

static dsi_dphy_cfg_struct dsi_cfg;

const uint8_t __attribute__((aligned(8))) logo[] = {
#ifdef LCM_CD320x240_TEST
#if PIXEL_FOMART == DSI_FMT_RGB888
#include "logo_320x240_rgb888.h"
#elif PIXEL_FOMART == DSI_FMT_RGB565
#include "color_bar_320x240_rgb565.h"
#endif
#endif /* LCM_CD320x240_TEST */
};

static lcm_info_struct lcm_info = {
#ifdef LCM_TEST
    .channel    = DSI_VIRTUAL_CHANNEL_0,
    .fps        = 60,
#ifdef LCM_VIDEO_MODE_TEST
    .mode       = VIDEO_BURST_MODE,
#else
    .mode       = COMMAND_MODE,
#endif /* LCM_VIDEO_MODE_TEST */
    .lane_num   = DATA_LANE_NUM,
    .format     = (pixel_format_enum)PIXEL_FOMART,
    .hact       = ACTIVE_WIDTH,
    .hsw        = HORIZONTAL_SYNCHRONOUS_PULSE,
    .hbp        = HORIZONTAL_BACK_PORCH,
    .hfp        = HORIZONTAL_FRONT_PORCH,
    .vact       = ACTIVE_HEIGHT,
    .vsw        = VERTICAL_SYNCHRONOUS_PULSE,
    .vbp        = VERTICAL_BACK_PORCH,
    .vfp        = VERTICAL_FRONT_PORCH,
#endif /* LCM_TEST */
};

static lcm_init_cmd_struct init_cmds[] = {
#ifdef LCM_CD320x240_TEST
    _INIT_CMD(MIPI_DSI_DCS_LONG_WRITE, LOW_POWER, 0, 0xe0, 0x00, 0x04, 0x0e, 0x08, 0x17, 0x0a, 0x40, 0x79, 0x4d, 0x07, 0x0e, 0x0a, 0x1a, 0x1d, 0x1f),
    _INIT_CMD(MIPI_DSI_DCS_LONG_WRITE, LOW_POWER, 0, 0xe1, 0x00, 0x1b, 0x1f, 0x02, 0x10, 0x05, 0x32, 0x34, 0x43, 0x02, 0x0a, 0x09, 0x33, 0x37, 0x0f),
    _INIT_CMD(MIPI_DSI_DCS_LONG_WRITE, LOW_POWER, 0, 0xc0, 0x18, 0x18),
    _INIT_CMD(MIPI_DSI_DCS_LONG_WRITE, LOW_POWER, 0, 0xc1, 0x45),
    _INIT_CMD(MIPI_DSI_DCS_LONG_WRITE, LOW_POWER, 0, 0xc5, 0x00, 0x45, 0x80),
    /* RGB:0x48 BGR:0x40 */
    _INIT_CMD(MIPI_DSI_DCS_LONG_WRITE, LOW_POWER, 0, 0x36, 0x40),
    /* interface mode control 16.7M(24bit RGB888) */
#if PIXEL_FOMART == DSI_FMT_RGB888
    _INIT_CMD(MIPI_DSI_DCS_LONG_WRITE, LOW_POWER, 0, 0x3a, 0x70), //RGB888
#elif PIXEL_FOMART == DSI_FMT_RGB565
    _INIT_CMD(MIPI_DSI_DCS_LONG_WRITE, LOW_POWER, 0, 0x3a, 0x55), //RGB565
#elif (PIXEL_FOMART == DSI_FMT_RGB666) || (PIXEL_FOMART == DSI_FMT_RGB666_PACKED)
    _INIT_CMD(MIPI_DSI_DCS_LONG_WRITE, LOW_POWER, 0, 0x3a, 0x66), //RGB666
#endif
    /* frame rate 70Hz */
    _INIT_CMD(MIPI_DSI_DCS_LONG_WRITE, LOW_POWER, 0, 0xb0, 0x10),
    _INIT_CMD(MIPI_DSI_DCS_LONG_WRITE, LOW_POWER, 0, 0xb4, 0x02),
    _INIT_CMD(MIPI_DSI_DCS_LONG_WRITE, LOW_POWER, 0, 0xe9, 0x00),

    /* last parameter may have effect on RGB666 and RGB666_packed */
    _INIT_CMD(MIPI_DSI_DCS_LONG_WRITE, LOW_POWER, 0, 0xf7, 0xa9, 0x51, 0x2c, 0x02),

    /* column address set 0-319 */
    _INIT_CMD(MIPI_DSI_DCS_LONG_WRITE, LOW_POWER, 0, 0x2a, 0x00, 0x00, 0x01, 0x3f),
    /* page address set 0-239 */
    _INIT_CMD(MIPI_DSI_DCS_LONG_WRITE, LOW_POWER, 0, 0x2b, 0x00, 0x00, 0x00, 0xef),

    _INIT_CMD(MIPI_DSI_DCS_LONG_WRITE, LOW_POWER, 0, 0xff, 0x77, 0x01, 0x00, 0x00, 0x10),
    _INIT_CMD(MIPI_DSI_DCS_LONG_WRITE, LOW_POWER, 0, 0xc7, 0x00),

    /* sleep out */
    _INIT_CMD(MIPI_DSI_DCS_SHORT_WRITE_PARAM, LOW_POWER, 120, 0x11),
    /* display on */
    _INIT_CMD(MIPI_DSI_DCS_SHORT_WRITE_PARAM, LOW_POWER, 0, 0x29),
    _INIT_CMD(MIPI_DSI_DCS_SHORT_WRITE_PARAM, LOW_POWER, 0, 0x35, 0x00),
#endif /* LCM_CD320x240_TEST */
};

void dsi_command_mode_supported_set(uint32_t supported);
void dsi_wrapper_rgb_data_format_set(uint32_t format);

static uint32_t dsi_safe_read_u32(const void *ptr, uint32_t offset)
{
    uint32_t value = 0U;
    const uint8_t *byte_ptr = (const uint8_t *)ptr;

    (void)memcpy(&value, &byte_ptr[offset], sizeof(uint32_t));
    return value;
}

static uint16_t dsi_safe_read_u16(const void *ptr, uint32_t offset)
{
    uint16_t value = 0U;
    const uint8_t *byte_ptr = (const uint8_t *)ptr;

    (void)memcpy(&value, &byte_ptr[offset], sizeof(uint16_t));
    return value;
}

static uint8_t dsi_safe_read_u8(const void *ptr, uint32_t offset)
{
    const uint8_t *byte_ptr = (const uint8_t *)ptr;
    return byte_ptr[offset];
}

static void dsi_safe_write_u32(void *ptr, uint32_t offset, uint32_t value)
{
    uint8_t *byte_ptr = (uint8_t *)ptr;

    (void)memcpy(&byte_ptr[offset], &value, sizeof(uint32_t));
}

/*!
  \brief      configure dsi clock
  \param[in]  none
  \param[out] none
  \retval     none
*/
void dsi_clk_config(void)
{
    rcu_periph_clock_enable(RCU_DSI);
    rcu_dsi_preceding_stage_clock_config(RCU_DSIPRESEL_HXTAL);
    rcu_dsi_clock_div_config(RCU_DSI_DIV2);
}

/*!
  \brief      calculate dphy timing
  \param[in]  lpclk: lp clock
  \param[in]  mipi_rate: mipi dsi rate
  \param[in]  cfg: dhpy timing configuration pointer
  \param[out] none
  \retval     none
*/
void cal_dphy_timing(uint32_t lpclk, uint32_t mipi_rate, dphy_timing_cfg_struct *cfg)
{
    uint32_t byteclk_ns;

    byteclk_ns = 1000000000 / mipi_rate * 8;

    cfg->pll_divider = mipi_rate / 2 / lpclk;

    if(lpclk > 20000000u) {
        cfg->esc_ck_sel = 0x1u;
    } else {
        cfg->esc_ck_sel = 0x0u;
    }

    if(mipi_rate > 200000000) {
        cfg->freq_mode = 1;
    } else {
        cfg->freq_mode = 0;
    }

    /* 50ns < Tlpx */
    cfg->lp_clkc = ceil(60, byteclk_ns) - 1;
    /* 40ns + 4UI < Ths_prep 40ns + 4UI < 85ns +6UI*/
    cfg->dhs_prep  = (ceil((40 + 4 * byteclk_ns / 8), byteclk_ns) - 1);
    /* Ths_prep + Ths_zero > 145ns + 10UI, so Ths_zero 105ns + 6UI */
    cfg->dhs_zero  = ceil((90 + 6 * byteclk_ns / 8), byteclk_ns) - 1;
    /* Ths_trai > max(n * 8 * ui, 60 + n * 4 * ui), n:1 forward_hs, n:4 revers_hs */
    cfg->dhs_trai  = ceil((max((1 * 8 * byteclk_ns / 8), (260 + 1 * 4 * byteclk_ns / 8))), byteclk_ns) - 1;
    /* Ths_exit > 100ns*/
    cfg->dhs_exit  = ceil(230, byteclk_ns) - 1;

    /* 38ns < Tclk_prep < 95ns, so Tclk_prep 50ns */
    cfg->ckhs_prep = (ceil((50), byteclk_ns) - 1) | (0x00);
    /* Tclk_prep + Tclk_zero > 300, so Tclk_zero 320ns*/
    cfg->ckhs_zero = ceil(270, byteclk_ns) - 1;
    /* Tclk_trai > 60ns, so Tclk_trai 60ns */
    cfg->ckhs_trai = ceil((200), byteclk_ns) - 1;
    /* Ths_exit > 100ns*/
    cfg->ckhs_exit = ceil((230), byteclk_ns) - 1;

    /* dlcfg =  4 * Tlpx + Ths_prep(programmed) + Ths_zero(programmed) + 4 */
    cfg->dlcfg = cfg->lp_clkc + cfg->dhs_prep + cfg->dhs_zero + 4;

    /* ck_hs2lp = Tclk_tail + Ths_exit + 3 */
    cfg->ck_hs2lp = cfg->ckhs_trai + cfg->ckhs_exit + 3;
    /* ck_lp2hs = 4 * Tlpx + Tclk_prep + Tclk_zero + Tclk_pre(8UI) + 2byteclk */
    cfg->ck_lp2hs = cfg->lp_clkc + cfg->ckhs_prep + 1 + cfg->ckhs_zero + 1 + 1 + 2;
}

/*!
  \brief      get dsi dphy configuration
  \param[in]  lcm: lcm struct pointer
  \param[in]  cfg: dphy configuration pointer
  \param[out] none
  \retval     none
*/
static void dsi_dphy_cfg_get(lcm_info_struct *lcm, dsi_dphy_cfg_struct *cfg)
{
    uint8_t bpp = 0;

    cfg->dsi_timing.hact = lcm->hact;
    cfg->dsi_timing.vact = lcm->vact;
    cfg->format = lcm->format;
    cfg->lane_num = lcm->lane_num;
    cfg->channel = lcm->channel;
    cfg->mode = lcm->mode;

    switch(lcm->format) {
    case MIPI_DSI_FMT_RGB565:
        bpp = 16;
        break;
    case MIPI_DSI_FMT_RGB666:
        bpp = 18;
        break;
    case MIPI_DSI_FMT_RGB666_PACKED:
    case MIPI_DSI_FMT_RGB888:
    default:
        bpp = 24;
        break;
    }

    /* for hswc/hbpc/hfpc/hactc  */
    cfg->dsi_timing.hswc = (uint16_t)(lcm->hsw * ((double)hs_rate / 8 / pixel_clk));
    cfg->dsi_timing.hbpc = (uint16_t)(lcm->hbp * ((double)hs_rate / 8 / pixel_clk));
    cfg->dsi_timing.hfpc = (uint16_t)(lcm->hfp * ((double)hs_rate / 8 / pixel_clk));
    cfg->dsi_timing.hactc = (uint16_t)(lcm->hact * ((double)hs_rate / 8 / pixel_clk));

    if(lcm->vsw < 2) {
        LOG("[error]lcm vsyc width is %u < 2\r\n", lcm->vsw);
    } else {
        cfg->dsi_timing.vswc = lcm->vsw;
    }
    if(lcm->vbp < 1) {
        LOG("[error]lcm vbpc width is %u < 2\r\n", lcm->vbp);
    } else {
        cfg->dsi_timing.vbpc = lcm->vbp;
    }
    if(lcm->vfp < 1) {
        LOG("[error]lcm vfpc width is %u < 2\r\n", lcm->vfp);
    } else {
        cfg->dsi_timing.vfpc = lcm->vfp;
    }

    cfg->total_pixel_byte = bpp * cfg->dsi_timing.vact * cfg->dsi_timing.hact / 8;
    cal_dphy_timing(lpclk, hs_rate, &(cfg->dphy_timing));
}

/*!
  \brief      mipi dsi packet format is long or not
  \param[in]  type: MIPI DSI data type of the packet
  \param[out] none
  \retval     ret: true if the packet for the given data type is a long packet
*/
static bool mipi_dsi_packet_format_is_long(uint8_t type)
{
    switch(type) {
    case MIPI_DSI_PPS_LONG_WRITE:
    case MIPI_DSI_NULL_PACKET:
    case MIPI_DSI_BLANKING_PACKET:
    case MIPI_DSI_GENERIC_LONG_WRITE:
    case MIPI_DSI_DCS_LONG_WRITE:
    case MIPI_DSI_LOOSELY_PACKED_PIXEL_STREAM_YCBCR20:
    case MIPI_DSI_PACKED_PIXEL_STREAM_YCBCR24:
    case MIPI_DSI_PACKED_PIXEL_STREAM_YCBCR16:
    case MIPI_DSI_PACKED_PIXEL_STREAM_30:
    case MIPI_DSI_PACKED_PIXEL_STREAM_36:
    case MIPI_DSI_PACKED_PIXEL_STREAM_YCBCR12:
    case MIPI_DSI_PACKED_PIXEL_STREAM_16:
    case MIPI_DSI_PACKED_PIXEL_STREAM_18:
    case MIPI_DSI_PIXEL_STREAM_3BYTE_18:
    case MIPI_DSI_PACKED_PIXEL_STREAM_24:
        return true;
    }

    return false;
}

/*!
  \brief      mipi dsi hs write data
  \param[in]  msg: pointer a message to transfer
  \param[out] none
  \retval     none
*/
static void mipi_dsi_hs_write(mipi_dsi_msg_struct *msg)
{
    uint32_t fifo_st = 0U;
    uint32_t data_word = 0U;
    uint32_t i = 0U;
    uint32_t remaining_bytes = 0U;

    /* determine whether the packet format is long */
    if(mipi_dsi_packet_format_is_long(msg->type)) {
        for(i = 0U; i < msg->tx_len; i += 4U) {
            do {
                fifo_st = dsi_fifo_status_get(DSI_FIFOSTAT_HSDTFIFOF);
            } while(fifo_st != 0U);

            /* safely read 4 bytes to avoid alignment issues */
            remaining_bytes = msg->tx_len - i;
            if(remaining_bytes >= 4U) {
                data_word = dsi_safe_read_u32(msg->tx_buf, i);
            } else {
                /* handle remaining bytes */
                data_word = 0U;
                (void)memcpy(&data_word, &((const uint8_t *)msg->tx_buf)[i], remaining_bytes);
            }
            /* HS write data */
            dsi_hs_data_write(data_word);
        }

        do {
            fifo_st = dsi_fifo_status_get(DSI_FIFOSTAT_HSCTLFIFOF);
        } while(fifo_st);

        dsi_hs_control_set(msg->type, msg->channel, msg->tx_len);
    } else {
        do {
            fifo_st = dsi_fifo_status_get(DSI_FIFOSTAT_HSCTLFIFOF);
        } while(fifo_st != 0U);

        if(0U == (msg->tx_len)) {
            dsi_hs_control_set(msg->type, msg->channel, msg->tx_len);
        } else if(1U == (msg->tx_len)) {
            uint32_t byte_data = (uint32_t)dsi_safe_read_u8(msg->tx_buf, 0U);
            dsi_hs_control_set(msg->type, msg->channel, byte_data);
        } else {
            uint32_t short_data = (uint32_t)dsi_safe_read_u16(msg->tx_buf, 0U);
            dsi_hs_control_set(msg->type, msg->channel, short_data);
        }
    }
}

/*!
  \brief      mipi dsi lp write data
  \param[in]  msg: pointer a message to transfer
  \param[out] none
  \retval     none
*/
static void mipi_dsi_lp_write(mipi_dsi_msg_struct *msg)
{
    uint32_t fifo_st = 0U;
    uint32_t data_word = 0U;
    uint32_t i = 0U;
    uint32_t remaining_bytes = 0U;

    if(mipi_dsi_packet_format_is_long(msg->type)) {
        /* transmit using LP data FIFO */
        for(i = 0U; i < msg->tx_len; i += 4U) {
            do {
                fifo_st = dsi_fifo_status_get(DSI_FIFOSTAT_LPDTFIFOF);
            } while(fifo_st != 0U);

            /* safely read 4 bytes to avoid alignment issues */
            remaining_bytes = msg->tx_len - i;
            if(remaining_bytes >= 4U) {
                data_word = dsi_safe_read_u32(msg->tx_buf, i);
            } else {
                /* handle remaining bytes */
                data_word = 0U;
                (void)memcpy(&data_word, &((const uint8_t *)msg->tx_buf)[i], remaining_bytes);
            }
            dsi_lp_data_write(data_word);
        }

        /* wait for FIFO empty */
        do {
            fifo_st = dsi_fifo_status_get(DSI_FIFOSTAT_LPCTLFIFOF);
        } while(fifo_st);

        dsi_lp_control_set(msg->type, msg->channel, msg->tx_len);
    } else {
        /* wait for FIFO empty */
        do {
            fifo_st = dsi_fifo_status_get(DSI_FIFOSTAT_LPCTLFIFOF);
        } while(fifo_st != 0U);

        if(0U == (msg->tx_len)) {
            dsi_lp_control_set(msg->type, msg->channel, msg->tx_len);
        } else if(1U == (msg->tx_len)) {
            uint32_t byte_data = (uint32_t)dsi_safe_read_u8(msg->tx_buf, 0U);
            dsi_lp_control_set(msg->type, msg->channel, byte_data);
        } else {
            uint32_t short_data = (uint32_t)dsi_safe_read_u16(msg->tx_buf, 0U);
            dsi_lp_control_set(msg->type, msg->channel, short_data);
        }
    }
}

/*!
  \brief      mipi dsi write data
  \param[in]  msg: pointer a message to transfer
  \param[out] none
  \retval     none
*/
static void mipi_dsi_write(mipi_dsi_msg_struct *msg)
{
    /* DCS/Generic word count values need to be written before DCS or Generic long write */
    if(mipi_dsi_packet_format_is_long(msg->type)) {
        dsi_dcs_word_count_set(msg->tx_len);
    } else {
        dsi_dcs_word_count_set(0);
    }

    if(msg->mode) {
        mipi_dsi_lp_write(msg);
    } else {
        mipi_dsi_hs_write(msg);
    }
}

/*!
  \brief      mipi dsi hs read data
  \param[in]  msg: pointer a message to read
  \param[out] none
  \retval     none
*/
static void mipi_dsi_hs_read(mipi_dsi_msg_struct *msg)
{
    uint32_t data_word = 0U;
    uint32_t word_cnt = 0U;
    uint32_t i = 0U;
    uint32_t remaining_bytes = 0U;

    /* waiting for rx data available */
    do {
    } while(0U == lp_rd_avail);

    /* read word_cnt words */
    word_cnt = msg->rx_len / 4U;
    for(i = 0U; i < word_cnt; i++) {
        data_word = dsi_lp_data_read();
        dsi_safe_write_u32(msg->rx_buf, i * 4U, data_word);
    }

    /* need read other bytes */
    remaining_bytes = msg->rx_len % 4U;
    if(remaining_bytes != 0U) {
        data_word = dsi_hs_data_read();
        (void)memcpy(&((uint8_t *)msg->rx_buf)[word_cnt * 4U], &data_word, remaining_bytes);
    }
}

/*!
  \brief      mipi dsi lp read data
  \param[in]  msg: pointer a message to transfer
  \param[out] none
  \retval     none
*/
static void mipi_dsi_lp_read(mipi_dsi_msg_struct *msg)
{
    uint32_t data_word = 0U;
    uint32_t word_cnt = 0U;
    uint32_t i = 0U;
    uint32_t remaining_bytes = 0U;

    /* waiting for rx data available */
    do {
    } while(0U == lp_rd_avail);

    /* read word_cnt words */
    word_cnt = msg->rx_len / 4U;
    for(i = 0U; i < word_cnt; i++) {
        data_word = dsi_lp_data_read();
        dsi_safe_write_u32(msg->rx_buf, i * 4U, data_word);
    }

    /* need read other bytes */
    remaining_bytes = msg->rx_len % 4U;
    if(remaining_bytes != 0U) {
        data_word = dsi_lp_data_read();
        (void)memcpy(&((uint8_t *)msg->rx_buf)[word_cnt * 4U], &data_word, remaining_bytes);
    }
}

/*!
  \brief      mipi dsi read data
  \param[in]  msg: pointer a message to transfer
  \param[out] none
  \retval     none
*/
static void mipi_dsi_read(mipi_dsi_msg_struct *msg)
{
    if(msg->mode) {
        mipi_dsi_lp_read(msg);
    } else {
        mipi_dsi_hs_read(msg);
    }
}

/*!
  \brief      mipi dsi device transfer
  \param[in]  msg: pointer to a message to transfer
  \param[out] none
  \retval     none
*/
void mipi_dsi_device_transfer(mipi_dsi_msg_struct *msg)
{
    uint32_t data = 0;
    uint32_t stop_auto_refresh = 0;

    /* need stop autofresh and waiting for frame done */
    if(DSI_WCTL & DSI_WCTL_ARM) {
        dsi_wrapper_auto_refresh_mode_config(DSI_AUTO_REFRESH_MODE_DISABLE);
        do {
        } while(ar_mode_bus_busy);
        stop_auto_refresh = 1;
    }

    if(msg->rx_buf && msg->rx_len) {
        if(msg->mode) {
            data = 1 << 15;
        } else {
            data = 0;
        }
        data |= ((msg->rx_len) & 0x7FF);
        DSI_DCSWC = 1;
        DSI_MAXRPKTCTL = data;
    }

    /* write message */
    mipi_dsi_write(msg);

    if(msg->rx_buf && msg->rx_len) {
        mipi_dsi_read(msg);
    }

    if(msg->mdelay) {
        delay_1ms(msg->mdelay);
    }

    if(stop_auto_refresh) {
        DSI_WCTL |= DSI_WCTL_ARM;
    }
}

/*!
  \brief      lcm partial upadte
  \param[in]  start_x: start position of x
  \param[in]  end_x: end position of x
  \param[in]  start_y: start position of y
  \param[in]  end_y: end position of y
  \param[out] none
  \retval     none
*/
void lcm_partial_upadte_set(uint16_t start_x, uint16_t end_x, uint16_t start_y, uint16_t end_y)
{
    uint32_t total_byte = 0;
    uint8_t data[5] = {0};
    uint8_t bpp = 0;
    mipi_dsi_msg_struct msg = {0};

    /* Determine the number of bytes corresponding to different color formats */
    switch(lcm_info.format) {
    case MIPI_DSI_FMT_RGB565:
        bpp = 16;
        break;
    case MIPI_DSI_FMT_RGB666:
        bpp = 18;
        break;
    case MIPI_DSI_FMT_RGB666_PACKED:
    case MIPI_DSI_FMT_RGB888:
    default:
        bpp = 24;
        break;
    }

    total_byte = (end_x - start_x + 1) * (end_y - start_y + 1) * bpp / 8;
    msg.channel = lcm_info.channel;
    msg.type = MIPI_DSI_DCS_LONG_WRITE;
    msg.mode = LOW_POWER;
    msg.mdelay = 0;
    msg.tx_len = 5;

    /* set column address (x coordinate) */
    data[0] = MIPI_DCS_SET_COLUMN_ADDRESS;          /* DCS command */
    data[1] = (uint8_t)((start_x >> 8) & 0xFF);
    data[2] = (uint8_t)((start_x) & 0xFF);
    data[3] = (uint8_t)((end_x >> 8) & 0xFF);
    data[4] = (uint8_t)((end_x) & 0xFF);
    msg.tx_buf = data;
    mipi_dsi_device_transfer(&msg);

    /* set page address (y coordinate) */
    data[0] = MIPI_DCS_SET_PAGE_ADDRESS;            /* DCS command */
    data[1] = (uint8_t)((start_y >> 8) & 0xFF);
    data[2] = (uint8_t)((start_y) & 0xFF);
    data[3] = (uint8_t)((end_y >> 8) & 0xFF);
    data[4] = (uint8_t)((end_y) & 0xFF);
    msg.tx_buf = data;
    mipi_dsi_device_transfer(&msg);

    dsi_wrapper_wms_size_set((MDMA_MAX_BYTES >> 2), total_byte >> 2);
}

/*!
  \brief      dsi initialization
  \param[in]  dsi_cfg: dsi controller config
  \param[out] none
  \retval     none
*/
void dsi_dphy_init(dsi_dphy_cfg_struct *dsi_cfg)
{
    uint32_t int_flag = 0;

    /* enable DSI DPHY */
    dsi_dphy_dfe_reset_enable();
    /* enable DSI interrupt */
    dsi_interrupt_enable(DSI_INT_RXSOTE);
    dsi_interrupt_enable(DSI_INT_RXSOTSYNCE);
    dsi_interrupt_enable(DSI_INT_RXEOTSYNCE);
    dsi_interrupt_enable(DSI_INT_RXESCMEE);
    dsi_interrupt_enable(DSI_INT_RXLPTXSYNCE);
    dsi_interrupt_enable(DSI_INT_RXPTOE);
    dsi_interrupt_enable(DSI_INT_RXFCTLE);
    dsi_interrupt_enable(DSI_INT_RXECCSE);
    dsi_interrupt_enable(DSI_INT_RXECCME);
    dsi_interrupt_enable(DSI_INT_RXCHKE);
    dsi_interrupt_enable(DSI_INT_RXDSIDTNR);
    dsi_interrupt_enable(DSI_INT_RXDSIVCIDINV);
    dsi_interrupt_enable(DSI_INT_TXFCTLE);
    dsi_interrupt_enable(DSI_INT_TXECCSE);
    dsi_interrupt_enable(DSI_INT_TXECCME);
    dsi_interrupt_enable(DSI_INT_TXCHKE);
    dsi_interrupt_enable(DSI_INT_TXDSIDTNR);
    dsi_interrupt_enable(DSI_INT_TXDSIVCIDINV);
    dsi_interrupt_enable(DSI_INT_HCONT);
    dsi_interrupt_enable(DSI_INT_LCONT);
    dsi_interrupt_enable(DSI_INT_HSTXTO);
    dsi_interrupt_enable(DSI_INT_LPRXTO);
    dsi_interrupt_enable(DSI_INT_TAACKTO);
    dsi_interrupt_enable(DSI_INT_ACKNE);
    dsi_interrupt_enable(DSI_INT_RXINVTXCE);
    dsi_interrupt_enable(DSI_INT_RXDSIPV);
    dsi_interrupt_enable(DSI_INT_SPKTCMDS);
    dsi_interrupt_enable(DSI_INT_RXCONTDET);
    dsi_interrupt_enable(DSI_INT_DPILTO);
    dsi_interrupt_enable(DSI_INT_DPIPE);
    dsi_wrapper_interrupt_enable(DSI_INT_PLL_UNLOCK);

    /* disable eot short packet */
    dsi_eot_transmission_disable();
    /* disable clock in blank packet */
    dsi_clock_stopping_enable();

    /* DSI data line number set */
    dsi_data_lanes_number_set(dsi_cfg->lane_num);
    /* 0:disable command mode, 1:enable DSC or generic transfer */
    dsi_command_mode_supported_set(0x01);
    /* sel low power data from ip */
    dsi_wrapper_lp_cmd_data_source_set(DSI_LP_DATASRC_LPDATA);
    DSI_WCTL &= (~(DSI_WCTL_CLM | DSI_WCTL_SHTD));
    /* DSI function program register */
    if(COMMAND_MODE == dsi_cfg->mode) {
        dsi_command_mode_virtual_channel_set(dsi_cfg->channel);
        /* select DMA for DSI */
        dsi_wrapper_tli_dma_mode_config(DSI_TLI_DMA_MODE_ENABLE);
        /* 0: disable video mode */
        dsi_video_mode_color_format_set(0x00);
        /* hs data from wrapper */
        dsi_wrapper_hs_cmd_data_source_set(DSI_HS_DATASRC_TLI_DMA);
        dsi_wrapper_wms_size_set((MDMA_MAX_BYTES >> 2), ((dsi_cfg->total_pixel_byte) >> 2));
        /* attention te_cfg */
        DSI_WCTL |= DSI_WCTL_TESRC;
        DSI_WINTEN |= DSI_WINTEN_TEIE;
        /* wms end interrupt enable */
        DSI_WWMS_INTMSK |= DSI_WWMS_WCTL_WMS_END;
    } else {
        /* select DMA disable for DSI */
        dsi_wrapper_tli_dma_mode_config(DSI_TLI_DMA_MODE_DISABLE);

        dsi_video_mode_virtual_channel_set(dsi_cfg->channel);
        /* DSI wrapper format set */
        dsi_wrapper_rgb_data_format_set(dsi_cfg->format);
        dsi_video_mode_color_format_set(dsi_cfg->format);
        /* DSI video mode format */
        dsi_video_mode_format_set(dsi_cfg->mode);
        /* must disable bta */
        dsi_video_bta_disable();
    }

    /* DSI high speed tranmission timeout */
    dsi_hs_tx_timeout_set(0xFFFFFF);
    /* DSI low power reception timeout */
    dsi_lp_rx_timeout_set(0xFFFFFF);
    /* DSI turn around timeout */
    dsi_turnaround_timeout_set(0x1F);
    /* DSI device reset timer */
    dsi_device_reset_timer_set(0xFFFF);
    /* DSI DPI resolution */
    dsi_dpi_resolution_set(dsi_cfg->dsi_timing.hact, dsi_cfg->dsi_timing.vact);

    /* DSI horizontal sync active count configuration */
    dsi_horizontal_sync_active_count_set(dsi_cfg->dsi_timing.hswc);
    /* DSI horizontal front porch count configuration */
    dsi_horizontal_front_porch_count_set(dsi_cfg->dsi_timing.hfpc);
    /* DSI horizontal back porch count configuration */
    dsi_horizontal_back_porch_count_set(dsi_cfg->dsi_timing.hbpc);
    /* DSI horizontal active area count configuration */
    dsi_horizontal_active_count_set(dsi_cfg->dsi_timing.hactc);

    /* DSI vertical sync active count configuration */
    dsi_vertical_sync_active_count_set(dsi_cfg->dsi_timing.vswc);
    /* DSI vertical front porch count configuration */
    dsi_vertical_front_porch_count_set(dsi_cfg->dsi_timing.vfpc);
    /* DSI vertical back porch count configuration */
    dsi_vertical_back_porch_count_set(dsi_cfg->dsi_timing.vbpc);

    /* DSI initialization count configuration */
    dsi_init_counter_set(0xD07);
    /* DSI error auto recovery control */
    dsi_error_auto_recovery_disable(DSI_ERR_AUTO_REC_ECC_MUL);
    dsi_error_auto_recovery_disable(DSI_ERR_AUTO_REC_INVLD_DT);
    dsi_error_auto_recovery_disable(DSI_ERR_AUTO_REC_HI_CONT);
    dsi_error_auto_recovery_disable(DSI_ERR_AUTO_REC_LO_CONT);
    dsi_error_auto_recovery_disable(DSI_ERR_AUTO_REC_HS_RX_TO);
    dsi_error_auto_recovery_disable(DSI_ERR_AUTO_REC_LP_RX_TO);

    /* DSI Dphy parameter configuration */
    dsi_dphy_hs_prepare_time_set(dsi_cfg->dphy_timing.dhs_prep);
    dsi_dphy_hs_zero_time_set(dsi_cfg->dphy_timing.dhs_zero);
    dsi_dphy_hs_trail_time_set(dsi_cfg->dphy_timing.dhs_trai);
    dsi_dphy_hs_exit_time_set(dsi_cfg->dphy_timing.dhs_exit);

    /* DSI Dphy clock lane timing parameter configuration */
    dsi_dphy_clk_prepare_time_set(dsi_cfg->dphy_timing.ckhs_prep);
    dsi_dphy_clk_zero_time_set(dsi_cfg->dphy_timing.ckhs_zero);
    dsi_dphy_clk_trail_time_set(dsi_cfg->dphy_timing.ckhs_trai);
    dsi_dphy_clk_exit_time_set(dsi_cfg->dphy_timing.ckhs_exit);

    /* DSI Dphy trim1 control */
    DSI_DPHY_TRIMCTL1 = ((dsi_cfg->dphy_timing.pll_divider) & 0x7F) |
                        (((uint32_t)(dsi_cfg->dphy_timing.esc_ck_sel) & 0x1) << 31);
    DSI_DPHY_TRIMCTL1 |= DSI_DPHY_TRIMCTL1_CDPD0;

    /* DSI frequency mode control */
    dsi_ddr_freq_mode_set(dsi_cfg->dphy_timing.freq_mode);
    /* DSI low power byte clock configuration */
    dsi_lp_byte_clk_config(dsi_cfg->dphy_timing.lp_clkc);

    /* DSI data lane high low switch count configuration */
    dsi_data_lane_switch_count_set(dsi_cfg->dphy_timing.dlcfg);
    /*  DSI clock lane switching configuration */
    dsi_clk_hs_to_lp_switch_count_set(dsi_cfg->dphy_timing.ck_hs2lp);
    dsi_clk_lp_to_hs_switch_count_set(dsi_cfg->dphy_timing.ck_lp2hs);

    /* DSI device ready */
    dsi_device_ready_set(0x1);

    /* waiting for initialization done */
    do {
        int_flag = DSI_INTSTAT0;
    } while(0 == (DSI_INTSTAT0_INIT_DONE & int_flag));
}

/*!
  \brief      read register
  \param[in]  addr: address
  \param[in]  len: length
  \param[out] none
  \retval     none
*/
void lcm_reg_read(uint8_t addr, uint8_t len)
{
    uint8_t tx_para = 0;
    uint32_t read_val = 0;

    tx_para = addr;
    mipi_dsi_msg_struct rx_msg = {
        .channel = 0,
        .type = MIPI_DSI_GENERIC_READ_REQUEST_1_PARAM,
        .mode = LOW_POWER,
        .mdelay = 0,
        .tx_len = 1,
        .tx_buf = &tx_para,
        .rx_len = len,
        .rx_buf = &read_val,
    };

    mipi_dsi_device_transfer(&rx_msg);
}

/*!
  \brief      test LCM DSI DPHY
  \param[in]  none
  \param[out] none
  \retval     none
*/
void dsi_dphy_lcm_test(void)
{
#ifdef LCM_TEST
    uint32_t i = 0;
    uint32_t len_cmd = sizeof(init_cmds) / sizeof(init_cmds[0]);
    mipi_dsi_msg_struct msg = {0};

    dsi_dphy_cfg_get(&lcm_info, &dsi_cfg);

    rcu_periph_clock_enable(BACKLIGHT_GPIO_CLK);
    gpio_mode_set(BACKLIGHT_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, BACKLIGHT_PIN);
    gpio_output_options_set(BACKLIGHT_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_100_220MHZ, BACKLIGHT_PIN);
    gpio_bit_reset(BACKLIGHT_GPIO_PORT, BACKLIGHT_PIN);

    rcu_periph_clock_enable(LCD_RST_GPIO_CLK);
    gpio_mode_set(LCD_RST_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, LCD_RST_PIN);
    gpio_output_options_set(LCD_RST_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_100_220MHZ, LCD_RST_PIN);
    gpio_bit_set(LCD_RST_GPIO_PORT, LCD_RST_PIN);

    /* res = 1, mdelay 1ms */
    gpio_bit_set(LCD_RST_GPIO_PORT, LCD_RST_PIN);
    delay_1ms(4);
    /* res = 0, mdelay 10ms */
    gpio_bit_reset(LCD_RST_GPIO_PORT, LCD_RST_PIN);
    delay_1ms(12);
    /* res = 1, mdelay 120ms */
    gpio_bit_set(LCD_RST_GPIO_PORT, LCD_RST_PIN);
    delay_1ms(120);

    /* backlight */
    gpio_bit_set(BACKLIGHT_GPIO_PORT, BACKLIGHT_PIN);

    if(COMMAND_MODE == dsi_cfg.mode) {
        /* enable dsi te */
        rcu_periph_clock_enable(DSI_TE_GPIO_CLK);
        gpio_af_set(DSI_TE_GPIO_PORT, DSI_TE_AFIO, DSI_TE_PIN);
        gpio_mode_set(DSI_TE_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, DSI_TE_PIN);
        gpio_output_options_set(DSI_TE_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_60MHZ, DSI_TE_PIN);
    }

    dsi_dphy_init(&dsi_cfg);

    for(i = 0; i < len_cmd; i++) {
        msg.channel = lcm_info.channel;
        msg.type = init_cmds[i].type;
        msg.mode = init_cmds[i].is_lp;
        msg.mdelay = init_cmds[i].mdelay;
        msg.tx_len = init_cmds[i].len;
        msg.tx_buf = init_cmds[i].data;

        /* transfer DSI device message */
        mipi_dsi_device_transfer(&msg);
    }
#endif  /* LCM_TEST */
}

/*!
  \brief      configure dsi command mode supported function
  \param[in]  supported: dsi host command mode supported
  \param[out] none
  \retval     none
  */
void dsi_command_mode_supported_set(uint32_t supported)
{
    uint32_t reg_val = 0;
    if(0 == supported) {
        reg_val = (DSI_FUNCPRG) & (~DSI_FUNCPRG_SDTWCM);
        DSI_FUNCPRG = reg_val;
    } else {
        reg_val = (DSI_FUNCPRG) & (~DSI_FUNCPRG_SDTWCM);
        DSI_FUNCPRG = ((1 << 13) & DSI_FUNCPRG_SDTWCM) | reg_val;
    }
}

/*!
  \brief      dsi_wrapper_rgb_data_format_set
  \param[in]  format: RGB data format for wrapper
  \param[out] none
  \retval     none
  */
void dsi_wrapper_rgb_data_format_set(uint32_t format)
{
    uint32_t tli_data_format = 0;

    switch(format) {
    case MIPI_DSI_FMT_RGB565:
        tli_data_format = 1;
        break;
    case MIPI_DSI_FMT_RGB666:
        tli_data_format = 2;
        break;
    case MIPI_DSI_FMT_RGB666_PACKED:
    case MIPI_DSI_FMT_RGB888:
    default:
        tli_data_format = 0;
        break;
    }

    DSI_WRGBDF = tli_data_format;
}

/*!
  \brief      dsi irq0 handler
  \param[in]  none
  \param[out] none
  \retval     none
  */
static void dsi_irq0_handler(void)
{
    uint32_t int_st = 0;
    uint32_t int_en = 0;
    uint32_t mask = 0;

    int_st = DSI_INTSTAT0;
    int_en = DSI_INTEN;
    mask = int_st & int_en;

    if(!mask) {
        return;
    }

    if(mask & DSI_INTSTAT0_RXSOTE) {
        LOG("Rx Sot Error Interrupt \r\n");
    }

    if(mask & DSI_INTSTAT0_RXSOTSYNCE) {
        LOG("Rx Sot Sync Error Interrupt \r\n");
    }

    if(mask & DSI_INTSTAT0_RXEOTSYNCE) {
        LOG("Rx Eot Sync Error Interrupt \r\n");
    }

    if(mask & DSI_INTSTAT0_RXESCMEE) {
        LOG("Rx Escape Mode Entry Error Interrupt");
    }

    if(mask & DSI_INTSTAT0_RXLPTXSYNCE) {
        LOG("Rx LP Tx Sync Error Interrupt \r\n");
    }

    if(mask & DSI_INTSTAT0_RXPTOE) {
        LOG("Rx Peripheral Timeout Error Interrupt \r\n");
    }

    if(mask & DSI_INTSTAT0_RXFCTLE) {
        LOG("Rx False Control Error Interrupt \r\n");
    }

    if(mask & DSI_INTSTAT0_RXECCSE) {
        LOG("Rx ECC Single Bit Error Interrupt \r\n");
    }

    if(mask & DSI_INTSTAT0_RXECCME) {
        LOG("Rx ECC Multi Bit Error Interrupt \r\n");
    }

    if(mask & DSI_INTSTAT0_RXCHKE) {
        LOG("Rx Checksum Error Interrupt \r\n");
    }

    if(mask & DSI_INTSTAT0_RXDSIDTNR) {
        LOG("Rx DSI Data Type Not Recognized Interrupt \r\n");
    }

    if(mask & DSI_INTSTAT0_RXDSIVCIDINV) {
        LOG("Rx DSI VC ID Invalid Interrupt \r\n");
    }

    if(mask & DSI_INTSTAT0_TXFCTLE) {
        LOG("Tx False Control Error Interrupt \r\n");
    }

    if(mask & DSI_INTSTAT0_TXECCSE) {
        LOG("Tx ECC Single Bit Error Interrupt \r\n");
    }

    if(mask & DSI_INTSTAT0_TXECCME) {
        LOG("Tx ECC Multi Bit Error Interrupt \r\n");
    }

    if(mask & DSI_INTSTAT0_TXCHKE) {
        LOG("Tx Checksum Error Interrupt \r\n");
    }

    if(mask & DSI_INTSTAT0_TXDSIDTNR) {
        LOG("Tx DSI Data Type Not Recognized Interrupt \r\n");
    }

    if(mask & DSI_INTSTAT0_TXDSIVCIDINV) {
        LOG("Tx DSI VC ID Invalid Interrupt \r\n");
    }

    if(mask & DSI_INTSTAT0_HCONT) {
        LOG("High Contention Interrupt \r\n");
    }

    if(mask & DSI_INTSTAT0_LCONT) {
        LOG("Low Contention Interrupt \r\n");
    }

    if(mask & DSI_INTSTAT0_FIFOE) {
        LOG("Fifo Empty Interrupt \r\n");
    }

    if(mask & DSI_INTSTAT0_HSTXTO) {
        LOG("HS Tx Timeout Interrupt \r\n");
    }

    if(mask & DSI_INTSTAT0_LPRXTO) {
        LOG("LP Rx Timeout Interrupt \r\n");
    }

    if(mask & DSI_INTSTAT0_TAACKTO) {
        LOG("Turn_around Ack Timeout Interrupt \r\n");
    }

    if(mask & DSI_INTSTAT0_ACKNE) {
        LOG("ACK With No Error Interrupt \r\n");
    }

    if(mask & DSI_INTSTAT0_RXINVTXCE) {
        LOG("Rx Invalid Transmission Count Error Interrupt \r\n");
    }

    if(mask & DSI_INTSTAT0_RXDSIPV) {
        LOG("Rx DSI Protocol Violation Interrupt \r\n");
    }

    if(mask & DSI_INTSTAT0_SPKTCMDS) {
        LOG("Special Packet Command Sent Interrupt \r\n");
    }

    if(mask & DSI_INTSTAT0_INIT_DONE) {
        LOG("Init Done Interrupt \r\n");
    }

    if(mask & DSI_INTSTAT0_RXCONTDET) {
        LOG("Rx Contention Detected Interrupt \r\n");
    }

    if(mask & DSI_INTSTAT0_DPILTO) {
        LOG("DPI Line Timeout Interrupt \r\n");
    }

    if(mask & DSI_INTSTAT0_DPIPE) {
        LOG("DPI Program Error Interrupt \r\n");
    }

    DSI_INTSTAT0 = mask;

    return;
}

/*!
  \brief      dsi irq1 handler
  \param[in]  none
  \param[out] none
  \retval     none
*/
static void dsi_irq1_handler(void)
{
    uint32_t int_st = 0;

    int_st = DSI_INTSTAT1;

    if(!int_st) {
        return;
    }

    if(int_st & DSI_INTSTAT1_HSDTFIFOFIF) {
        LOG("HS data fifo full\r\n");
    }

    if(int_st & DSI_INTSTAT1_LPDTFIFOFIF) {
        LOG("LP data fifo full\r\n");
    }

    if(int_st & DSI_INTSTAT1_LPCTLFIFOFIF) {
        LOG("LP ctrl fifo full\r\n");
    }

    if(int_st & DSI_INTSTAT1_HSCTLFIFOFIF) {
        LOG("HS ctrl fifo full\r\n");
    }

    if(int_st & DSI_INTSTAT1_RDDTAIF) {
        lp_rd_avail = 1;
    }

    if(int_st & DSI_INTSTAT1_TETGIF) {
        LOG("TE trigger message received\r\n");
    }

    DSI_INTSTAT1 = int_st;
}

/*!
  \brief      dsi irq2 handler
  \param[in]  none
  \param[out] none
  \retval     none
*/
static void dsi_irq2_handler(void)
{
    uint32_t int_st = 0;
    uint32_t int_en = 0;
    uint32_t mask = 0;

    int_st = DSI_WINTSTAT;
    int_en = DSI_WINTEN;
    mask = int_st & int_en;

    if(!mask) {
        return;
    }

    if(mask & DSI_WINTSTAT_TEIF) {
        if(DSI_WCTL & DSI_WCTL_ARM) {
            /* waiting for te to start, and bus busy */
            ar_mode_bus_busy = 1;
        }
    }

    if(mask & DSI_WINTSTAT_PLLLKIF) {
        LOG("PLL lock\r\n");
    }

    if(mask & DSI_WINTSTAT_PLLULKIF) {
        LOG("PLL unlock\r\n");
    }

    DSI_WINTC = mask;
}

/*!
  \brief      dsi irq3 handler
  \param[in]  none
  \param[out] none
  \retval     none
*/
static void dsi_irq3_handler(void)
{
    uint32_t int_st = 0;
    uint32_t int_en = 0;
    uint32_t mask = 0;

    int_en = DSI_WWMS_INTMSK;
    int_st = DSI_WWMS_WCTL;
    mask = int_en & int_st;

    if(!mask) {
        return;
    }

    if(mask & DSI_WWMS_WCTL_WMS_END) {
        /* frame end, and bus idle */
        ar_mode_bus_busy = 0;
        DSI_WWMS_WCTL |= DSI_WWMS_WCTL_WMS_END;
    }

    return;
}

/*!
  \brief      mipi dsi irq handler
  \param[in]  none
  \param[out] none
  \retval     none
*/
void mipi_dsi_irq_handler(void)
{
    dsi_irq0_handler();
    dsi_irq1_handler();
    dsi_irq2_handler();
    dsi_irq3_handler();
}
