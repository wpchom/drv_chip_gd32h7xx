/*!
    \file    main.c
    \brief   ADC0 channel differential mode

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
#include "gd32h77ei_eval.h"

#define BUFFER_SIZE           32
__attribute__((aligned(32))) uint32_t adc_value;
float volt_value;

/* configure the different system clocks */
void rcu_config(void);
/* configure the DMA peripheral */
void dma_config(void);
/* configure the ADC peripheral */
void adc_config(void);
/* enable the CPU cache */
void cache_enable(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* enable the CPU cache */
    cache_enable();
    /* system clocks configuration */
    rcu_config();
    /* systick configuration */
    systick_config();
    /* configure COM port */
    gd_eval_com_init(EVAL_COM);
    /* DMA configuration */
    dma_config();
    /* ADC configuration */
    adc_config();

    while(1) {
        delay_1ms(1000);
        /* D-Cache Invalidate by address */
        SCB_InvalidateDCache_by_Addr((uint32_t *)&adc_value, BUFFER_SIZE);
        volt_value = (2 * 3.3f * adc_value) / 0x3fff - 3.3f;
        printf("\r ******  Channel IN10 differential mode  ******\r\n");
        printf(" ADC0 sampling data    = 0x%04X \r\n", adc_value);
        printf(" ADC0 sampling voltage = %5.3fV \r\n", volt_value);
    }
}

/*!
    \brief      configure the different system clocks
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_config(void)
{
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* enable ADC clock */
    rcu_periph_clock_enable(RCU_ADC0);
    /* enable DMA clock */
    rcu_periph_clock_enable(RCU_DMA0);
    rcu_periph_clock_enable(RCU_DMAMUX);
}

/*!
    \brief      configure the DMA peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dma_config(void)
{
    /* ADC_DMA_channel configuration */
    dma_single_data_parameter_struct dma_single_data_parameter;

    /* ADC DMA_channel configuration */
    dma_channel_deinit(DMA0, DMA_CH0);

    /* initialize DMA single data mode */
    dma_single_data_para_struct_init(&dma_single_data_parameter);
    dma_single_data_parameter.request = DMA_REQUEST_ADC0_ROUTINE;
    dma_single_data_parameter.periph_addr = (uint32_t)(&ADC_RDATA(ADC0));
    dma_single_data_parameter.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_single_data_parameter.memory0_addr = (uint32_t)(&adc_value);
    dma_single_data_parameter.memory_inc = DMA_MEMORY_INCREASE_DISABLE;
    dma_single_data_parameter.periph_memory_width = DMA_PERIPH_WIDTH_32BIT;
    dma_single_data_parameter.direction = DMA_PERIPH_TO_MEMORY;
    dma_single_data_parameter.number = 1;
    dma_single_data_parameter.priority = DMA_PRIORITY_HIGH;
    dma_single_data_mode_init(DMA0, DMA_CH0, &dma_single_data_parameter);

    /* enable DMA circulation mode */
    dma_circulation_enable(DMA0, DMA_CH0);

    /* enable DMA channel */
    dma_channel_enable(DMA0, DMA_CH0);
}

/*!
    \brief      configure the ADC peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void adc_config(void)
{
    /* reset ADC */
    adc_deinit(ADC0);
    /* ADC clock config */
    adc_clock_config(ADC0, ADC_CLK_SYNC_HCLK_DIV6);
    /* ADC contineous function enable */
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, ENABLE);
    /* ADC scan mode enable */
    adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);
    /* ADC data alignment config */
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);

    /* ADC channel length config */
    adc_channel_length_config(ADC0, ADC_ROUTINE_CHANNEL, 1);

    /* ADC routine channel config */
    adc_routine_channel_config(ADC0, 0, ADC_CHANNEL_0, 240);

    /* ADC trigger config */
    adc_external_trigger_config(ADC0, ADC_ROUTINE_CHANNEL, EXTERNAL_TRIGGER_DISABLE);

    /* ADC routine channel differential mode configure */
    adc_channel_differential_mode_config(ADC0, ADC_DIFFERENTIAL_MODE_CHANNEL_0, ENABLE);

    /* ADC DMA function enable */
    adc_dma_request_after_last_enable(ADC0, ADC_ROUTINE_CHANNEL);
    adc_dma_mode_enable(ADC0, ADC_ROUTINE_CHANNEL);

    /* enable ADC interface */
    adc_enable(ADC0);
    /* wait for ADC stability */
    delay_1ms(1);
    /* ADC calibration number config */
    adc_calibration_number(ADC0, ADC_CALIBRATION_NUM1);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC0);

    /* ADC software trigger enable */
    adc_software_trigger_enable(ADC0, ADC_ROUTINE_CHANNEL);
}

/*!
    \brief      enable the CPU cache
    \param[in]  none
    \param[out] none
    \retval     none
*/
void cache_enable(void)
{
    /* enable i-cache */
    SCB_EnableICache();
    /* enable d-cache */
    SCB_EnableDCache();
}
