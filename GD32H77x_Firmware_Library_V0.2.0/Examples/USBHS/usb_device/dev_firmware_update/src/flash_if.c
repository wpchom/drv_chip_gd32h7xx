/*!
    \file    flash_if.c
    \brief   USB DFU device flash interface functions

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

#include "flash_if.h"
#include "bss.h"

__IO uint8_t vfimg_set = 0;
__IO uint32_t certificate_size = 0;

/* local function prototypes ('static') */
static uint8_t flash_if_init(void);
static uint8_t flash_if_deinit(void);
static uint8_t flash_if_erase(uint32_t addr);
static uint8_t flash_if_write(uint8_t *buf, uint32_t addr, uint32_t len);
static uint8_t* flash_if_read(uint8_t *buf, uint32_t addr, uint32_t len);
static uint8_t flash_if_checkaddr(uint32_t addr);

static nvmc_ob_state_enum nvmc_ob_ready_wait(uint32_t timeout);
static envm_state_enum nvmc_envm_state_get(void);
static flash_state_enum nvmc_flash_state_get(void);
static nvmc_ob_state_enum nvmc_ob_state_get(void);

dfu_mem_prop dfu_flash_cb =
{
    (const uint8_t *)FLASH_IF_STRING,

    flash_if_init,
    flash_if_deinit,
    flash_if_erase,
    flash_if_write,
    flash_if_read,
    flash_if_checkaddr,
    60U, /* flash erase timeout in ms */
    80U  /* flash programming timeout in ms (80us * RAM Buffer size (1024 Bytes) */
};

/*!
    \brief      write the option byte
    \param[in]  Mem_Add: memory address
    \param[in]  data: pointer to the data
    \param[out] none
    \retval     state of FMC, refer to fmc_state_enum
*/
nvmc_ob_state_enum option_byte_write(uint32_t mem_addr, uint8_t* data)
{
    nvmc_ob_state_enum status ;

    /* clear pending flags */
    NVMC_FLASH_STAT = 0xFFFFFFFFU;

    uint32_t write_data = WORD_RECOMBINE_BYTE(data);

    if (write_data != REG32(mem_addr - 4)) {
        status = nvmc_ob_ready_wait(FLASH_TIMEOUT_COUNT);

        if (NVMC_OB_READY != status) {
            return status;
        }

        REG32(mem_addr) = write_data;
    }

    return status;
}


void ob_write_all(uint8_t *data)
{
    uint32_t ob_addr = NVMC_OB_START + 4;
    uint8_t *ob_data_addr = data + 4;

    option_byte_write(ob_addr, ob_data_addr);
    option_byte_write(ob_addr + 0x0C, ob_data_addr + 0x0C);
    option_byte_write(ob_addr + 0x14, ob_data_addr + 0x14);
    option_byte_write(ob_addr + 0x1C, ob_data_addr + 0x1C);
    option_byte_write(ob_addr + 0x24, ob_data_addr + 0x24);
    option_byte_write(ob_addr + 0x34, ob_data_addr + 0x34);
    option_byte_write(ob_addr + 0x68, ob_data_addr + 0x68);
    option_byte_write(ob_addr + 0x6C, ob_data_addr + 0x6C);
    option_byte_write(ob_addr + 0x74, ob_data_addr + 0x74);
    option_byte_write(ob_addr + 0x80, ob_data_addr + 0x80);
    option_byte_write(ob_addr + 0x84, ob_data_addr + 0x84);
    option_byte_write(ob_addr + 0x98, ob_data_addr + 0x98);
    option_byte_write(ob_addr + 0x9C, ob_data_addr + 0x9C);
    option_byte_write(ob_addr + 0xA0, ob_data_addr + 0xA0);
    option_byte_write(ob_addr + 0xA4, ob_data_addr + 0xA4);
}

/*!
    \brief      flash memory interface initialization routine
    \param[in]  none
    \param[out] none
    \retval     MAL_OK if the operation is right, MAL_FAIL else
*/
static uint8_t flash_if_init(void)
{
    /* unlock the internal flash */
    nvmc_envm_unlock();
  
    nvmc_flash_unlock();

    return MEM_OK;
}

