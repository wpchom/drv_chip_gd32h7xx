/*!
    \file    main.c
    \brief   eNVM program, erase

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
void nvmc_envm_all_flags_clear(void);
void envm_erase_sectors(void);
void envm_program(void);
void envm_erase_sectors_check(void);
void envm_program_check(void);

#define ENVM_WRITE_START_ADDR    ((uint32_t)0x08040000U)
#define ENVM_WRITE_END_ADDR      ((uint32_t)0x08060000U)

uint64_t *ptrd;
uint32_t address = 0x00000000U;
uint64_t data[] = {0x1122334455667788, 0x2233445566778899, 0x33445566778899AA, 0x445566778899AABB};

led_typedef_enum led_num;

/* calculate the count of envm sector to be programmed/erased */
uint32_t envm_sector_cnt = (ENVM_WRITE_END_ADDR - ENVM_WRITE_START_ADDR) / SECTOR_SIZE;
/* calculate the count of envm doubleword to be programmed/erased */
uint32_t envm_doubleword_cnt = ((ENVM_WRITE_END_ADDR - ENVM_WRITE_START_ADDR) >> 3);

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
    \brief      clear all ENVM flag status
    \param[in]  none
    \param[out] none
    \retval     none
*/
void nvmc_envm_all_flags_clear(void)
{
    nvmc_envm_flag_clear(ENVM_FLAG_ENDF | ENVM_FLAG_WPERR | ENVM_FLAG_PGSERR | ENVM_FLAG_RPERR | ENVM_FLAG_RSERR | ENVM_FLAG_OTAFAIL | ENVM_FLAG_OTASUC
                         | ENVM_FLAG_COPYSTRERR | ENVM_FLAG_CRCCALEND | ENVM_FLAG_CRCRERR);
}

/*!
    \brief      erase envm sectors from ENVM_WRITE_START_ADDR to ENVM_WRITE_END_ADDR
    \param[in]  none
    \param[out] none
    \retval     none
*/
void envm_erase_sectors(void)
{
    uint32_t erase_count;

    /* unlock register NVMC_ENVM_CTL */
    nvmc_envm_unlock();

    /* clear ENVM all pending flags */
    nvmc_envm_all_flags_clear();

    /* erase the envm sectors */
    for(erase_count = 0U; erase_count < envm_sector_cnt; erase_count++) {
        nvmc_envm_sector_erase(ENVM_WRITE_START_ADDR + (SECTOR_SIZE * erase_count));
        nvmc_envm_all_flags_clear();
    }

    /* lock register NVMC_ENVM_CTL after the erase operation */
    nvmc_envm_lock();
}

/*!
    \brief      program envm 32 bytes by 32 bytes from ENVM_WRITE_START_ADDR to ENVM_WRITE_END_ADDR
    \param[in]  none
    \param[out] none
    \retval     none
*/
void envm_program(void)
{
    /* unlock register NVMC_ENVM_CTL */
    nvmc_envm_unlock();

    address = ENVM_WRITE_START_ADDR;

    /* program envm */
    while(address < ENVM_WRITE_END_ADDR) {
        nvmc_envm_32bytes_program(address, data);
        address += 32U;
        nvmc_envm_all_flags_clear();
    }

    /* lock register NVMC_ENVM_CTL after the program operation */
    nvmc_envm_lock();
}

/*!
    \brief      check envm erase result
    \param[in]  none
    \param[out] none
    \retval     none
*/
void envm_erase_sectors_check(void)
{
    uint32_t i;

    ptrd = (uint64_t *)ENVM_WRITE_START_ADDR;

    /* check envm whether has been erased */
    for(i = 0U; i < envm_doubleword_cnt; i++) {
        if(0U != (*ptrd)) {
            led_num = LED1;
            gd_eval_led_on(led_num);
            while(1);
        } else {
            ptrd++;
        }
    }
}

/*!
    \brief      check envm program result
    \param[in]  none
    \param[out] none
    \retval     none
*/
void envm_program_check(void)
{
    uint32_t i;
    uint32_t j;

    ptrd = (uint64_t *)ENVM_WRITE_START_ADDR;

    /* check envm whether has been programmed */
    for(i = 0U; i < envm_doubleword_cnt / 4U; i++) {
        for(j = 0U; j < 32U / 8U; j++) {
            if((*ptrd) != data[j]) {
                led_num = LED2;
                gd_eval_led_on(led_num);
                while(1);
            } else {
                ptrd++;
            }
        }
    }
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

    /* initialize led and USART on the board */
    gd_eval_led_init(LED1);
    gd_eval_led_init(LED2);
    gd_eval_com_init(EVAL_COM);

    /* step1: envm erase sectors */
    envm_erase_sectors();

    /* step2: envm program and check if it is successful. If not, light the LED2. */
    envm_program();
    SCB_CleanInvalidateDCache();
    envm_program_check();

    /* step3: envm erase sectors and check if it is successful. If not, light the LED1. */
    envm_erase_sectors();
    SCB_CleanInvalidateDCache();
    envm_erase_sectors_check();

    printf("\r\n eNVM program and erase successful!\r\n");

    while(1) {
    }
}
