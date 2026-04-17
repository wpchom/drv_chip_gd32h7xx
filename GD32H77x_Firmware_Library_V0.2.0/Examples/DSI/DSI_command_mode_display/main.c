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
#include "img_320_240.h"
#include "logo_320_240.h"

void cache_enable(void);
void tli_clk_config(void);
void tli_partial_upadte_set(uint16_t start_x, uint16_t end_x, uint16_t start_y, uint16_t end_y);
void lcm_command_partial_fb_update(uint16_t start_x, uint16_t end_x, uint16_t start_y, uint16_t end_y);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
#if (PIXEL_FOMART == DSI_FMT_RGB565)
    static uint8_t i = 0;
#endif
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

#ifdef LCM_COMMAND_MODE_TEST
    lcm_command_partial_fb_update(0, (ACTIVE_WIDTH - 1), 0, (ACTIVE_HEIGHT - 1));
    DSI_WCTL |= DSI_WCTL_ARM;
    ar_mode_bus_busy = 1;
#endif
#endif /* LCM_TEST*/
    delay_1ms(2000);

    while(1) {
#if (PIXEL_FOMART == DSI_FMT_RGB565)
        /* wait for frame end and bus idle */
        while(ar_mode_bus_busy == 1);
        if(0 == i) {
            i = 1;
            TLI_LXFBADDR(LAYER0) = (uint32_t)gImage_logo_320_240;
        } else {
            i = 0;
            TLI_LXFBADDR(LAYER0) = (uint32_t)gImage_img_320_240;
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

/*!
    \brief      configure TLI partial update
    \param[in]  start_x: start pos of x
    \param[in]  end_x: end pos of x
    \param[in]  start_y: start pos of y
    \param[in]  end_y: end pos of y
    \param[out] none
    \retval     none
*/
void tli_partial_upadte_set(uint16_t start_x, uint16_t end_x, uint16_t start_y, uint16_t end_y)
{
    /* configure layer packeted pixel format */
    TLI_LXPPF(LAYER0) &= ~(TLI_LXPPF_PPF);
#if PIXEL_FOMART == DSI_FMT_RGB888
    TLI_LXPPF(LAYER0) = LAYER_PPF_RGB888;
#elif PIXEL_FOMART == DSI_FMT_RGB565
    TLI_LXPPF(LAYER0) = LAYER_PPF_RGB565;
#endif

    /* configure layer frame buffer base address */
    TLI_LXFBADDR(LAYER0) &= ~(TLI_LXFBADDR_FBADD);

    /* configure layer frame line length */
    TLI_LXFLLEN(LAYER0) &= ~(TLI_LXFLLEN_FLL | (TLI_LXFLLEN_STDOFF));
#if PIXEL_FOMART == DSI_FMT_RGB888
    TLI_LXFBADDR(LAYER0) = (uint32_t)&logo + (ACTIVE_WIDTH * start_y + start_x) * 3;
    TLI_LXFLLEN(LAYER0) = (uint32_t)(((uint32_t)(end_x - start_x + 1) * 3 + 7) | (((uint32_t)ACTIVE_WIDTH * 3) << 16U));
#elif PIXEL_FOMART == DSI_FMT_RGB565
    TLI_LXFBADDR(LAYER0) = (uint32_t)&logo + (ACTIVE_WIDTH * start_y + start_x) * 2;
    TLI_LXFLLEN(LAYER0) = (uint32_t)(((uint32_t)(end_x - start_x + 1) * 2 + 7) | (((uint32_t)ACTIVE_WIDTH * 2) << 16U));
#endif

    /* configure layer frame total line number */
    TLI_LXFTLN(LAYER0) &= ~(TLI_LXFTLN_FTLN);
    TLI_LXFTLN(LAYER0) = (uint32_t)(end_y - start_y + 1);

    // enable layer
    tli_layer_enable(LAYER0);

    // reload layer configuration
    tli_reload_config(TLI_REQUEST_RELOAD_EN);
}

/*!
    \brief      configure LCM command partial frame buffer update
    \param[in]  start_x: start pos of x
    \param[in]  end_x: end pos of x
    \param[in]  start_y: start pos of y
    \param[in]  end_y: end pos of y
    \param[out] none
    \retval     none
*/
void lcm_command_partial_fb_update(uint16_t start_x, uint16_t end_x, uint16_t start_y, uint16_t end_y)
{
    tli_partial_upadte_set(start_x, end_x, start_y, end_y);
    lcm_partial_upadte_set(start_x, end_x, start_y, end_y);
}