/*!
    \brief      flash memory interface de-initialization routine
    \param[in]  none
    \param[out] none
    \retval     MAL_OK if the operation is right, MAL_FAIL else
*/
static uint8_t flash_if_deinit(void)
{
    /* lock the internal flash */
    nvmc_envm_lock();
  
    nvmc_flash_lock();

    return MEM_OK;
}

/*!
    \brief      erase flash sector
    \param[in]  addr: flash address to be erased
    \param[out] none
    \retval     MAL_OK if the operation is right, MAL_FAIL else
*/
static uint8_t flash_if_erase(uint32_t addr)
{
//    nvmc_envm_unlock();

//    fmc_sector_erase(addr);

//    nvmc_envm_lock();

    return MEM_OK;
}

/*!
    \brief      flash memory write routine
    \param[in]  buf: data buffer pointer
    \param[in]  addr: flash address to be written
    \param[in]  len: length of data to be written (in bytes)
    \param[out] none
    \retval     MAL_OK if the operation is right, MAL_FAIL else
*/
static uint8_t flash_if_write(uint8_t *buf, uint32_t addr, uint32_t len)
{
    __IO uint32_t idx = 0U, spc = 0U;
    uint16_t add_num = 0U;
  
    nvmc_ob_state_enum status = NVMC_OB_BUSY;
    flash_state_enum fmc_state = FLASH_BUSY;
    ErrStatus envm_status = ERROR;

    if((addr >= NVMC_OB_START) && (addr <= NVMC_OB_END)) {
        nvmc_ob_unlock();

        ob_write_all(buf);

        if (0U == spc_level_get()) {
        NVMC_ENVM_STAT = 0xFFFFFFFFU;

        nvmc_ob_start();
        nvmc_ob_lock();

        status = nvmc_ob_ready_wait(FLASH_TIMEOUT_COUNT);
        while(status == NVMC_OB_READY);

        NVIC_SystemReset();
        }
    }  else if ((addr >= ENVM_START) && (addr < ENVM_END)) {
        /* unlock the flash program erase controller */
        nvmc_envm_unlock();
      
        /* clear envm pending flags */
        NVMC_ENVM_STAT = 0xFFFFFFFFU;

            /* 32byte (256-bit) aligned */
        addr = addr & (~0x1FU);

        if ((addr + len) > ENVM_END) {
            len = ENVM_END - addr + 1U;
        }

        /* multiples of 32 bytes */
        add_num = ((uint16_t)32 - (len & 0x1FU)) & 0x1FU;

        for(idx = 0U; idx < add_num; idx++) {
            buf[len + idx] = 0x00U;
        }

        for(idx = 0U; idx < len; idx = idx + 32U) {
            envm_status = nvmc_envm_32bytes_program(addr, (uint64_t *)&buf[idx]);

            if (SUCCESS != envm_status) {
                break;
            }

            addr = addr + 32U;
        }

        nvmc_envm_lock();
    
      } else if ((addr >= FLASH_START) && (addr < FLASH_END)) {
        /* unlock the flash program erase controller */
        nvmc_flash_unlock();

        NVMC_FLASH_STAT = 0xFFFFFFFFU;

        /* 32 bytes (256-bit) aligned */
        addr = addr & (~0x1FU);

        if ((addr + len) > FLASH_END) {
            len = FLASH_END - addr + 1U;
        }

        /* multiples of 32 bytes, receive data len = data[0] + 1 */
        add_num = ((uint16_t)32 - (len & 0x1FU)) & 0x1FU;

        for(idx = 0; idx < add_num; idx++) { /* from data[(data[0]+1)+1] byte to write 0xFF */
            buf[len + idx] = 0xFFU;
        }

        for(idx = 0U; idx < len; idx = idx + 32U) {
            fmc_state = nvmc_flash_32bytes_program(addr, (uint64_t *)&buf[idx]);
            if(FLASH_READY != fmc_state) {
                break;
            }

            addr = addr + 32U;
        }

        /* lock the flash operation*/
        nvmc_flash_lock();

    } else if((addr >= OCRAM_START) && (addr < OCRAM_END)) {
        for(idx = 0U; idx < len; idx++) {
            *(uint8_t *)addr = *(buf + idx);
            addr++;
        }
    } else {
        /* no operation */
    }

    return MEM_OK;
}

