/*!
    \file    main.c
    \brief   OTP user data write

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
#include <stdio.h>
#include "gd32h77ei_eval.h"

void cache_enable(void);

uint32_t OTP_USER_DATA_ADDR = 0x1FFF02C0U;
uint16_t data = 0x0002U;

/*!
    \brief      enable the CPU cache
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
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* enable the CPU Cache */
    cache_enable();

    /* initialize led on the board */
    gd_eval_led_init(LED1);
    gd_eval_led_init(LED2);

    /* otp program and read check. */
    nvmc_envm_unlock();
    /* clear OTP all pending flags */
    nvmc_otp_flag_clear(OTP_FLAG_ENDF | OTP_FLAG_DATAERR | OTP_FLAG_PGLKERR | OTP_FLAG_PGSERR | OTP_FLAG_MULTI_DATAERR);
    nvmc_otp_halfword_program(OTP_USER_DATA_ADDR, data);

    /* check OTP whether has been programmed */
    if((*(volatile uint16_t *)(OTP_USER_DATA_ADDR)) != data) {
        gd_eval_led_on(LED1);
    } else {
        gd_eval_led_on(LED2);
    }

    while(1) {
    }
}
