/*!
    \file    main.c
    \brief   RDCM simulation position get example

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

/* I/Q channel simulink data*/
uint32_t simulink_data[128] = {
    864498567, 864105345, 862925679, 860959569, 858207015, 854668017, 850408112, 845361763, 839528970, 833040807, 825831737, 817901760,
    809250876, 800010159, 790114072, 779628152, 768552399, 756886813, 744696931, 731982753, 718875353, 705309194, 691284276, 676931673,
    662185848, 647177875, 631907754, 616441022, 600712142, 584917725, 568926697, 552870132, 536813567, 520757002, 504700437, 488709409,
    472914992, 457186112, 441719380, 426449259, 411441286, 396695461, 382342858, 368317940, 354751781, 341644381, 328930203, 316740321,
    305074735, 293998982, 283513062, 273616975, 264376258, 255725374, 247795397, 240586327, 234098164, 228265371, 223219022, 218959117,
    215420119, 212667565, 210701455, 209521789, 209128567, 209521789, 210701455, 212667565, 215420119, 218959117, 223219022, 228265371,
    234098164, 240586327, 247795397, 255725374, 264376258, 273616975, 283513062, 293998982, 305074735, 316740321, 328930203, 341644381,
    354751781, 368317940, 382342858, 396695461, 411441286, 426449259, 441719380, 457186112, 472914992, 488709409, 504700437, 520757002,
    536813567, 552870132, 568926697, 584917725, 600712142, 616441022, 631907754, 647177875, 662185848, 676931673, 691284276, 705309194,
    718875353, 731982753, 744696931, 756886813, 768552399, 779628152, 790114072, 800010159, 809250876, 817901760, 825831737, 833040807,
    839528970, 845361763, 850408112, 854668017, 858207015, 860959569, 862925679, 864105345
};

/* position data */
uint32_t pos_data = 0U;

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
    \brief      configure RCU peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_config(void)
{
    /* enable RDCM clock */
    rcu_periph_clock_enable(RCU_RDCM);
    /* enable TRIGSEL clock */
    rcu_periph_clock_enable(RCU_TRIGSEL);
    /* enable TIMER1 clock */
    rcu_periph_clock_enable(RCU_TIMER1);
    /* enable ADC0 clock */
    rcu_periph_clock_enable(RCU_ADC0);
    /* enable DMA clock */
    rcu_periph_clock_enable(RCU_DMA0);
    rcu_periph_clock_enable(RCU_DMAMUX);
}

/*!
    \brief      configure TIMER0
    \param[in]  none
    \param[out] none
    \retval     none
*/
void timer_config(void)
{
    /* ----------------------------------------------------------------------------
    TIMER1 Configuration: 
    TIMER1CLK = 300MHz.
    the period is 0.5 us, the frequence is 2MHz.
    ---------------------------------------------------------------------------- */
    timer_parameter_struct timer_initpara;
    timer_struct_para_init(&timer_initpara);
    
    timer_deinit(TIMER1);
    
    /* TIMER1 configuration */
    timer_initpara.prescaler         = 0;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 149;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER1,&timer_initpara);
    
    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER1);
    /* select the master slave mode */
    timer_master_slave_mode_config(TIMER1, TIMER_MASTER_SLAVE_MODE_ENABLE);
    /* TIMER1 update event is used as trigger output */
    timer_master_output0_trigger_source_select(TIMER1, TIMER_TRI_OUT0_SRC_UPDATE);
    /* enable TIMER1 dma */
    timer_dma_enable(TIMER1, TIMER_DMA_UPD);
}

/*!
    \brief      configure TRIGSEL
    \param[in]  none
    \param[out] none
    \retval     none
*/
void trigsel_config(void)
{
    /* deinit TRIGSEL */
    trigsel_deinit();
    /* configure TRIGSEL input and output */
    trigsel_init(TRIGSEL_OUTPUT_RDCM_NCO, TRIGSEL_INPUT_TIMER1_TRGO0);
    trigsel_init(TRIGSEL_OUTPUT_RDCM_MAINCAL, TRIGSEL_INPUT_TIMER1_TRGO0);
    trigsel_register_lock_set(TRIGSEL_OUTPUT_RDCM_NCO);
    trigsel_register_lock_set(TRIGSEL_OUTPUT_RDCM_MAINCAL);
}

/*!
    \brief      configure DMA
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dma_config(void)
{
    /* RDCM DMA configuration */
    /* RDCM_DMA_channel configuration */
    dma_single_data_parameter_struct dma_single_data_parameter;

    /* ADC DMA_channel configuration */
    dma_deinit(DMA0);

    /* initialize DMA single data mode */
    dma_single_data_parameter.request = DMA_REQUEST_TIMER1_UP;
    dma_single_data_parameter.periph_addr = (uint32_t)(&RDCM_SIMDI);
    dma_single_data_parameter.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_single_data_parameter.memory0_addr = (uint32_t)(simulink_data);
    dma_single_data_parameter.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_single_data_parameter.periph_memory_width = DMA_PERIPH_WIDTH_32BIT;
    dma_single_data_parameter.direction = DMA_MEMORY_TO_PERIPH;
    dma_single_data_parameter.number = 128;
    dma_single_data_parameter.priority = DMA_PRIORITY_HIGH;
    dma_single_data_mode_init(DMA0, DMA_CH0, &dma_single_data_parameter);

    /* enable DMA circulation mode */
    dma_circulation_enable(DMA0, DMA_CH0);

    /* enable DMA channel */
    dma_channel_enable(DMA0, DMA_CH0);
}

