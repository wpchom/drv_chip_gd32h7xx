/*!
    \file    main.c
    \brief   USART loopback

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
#include <stdio.h>

void com_usart_init(void);
void cache_enable(void);
ErrStatus memory_compare(uint8_t *src, uint8_t *dst, uint16_t length);

uint8_t transmit_size = 10U;
uint8_t transfersize = 10U;
uint8_t transmitter_buffer[10U] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
uint8_t receiver_buffer[10U];
__IO uint8_t txcount = 0;
__IO uint16_t rxcount = 0;
ErrStatus state1 = ERROR;

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

    /* initialize the com */
    com_usart_init();
    gd_eval_led_init(LED1);
    gd_eval_led_init(LED2);

    while(transfersize--) {
        /* wait until end of transmit */
        while(RESET == usart_flag_get(USART0, USART_FLAG_TBE)) {
        }

        usart_data_transmit(USART0, transmitter_buffer[txcount++]);

        while(RESET == usart_flag_get(USART0, USART_FLAG_RBNE)) {
        }

        /* store the received byte in the receiver_buffer */
        receiver_buffer[rxcount++] = usart_data_receive(USART0);
    }
    /* compare the received data with the send ones */
    state1 = memory_compare(transmitter_buffer, receiver_buffer, transmit_size);

    if(SUCCESS == state1) {
        /* if the sent data is the same as the received data. */
        gd_eval_led_on(LED1);
    } else {
        /* if the sent data is different from the received data. */
        gd_eval_led_on(LED2);
    }

    while(1) {
    }
}

/*!
    \brief      initialize the USART configuration of the com
    \param[in]  none
    \param[out] none
    \retval     none
*/
void com_usart_init(void)
{
    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART0);

    /* USART configure */
    usart_deinit(USART0);
    usart_word_length_set(USART0, USART_WL_8BIT);
    usart_stop_bit_set(USART0, USART_STB_1BIT);
    usart_parity_config(USART0, USART_PM_NONE);
    usart_baudrate_set(USART0, 115200U);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_loopback_enable(USART0);
    usart_enable(USART0);
}

/*!
    \brief      memory compare function
    \param[in]  src : source data
    \param[in]  dst : destination data
    \param[in]  length : the compare data length
    \param[out] none
    \retval     ErrStatus : ERROR or SUCCESS
*/
ErrStatus memory_compare(uint8_t *src, uint8_t *dst, uint16_t length)
{
    while(length--) {
        if(*src++ != *dst++) {
            return ERROR;
        }
    }

    return SUCCESS;
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
