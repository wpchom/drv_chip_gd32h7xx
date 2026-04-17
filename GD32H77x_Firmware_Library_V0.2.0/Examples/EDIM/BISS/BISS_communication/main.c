/*!
    \file    main.c
    \brief   EDIM_BISS communication example

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

uint32_t pos = 0;

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
    \brief      configure rcu peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_config(void)
{   
    /* enable EDIM_AFMT clock */
    rcu_periph_clock_enable(RCU_BISS);
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOD);
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
    /* configure EDIM_DI(PD8), EDIM_TCLK(PB3) GPIO */
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_3);
    gpio_af_set(GPIOB, GPIO_AF_16, GPIO_PIN_3);
    gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_8);
    gpio_af_set(GPIOD, GPIO_AF_5, GPIO_PIN_8);
}

/*!
    \brief      configure EDIM_BISS peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void edim_biss_config(void)
{
    edim_biss_init_parameter_struct biss_init;
    syscfg_coder_io_select(SYSCFG_CODER_IO_BISS_C);
    /* break edim_biss transmission */
    edim_biss_break();
    delay_1ms(40);
    
    /* configure EDIM_BISS process data main clock, slave number and MA line control */ 
    biss_init.ma_perscaler = 74;
    biss_init.n_slave = 1;
    biss_init.ma_force_control_level = EDIM_BISS_MA_NO_FORCE_CONTROL;
    biss_init.slave_config[0].crc_init_value = 0;
    biss_init.slave_config[0].crc_poly = EDIM_BISS_SLAVE_CRC_NO_CRC;
    biss_init.slave_config[0].data_length = 32;
    edim_biss_init(&biss_init);
    
    /* enable EDIM_BISS slave0 */
    edim_biss_slave_enable(EDIM_BISS_SLAVE0);
    /* configure EDIM_BISS counter value */
    edim_biss_frame_count_value_config(6000);
    /* send initialization sequence */
    edim_biss_initialization_sequence_config(ENABLE);
    delay_1ms(1);
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
    /* COM port configuration */
    gd_eval_com_init(EVAL_COM);
    /* EDIM_BISS configuration */
    edim_biss_config();
    
    /* D-Cache Invalidate by address */
    SCB_InvalidateDCache_by_Addr((uint32_t *)&pos, 32);

    while(1) {
        /* send EDIM_BISS frame */
        edim_biss_send_frame(EDIM_BISS_SEND_SINGLE_DATA_FRAME_WITH_CDM_1);
        delay_1ms(1000);
        pos = edim_biss_data_read(0);
        printf("\r\n EDIM_BISS encoder 0 process data = 0x%08X \r\n", pos);
    }
}

