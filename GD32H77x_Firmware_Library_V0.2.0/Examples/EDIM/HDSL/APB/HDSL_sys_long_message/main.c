/*!
    \file    main.c
    \brief   EDIM_HDSL long meesage get with free running mode example

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

uint64_t long_msg_data = 0;
uint8_t data[4] = {0};

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
    
    /* PLL1P = CK_HXTAL / 5 * 90 / 1 = 450 MHz */
    RCU_PLL1 &= ~(RCU_PLL1_PLL1PSC | RCU_PLL1_PLL1N | RCU_PLL1_PLL1P | RCU_PLL1_PLL1R);
    RCU_PLL1 |= ((89 << 6U) | (5U) | (0U << 16U) |
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
    /* configure EDIM_HDSL clock */
    rcu_hdsl_clock_config(RCU_HDSL_PLL1P);
    rcu_hdsl_clock_div_config(RCU_HDSL_DIV6);
    /* enable EDIM_HDSL clock */
    rcu_periph_clock_enable(RCU_HDSL);
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
    \brief      configure EDIM_HDSL peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void edim_hdsl_config(void)
{

    /* select EDIM IO: EIDM_HDSL */
    syscfg_coder_io_select(SYSCFG_CODER_IO_HDSL);
    /* configure EDIM_HDSL interface: APB BUS */
    edim_hdsl_interface_select(EDIM_HDSL_DRIVE, EDIM_HDSL_BUS_INTERFACE);
    edim_hdsl_interface_select(EDIM_HDSL_SAFE_CHANNEL1, EDIM_HDSL_BUS_INTERFACE);
    edim_hdsl_interface_select(EDIM_HDSL_SAFE_CHANNEL2, EDIM_HDSL_BUS_INTERFACE);
    /* init EDIM_HDSL */
    edim_hdsl_init(0, EDIM_HDSL_SYNC_LEADING, 0x1F);
}

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    uint32_t try_count = 0;
    edim_hdsl_long_msg_addr_struct long_init;

    /* enable the CPU Cache */
    cache_enable();
    /* systick configuration */
    systick_config();
    /* RCU configuration */
    rcu_config();
    /* GPIO port configuration */
    gpio_config();
    /* EDIM_HDSL configuration */
    edim_hdsl_config();
    /* COM port configuration */
    gd_eval_com_init(EVAL_COM);
    
    /* link HDSL encoder */
    edim_hdsl_output_enable();
    delay_1ms(500);
    edim_hdsl_protocol_reset(DISABLE);
    delay_1ms(30);
    
    for (try_count = 0; try_count < 1000; try_count++)
    {
        if (SET == edim_hdsl_protocol_connection_status_get())
        {
            break;
        }
    }
    
    if (try_count >= 1000)
    {
        printf("\r\n EDIM_HDSL link failed\r\n");
        while(1);    
    }

    SCB_InvalidateDCache_by_Addr((uint32_t *)&long_msg_data, 32);
    
    /* configure EDIM_HDSL long message */
    long_init.direction = EDIM_HDSL_LONG_MSG_READ;
    long_init.d_i_mode = EDIM_HDSL_LONG_MSG_DIRECT;
    long_init.offset = EDIM_HDSL_LONG_MSG_WITHOUT_OFFSET;
    long_init.len = EDIM_HDSL_LONG_MSG_LEN_4;
    long_init.laddh = 00;
    long_init.laddl = 0x00;
    long_init.ladd_off_h = 0x00;
    long_init.ladd_off_l = 0x00;
    edim_hdsl_long_message_addr_init(&long_init);
    edim_hdsl_long_message_transaction_start();
    
    /* read HDSL encoder long message */
    do{
        edim_hdsl_dummy_read();
        edim_hdsl_dummy_read();
    }while(RESET == edim_hdsl_online_d_flag_get(EDIM_HDSL_ONLINE_D_FLAG_FREL));
    long_msg_data = edim_hdsl_long_message_read();

    data[0] = (uint8_t)long_msg_data;
    data[1] = (uint8_t)(long_msg_data >> 8);
    data[2] = (uint8_t)(long_msg_data >> 16);
    data[3] = (uint8_t)(long_msg_data >> 24);
    
    while(1) {
        delay_1ms(1000);
        printf("\r\n EDIM_HDSL encoder 0 long message ASCII code = %s\r\n", data);
    }
}

