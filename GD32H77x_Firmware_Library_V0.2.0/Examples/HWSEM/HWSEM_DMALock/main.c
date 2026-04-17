/*!
    \file    main.c
    \brief   HWSEM DMA Lock and Unlock example

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
#include "gd32h77ei_eval.h"
#include <string.h>

#define HWSEM_ID0           0U
#define HWSEM_PROCESS_ID0   0U
#define UNLOCK_KEY          0xA55AU

__attribute__((aligned(32))) uint32_t source_lock = HWSEM_CTL_LK | HWSEM_PROCESS_ID0 | CTL_MID(HWSEM_DMA0_MASTER_ID);
__attribute__((aligned(32))) uint32_t source_unlock = HWSEM_PROCESS_ID0 | CTL_MID(HWSEM_DMA0_MASTER_ID);
__attribute__((aligned(32))) uint32_t all_unlock = (UNLOCK_KEY << 16U) | UNLK_MID(HWSEM_DMA0_MASTER_ID);
__attribute__((aligned(32))) uint32_t dest_lock;
uint32_t operation_times = 0;

void cache_enable(void);
void dma_config(uint32_t source_address, uint32_t destination_address, uint32_t number);
void dma_lock_unlock(void);

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
    /* init LED1 */
    gd_eval_led_init(LED1);
    /* enable hwsem clock */
    rcu_periph_clock_enable(RCU_HWSEM);
    /* enable DMA clock */
    rcu_periph_clock_enable(RCU_DMA0);

    /* DMA lock and unlock hwsem */
    dma_lock_unlock();
    /* if all operations are success, turn on LED1 */
    if(4 == operation_times) {
        gd_eval_led_on(LED1);
    }

    while(1) {
    }
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

/*!
    \brief      configure DMA channel
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dma_config(uint32_t source_address, uint32_t destination_address, uint32_t number)
{
    dma_single_data_parameter_struct dma_init_struct;

    /* initialize DMA0 channel 1 */
    dma_channel_deinit(DMA0, DMA_CH1);
    dma_single_data_para_struct_init(&dma_init_struct);

    dma_init_struct.request             = DMA_REQUEST_M2M;
    dma_init_struct.direction           = DMA_MEMORY_TO_MEMORY;
    dma_init_struct.memory0_addr        = (uint32_t)destination_address;
    dma_init_struct.memory_inc          = DMA_MEMORY_INCREASE_DISABLE;
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_32BIT;
    dma_init_struct.number              = number;
    dma_init_struct.periph_addr         = (uint32_t)source_address;
    dma_init_struct.periph_inc          = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.priority            = DMA_PRIORITY_ULTRA_HIGH;
    dma_single_data_mode_init(DMA0, DMA_CH1, &dma_init_struct);
    /* configure DMA mode */
    dma_circulation_disable(DMA0, DMA_CH1);

    /* enable DMA0 channel 1 */
    dma_channel_enable(DMA0, DMA_CH1);
}

/*!
    \brief      use DMA0 to lock and unlock semaphore
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dma_lock_unlock(void)
{
    /* DMA0 read to lock hwsem */
    dma_config((uint32_t)(&HWSEM_RLK(HWSEM_ID0)), (uint32_t)&dest_lock, 1);
    while(RESET == dma_flag_get(DMA0, DMA_CH1, DMA_FLAG_FTF)) {
    }
    if((HWSEM_CTL_LK | CTL_MID(HWSEM_DMA0_MASTER_ID)) == HWSEM_RLK(HWSEM_ID0)) {
        operation_times++;
    }

    /* DMA0 unlock hwsem */
    dma_config((uint32_t)&source_unlock, (uint32_t)(&HWSEM_CTL(HWSEM_ID0)), 1);
    while(RESET == dma_flag_get(DMA0, DMA_CH1, DMA_FLAG_FTF)) {
    }
    if(0 == HWSEM_CTL(HWSEM_ID0)) {
        operation_times++;
    }

    /* DMA0 write to lock hwsem */
    dma_config((uint32_t)&source_lock, (uint32_t)(&HWSEM_CTL(HWSEM_ID0)), 1);
    while(RESET == dma_flag_get(DMA0, DMA_CH1, DMA_FLAG_FTF)) {
    }
    if(source_lock == HWSEM_CTL(HWSEM_ID0)) {
        operation_times++;
    }

    /* CORE set key and unlock all hwsem of DMA0 */
    hwsem_key_set(UNLOCK_KEY);
    dma_config((uint32_t)&all_unlock, (uint32_t)(&HWSEM_UNLK), 1);
    while(RESET == dma_flag_get(DMA0, DMA_CH1, DMA_FLAG_FTF)) {
    }
    if(0 == HWSEM_CTL(HWSEM_ID0)) {
        operation_times++;
    }
}
