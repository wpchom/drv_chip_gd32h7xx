/*!
    \file    main.c
    \brief   calculate sin by dma mode example

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

/* the value of pi */
#define DEMO_PI             (3.14159265f)
#define TMU_WRITE_ADDRESS   ((uint32_t)0x48024404U)
#define TMU_READ_ADDRESS    ((uint32_t)0x48024408U)

/* the first input data: angle */
float theta = DEMO_PI/2.0f;
/* the second input data: modulus */
float m = 100.0f;
/* the calculation results: result=m*sin(theta) */
float result = 0;

/* TMU input data in q31 format */
__attribute__ ((aligned(32))) uint32_t in_data_q31[2] = {0};
/* TMU output data in q31 format */
__attribute__ ((aligned(32))) uint32_t out_data_q31[2] = {0};
/* TMU input data in floating point format */
__attribute__ ((aligned(32))) float in_data_f32[2] = {0};
/* TMU output data in floating point format */
__attribute__ ((aligned(32))) float out_data_f32[2] = {0};

/* function declaration */
/* configure RCU */
void rcu_config(void);
/* configure TMU input and output data in q31 format */
void tmu_config_q31(void);
/* configure TMU input and output data in floating point format */
void tmu_config_f32(void);
/* configure DMA when TMU in q31 format */
void dma_config_q31(void);
/* configure DMA when TMU in floating point format */
void dma_config_f32(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    uint16_t scaling_factor = 128U;

    /* configure RCU */
    rcu_config();
    /* configure systick */
    systick_config();
    /* configure COM port */
    gd_eval_com_init(EVAL_COM);

    /* software processes the input data */
    in_data_q31[0] = (uint32_t)((int32_t)(theta / DEMO_PI * 0x80000000U));
    in_data_q31[1] = (uint32_t)((int32_t)(m / scaling_factor * 0x80000000U));

    /* configure TMU input and output data in q31 format */
    tmu_config_q31();
    /* configure DMA when TMU in q31 format */
    dma_config_q31();
    
    /* wait for the DMA transfer complete */
    while(RESET == dma_flag_get(DMA1, DMA_CH1, DMA_FLAG_FTF));
    result = scaling_factor * (float)((int32_t)out_data_q31[0]) / 0x80000000U;

    /* software processes the input data */
    in_data_f32[0] = theta / DEMO_PI;
    in_data_f32[1] = m;
    /* configure TMU input and output data in floating point format */
    tmu_config_f32();
    /* configure DMA when TMU in floating point format */
    dma_config_f32();
    /* wait for the DMA transfer complete */
    while(RESET == dma_flag_get(DMA1, DMA_CH1, DMA_FLAG_FTF));

    while(1){
        delay_1ms(1000);
        printf("\n in q31 format: %3.2f*sin(%3.2f)=%3.2f\n", m, theta, result);

        delay_1ms(1000);
        printf("\n in floating point format: %3.2f*sin(%3.2f)=%3.2f\n", m, theta, out_data_f32[0]);
    }
}

/*!
    \brief      configure RCU 
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_config(void)
{
    /* enable TMU clock */
    rcu_periph_clock_enable(RCU_TMU);
    /* enable DMA1 clock */
    rcu_periph_clock_enable(RCU_DMA1);
    /* enable DMAMUX clock */
    rcu_periph_clock_enable(RCU_DMAMUX);
}

/*!
    \brief      configure TMU input and output data in q31 format
    \param[in]  none
    \param[out] none
    \retval     none
*/
void tmu_config_q31(void)
{
    tmu_parameter_struct tmu_init_struct;

    tmu_struct_para_init(&tmu_init_struct);
    /* reset the TMU */
    tmu_deinit();
    /* configure TMU peripheral */
    tmu_init_struct.mode = TMU_MODE_SIN;
    tmu_init_struct.iterations_number = TMU_ITERATION_STEPS_24;
    tmu_init_struct.scale = TMU_SCALING_FACTOR_1;
    tmu_init_struct.output_floating = TMU_OUTPUT_FLOAT_DISABLE;
    tmu_init_struct.input_floating = TMU_INPUT_FLOAT_DISABLE;
    tmu_init_struct.dma_read = TMU_READ_DMA_ENABLE;
    tmu_init_struct.dma_write = TMU_WRITE_DMA_ENABLE;
    tmu_init_struct.read_times = TMU_READ_TIMES_2;
    tmu_init_struct.write_times = TMU_WRITE_TIMES_2;
    tmu_init_struct.output_width = TMU_OUTPUT_WIDTH_32;
    tmu_init_struct.input_width = TMU_INPUT_WIDTH_32;
    tmu_init(&tmu_init_struct);
}

