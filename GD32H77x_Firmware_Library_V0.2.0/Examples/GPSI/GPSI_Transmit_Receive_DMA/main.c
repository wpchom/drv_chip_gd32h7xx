/*!
    \file    main.c
    \brief   GPSI data transfer by dma

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
#include "systick.h"
#include <stdio.h>
#include "main.h"

#define GPSI_ARR_SIZE   2
#define TRANSMIT_BOARD

__attribute__ ((aligned(32))) uint32_t tx_data[GPSI_ARR_SIZE] = {0xFEDC1234, 0x53A17DFE}; 
__attribute__ ((aligned(32))) uint32_t rx_data[GPSI_ARR_SIZE];                          

/* enable the CPU cache */
void cache_enable(void);
/* configure relevant peripheral clock */
void rcu_config(void);
/* configure GPIO peripheral */
void gpio_config(void);
/* init GPSI transmit or receive */
void gpsi_transmit_or_receive_init(void);
/* configure GPSI receive DMA */
void gpsi_receive_dma_config(uint32_t *data, uint32_t length);
/* configure GPSI transmit DMA */
void gpsi_transmit_dma_config(uint32_t *data, uint32_t length);
/* generate clock out by using IRC64M oscillator */
void gpsi_generate_clock_output_irc64(void);
/* generate clock out by using GPIO */
void gpsi_generate_clock_output_gpio(uint32_t num);
/* compare receive data */
ErrStatus memory_compare(uint32_t* src, uint32_t* dst, uint8_t length);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{   

    /* enable the CPU Chache */
    cache_enable();    
    /* configure Systick */
    systick_config();
    /* rcu configure */
    rcu_config();
    /* GPIO configure */
    gpio_config();

    /* init GPSI transmit or receive */
    gpsi_transmit_or_receive_init();
    /* generate communication clock */
    gpsi_generate_clock_output_irc64();

    /* GPSI DMA enable */
    gpsi_dma_enable();

#ifdef TRANSMIT_BOARD
    /* GPSI master transmit */
    gpsi_transmit_dma_config(tx_data, GPSI_ARR_SIZE);
    /* enable gpsi */
    gpsi_enable();
    /* generate 25 clock */
    gpsi_generate_clock_output_gpio(25);

    /* wait DMA transmit complete */
    while (!dma_flag_get(DMA1, DMA_CH7, DMA_FLAG_FTF));
    /* clear DMA receive complete flag */
    dma_flag_clear(DMA1, DMA_CH7, DMA_FLAG_FTF);

#else
    /* GPSI slave receive */
    gpsi_receive_dma_config(rx_data, GPSI_ARR_SIZE);
    /* enable gpsi */
    gpsi_enable();
    /* generate 25 clock */
    gpsi_generate_clock_output_gpio(25);
    
    /* wait DMA transmit complete */
    while (!dma_flag_get(DMA1, DMA_CH7, DMA_FLAG_FTF));
    /*  need clear cache */
    SCB_CleanInvalidateDCache();   
    
    /* clear DMA receive complete flag */
    dma_flag_clear(DMA1, DMA_CH7, DMA_FLAG_FTF);
    
    /* compare receive data with send data */
    if(ERROR != memory_compare(rx_data, tx_data, GPSI_ARR_SIZE)){
        gd_eval_led_on(LED1);
    }else{
        gd_eval_led_off(LED1);
    }
    
#endif

    while(1){
    }
}

/*!
    \brief      enable the CPU Chache
    \param[in]  none
    \param[out] none
    \retval     none
*/
void cache_enable(void)
{
    /* enable I-Cache */
    SCB_EnableICache();
    /* enable D-Cache */
    SCB_EnableDCache();
}

/*!
    \brief      configure different peripheral clocks
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_config(void)
{
    /* enable the GPSI pin clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_GPIOE);
    rcu_periph_clock_enable(RCU_GPIOH);
    /* gpio clk generate gpio config */
    rcu_periph_clock_enable(RCU_GPIOF);
    /* enable the GPSI peripheral clock */
    rcu_periph_clock_enable(RCU_GPSI);
    
    /* enable DMA clock */
    rcu_periph_clock_enable(RCU_DMA1);
    rcu_periph_clock_enable(RCU_DMAMUX);
}

