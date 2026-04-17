/*!
    \file    main.c
    \brief   CAU cmac example

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

#include "gd32h77ei_eval.h"
#include <string.h>
#include "main.h"

void cau_parameter_printf(char *name, uint8_t *data, uint32_t data_size);
/* check data in bytes */
void data_check(uint8_t *src, uint8_t *dst, uint32_t len);

uint8_t cmac[CMAC_SIZE];

__IO uint8_t check_flag = 0;

/*!
    \brief      enable the CPU chache
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
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    cau_parameter_struct cau_cmac_parameter;
    /* enable the CPU cache */
    cache_enable();

    gd_eval_led_init(LED1);
    gd_eval_com_init(EVAL_COM);

    rcu_periph_clock_enable(RCU_CAU);

    /* print input parameters */
    cau_parameter_printf("plain text", (uint8_t *)plaintext, PLAINTEXT_SIZE);

    /* calculate cmac */
    cau_deinit();
    cau_cmac_parameter.alg_dir     = CAU_ENCRYPT;
    cau_cmac_parameter.key         = (uint8_t *)key_128;
    cau_cmac_parameter.key_size    = KEY_SIZE;
    cau_cmac_parameter.input       = (uint8_t *)plaintext;
    cau_cmac_parameter.in_length   = PLAINTEXT_SIZE;
    cau_cmac_parameter.key_select  = CAU_REG_KEY;
    cau_aes_cmac(&cau_cmac_parameter, cmac, CMAC_SIZE);
    printf("\r\nencryption in CMAC mode\r\n");
    /* print output parameters */
    cau_parameter_printf("cmac", cmac, CMAC_SIZE);
    data_check((uint8_t *)&cmac[0], (uint8_t *)&cmac_exp[0], CMAC_SIZE);

    /* check result */
    if(0U == check_flag) {
        printf("Successful execution\r\n");
        gd_eval_led_on(LED1);
    }

    while(1) {
    }
}

/*!
    \brief      print cau parameter
    \param[in]  name: parameter name
    \param[in]  data: parameter raw data
    \param[in]  data_size: raw data size
    \param[out] none
    \retval     none
*/
void cau_parameter_printf(char *name, uint8_t *data, uint32_t data_size)
{
    uint32_t i = 0, count = 0;

    printf("%s:\n", name);
    for(i = 0; i < data_size; i++) {
        printf("0x%02X ", data[i]);
        count++;

        if(8 == count) {
            count = 0;
            printf("\r\n");
        }
    }
}

/*!
    \brief      check data in bytes
    \param[in]  src: source
    \param[in]  dst: destination
    \param[in]  len: length of the data to check
    \param[out] none
    \retval     none
*/
void data_check(uint8_t *src, uint8_t *dst, uint32_t len)
{
    if(memcmp((const void *)src, (const void *)dst, len) != 0) {
        check_flag++;
    }
}