/*!
    \brief      configure TMU input and output data in floating point format
    \param[in]  none
    \param[out] none
    \retval     none
*/
void tmu_config_f32(void)
{
    tmu_parameter_struct tmu_init_struct;

    tmu_struct_para_init(&tmu_init_struct);
    /* reset the TMU */
    tmu_deinit();
    /* configure TMU peripheral */
    tmu_init_struct.mode = TMU_MODE_SIN;
    tmu_init_struct.iterations_number = TMU_ITERATION_STEPS_24;
    tmu_init_struct.scale = TMU_SCALING_FACTOR_1;
    tmu_init_struct.output_floating = TMU_OUTPUT_FLOAT_ENABLE;
    tmu_init_struct.input_floating = TMU_INPUT_FLOAT_ENABLE;
    tmu_init_struct.dma_read = TMU_READ_DMA_ENABLE;
    tmu_init_struct.dma_write = TMU_WRITE_DMA_ENABLE;
    tmu_init_struct.read_times = TMU_READ_TIMES_2;
    tmu_init_struct.write_times = TMU_WRITE_TIMES_2;
    tmu_init_struct.output_width = TMU_OUTPUT_WIDTH_32;
    tmu_init_struct.input_width = TMU_INPUT_WIDTH_32;
    tmu_init(&tmu_init_struct);
}

/*!
    \brief      configure DMA when TMU in q31 format
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dma_config_q31(void)
{
    dma_single_data_parameter_struct dma_init_struct;

    /* TMU write request: DMA1 channel */
    dma_deinit(DMA1);
    dma_single_data_para_struct_init(&dma_init_struct);
    dma_channel_deinit(DMA1, DMA_CH0);
    dma_init_struct.request             = DMA_REQUEST_TMU_WRITE;
    dma_init_struct.direction           = DMA_MEMORY_TO_PERIPH;
    dma_init_struct.memory0_addr        = (uint32_t)in_data_q31;
    dma_init_struct.memory_inc          = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_32BIT;
    dma_init_struct.number              = 2;
    dma_init_struct.periph_addr         = TMU_WRITE_ADDRESS;
    dma_init_struct.periph_inc          = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.priority            = DMA_PRIORITY_ULTRA_HIGH;
    dma_single_data_mode_init(DMA1, DMA_CH0, &dma_init_struct);
    dma_circulation_disable(DMA1, DMA_CH0);

    /* TMU read request: DMA1 channel */
    dma_single_data_para_struct_init(&dma_init_struct);
    dma_channel_deinit(DMA1, DMA_CH1);
    dma_init_struct.request             = DMA_REQUEST_TMU_READ;
    dma_init_struct.direction           = DMA_PERIPH_TO_MEMORY;
    dma_init_struct.memory0_addr        = (uint32_t)out_data_q31;
    dma_init_struct.memory_inc          = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_32BIT;
    dma_init_struct.number              = 2;
    dma_init_struct.periph_addr         = TMU_READ_ADDRESS;
    dma_init_struct.periph_inc          = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.priority            = DMA_PRIORITY_ULTRA_HIGH;
    dma_single_data_mode_init(DMA1, DMA_CH1, &dma_init_struct);
    /* configure DMA mode */
    dma_circulation_disable(DMA1, DMA_CH1);
    
    /* enable DMA transfer */
    dma_flag_clear(DMA1, DMA_CH0, DMA_FLAG_FTF);
    dma_flag_clear(DMA1, DMA_CH1, DMA_FLAG_FTF);
    dma_channel_enable(DMA1, DMA_CH0);
    dma_channel_enable(DMA1, DMA_CH1);
}

/*!
    \brief      configure DMA when TMU in floating point format
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dma_config_f32(void)
{
    dma_single_data_parameter_struct dma_init_struct;

    /* TMU write request: DMA1 channel */
    dma_deinit(DMA1);
    dma_single_data_para_struct_init(&dma_init_struct);
    dma_channel_deinit(DMA1, DMA_CH0);
    dma_init_struct.request             = DMA_REQUEST_TMU_WRITE;
    dma_init_struct.direction           = DMA_MEMORY_TO_PERIPH;
    dma_init_struct.memory0_addr        = (uint32_t)in_data_f32;
    dma_init_struct.memory_inc          = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_32BIT;
    dma_init_struct.number              = 2;
    dma_init_struct.periph_addr         = TMU_WRITE_ADDRESS;
    dma_init_struct.periph_inc          = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.priority            = DMA_PRIORITY_ULTRA_HIGH;
    dma_single_data_mode_init(DMA1, DMA_CH0, &dma_init_struct);
    dma_circulation_disable(DMA1, DMA_CH0);

    /* TMU read request: DMA1 channe2 */
    dma_single_data_para_struct_init(&dma_init_struct);
    dma_channel_deinit(DMA1, DMA_CH1);
    dma_init_struct.request             = DMA_REQUEST_TMU_READ;
    dma_init_struct.direction           = DMA_PERIPH_TO_MEMORY;
    dma_init_struct.memory0_addr        = (uint32_t)out_data_f32;
    dma_init_struct.memory_inc          = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_32BIT;
    dma_init_struct.number              = 2;
    dma_init_struct.periph_addr         = TMU_READ_ADDRESS;
    dma_init_struct.periph_inc          = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.priority            = DMA_PRIORITY_ULTRA_HIGH;
    dma_single_data_mode_init(DMA1, DMA_CH1, &dma_init_struct);
    /* configure DMA mode */
    dma_circulation_disable(DMA1, DMA_CH1);

    /* enable DMA transfer */
    dma_flag_clear(DMA1, DMA_CH0, DMA_FLAG_FTF);
    dma_flag_clear(DMA1, DMA_CH1, DMA_FLAG_FTF);
    dma_channel_enable(DMA1, DMA_CH0);
    dma_channel_enable(DMA1, DMA_CH1);
}
