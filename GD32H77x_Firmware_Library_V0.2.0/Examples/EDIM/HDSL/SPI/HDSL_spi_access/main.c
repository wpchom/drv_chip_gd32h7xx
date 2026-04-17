/*!
    \file    main.c
    \brief   EDIM_HDSL fast position get via SPI access example

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

#define SYS_CTRL            (0x00)
#define SYNC_CTRL           (0x01)
#define MASTER_QM           (0x03)
#define POS4                (0x10)
#define POS3                (0x11)
#define POS2                (0x12)
#define POS1                (0x13)
#define POS0                (0x14)
#define ACC_ERR_CNT         (0x38)
#define DUMMY               (0x3F)
#define WRITE_CMD(cmd)      (cmd)
#define READ_CMD(cmd)       (0x80 + cmd)

uint64_t drv_pos = 0;
uint8_t spi0_send_array[10] = {0};
uint8_t spi0_read_array[10] = {0};

/* SPI command function */
static void spi_cmd(uint8_t* send_addr, uint8_t len);

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
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_GPIOE);
    /* enable TRIGSEL clock */
    rcu_periph_clock_enable(RCU_TRIGSEL);
    /* enable TIMER0 clock */
    rcu_periph_clock_enable(RCU_TIMER0);
    /* enable SYSCFG clock */
    rcu_periph_clock_enable(RCU_SYSCFG);
    /* enable SPI clock */
    rcu_periph_clock_enable(RCU_SPI0);
    /* configure SPI clock */
    rcu_spi_clock_config(IDX_SPI0, RCU_SPISRC_PLL1P);
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
    
    /* configure HDSL_SEL1(PE2), HDSL_CLK1(PA9), HDSL_MISO1(PE5), HDSL_MOSI1(PE6) */
    gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_2|GPIO_PIN_5|GPIO_PIN_6);
    gpio_af_set(GPIOE, GPIO_AF_7, GPIO_PIN_2|GPIO_PIN_5|GPIO_PIN_6);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);
    gpio_af_set(GPIOA, GPIO_AF_8, GPIO_PIN_9);
    
    /* configure SPI0_NSS(PA4), SPI0_SCK(PA5), SPI0_MISO(PA6), SPI0_MOSI(PA7) */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    gpio_af_set(GPIOA, GPIO_AF_5, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_100_220MHZ, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_4);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_100_220MHZ, GPIO_PIN_4);
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
      (299+1), so the TIMER0 counter frequency is 1MHz. The CH0 PWM period is 8kHz
    ----------------------------------------------------------------------- */
    timer_parameter_struct timer_initpara;
    timer_oc_parameter_struct timer_ocinitpara;

    timer_deinit(TIMER0);
    /* TIMER0 configuration */
    timer_struct_para_init(&timer_initpara);
    timer_initpara.prescaler         = 299;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.period            = 125;
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

    timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_0, 100);
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
    trigsel_init(TRIGSEL_OUTPUT_HDSL, TRIGSEL_INPUT_TIMER0_CH0);
    trigsel_register_lock_set(TRIGSEL_OUTPUT_HDSL);
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
    /* configure EDIM_HDSL interface: SPI */
    edim_hdsl_interface_select(EDIM_HDSL_DRIVE, EDIM_HDSL_SPI_INTERFACE);
    /* SPI0 enable and start transfer */
    gpio_bit_set(GPIOA, GPIO_PIN_4);
    spi_enable(SPI0);
    gpio_bit_reset(GPIOA, GPIO_PIN_4);
    spi_master_transfer_start(SPI0, SPI_TRANS_START);
    /* init EDIM_HDSL */
    spi0_send_array[0] = WRITE_CMD(SYS_CTRL);
    spi0_send_array[1] = 0x02;
    spi0_send_array[2] = 0x0A;
    spi_cmd(spi0_send_array, 3);
    
    spi0_send_array[0] = WRITE_CMD(ACC_ERR_CNT);
    spi0_send_array[1] = 0x1F;
    spi_cmd(spi0_send_array, 2);
}

