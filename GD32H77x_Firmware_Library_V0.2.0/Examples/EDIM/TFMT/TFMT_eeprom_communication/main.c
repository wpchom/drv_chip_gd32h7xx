/*!
    \file    main.c
    \brief   EDIM_TFMT eeprom communication example

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

#define ROM_ADDR        0x3E
#define ROM_DATA        0x5D

uint32_t rdata_buffer_0;
uint32_t rdata_buffer_1;

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
    
    /* PLL1P = CK_HXTAL / 5 * 96 / 1 = 480 MHz */
    RCU_PLL1 &= ~(RCU_PLL1_PLL1PSC | RCU_PLL1_PLL1N | RCU_PLL1_PLL1P | RCU_PLL1_PLL1R);
    RCU_PLL1 |= ((95 << 6U) | (5U) | (0U << 16U) |
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
    /* configure EDIM_TFMT clock */
    rcu_encoder_clock_config(RCU_ENCODER_PLL1R);
    rcu_tfmt_clock_div_config(RCU_TFMT_DIV6);
    /* enable EDIM_TFMT clock */
    rcu_periph_clock_enable(RCU_TFMT);
    /* enable GPIO clock */
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
    /* configure EDIM_DI(PD8), EDIM_DE(PC11), EDIM_DO(PE7) GPIO */
    gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_7);
    gpio_af_set(GPIOE, GPIO_AF_11, GPIO_PIN_7);
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_11);
    gpio_af_set(GPIOC, GPIO_AF_4, GPIO_PIN_11);
    gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_8);
    gpio_af_set(GPIOD, GPIO_AF_5, GPIO_PIN_8);
}

/*!
    \brief      configure EDIM_TFMT peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void edim_tfmt_config(void)
{
    /* select encoder io is EDIM_TFMT */
    syscfg_coder_io_select(SYSCFG_CODER_IO_TFMT);
    /* enable EDIM_TFMT interrupt */
    edim_tfmt_interrupt_enable(EDIM_TFMT_INT_EOC);
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
    /* EDIM_TFMT configuration */
    edim_tfmt_config();
    /* COM port configuration */
    gd_eval_com_init(EVAL_COM);
    
    SCB_InvalidateDCache_by_Addr((uint32_t *)&rdata_buffer_0, 32);
    SCB_InvalidateDCache_by_Addr((uint32_t *)&rdata_buffer_1, 32);
    
    /* send EDIM_TFMT eeprom write command */
    edim_tfmt_command_with_eeprom_write(ROM_ADDR, ROM_DATA);
    edim_tfmt_software_trigger();
    delay_1ms(1);
    rdata_buffer_0 = edim_tfmt_data_read(EDIM_TFMT_RDATA1);
    
    /* confirm writing to eeprom is normally completed */
    while(edim_tfmt_flag_get(EDIM_TFMT_FLAG_EEPROMBUSY));
    edim_tfmt_command_with_eeprom_read(ROM_ADDR);
    edim_tfmt_software_trigger();
    delay_1ms(1);
    
    /* send EDIM_TFMT eeprom read command */
    edim_tfmt_command_with_eeprom_read(ROM_ADDR);
    edim_tfmt_software_trigger();
    delay_1ms(1);
    rdata_buffer_1 = edim_tfmt_data_read(EDIM_TFMT_RDATA1);
    
    while(edim_tfmt_flag_get(EDIM_TFMT_FLAG_EEPROMBUSY));

    while(1) {
        delay_1ms(1000);
        printf("\r\n EDIM_TFMT encoder data register 1 in eeprom write mode: 0x%08X \r\n", rdata_buffer_0);
        printf("\r\n EDIM_TFMT encoder data register 1 in eeprom read mode: 0x%08X \r\n", rdata_buffer_1);
    }
}