/*!
    \brief      configure relevant peripheral clock
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_config(void)
{
    /* configure GPSI_PCLK(PA6), GPSI_RDY(PB7), GPSI_DE(PH8) */
    gpio_af_set(GPSI_PCLK_GPIO_PORT, GPSI_PCLK_AF, GPSI_PCLK_PIN);
    gpio_af_set(GPSI_VSYNC_RDY_GPIO_PORT, GPSI_VSYNC_RDY_AF, GPSI_VSYNC_RDY_PIN);
    gpio_af_set(GPSI_HSYNC_DE_GPIO_PORT, GPSI_HSYNC_DE_AF, GPSI_HSYNC_DE_PIN);
    /* configure  GPSI_D0(PA9), GPSI_D1(PA10) GPSI_D2(PB13), GPSI_D3(PC9), GPSI_D4(PE4), GPSI_D5(PB6), GPSI_D6(PB8), GPSI_D7(PB9) */
    gpio_af_set(GPSI_D0_GPIO_PORT, GPSI_D0_AF, GPSI_D0_PIN);
    gpio_af_set(GPSI_D1_GPIO_PORT, GPSI_D1_AF, GPSI_D1_PIN);
    gpio_af_set(GPSI_D2_GPIO_PORT, GPSI_D2_AF, GPSI_D2_PIN);
    gpio_af_set(GPSI_D3_GPIO_PORT, GPSI_D3_AF, GPSI_D3_PIN);
    gpio_af_set(GPSI_D4_GPIO_PORT, GPSI_D4_AF, GPSI_D4_PIN);
    gpio_af_set(GPSI_D5_GPIO_PORT, GPSI_D5_AF, GPSI_D5_PIN);
    gpio_af_set(GPSI_D6_GPIO_PORT, GPSI_D6_AF, GPSI_D6_PIN);
    gpio_af_set(GPSI_D7_GPIO_PORT, GPSI_D7_AF, GPSI_D7_PIN);
    
    /* configure GPSI_D8(PH6), GPSI_D9(PH7) GPSI_D10(PB5), GPSI_D11(PD2), GPSI_D12(PD12), GPSI_D13(PD13), GPSI_D14(PA5), GPSI_D15(PC4) */
    gpio_af_set(GPSI_D8_GPIO_PORT, GPSI_D8_AF, GPSI_D8_PIN);
    gpio_af_set(GPSI_D9_GPIO_PORT, GPSI_D9_AF, GPSI_D9_PIN);
    gpio_af_set(GPSI_D10_GPIO_PORT, GPSI_D10_AF, GPSI_D10_PIN);
    gpio_af_set(GPSI_D11_GPIO_PORT, GPSI_D11_AF, GPSI_D11_PIN);
    gpio_af_set(GPSI_D12_GPIO_PORT, GPSI_D12_AF, GPSI_D12_PIN);
    gpio_af_set(GPSI_D13_GPIO_PORT, GPSI_D13_AF, GPSI_D13_PIN);
    gpio_af_set(GPSI_D14_GPIO_PORT, GPSI_D14_AF, GPSI_D14_PIN);
    gpio_af_set(GPSI_D15_GPIO_PORT, GPSI_D15_AF, GPSI_D15_PIN);
    
    /* GPSI pin gpio mode set */
    gpio_mode_set(GPSI_PCLK_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, GPSI_PCLK_PIN);
    gpio_output_options_set(GPSI_PCLK_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_12MHZ, GPSI_PCLK_PIN);
    gpio_mode_set(GPSI_VSYNC_RDY_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPSI_VSYNC_RDY_PIN);
    gpio_output_options_set(GPSI_VSYNC_RDY_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_12MHZ, GPSI_VSYNC_RDY_PIN);
    gpio_mode_set(GPSI_HSYNC_DE_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPSI_HSYNC_DE_PIN);
    gpio_output_options_set(GPSI_HSYNC_DE_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_12MHZ, GPSI_HSYNC_DE_PIN);
    
    gpio_mode_set(GPSI_D0_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPSI_D0_PIN);
    gpio_mode_set(GPSI_D1_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPSI_D1_PIN);
    gpio_mode_set(GPSI_D2_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPSI_D2_PIN);
    gpio_mode_set(GPSI_D3_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPSI_D3_PIN);
    gpio_mode_set(GPSI_D4_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPSI_D4_PIN);
    gpio_mode_set(GPSI_D5_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPSI_D5_PIN);
    gpio_mode_set(GPSI_D6_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPSI_D6_PIN);
    gpio_mode_set(GPSI_D7_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPSI_D7_PIN);
    gpio_mode_set(GPSI_D8_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPSI_D8_PIN);
    gpio_mode_set(GPSI_D9_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPSI_D9_PIN);
    gpio_mode_set(GPSI_D10_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPSI_D10_PIN);
    gpio_mode_set(GPSI_D11_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPSI_D11_PIN);
    gpio_mode_set(GPSI_D12_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPSI_D12_PIN);
    gpio_mode_set(GPSI_D13_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPSI_D13_PIN);
    gpio_mode_set(GPSI_D14_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPSI_D14_PIN);
    gpio_mode_set(GPSI_D15_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPSI_D15_PIN);
    
    /* GPSI pin gpio output mode set */
    gpio_output_options_set(GPSI_D0_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_12MHZ, GPSI_D0_PIN);
    gpio_output_options_set(GPSI_D1_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_12MHZ, GPSI_D1_PIN);
    gpio_output_options_set(GPSI_D2_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_12MHZ, GPSI_D2_PIN);
    gpio_output_options_set(GPSI_D3_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_12MHZ, GPSI_D3_PIN);
    gpio_output_options_set(GPSI_D4_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_12MHZ, GPSI_D4_PIN);
    gpio_output_options_set(GPSI_D5_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_12MHZ, GPSI_D5_PIN);
    gpio_output_options_set(GPSI_D6_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_12MHZ, GPSI_D6_PIN);
    gpio_output_options_set(GPSI_D7_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_12MHZ, GPSI_D7_PIN);
    gpio_output_options_set(GPSI_D8_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_12MHZ, GPSI_D8_PIN);
    gpio_output_options_set(GPSI_D9_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_12MHZ, GPSI_D9_PIN);
    gpio_output_options_set(GPSI_D10_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_12MHZ, GPSI_D10_PIN);
    gpio_output_options_set(GPSI_D11_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_12MHZ, GPSI_D11_PIN);
    gpio_output_options_set(GPSI_D12_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_12MHZ, GPSI_D12_PIN);
    gpio_output_options_set(GPSI_D13_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_12MHZ, GPSI_D13_PIN);
    gpio_output_options_set(GPSI_D14_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_12MHZ, GPSI_D14_PIN);
    gpio_output_options_set(GPSI_D15_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_12MHZ, GPSI_D15_PIN);
    
    /* configure clk send PIN PF14 */
    gpio_mode_set(GPIOF, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_14);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_12MHZ, GPIO_PIN_14);
}

