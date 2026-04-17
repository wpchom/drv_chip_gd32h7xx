/*!
    \file    dfu_mal.c
    \brief   USB DFU device media access layer functions

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

#include "dfu_mem.h"
#include "flash_if.h"

#include <stdio.h>

extern struct {
    uint8_t buf[TRANSFER_SIZE];
    uint16_t data_len;
    uint16_t block_num;
    uint32_t base_addr;
} prog;

dfu_mem_prop *mem_tab[MAX_USED_MEMORY_MEDIA] = {
    &dfu_flash_cb
};

/* The list of memory interface string descriptor pointers. This list
   can be updated whenever a memory has to be added or removed */
const uint8_t *usbd_dfu_strdesc[MAX_USED_MEMORY_MEDIA] = {
    (const uint8_t *)FLASH_IF_STRING
};

static uint8_t dfu_mem_checkaddr(uint32_t addr);

/*!
    \brief      initialize the memory media (API_ID(0xD501U))
    \param[in]  none
    \param[out] none
    \retval     MEM_OK
*/
uint8_t dfu_mem_init(void)
{
    uint32_t mem_index = 0U;

    /* initialize all supported memory medias */
    for(mem_index = 0U; mem_index < MAX_USED_MEMORY_MEDIA; mem_index++) {
        /* check if the memory media exists */
        if(NULL != mem_tab[mem_index]->mem_init) {
            mem_tab[mem_index]->mem_init();
        }
    }

    return MEM_OK;
}

/*!
    \brief      deinitialize the memory media (API_ID(0xD502U))
    \param[in]  none
    \param[out] none
    \retval     MEM_OK
*/
uint8_t dfu_mem_deinit(void)
{
    uint32_t mem_index = 0U;

    /* deinitialize all supported memory medias */
    for(mem_index = 0U; mem_index < MAX_USED_MEMORY_MEDIA; mem_index++) {
        /* check if the memory media exists */
        if(NULL != mem_tab[mem_index]->mem_deinit) {
            mem_tab[mem_index]->mem_deinit();
        }
    }

    return MEM_OK;
}

/*!
    \brief      erase a memory sector (API_ID(0xD503U))
    \param[in]  addr: memory sector address/code
    \param[out] none
    \retval     MEM_OK
*/
uint8_t dfu_mem_erase(uint32_t addr)
{
    if (0xFFFFFFFFU == addr) {
        chip_erase();
    } else if (0xFFFFFFFEU == addr) {
        envm_mass_erase();
    } else if (0xFFFFFFFDU == addr) {
        flash_mass_erase();
    } else if ((addr >= ENVM_START) && (addr <= ENVM_END)) {
        nvmc_envm_unlock();

        NVMC_ENVM_STAT = 0xFFFFFFFFU;

        nvmc_envm_sector_erase(addr);

        nvmc_envm_lock();
    } else if ((addr >= FLASH_START) && (addr <= FLASH_END)) {
        nvmc_flash_unlock();

        NVMC_FLASH_STAT = 0xFFFFFFFFU;

        nvmc_flash_page_erase(addr);

        nvmc_flash_lock();
    } else {
    }

    SCB_CleanInvalidateDCache();

    return MEM_OK;
}

/*!
    \brief      mass erase envm (API_ID(0xD504U))
    \param[in]  none
    \param[out] none
    \retval     state of FMC, refer to fmc_state_enum
*/
envm_state_enum envm_mass_erase(void)
{
    envm_state_enum fmc_state = ENVM_BUSY;

    /* unlock the flash operation*/
    nvmc_envm_unlock();

    /* clear flash pending flags */
    NVMC_ENVM_STAT = 0xFFFFFFFFU;

    /* execute erase */
    fmc_state = nvmc_envm_mass_erase();

    while(ENVM_READY != nvmc_envm_ready_wait(ENVM_TIMEOUT_COUNT)) {
    }

    while(SET != nvmc_envm_flag_get(ENVM_FLAG_ENDF)) {
    }

    /* lock the flash operation*/
    nvmc_envm_lock();

    return fmc_state;
}

