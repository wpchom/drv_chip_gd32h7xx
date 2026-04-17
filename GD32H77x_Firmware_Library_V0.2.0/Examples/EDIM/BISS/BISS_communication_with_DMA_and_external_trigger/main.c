/*!
    \file    main.c
    \brief   EDIM_BISS communication with DMA and external trigger example

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
    /* enable TRIGSEL clock */
    rcu_periph_clock_enable(RCU_TRIGSEL);
    /* enable TIMER0 clock */
    rcu_periph_clock_enable(RCU_TIMER0);
    /* enable DMA/DMAMUX clock */
    rcu_periph_clock_enable(RCU_DMA0);
    rcu_periph_clock_enable(RCU_DMAMUX);
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
    
    /* configure EDIM_BISS auto transmission trigger mode: TRIGSEL */
    edim_biss_auto_transmission_config(EDIM_BISS_AUTO_DATA_TRANSMIT_EXT_TRIGGER);
    /* enable EDIM_BISS slave0 */
    edim_biss_slave_enable(EDIM_BISS_SLAVE0);
    /* select EDIM_BISS DMA data source */
    edim_biss_dma_select(EDIM_BISS_DMA_SEL_SLV0_DATA0);
    /* enable EDIM_BISS DMA */
    edim_biss_dma_enable();
    /* send initialization sequence */
    edim_biss_initialization_sequence_config(ENABLE);
    delay_1ms(1);
    /* send EDIM_BISS frame */
    edim_biss_send_frame(EDIM_BISS_SEND_AUTO_DATA_FRAME_WITH_CDM_1);
}

/*!
    \brief      configure TIMER peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void timer_config(void)
{
    /* -----------------------------------------------------------------------
    TIMER0 configuration:
    - generate 1 PWM signals.
    - TIMER0 frequency is fixed to systemcoreclock, TIMER0 prescaler is equal to
      (299+1), so the TIMER0 counter frequency is 1MHz.
    ----------------------------------------------------------------------- */
    timer_parameter_struct timer_initpara;
    timer_oc_parameter_struct timer_ocinitpara;

    timer_deinit(TIMER0);
    /* TIMER0 configuration */
    timer_struct_para_init(&timer_initpara);
    timer_initpara.prescaler         = 299;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.period            = 1000;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER0, &timer_initpara);

    /* initialize TIMER channel output parameter struct */
    timer_channel_output_struct_para_init(&timer_ocinitpara);
    /* CH0 configuration in PWM0 mode */
    timer_ocinitpara.outputstate  = TIMER_CCX_ENABLE;
    timer_ocinitpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocinitpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
    timer_ocinitpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
    timer_ocinitpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
    timer_ocinitpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
    timer_channel_output_config(TIMER0, TIMER_CH_0, &timer_ocinitpara);

    timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_0, 500);
    timer_channel_output_mode_config(TIMER0, TIMER_CH_0, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER0, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);
    timer_channel_primary_output_config(TIMER0, TIMER_CH_0, ENABLE);
    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER0);
    /* TIMER0 primary output function enable */
    timer_primary_output_config(TIMER0, ENABLE);
}

/*!
    \brief      configure TRIGSEL peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void trigsel_config(void)
{
    trigsel_init(TRIGSEL_OUTPUT_BISS, TRIGSEL_INPUT_TIMER0_CH0);
    trigsel_register_lock_set(TRIGSEL_OUTPUT_BISS);
}

/*!
    \brief      configure DMA peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dma_config(void)
{
    dma_single_data_parameter_struct dma_init_struct;

    dma_deinit(DMA0);
    dma_channel_deinit(DMA0, DMA_CH0);
    dma_single_data_para_struct_init(&dma_init_struct);
    dma_init_struct.request             = DMA_REQUEST_BISS;
    dma_init_struct.direction           = DMA_PERIPH_TO_MEMORY;
    dma_init_struct.memory0_addr        = (uint32_t)(&pos);
    dma_init_struct.memory_inc          = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_32BIT;
    dma_init_struct.number              = 1;
    dma_init_struct.periph_addr         = (uint32_t)(&EDIM_BISS_FIFODATA);
    dma_init_struct.periph_inc          = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.priority            = DMA_PRIORITY_ULTRA_HIGH;
    dma_single_data_mode_init(DMA0, DMA_CH0, &dma_init_struct);
    /* configure DMA mode */
    dma_circulation_enable(DMA0, DMA_CH0);
    dma_channel_enable(DMA0, DMA_CH0);
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
    /* EDIM_BISS configuration */
    edim_biss_config();
    /* TRIGSEL configuration */
    trigsel_config();
    /* DMA configuration */
    dma_config();
    /* TIMER configuration */
    timer_config();
    /* COM port configuration */
    gd_eval_com_init(EVAL_COM);

    /* TIMER0 counter enable */
    timer_enable(TIMER0);

    /* D-Cache Invalidate by address */
    SCB_InvalidateDCache_by_Addr((uint32_t *)&pos, 32);

    while(1) {
        delay_1ms(1000);
        printf("\r\n EDIM_BISS encoder 0 process data = 0x%08X \r\n", pos);
    }
}