/*!
    \brief      init GPSI transmit or receive
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpsi_transmit_or_receive_init(void) 
{
    gpsi_parameter_struct gpsi_config;
#ifdef TRANSMIT_BOARD
    gpsi_config.data_direction = GPSI_TRNSMIT_MODE;
    gpsi_config.data_en_ready_config = GPSI_DE_ENABLE;
#else
    gpsi_config.data_direction = GPSI_RECEIVE_MODE;
    gpsi_config.data_en_ready_config = GPSI_DE_ENABLE;
#endif 
    gpsi_config.de_polarity = GPSI_DEPOL_ACTIVE_LOW;
    gpsi_config.clock_polarity = GPSI_RISING_EDGE;          
    gpsi_config.data_width = GPSI_8LINES;

    /* gpsi init */
    gpsi_init(&gpsi_config);

}

/*!
    \brief      configure GPSI receive data by using DMA
    \param[in]  data: data which need to receive
    \param[in]  length: size data which need to transmit
    \retval     none
*/
void gpsi_receive_dma_config(uint32_t *data, uint32_t length) 
{
    /* deinitialize DMA registers of a channel */
    dma_single_data_parameter_struct dma_init_struct;
    /* GPSI receive DMA config: DMA1_CH7 */
    dma_deinit(DMA1);
    dma_channel_deinit(DMA1, DMA_CH7);
    dma_init_struct.request = DMA_REQUEST_DCI; 
    dma_init_struct.periph_addr = (uint32_t)&GPSI_DATA; 
    dma_init_struct.memory0_addr = (uint32_t)data;      
    dma_init_struct.direction = DMA_PERIPH_TO_MEMORY;
    dma_init_struct.number = length;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_32BIT;
    dma_init_struct.priority = DMA_PRIORITY_HIGH;

    dma_single_data_mode_init(DMA1, DMA_CH7, &dma_init_struct);

    /* enable DMA channel */
    dma_channel_enable(DMA1, DMA_CH7);
}

