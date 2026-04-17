/*!
    \file    flash_if.h
    \brief   USB DFU device flash interface header file

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

#ifndef FLASH_IF_H
#define FLASH_IF_H

#include "dfu_mem.h"
#include "gd32h77x_nvmc.h"

#define FLASH_START             0x08200000
#define FLASH_END               0x0857FFFF

#define FMC_OB_START            (FMC_BASE + 0x1C)
#define FMC_OB_END              (FMC_BASE + 0x57)

#define SYSTEM_START            0x1FEF0000
#define SYSTEM_END              0x1FF1FFFF

#define OCRAM_START             0x24010000U
#define OCRAM_END               0x2407FFFFU

#define ENVM_START              0x08000000
#define ENVM_END                0x081FFFFF

#define NVMC_OB_START           0x5200201C
#define NVMC_OB_END             0x520020C4

#define OTP_USER_DATA_ADDR                          ((uint32_t)0x1FFF02C0U)                   /*!< AXI access address of OTP user data parameter */

#define OTP_USER_CONFIG_LEN     320U
#define OTP_USER_CONFIG_START   OTP_BASE_ADDRESS
#define OTP_USER_CONFIG_END     (OTP_USER_CONFIG_START + OTP_USER_CONFIG_LEN)

#define OTP_USER_DATA_LEN       5952U
#define OTP_USER_DATA_START     OTP_USER_DATA_ADDR
#define OTP_USER_DATA_END       (OTP_USER_DATA_START + OTP_USER_DATA_LEN)

#define MAL_MASK_OB             0xFFFFFF00U

#define FLASH_IF_STRING         "@Internal Flash   /0x08000000/16*002Ka,112*002Kg"

/* recombine word from byte */
#define WORD_RECOMBINE_BYTE(addr)            (((uint32_t)(*(uint8_t *)(addr))) | ((uint32_t)(*(uint8_t *)(addr + 1)) << 8) | \
                                                ((uint32_t)(*(uint8_t *)(addr + 2)) << 16) | ((uint32_t)(*(uint8_t *)(addr + 3)) << 24))

extern dfu_mem_prop dfu_flash_cb;

/* function declarations */
/* write the option byte */
nvmc_ob_state_enum option_byte_write(uint32_t mem_add, uint8_t* data);

envm_state_enum nvmc_envm_ready_wait(uint32_t timeout);
flash_state_enum nvmc_flash_ready_wait(uint32_t timeout);

uint8_t spc_level_get(void);

ErrStatus chip_erase(void);
envm_state_enum envm_mass_erase(void);
flash_state_enum flash_mass_erase(void);

#endif /* FLASH_IF_H */
