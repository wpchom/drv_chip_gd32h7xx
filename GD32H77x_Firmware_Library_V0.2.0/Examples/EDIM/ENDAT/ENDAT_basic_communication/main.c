/*!
    \file    main.c
    \brief   EDIM_ENDAT basic communication example

    \version 2025-09-29, V0.2.0, firmware for GD32H77x
*/

/*
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
#include "gd32h77ei_eval.h"

uint32_t pos;

/*!
    \brief      enable the CPU Chache
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
    \brief      configure PLL1 clock
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pll1_config(void)
{
    RCU_PLLALL &= ~(RCU_PLLALL_PLL1VCOSEL | RCU_PLLALL_PLL1RNG);
    RCU_PLLALL |= RCU_PLL1RNG_4M_8M;
    
    /* PLL1P = CK_HXTAL / 5 * 100 / 1 = 500 MHz */
    RCU_PLL1 &= ~(RCU_PLL1_PLL1PSC | RCU_PLL1_PLL1N | RCU_PLL1_PLL1P | RCU_PLL1_PLL1R);
    RCU_PLL1 |= ((99 << 6U) | (5U) | (0U << 16U) |
                 (0U << 24U));

    /* enable PLL1P, PLL1R */
    RCU_PLLADDCTL |= RCU_PLLADDCTL_PLL1PEN | RCU_PLLADDCTL_PLL1REN;

    /* enable PLL1 */
    RCU_CTL |= RCU_CTL_PLL1EN;

    /* wait until PLL0 is stable */
    while(0U == (RCU_CTL & RCU_CTL_PLL1STB)) {
    }
}

/*!
    \brief      configure rcu peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_config(void)
{   
    /* configure pll1 clock */
    pll1_config();
    /* configure EDIM_ENDAT clock */
    rcu_encoder_clock_config(RCU_ENCODER_PLL1R);
    rcu_endat_clock_div_config(RCU_ENDAT_DIV5);
    /* enable EDIM_ENDAT clock */
    rcu_periph_clock_enable(RCU_ENDAT);
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_GPIOE);
    /* enable SYSCFG clock */
    rcu_periph_clock_enable(RCU_SYSCFG);
}

/*!
    \brief      configure GPIO peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_config(void)
{
    /* configure EDIM_DI(PD8), EDIM_DE(PC11), EDIM_DO(PE7) EDIM_TCLK(PB3) GPIO */
    gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_7);
    gpio_af_set(GPIOE, GPIO_AF_11, GPIO_PIN_7);
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_11);
    gpio_af_set(GPIOC, GPIO_AF_4, GPIO_PIN_11);
    gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_8);
    gpio_af_set(GPIOD, GPIO_AF_5, GPIO_PIN_8);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_3);
    gpio_af_set(GPIOB, GPIO_AF_16, GPIO_PIN_3);
}

/*!
    \brief      configure EDIM_ENDAT peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void edim_endat_config(void)
{
    edim_endat_parameter_struct endat_config;
    syscfg_coder_io_select(SYSCFG_CODER_IO_ENDAT);

    /* configure EDIM_ENDAT */ 
    endat_config.clk_rate = EDIM_ENDAT_TCLK_1MHz;
    endat_config.recovery_time = EDIM_ENDAT_RECOVERY_III_2us;
    endat_config.filter = EDIM_ENDAT_FILTER_6CYCLE;
    endat_config.watchdog_value = EDIM_ENDAT_TOTH_STOP;
    
    edim_endat_init(&endat_config);
    
    /* enable EDIM_ENDAT cable propagation compensation */
    edim_endat_cable_propagation_compensation_enable();
    /* configure EDIM_ENDAT data length */
    edim_endat_data_length(0x1F);
    /* select EDIM_ENDAT update data condition */
    edim_endat_update_unconditional_data_enable();
    /* enable EDIM_ENDAT auto reset */
    edim_endat_auto_reset_enable();
    /* disable EDIM_ENDAT continue clock */
    edim_endat_continue_clock_disable();
}

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* enable the CPU Cache */
    cache_enable();
    /* systick configuration */
    systick_config();
    /* RCU configuration */
    rcu_config();
    /* GPIO port configuration */
    gpio_config();
    /* EDIM_ENDAT configuration */
    edim_endat_config();
    /* COM port configuration */
    gd_eval_com_init(EVAL_COM);

    /* D-Cache Invalidate by address */
    SCB_InvalidateDCache_by_Addr((uint32_t *)&pos, 32);

    /* send reset command to encoder */
    edim_endat_command(EDIM_ENDAT_MODE_RECEIVE_RESET, 0, 0);
    edim_endat_data_transmission_software_trigger();
    delay_1ms(50);
    
    /* send position data command to encoder */
    edim_endat_command(EDIM_ENDAT_MODE_GET_POS, 0, 0);

    while(1) {
        /* software trigger EDIM_ENDAT data transmission */
        edim_endat_data_transmission_software_trigger();
        while(RESET == edim_endat_flag_get(EDIM_ENDAT_FLAG_NEXTRDY));
        pos = edim_endat_data_low_read();
        delay_1ms(1000);
        printf("\r\n EDIM_ENDAT encoder 0 position data = 0x%08X \r\n", pos);
    }
}