/*!
    \brief      configure RDCM
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rdcm_config(void)
{
    /* RDCM trigger parameter */
    rdcm_trigger_parameter_struct trigger_init;
    /* RDCM excitor output parameter */
    rdcm_excitor_output_parameter_struct exc_init;
    /* RDCM reference frequence mixer parameter */
    rdcm_rfm_parameter_struct rfm_init;
    /* RDCM demodulator parameter */    
    rdcm_demodulator_parameter_struct dem_init;
    /* RDCM track loop parameter */
    rdcm_track_loop_parameter_struct trackloop_init;
    adc_clock_config(ADC0, ADC_CLK_SYNC_HCLK_DIV4);
    /* configure RDCM trigger */
    trigger_init.cal_trigger_edge = RDCM_CALETM_RISING;
    trigger_init.cal_trigger_sel = RDCM_CALSEL_EXTERNAL_TRIGGER;
    trigger_init.nco_trigger_sel = RDCM_NCOETM_RISING;
    rdcm_trigger_config(&trigger_init);
    
    /* configure RDCM excitor: frequence is 2MHz*128/16384 = 15625Hz, initial phase is pi/2 */
    exc_init.exc_amp = 4095;
    exc_init.exc_phase = 4096;
    exc_init.exc_fre = 128;
    exc_init.exc_div = 0;
    exc_init.exc_en = RDCM_EXC_ENABLE;
    rdcm_excitor_output_config(&exc_init);
    
    /* configure RDCM reference frequence mixer */
    rfm_init.i_hf_mode = RDCM_IHFM_DISABLE;
    rfm_init.i_rfm_fre = 128;
    rfm_init.i_rfm_phase = 4096;
    rfm_init.i_zc_mode = RDCM_IZCDMS_DISABLE;
    rfm_init.i_zc_th = 64;
    rfm_init.i_zc_th_var = 2;
    rfm_init.i_average = 7;
    rfm_init.i_rfm_en = RDCM_RFM_ENABLE;
    
    rfm_init.q_hf_mode = RDCM_QHFM_DISABLE;
    rfm_init.q_rfm_fre = 128;
    rfm_init.q_rfm_phase = 4096;
    rfm_init.q_zc_mode = RDCM_QZCDMS_DISABLE;
    rfm_init.q_zc_th = 64;
    rfm_init.q_zc_th_var = 2;
    rfm_init.q_average = 7;
    rfm_init.q_rfm_en = RDCM_RFM_ENABLE;
    rdcm_reference_frequency_mixer_config(&rfm_init);
    rdcm_channel_data_offset_config(RDCM_CHANNEL_I, 8191);
    rdcm_channel_data_offset_config(RDCM_CHANNEL_Q, 8191);
    
    /*configure RDCM demodulator */
    dem_init.i_cic3_gain = 4295;
    dem_init.i_cic3_shift = 31;
    dem_init.i_cic3_dec = 99;
    dem_init.i_cic3_val = 99;
    dem_init.i_fir1_dec = RDCM_FIR1_DEC_2;
    dem_init.i_cic3_en = RDCM_CIC3_ENABLE;
    dem_init.i_dem_en = RDCM_DEM_ENABLE;
    dem_init.i_fir0_en = RDCM_FIR0_ENABLE;
    dem_init.i_fir1_en = RDCM_FIR1_ENABLE;
    dem_init.q_cic3_gain = 4295;
    dem_init.q_cic3_shift = 31;
    dem_init.q_cic3_dec = 99;
    dem_init.q_cic3_val = 99;
    dem_init.q_fir1_dec = RDCM_FIR1_DEC_2;
    dem_init.q_cic3_en = RDCM_CIC3_ENABLE;
    dem_init.q_dem_en = RDCM_DEM_ENABLE;
    dem_init.q_fir0_en = RDCM_FIR0_ENABLE;
    dem_init.q_fir1_en = RDCM_FIR1_ENABLE;
    rdcm_demodulator_config(&dem_init);
    
    /* configure RDCM track loop */
    trackloop_init.npm_x = 16384;
    trackloop_init.npm_y = 0;
    trackloop_init.trk_gain = 0x1394FF16;
    trackloop_init.position_int_gain = 0x18;
    trackloop_init.velocity_int_gain = 0x18; 
    trackloop_init.trk_cmp_pole = 0x3FE7;
    trackloop_init.trk_cmp_zero = 0X3FFE;
    trackloop_init.velocity_smooth_factor = 0;
    trackloop_init.trk_en = RDCM_TRK_ENABLE;
    rdcm_track_loop_config(&trackloop_init);

    /* configure channel data input source: simulation */
    rdcm_channel_data_input_source_config(RDCM_DISEL_SIMULATION);
    /* enable RDCM */
    rdcm_enable();
    
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
    /* configure RCU */
    rcu_config();
    /* systick configuration */
    systick_config();
    /* configure EVAL_COM */
    gd_eval_com_init(EVAL_COM);
    /* TRIGSEL configuration */
    trigsel_config();
    /* DMA configuration */
    dma_config();
    /* TIMER configuration */
    timer_config();
    /* RDCM configuration */
    rdcm_config();

    /* enable TIMER1 counter */
    timer_enable(TIMER1);
    
    /* D-Cache Invalidate by address */
    SCB_InvalidateDCache_by_Addr((uint32_t *)&pos_data, 32);
    
    while(1) {
        delay_1ms(1000);
        /* read position data data */
        pos_data = rdcm_position_read();     
        printf("\n rdcm position data is: %.1f degree\r\n", (pos_data/16384.0f*360.0f));
    }
}