/*!
    \brief      configure GPSI transmit data by using DMA
    \param[in]  data: data which need to transmit
    \param[in]  length: data size which need to transmit
    \retval     none
*/
void gpsi_transmit_dma_config(uint32_t *data, uint32_t length) 
{
    /* deinitialize DMA registers of a channel */
    dma_single_data_parameter_struct dma_init_struct;
    /* GPSI receive DMA config: DMA1_CH7 */
    dma_deinit(DMA1);
    dma_channel_deinit(DMA1, DMA_CH7);
    dma_init_struct.request = DMA_REQUEST_DCI; 
    dma_init_struct.periph_addr = (uint32_t)&GPSI_DATA; 
    dma_init_struct.memory0_addr = (uint32_t)data;      
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPH;
    dma_init_struct.number = length;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_32BIT;
    dma_init_struct.priority = DMA_PRIORITY_HIGH;

    dma_single_data_mode_init(DMA1, DMA_CH7, &dma_init_struct);

    /* enable DMA channel */
    dma_channel_enable(DMA1, DMA_CH7);
}

/*!
    \brief      generate clock out by using IRC64M oscillator
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpsi_generate_clock_output_irc64(void)
{
    /* peripheral clock enable */
    rcu_periph_clock_enable(RCU_GPIOA);

    /* enable IRC64M */
    rcu_osci_on(RCU_IRC64M);
    while(ERROR == rcu_osci_stab_wait(RCU_IRC64M));

    rcu_ckout0_config(RCU_CKOUT0SRC_HXTAL, RCU_CKOUT0_DIV15);
    /* output IRC64M on CKOUT0 (PA8) */
    gpio_af_set(GPIOA, GPIO_AF_0, GPIO_PIN_8);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_OSPEED_100_220MHZ, GPIO_PIN_8);
}

/*!
    \brief      generate clock out by using GPIO
    \param[in]  num: clock number
    \param[out] none
    \retval     none
*/
void gpsi_generate_clock_output_gpio(uint32_t num)
{
    uint32_t i = 0;
    uint32_t count = num;

    while(count--) {
        gpio_bit_reset(GPIOF, GPIO_PIN_14);
        for(i = 0; i < 10; i++);
        gpio_bit_set(GPIOF, GPIO_PIN_14);
        for(i = 0; i < 10; i++);
    }
}

/*!
    \brief      memory compare function
    \param[in]  src: source data pointer
    \param[in]  dst: destination data pointer
    \param[in]  length: the compare data length
    \param[out] none
    \retval     ErrStatus : ERROR or SUCCESS
*/
ErrStatus memory_compare(uint32_t* src, uint32_t* dst, uint8_t length) 
{
    while(length--){
        if(*src++ != *dst++)
            return ERROR;
    }
    return SUCCESS;
}
