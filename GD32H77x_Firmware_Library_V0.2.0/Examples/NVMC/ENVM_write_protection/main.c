/*!
    \file    main.c
    \brief   eNVM program, erase and protect

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

#define ENVM_SECTOR_PROGRAM
#define WRITE_PROTECTION_ENABLE
//#define WRITE_PROTECTION_DISABLE

typedef enum {FAILED = 0, PASSED = !FAILED} test_state;
#define ENVM_SECTOR_SIZE          ((uint32_t)0x10000U)
#define ENVM_SECTORS_TO_PROTECT   (OB_ENVM_WP_7)

#define ENVM_WRITE_START_ADDR     ((uint32_t)0x080E0000U)    /* sector 14 */
#define ENVM_WRITE_END_ADDR       ((uint32_t)0x080E0200U)

uint32_t i;
uint32_t erase_counter = 0x0U, address = 0x0U;
uint64_t data[] = {0x1122334455667788, 0x2233445566778899, 0x33445566778899AA, 0x445566778899AABB};
uint32_t wp_value = OB_ENVM_WP_ALL, protected_sectors = 0x0U;
uint32_t sector_number = 1U;
__IO envm_state_enum envm_state = ENVM_READY;
__IO nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;
__IO ErrStatus status = SUCCESS;
__IO test_state program_state = PASSED;

void cache_enable(void);
void nvmc_envm_all_flags_clear(void);


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
    /* unlock the renvm program/erase controller */
    nvmc_envm_unlock();
    nvmc_ob_unlock();

    nvmc_envm_all_flags_clear();
    nvmc_ob_flag_clear(NVMC_FLAG_OBMERR);

    /* get sectors write protection status */
    wp_value = nvmc_ob_write_protection_get(WP_AREA_ENVM) & OB_ENVM_WP_ALL;

#ifdef WRITE_PROTECTION_DISABLE
    /* get sectors already write protected */
    protected_sectors = ~(wp_value | ENVM_SECTORS_TO_PROTECT) & OB_ENVM_WP_ALL;
    /* check if desired sectors are already write protected */
    if(((wp_value | (~ENVM_SECTORS_TO_PROTECT)) & OB_ENVM_WP_ALL) != OB_ENVM_WP_ALL) {
        /* already write protected */
        nvmc_ob_write_protection_disable(WP_AREA_ENVM, OB_ENVM_WP_ALL);
        /* check if there is write protected sectors */
        if(protected_sectors != 0x0U) {
            /* Restore write protected sectors */
            NVMC_WP_ENVM_MDF = (~protected_sectors & OB_ENVM_WP_ALL);
            nvmc_ob_start();
        }
        nvmc_ob_lock();
        nvmc_envm_lock();
        /* generate system reset to load the new option byte values */
        NVIC_SystemReset();
    }
#elif defined WRITE_PROTECTION_ENABLE
    /* get current write protected sectors and the new sectors to be protected */
    protected_sectors = ((~wp_value) | ENVM_SECTORS_TO_PROTECT) & OB_ENVM_WP_ALL;

    /* check if desired sectors are not yet write protected */
    if(((~wp_value) & ENVM_SECTORS_TO_PROTECT) != ENVM_SECTORS_TO_PROTECT) {

        /* enable the sectors write protection */
        nvmc_ob_state = nvmc_ob_write_protection_enable(WP_AREA_ENVM, (uint64_t)protected_sectors);
        nvmc_ob_lock();
        nvmc_envm_lock();
        /* generate system reset to load the new option byte values */
        NVIC_SystemReset();
    }
#endif /* WRITE_PROTECTION_DISABLE */

#ifdef ENVM_SECTOR_PROGRAM

    /* the selected sectors are not write protected */
    if(0x00U != (wp_value & ENVM_SECTORS_TO_PROTECT)) {
        /* clear all pending flags */
        nvmc_envm_all_flags_clear();
        nvmc_ob_flag_clear(NVMC_FLAG_OBMERR);

        /* erase the envm sectors */
        for(erase_counter = 0U; (erase_counter < sector_number) && (ENVM_READY == envm_state); erase_counter++) {
            envm_state = nvmc_envm_sector_erase(ENVM_WRITE_START_ADDR + (ENVM_SECTOR_SIZE * erase_counter));
        }

        /* envm 32 bytes program of data at addresses defined by ENVM_WRITE_START_ADDR and ENVM_WRITE_END_ADDR */
        address = ENVM_WRITE_START_ADDR;

        while((address < ENVM_WRITE_END_ADDR) && (ENVM_READY == envm_state)) {
            status = nvmc_envm_32bytes_program(address, data);
            address = address + 32U;
        }
        /* check the correctness of written data */
        address = ENVM_WRITE_START_ADDR;

        while((address < ENVM_WRITE_END_ADDR) && (FAILED != program_state)) {
            for(i = 0U; i < 4U; i++) {
                if(REG64(address) != data[i]) {
                    program_state = FAILED;
                } else {
                    address += 8U;
                }
            }
        }
        gd_eval_led_on(LED2);
    } else {
        /* error to program the envm: the desired sectors are write protected */
        program_state = FAILED;
        gd_eval_led_on(LED1);
    }
#endif /* ENVM_SECTOR_PROGRAM */
    while(1) {
    }
}