/*!
    \brief      flash memory read routine
    \param[in]  buf: data buffer pointer
    \param[in]  addr: flash address to be read from
    \param[in]  len: length of data to be read (in bytes)
    \param[out] none
    \retval     pointer to the physical address where data should be read
*/
static uint8_t *flash_if_read(uint8_t *buf, uint32_t addr, uint32_t len)
{
    if(((addr >= FLASH_START) && (addr < FLASH_END)) || 
           ((addr >= OCRAM_START) && (addr < OCRAM_END)) || 
              ((addr >= NVMC_OB_START) && (addr < NVMC_OB_END))) {
        return  (uint8_t *)(addr);
    } else if((addr == OTP_USER_CONFIG_START) && (len == OTP_USER_CONFIG_END)) {
        return (uint8_t *)(addr);
    } else if(addr == 0xA0000300) {
        NVIC_SystemReset();

    } else if(addr == 0xA0000200) {
        certificate_size = 136;

        return (uint8_t *)(&certificate_size);
    } else if(addr == 0x20009000) {
        return (uint8_t *)(addr);
    } else if(addr == 0x1FF0FC40) {
        return (uint8_t *)(addr);
    } else {
        return buf;
    }
}

/*!
    \brief      check if the address is an allowed address for this memory
    \param[in]  addr: flash address to be checked
    \param[out] none
    \retval     MAL_OK if the operation is right, MAL_FAIL else
*/
static uint8_t flash_if_checkaddr(uint32_t addr)
{
    if((addr >= FLASH_START) && (addr <= FLASH_END)) {
        return MEM_OK;
    } else if ((addr >= ENVM_START) && (addr <= ENVM_END)) {
        return MEM_OK;
    } else if((addr >= OCRAM_START) && (addr <= OCRAM_END)) {
        return MEM_OK;
    } else if((addr >= NVMC_OB_START) && (addr <= NVMC_OB_END)) {
        return MEM_OK;
    } else if ((addr >= SYSTEM_START) && (addr <= SYSTEM_END)) {
        return MEM_OK;
    } else if((addr >= OTP_USER_CONFIG_START) && (addr <= OTP_USER_CONFIG_END)) {
        return MEM_OK;
    } else if ((addr >= OTP_USER_DATA_START) && (addr <= OTP_USER_DATA_END)) {
        return MEM_OK;
    } else {
        return MEM_FAIL;
    }
}

/*!
    \brief      get spc level
    \param[in]  none
    \param[out] none
    \retval     1: protection level low or high
                0: no protection
*/
uint8_t spc_level_get(void)
{
    uint8_t ob_spc = (NVMC_OBSTAT0_EFT & NVMC_OBSTAT0_EFT_SPC) >> 8;
    uint8_t efuse_spc_l = (NVMC_OTP_USER_CTL0 & NVMC_OTP_USER_CTL0_SPC_L) >> 19;
    uint8_t efuse_spc_h = (NVMC_OTP_USER_CTL0 & NVMC_OTP_USER_CTL0_SPC_H) >> 20;

    if ((efuse_spc_h == 0) && (efuse_spc_l == 0) && (ob_spc == 0xAA)) {
        return 0;
    } else {
        return 1;
    }
}

