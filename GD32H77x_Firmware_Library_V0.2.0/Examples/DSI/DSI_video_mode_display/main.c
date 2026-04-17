/*!
    \file    main.c
    \brief   DSI LCD example

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
#include "systick.h"
#include <stdio.h>
#include "gd32h77di_eval.h"
#include "dsi_lcd.h"
#include "img800_480.h"

extern const uint8_t logo[];

void cache_enable(void);
void tli_clk_config(void);
void tli_config(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    static uint8_t i = 0;
    /* enable the CPU Cache */
    cache_enable();
    /* configure systick */
    systick_config();

    /* mipi dsi pmu configuration */
    pmu_mipi_ldo_enable();
    while(1) {
        if(RESET != pmu_flag_get(PMU_FLAG_RDYF)) {
            break;
        }
    };
    pmu_mipi_power_enable();
    pmu_mipi_output_isolation_enable();

    /* deinit DSI and configure DSI clock */
    dsi_deinit();
    dsi_clk_config();

    nvic_irq_enable(DSI_IRQn, 2, 0);
    dsi_dphy_lcm_test();

#ifdef LCM_TEST
    tli_clk_config();

#ifdef LCM_VIDEO_MODE_TEST
    tli_config();
    tli_enable();
#endif
#endif /* LCM_TEST*/
    delay_1ms(2000);

    while(1) {
#if (PIXEL_FOMART == DSI_FMT_RGB565)
        if(0 == i) {
            i = 1;
            TLI_LXFBADDR(LAYER0) = (uint32_t)logo;
        } else {
            i = 0;
            TLI_LXFBADDR(LAYER0) = (uint32_t)gImage_img800_480;
        }
        tli_reload_config(TLI_REQUEST_RELOAD_EN);
        delay_1ms(2000);
#endif
    }
}

/*!
    \brief      enable the CPU Cache
    \param[in]  none
    \param[out] none
    \retval     none
*/
void cache_enable(void)
{
    /* Enable I-Cache */
    SCB_EnableICache();

    /* Enable D-Cache */
    SCB_EnableDCache();
}

/*!
    \brief      TLI clock configuration
    \param[in]  none
    \param[out] none
    \retval     none
*/
void tli_clk_config(void)
{
    /* configure PLLSAI to generate TLI clock */
    RCU_PLLALL &= ~(RCU_PLLALL_PLLSEL | RCU_PLLALL_PLL2VCOSEL | RCU_PLLALL_PLL2RNG);
    RCU_PLLALL |= (RCU_PLLSRC_HXTAL | RCU_PLL2VCO_150M_420M | RCU_PLL2RNG_1M_2M);
#ifdef LCM_COMMAND_MODE_TEST
    if(ERROR == rcu_pll2_config(25U, 400U, 1U, 1U, 50U)) {
        while(1);
    }
    rcu_tli_clock_div_config(RCU_PLL2R_DIV2);
#elif defined (LCM_VD240x320_TEST)
    if(ERROR == rcu_pll2_config(25U, 400U, 1U, 1U, 40U)) {
        while(1);
    }
    rcu_tli_clock_div_config(RCU_PLL2R_DIV2);
#elif defined (LCM_VD480x800_TEST)
    if(ERROR == rcu_pll2_config(25U, 260U, 1U, 1U, 10U)) {
        while(1);
    }
    rcu_tli_clock_div_config(RCU_PLL2R_DIV2);
#elif defined (LCM_VD720x1280_TEST)
    if(ERROR == rcu_pll2_config(25U, 400U, 1U, 1U, 4U)) {
        while(1);
    }
    rcu_tli_clock_div_config(RCU_PLL2R_DIV2);
#endif
    rcu_pll_clock_output_enable(RCU_PLL2P);
    rcu_pll_clock_output_enable(RCU_PLL2Q);
    rcu_pll_clock_output_enable(RCU_PLL2R);
    rcu_osci_on(RCU_PLL2_CK);
    if(ERROR == rcu_osci_stab_wait(RCU_PLL2_CK)) {
        while(1);
    }

    rcu_periph_clock_enable(RCU_TLI);
}