/*!
    \brief      mass erase flash (API_ID(0xD505U))
    \param[in]  none
    \param[out] none
    \retval     state of FMC, refer to fmc_state_enum
*/
flash_state_enum flash_mass_erase(void)
{
    flash_state_enum fmc_state = FLASH_BUSY;

    /* unlock the flash operation*/
    nvmc_flash_unlock();

    /* clear flash pending flags */
    NVMC_FLASH_STAT = 0xFFFFFFFFU;

    /* execute erase */
    fmc_state = nvmc_flash_mass_erase();

    while(FLASH_READY != nvmc_flash_ready_wait(FLASH_TIMEOUT_COUNT)) {
    }

    while(SET != nvmc_flash_flag_get(FLASH_FLAG_ENDF)) {
    }

    /* lock the flash operation*/
    nvmc_flash_lock();

    return fmc_state;
}

/*!
    \brief      write data to sectors of memory (API_ID(0xD506U))
    \param[in]  buf: the data buffer to be write
    \param[in]  addr: memory sector address/code
    \param[in]  len: data length
    \param[out] none
    \retval     MEM_OK
*/
uint8_t dfu_mem_write(uint8_t *buf, uint32_t addr, uint32_t len)
{
    uint32_t mem_index = dfu_mem_checkaddr(addr);

    if (mem_index < MAX_USED_MEMORY_MEDIA) {
        /* check if the operation is supported */
        if(NULL != mem_tab[mem_index]->mem_write) {
            return mem_tab[mem_index]->mem_write(buf, addr, len);
        } else {
            return MEM_FAIL;
        }
    } else {
        return MEM_FAIL;
    }
}

/*!
    \brief      read data from sectors of memory (API_ID(0xD507U))
    \param[in]  buf: the data buffer to be write
    \param[in]  addr: memory sector address/code
    \param[in]  len: data length
    \param[out] none
    \retval     pointer to buffer
*/
uint8_t *dfu_mem_read(uint8_t *buf, uint32_t addr, uint32_t len)
{
    uint32_t mem_index = 0U;

    if(mem_index < MAX_USED_MEMORY_MEDIA) {
        /* check if the operation is supported */
        if(NULL != mem_tab[mem_index]->mem_read) {
            return mem_tab[mem_index]->mem_read(buf, addr, len);
        } else {
            return buf;
        }
    } else {
        return buf;
    }
}

/*!
    \brief      get the status of a given memory and store in buffer (API_ID(0xD508U))
    \param[in]  addr: memory sector address/code
    \param[in]  cmd: 0 for erase and 1 for write
    \param[in]  buffer: pointer to the buffer where the status data will be stored
    \param[out] none
    \retval     MEM_OK if all operations are OK, MEM_FAIL else
*/
uint8_t dfu_mem_getstatus(uint32_t addr, uint8_t cmd, uint8_t *buffer)
{
    uint32_t mem_index = dfu_mem_checkaddr(addr);

    if(mem_index < MAX_USED_MEMORY_MEDIA) {
        if(cmd & 0x01U) {
            POLLING_TIMEOUT_SET(mem_tab[mem_index]->write_timeout);
        } else {
            POLLING_TIMEOUT_SET(mem_tab[mem_index]->erase_timeout);
        }

        return MEM_OK;
    } else {
        return MEM_FAIL;
    }
}

/*!
    \brief      check the address is supported
    \param[in]  addr: memory sector address/code
    \param[out] none
    \retval     index of the addressed memory
*/
static uint8_t dfu_mem_checkaddr(uint32_t addr)
{
    uint8_t mem_index = 0U;

    /* check with all supported memories */
    for(mem_index = 0U; mem_index < MAX_USED_MEMORY_MEDIA; mem_index++) {
        /* if the check address is supported, return the memory index */
        if(MEM_OK == mem_tab[mem_index]->mem_checkaddr(addr)) {
            return mem_index;
        }
    }

    /* if there is no memory found, return MAX_USED_MEMORY_MEDIA */
    return (MAX_USED_MEMORY_MEDIA);
}