/*!
    \brief      get envm state (exclude envm crc operation) (API_ID(0x0026U))
    \param[in]  none
    \param[out] none
    \retval     state of ENVM
      \arg        ENVM_READY: ENVM operation has been completed
      \arg        ENVM_BUSY: ENVM operation is in progress
      \arg        ENVM_WPERR: ENVM erase/program protection error
      \arg        ENVM_PGSERR: ENVM program sequence error
      \arg        ENVM_RPERR: ENVM read protection error
      \arg        ENVM_RSERR: ENVM read secure error
*/
static envm_state_enum nvmc_envm_state_get(void)
{
    envm_state_enum envm_state = ENVM_READY;

    if(ENVM_FLAG_BUSY == (NVMC_ENVM_STAT & ENVM_FLAG_BUSY)) {
        envm_state = ENVM_BUSY;
    } else if(RESET != (NVMC_ENVM_STAT & ENVM_FLAG_WPERR)) {
        envm_state = ENVM_WPERR;
    } else if(RESET != (NVMC_ENVM_STAT & ENVM_FLAG_RSERR)) {
        envm_state = ENVM_RSERR;
    } else if(RESET != (NVMC_ENVM_STAT & ENVM_FLAG_RPERR)) {
        envm_state = ENVM_RPERR;
    } else if(RESET != (NVMC_ENVM_STAT & ENVM_FLAG_PGSERR)) {
        envm_state = ENVM_PGSERR;
    } else {
        envm_state = ENVM_READY;
    }

    /* return the ENVM state */
    return envm_state;
}

/*!
    \brief      get the flash state (API_ID(0x0039U))
    \param[in]  none
    \param[out] none
    \retval     state of flash
      \arg        FLASH_READY: flash operation has been completed
      \arg        FLASH_BUSY: flash operation is in progress
      \arg        FLASH_WPERR: flash erase/program protection error
      \arg        FLASH_PGSERR: flash program sequence error
      \arg        FLASH_RPERR: flash read protection error
      \arg        FLASH_RSERR: flash read secure error
*/
static flash_state_enum nvmc_flash_state_get(void)
{
    flash_state_enum flash_state = FLASH_READY;

    if(FLASH_FLAG_BUSY == (NVMC_FLASH_STAT & FLASH_FLAG_BUSY)) {
        flash_state = FLASH_BUSY;
    } else if(RESET != (NVMC_FLASH_STAT & FLASH_FLAG_WPERR)) {
        flash_state = FLASH_WPERR;
    } else if(RESET != (NVMC_FLASH_STAT & FLASH_FLAG_RSERR)) {
        flash_state = FLASH_RSERR;
    } else if(RESET != (NVMC_FLASH_STAT & FLASH_FLAG_RPERR)) {
        flash_state = FLASH_RPERR;
    } else if(RESET != (NVMC_FLASH_STAT & FLASH_FLAG_PGSERR)) {
        flash_state = FLASH_PGSERR;
    } else {
        flash_state = FLASH_READY;
    }

    /* return the flash state */
    return flash_state;
}

/*!
    \brief      mass erase envm
    \param[in]  none
    \param[out] none
    \retval     state of FMC, refer to fmc_state_enum
*/
ErrStatus chip_erase(void)
{
    ErrStatus status = ERROR;

    /* unlock the envm operation */
    nvmc_envm_unlock();

    /* unlock the flash operation*/
    nvmc_flash_unlock();

    /* clear envm pending flags */
    NVMC_ENVM_STAT = 0xFFFFFFFFU;

    /* clear flash pending flags */
    NVMC_FLASH_STAT = 0xFFFFFFFFU;

    /* execute erase */
    status = nvmc_typical_chip_erase();

    while(ENVM_READY != nvmc_envm_ready_wait(ENVM_TIMEOUT_COUNT)) {
    }

    while(SET != nvmc_envm_flag_get(ENVM_FLAG_ENDF)) {
    }

    while(FLASH_READY != nvmc_flash_ready_wait(FLASH_TIMEOUT_COUNT)) {
    }

    while(SET != nvmc_flash_flag_get(FLASH_FLAG_ENDF)) {
    }

    /* lock the envm operation*/
    nvmc_envm_lock();

    /* lock the flash operation*/
    nvmc_flash_lock();

    return status;
}

