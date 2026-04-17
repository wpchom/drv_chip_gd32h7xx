/*!
    \file    main.c
    \brief   main flash program, erase

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

void nvmc_flash_all_flags_clear(void);
void flash_erase_sectors(void);
void flash_program(void);
void flash_erase_sectors_check(void);
void flash_program_check(void);

#define FLASH_WRITE_START_ADDR    ((uint32_t)0x08240000U)
#define FLASH_WRITE_END_ADDR      ((uint32_t)0x08260000U)

uint64_t *ptrd;
uint32_t address = 0x00000000U;
uint64_t data = 0x1122334455667788;

led_typedef_enum led_num;

/* calculate the count of flash sector to be programmed/erased */
uint32_t flash_sector_cnt = (FLASH_WRITE_END_ADDR - FLASH_WRITE_START_ADDR) / SECTOR_SIZE;
/* calculate the count of flash doubleword to be programmed/erased */
uint32_t flash_doubleword_cnt = ((FLASH_WRITE_END_ADDR - FLASH_WRITE_START_ADDR) >> 3);

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
    \brief      clear all flash flag status
    \param[in]  none
    \param[out] none
    \retval     none
*/
void nvmc_flash_all_flags_clear(void)
{
    nvmc_flash_flag_clear(FLASH_FLAG_ENDF | FLASH_FLAG_WPERR | FLASH_FLAG_PGSERR | FLASH_FLAG_RPERR | FLASH_FLAG_RSERR | FLASH_FLAG_ECCDET);
}

/*!
    \brief      erase flash sectors from FLASH_WRITE_START_ADDR to FLASH_WRITE_END_ADDR
    \param[in]  none
    \param[out] none
    \retval     none
*/
void flash_erase_sectors(void)
{
    uint32_t erase_count;

    /* unlock register FLASH_CTL */
    nvmc_flash_unlock();

    /* clear flash all pending flags */
    nvmc_flash_all_flags_clear();

    /* erase the flash sectors */
    for(erase_count = 0U; erase_count < flash_sector_cnt; erase_count++) {
        nvmc_flash_sector_erase(FLASH_WRITE_START_ADDR + (SECTOR_SIZE * erase_count));
        nvmc_flash_all_flags_clear();
    }

    /* lock register FLASH_CTL after the erase operation */
    nvmc_flash_lock();
}

/*!
    \brief      program flash double word by double word from FLASH_WRITE_START_ADDR to FLASH_WRITE_END_ADDR
    \param[in]  none
    \param[out] none
    \retval     none
*/
void flash_program(void)
{
    /* unlock register NVMC_FLASH_CTL */
    nvmc_flash_unlock();

    address = FLASH_WRITE_START_ADDR;

    /* program flash */
    while(address < FLASH_WRITE_END_ADDR) {
        nvmc_flash_doubleword_program(address, data);
        address += 8U;
        nvmc_flash_all_flags_clear();
    }

    /* lock register NVMC_FLASH_CTL after the program operation */
    nvmc_flash_lock();
}

/*!
    \brief      check flash erase result
    \param[in]  none
    \param[out] none
    \retval     none
*/
void flash_erase_sectors_check(void)
{
    uint32_t i;

    ptrd = (uint64_t *)FLASH_WRITE_START_ADDR;

    /* check flash whether has been erased */
    for(i = 0U; i < flash_doubleword_cnt; i++) {
        if(0xFFFFFFFFFFFFFFFFU != (*ptrd)) {
            led_num = LED1;
            gd_eval_led_on(led_num);
            while(1);
        } else {
            ptrd++;
        }
    }
}

/*!
    \brief      check flash program result
    \param[in]  none
    \param[out] none
    \retval     none
*/
void flash_program_check(void)
{
    uint32_t i;

    ptrd = (uint64_t *)FLASH_WRITE_START_ADDR;

    /* check flash whether has been programmed */
    for(i = 0U; i < flash_doubleword_cnt; i++) {
        if((*ptrd) != data) {
            led_num = LED2;
            gd_eval_led_on(led_num);
            while(1);
        } else {
            ptrd++;
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

    /* step1: flash erase sectors */
    flash_erase_sectors();

    /* step2: flash program and check if it is successful. If not, light the LED2. */
    flash_program();
    SCB_CleanInvalidateDCache();
    flash_program_check();

    /* step3: flash erase sectors and check if it is successful. If not, light the LED1. */
    flash_erase_sectors();
    SCB_CleanInvalidateDCache();
    flash_erase_sectors_check();

    printf("\r\n flash program and erase successful!\r\n");

    while(1) {
    }
}
