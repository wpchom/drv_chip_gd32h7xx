/*!
    \file    main.c
    \brief   EDIM_AFMT eeprom and id communication example

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

#define MEA             0
#define ROM_ADDR        0x7E
#define ROM_DATA        0x2557
#define ID_MEA          0x123456

uint32_t rdata_buffer_0[2];
uint32_t rdata_buffer_1[2];
uint32_t rdata_buffer_2[2];

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
    /* configure EDIM_AFMT clock */
    rcu_encoder_clock_config(RCU_ENCODER_PLL1R);
    rcu_afmt_clock_div_config(RCU_AFMT_DIV6);
    /* enable EDIM_AFMT clock */
    rcu_periph_clock_enable(RCU_AFMT);
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
    \brief      configure EDIM_AFMT peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void edim_afmt_config(void)
{
    edim_afmt_parameter_struct afmt_p;
    syscfg_coder_io_select(SYSCFG_CODER_IO_AFMT);
    
    /* init EDIM_AFMT baud and t2 parameter */
    edim_afmt_struct_para_init(&afmt_p);
    edim_afmt_init(&afmt_p);
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
    /* EDIM_AFMT configuration */
    edim_afmt_config();
    /* COM port configuration */
    gd_eval_com_init(EVAL_COM);

    SCB_InvalidateDCache_by_Addr((uint32_t *)&rdata_buffer_0, 32);
    SCB_InvalidateDCache_by_Addr((uint32_t *)&rdata_buffer_1, 32);
    SCB_InvalidateDCache_by_Addr((uint32_t *)&rdata_buffer_2, 32);
    
    /* send eeprom write command */
    edim_afmt_command_with_eeprom_write(EDIM_AFMT_ADDR(MEA), ROM_ADDR, ROM_DATA);
    edim_afmt_software_trigger();
    delay_1ms(1);
    for(uint8_t i = 0; i < 3; i++) {
        rdata_buffer_0[i] = edim_afmt_data_read(EDIM_AFMT_SLAVE0, i);
    }

    /* send eeprom read command */
    edim_afmt_command_with_eeprom_read(EDIM_AFMT_ADDR(MEA), ROM_ADDR);
    edim_afmt_software_trigger();
    delay_1ms(1);
    for(uint8_t i = 0; i < 3; i++) {
        rdata_buffer_1[i] = edim_afmt_data_read(EDIM_AFMT_SLAVE0, i);
    }

    /* send id write command */
    edim_afmt_command_with_id_write(EDIM_AFMT_ADDR(MEA), EDIM_AFMT_CMD_CC_IDWRI,ID_MEA);
    edim_afmt_software_trigger();
    delay_1ms(1);
    for(uint8_t i = 0; i < 3; i++) {
        rdata_buffer_2[i] = edim_afmt_data_read(EDIM_AFMT_SLAVE0, i);
    }

    printf("\r\n EDIM_AFMT encoder 0 eeprom write data register 0 = 0x%08X \r\n", rdata_buffer_0[0]);
    printf("\r\n EDIM_AFMT encoder 0 eeprom write data register 1 = 0x%08X \r\n", rdata_buffer_0[1]);
    printf("\r\n EDIM_AFMT encoder 0 eeprom read data register 0 = 0x%08X \r\n", rdata_buffer_1[0]);
    printf("\r\n EDIM_AFMT encoder 0 eeprom read data register 1 = 0x%08X \r\n", rdata_buffer_1[1]);
    printf("\r\n EDIM_AFMT encoder 0 id write data register 1 = 0x%08X \r\n", rdata_buffer_2[0]);
    printf("\r\n EDIM_AFMT encoder 0 id write data register 2 = 0x%08X \r\n", rdata_buffer_2[1]);
    
    while(1) {
        
    }
}