void tli_config(void)
{
    {
        tli_parameter_struct               tli_init_struct;
        tli_layer_parameter_struct         tli_layer_init_struct;

        {
            // configure TLI parameter struct
            tli_init_struct.signalpolarity_hs = TLI_HSYN_ACTLIVE_LOW;
            tli_init_struct.signalpolarity_vs = TLI_VSYN_ACTLIVE_LOW;
            tli_init_struct.signalpolarity_de = TLI_DE_ACTLIVE_LOW;
            tli_init_struct.signalpolarity_pixelck = TLI_PIXEL_CLOCK_TLI;
            // LCD display timing configuration
            tli_init_struct.synpsz_hpsz     = HORIZONTAL_SYNCHRONOUS_PULSE - 1;
            tli_init_struct.backpsz_hbpsz   = HORIZONTAL_SYNCHRONOUS_PULSE + HORIZONTAL_BACK_PORCH - 1;
            tli_init_struct.activesz_hasz   = HORIZONTAL_SYNCHRONOUS_PULSE + HORIZONTAL_BACK_PORCH + ACTIVE_WIDTH - 1;
            tli_init_struct.totalsz_htsz    = HORIZONTAL_SYNCHRONOUS_PULSE + HORIZONTAL_BACK_PORCH + ACTIVE_WIDTH + HORIZONTAL_FRONT_PORCH - 1;
            tli_init_struct.synpsz_vpsz     = VERTICAL_SYNCHRONOUS_PULSE - 1;
            tli_init_struct.backpsz_vbpsz   = VERTICAL_SYNCHRONOUS_PULSE + VERTICAL_BACK_PORCH - 1;
            tli_init_struct.activesz_vasz   = VERTICAL_SYNCHRONOUS_PULSE + VERTICAL_BACK_PORCH + ACTIVE_HEIGHT - 1;
            tli_init_struct.totalsz_vtsz    = VERTICAL_SYNCHRONOUS_PULSE + VERTICAL_BACK_PORCH + ACTIVE_HEIGHT + VERTICAL_FRONT_PORCH - 1;
            // configure LCD background R,G,B values
            tli_init_struct.backcolor_red   = 0xff;
            tli_init_struct.backcolor_green = 0x00;
            tli_init_struct.backcolor_blue  = 0x00;
            tli_init(&tli_init_struct);
        }

        // TLI layer0 configuration
        {
            // TLI window size configuration
            tli_layer_init_struct.layer_window_leftpos      = (0       + HORIZONTAL_SYNCHRONOUS_PULSE + HORIZONTAL_BACK_PORCH);
            tli_layer_init_struct.layer_window_rightpos     = (0 + ACTIVE_WIDTH + HORIZONTAL_SYNCHRONOUS_PULSE + HORIZONTAL_BACK_PORCH - 1);
            tli_layer_init_struct.layer_window_toppos       = (0        + VERTICAL_SYNCHRONOUS_PULSE + VERTICAL_BACK_PORCH);
            tli_layer_init_struct.layer_window_bottompos    = (0 + ACTIVE_HEIGHT   + VERTICAL_SYNCHRONOUS_PULSE + VERTICAL_BACK_PORCH - 1);
#if PIXEL_FOMART == DSI_FMT_RGB888
            tli_layer_init_struct.layer_ppf = LAYER_PPF_RGB888;
#elif PIXEL_FOMART == DSI_FMT_RGB565
            tli_layer_init_struct.layer_ppf = LAYER_PPF_RGB565;
#endif
            // TLI window specified alpha configuration
            tli_layer_init_struct.layer_sa = 255;
            // TLI layer default alpha R,G,B value configuration
            tli_layer_init_struct.layer_default_alpha   = 0x0;
            tli_layer_init_struct.layer_default_red     = 0x0;
            tli_layer_init_struct.layer_default_green   = 0x0;
            tli_layer_init_struct.layer_default_blue    = 0x0;
            // TLI window blend configuration
            tli_layer_init_struct.layer_acf1 = LAYER_ACF1_PASA;
            tli_layer_init_struct.layer_acf2 = LAYER_ACF2_PASA;
            // TLI layer frame buffer base address configuration
            tli_layer_init_struct.layer_frame_bufaddr           = (uint32_t)&logo;
#if PIXEL_FOMART == DSI_FMT_RGB888
            tli_layer_init_struct.layer_frame_line_length       = ((ACTIVE_WIDTH * 3) + 7);
            tli_layer_init_struct.layer_frame_buf_stride_offset = ((ACTIVE_WIDTH * 3));
#elif PIXEL_FOMART == DSI_FMT_RGB565
            tli_layer_init_struct.layer_frame_line_length       = ((ACTIVE_WIDTH * 2) + 7);
            tli_layer_init_struct.layer_frame_buf_stride_offset = ((ACTIVE_WIDTH * 2));
#endif
            tli_layer_init_struct.layer_frame_total_line_number = ACTIVE_HEIGHT;
            tli_layer_init(LAYER0, &tli_layer_init_struct);
        }
    }

    tli_dither_config(TLI_DITHER_DISABLE);

    nvic_irq_enable(TLI_ER_IRQn, 0, 0);
    tli_interrupt_enable(TLI_INT_FE);   //usually means that TLI request more data then data source can give, should lower the TLI clock
    tli_interrupt_enable(TLI_INT_TE);   //can't access address

    // enable layer
    tli_layer_enable(LAYER0);

    // reload layer configuration
    tli_reload_config(TLI_REQUEST_RELOAD_EN);
}