/*!
    \brief      check whether envm is ready or not (exclude envm crc operation) (API_ID(0x0027U))
    \param[in]  timeout: timeout count (0x00000000 - 0xFFFFFFFF)
    \param[out] none
    \retval     state of ENVM
      \arg        ENVM_READY: ENVM operation has been completed
      \arg        ENVM_BUSY: ENVM operation is in progress
      \arg        ENVM_WPERR: ENVM erase/program protection error
      \arg        ENVM_PGSERR: ENVM program sequence error
      \arg        ENVM_RPERR: ENVM read protection error
      \arg        ENVM_RSERR: ENVM read secure error
      \arg        ENVM_TOERR: ENVM timeout error
    \note       This function contains scenarios leading to an infinite loop.
                Modify according to the user's actual usage scenarios.
*/
envm_state_enum nvmc_envm_ready_wait(uint32_t timeout)
{
    envm_state_enum envm_state = ENVM_BUSY;

    /* wait for ENVM ready */
    do {
        /* get ENVM state */
        envm_state = nvmc_envm_state_get();
        timeout--;
    } while((ENVM_BUSY == envm_state) && (0U != timeout));

    if(ENVM_BUSY == envm_state) {
        envm_state = ENVM_TOERR;
    }
    /* return the ENVM state */
    return envm_state;
}

/*!
    \brief      check whether option byte is ready or not (API_ID(0x00A7U))
    \param[in]  timeout: timeout count (0x00000000 - 0xFFFFFFFF)
    \param[out] none
    \retval     state of option byte
      \arg        NVMC_OB_READY: option byte operation has been completed
      \arg        NVMC_OB_BUSY: option byte operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modify error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
    \note       This function contains scenarios leading to an infinite loop.
                Modify according to the user's actual usage scenarios.
*/
static nvmc_ob_state_enum nvmc_ob_ready_wait(uint32_t timeout)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_BUSY;

    /* wait for NVMC ready */
    do {
        /* get NVMC state */
        nvmc_ob_state = nvmc_ob_state_get();
        timeout--;
    } while((NVMC_OB_BUSY == nvmc_ob_state) && (0U != timeout));

    if(NVMC_OB_BUSY == nvmc_ob_state) {
        nvmc_ob_state = NVMC_OB_TOERR;
    }
    /* return the NVMC state */
    return nvmc_ob_state;
}

/*!
    \brief      check whether flash is ready or not (API_ID(0x003AU))
    \param[in]  timeout: timeout count (0x00000000 - 0xFFFFFFFF)
    \param[out] none
    \retval     state of flash
      \arg        FLASH_READY: flash operation has been completed
      \arg        FLASH_BUSY: flash operation is in progress
      \arg        FLASH_WPERR: flash erase/program protection error
      \arg        FLASH_PGSERR: flash program sequence error
      \arg        FLASH_RPERR: flash read protection error
      \arg        FLASH_RSERR: flash read secure error
      \arg        FLASH_TOERR: flash timeout error
    \note       This function contains scenarios leading to an infinite loop.
                Modify according to the user's actual usage scenarios.
*/
flash_state_enum nvmc_flash_ready_wait(uint32_t timeout)
{
    flash_state_enum flash_state = FLASH_BUSY;

    /* wait for flash ready */
    do {
        /* get flash state */
        flash_state = nvmc_flash_state_get();
        timeout--;
    } while((FLASH_BUSY == flash_state) && (0U != timeout));

    if(FLASH_BUSY == flash_state) {
        flash_state = FLASH_TOERR;
    }
    /* return the flash state */
    return flash_state;
}

/*!
    \brief      get the option byte state (API_ID(0x00A6U))
    \param[in]  none
    \param[out] none
    \retval     state of NVMC
      \arg        NVMC_OB_READY: option byte operation has been completed
      \arg        NVMC_OB_BUSY: option byte operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modify error
*/
static nvmc_ob_state_enum nvmc_ob_state_get(void)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

#ifndef NVM_VER_CASE4
    if(FLASH_FLAG_BUSY == (NVMC_FLASH_STAT & FLASH_FLAG_BUSY)) {
        nvmc_ob_state = NVMC_OB_BUSY;
    }
#else
    if(ENVM_FLAG_BUSY == (NVMC_ENVM_STAT & ENVM_FLAG_BUSY)) {
        nvmc_ob_state = NVMC_OB_BUSY;
    }
#endif
    else if(RESET != (NVMC_OBCTL & NVMC_OBCTL_OBMERR)) {
        nvmc_ob_state = NVMC_OB_OBMERR;
    } else {
        nvmc_ob_state = NVMC_OB_READY;
    }

    /* return the NVMC state */
    return nvmc_ob_state;
}