/*!
    \brief      configure SPI0
    \param[in]  none
    \param[out] none
    \retval     none
*/
void spi_config(void)
{   
    /* configure SPI0 */
    spi_parameter_struct spi_initpara;
    spi_initpara.device_mode = SPI_MASTER;
    spi_initpara.trans_mode = SPI_TRANSMODE_FULLDUPLEX;
    spi_initpara.data_size = SPI_DATASIZE_8BIT;
    spi_initpara.nss = SPI_NSS_SOFT;
    spi_initpara.endian = SPI_ENDIAN_MSB;
    spi_initpara.clock_polarity_phase = SPI_CK_PL_LOW_PH_2EDGE;
    spi_initpara.prescale = SPI_PSC_64;
    spi_init(SPI0, &spi_initpara);
    
    /* enable SPI0 byte access */
    spi_byte_access_enable(SPI0);
    /* enable SPI0 nss output */
    spi_nss_output_enable(SPI0);
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

    /* enable the CPU Cache */
    cache_enable();
    SCB_InvalidateDCache_by_Addr((uint32_t *)spi0_send_array, 32);
    SCB_InvalidateDCache_by_Addr((uint32_t *)spi0_read_array, 32);
    /* systick configuration */
    systick_config();
    /* RCU configuration */
    rcu_config();
    /* GPIO port configuration */
    gpio_config();
    /* TRIGSEL configuration */
    trigsel_config();
    /* TIMER configuration */
    timer_config();
    /* SPI0 configuration */
    spi_config();
    /* EDIM_HDSL configuration */
    edim_hdsl_config();
    /* COM port configuration */
    gd_eval_com_init(EVAL_COM);
    
    timer_enable(TIMER0);
    
    /* link HDSL encoder */
    spi0_send_array[0] = WRITE_CMD(SYS_CTRL);
    spi0_send_array[1] = 0x03;
    spi_cmd(spi0_send_array, 2);
    delay_1ms(200);
    
    for (try_count = 0; try_count < 1000; try_count++)
    {
        spi0_send_array[0] = READ_CMD(MASTER_QM);
        spi0_send_array[1] = READ_CMD(DUMMY);
        spi0_send_array[2] = READ_CMD(DUMMY);
        spi_cmd(spi0_send_array, 3);
        if (RESET != (spi0_read_array[2] & EDIM_HDSL_MASTER_QM_LINK))
        {
            break;
        }
    }
    
    if (try_count >= 1000)
    {
        printf("\r\n EDIM_HDSL link failed\r\n");
        while(1);    
    }
    
    while(1) {
        delay_1ms(1000);
        /* read fast position */
        spi0_send_array[0] = READ_CMD(POS4);
        spi0_send_array[1] = READ_CMD(POS3);
        spi0_send_array[2] = READ_CMD(POS2);
        spi0_send_array[3] = READ_CMD(POS1);
        spi0_send_array[4] = READ_CMD(POS0);
        spi0_send_array[5] = READ_CMD(DUMMY);
        spi0_send_array[6] = READ_CMD(DUMMY);
        spi_cmd(spi0_send_array, 7);
        printf("\r\n EDIM_HDSL encoder 0 POS4 register data = 0x%02X \r\n", spi0_read_array[2]);
        printf("\r\n EDIM_HDSL encoder 0 POS3 register data = 0x%02X \r\n", spi0_read_array[3]);
        printf("\r\n EDIM_HDSL encoder 0 POS2 register data = 0x%02X \r\n", spi0_read_array[4]);
        printf("\r\n EDIM_HDSL encoder 0 POS1 register data = 0x%02X \r\n", spi0_read_array[5]);
        printf("\r\n EDIM_HDSL encoder 0 POS0 register data = 0x%02X \r\n", spi0_read_array[6]);
    }
}

/*!
    \brief      SPI command function
    \param[in]  none
    \param[out] none
    \retval     none
*/

static void spi_cmd(uint8_t* send_addr, uint8_t len) 
{
    gpio_bit_set(GPIOA, GPIO_PIN_4);
    for (uint8_t i = 0; i < len; i++) {
        while(RESET == spi_i2s_flag_get(SPI0, SPI_FLAG_TP));
        spi_i2s_data_transmit(SPI0, send_addr[i]);
        while(RESET == spi_i2s_flag_get(SPI0, SPI_FLAG_RP));
        spi0_read_array[i] = spi_i2s_data_receive(SPI0);
    }
    gpio_bit_reset(GPIOA, GPIO_PIN_4);
}
