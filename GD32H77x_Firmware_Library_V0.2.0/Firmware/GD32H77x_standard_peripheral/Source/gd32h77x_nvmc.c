/*!
    \file    gd32h77x_nvmc.c
    \brief   NVMC driver

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

#include "gd32h77x_nvmc.h"

#if (defined(GD32H77EXW) || (GD32H77DXW) || (GD32H779XW))
#define NVM_VER_CASE1
#elif (defined(GD32H77EXP) || (GD32H77DXP) || (GD32H779XP))
#define NVM_VER_CASE2
#elif (defined(GD32H77EXN) || (GD32H77DXN) || (GD32H779XN))
#define NVM_VER_CASE3
#elif (defined(GD32H77EXI) || (GD32H77DXI) || (GD32H779XI))
#define NVM_VER_CASE4
#endif

/* NVMC register bit offset */
#define BTADDR_BOOT_ADDR0_OFFSET                    ((uint32_t)0x00000000U)       /*!< bit offset of BOOT_ADDR0 in NVMC_BTADDR_EFT/NVMC_BTADDR_MDF */
#define BTADDR_BOOT_ADDR1_OFFSET                    ((uint32_t)0x00000010U)       /*!< bit offset of BOOT_ADDR1 in NVMC_BTADDR_EFT/NVMC_BTADDR_MDF */
#define SCRADDR_SCR_AREA_START_OFFSET               ((uint32_t)0x00000000U)       /*!< bit offset of SCR_AREA_START in NVMC_SCRADDR_EFT/NVMC_SCRADDR_MDF */
#define SCRADDR_SCR_AREA_END_OFFSET                 ((uint32_t)0x00000010U)       /*!< bit offset of SCR_AREA_END in NVMC_SCRADDR_EFT/NVMC_SCRADDR_MDF */
#define DCRPADDR_DCRP_AREA_START_OFFSET             ((uint32_t)0x00000000U)       /*!< bit offset of DCRP_AREA_START in NVMC_DCRPADDR_EFT/NVMC_DCRPADDR_MDF */
#define DCRPADDR_DCRP_AREA_END_OFFSET               ((uint32_t)0x00000010U)       /*!< bit offset of DCRP_AREA_END in NVMC_DCRPADDR_EFT/NVMC_DCRPADDR_MDF */
#define OBSTAT0_SPC_OFFSET                          ((uint32_t)0x00000008U)       /*!< bit offset of SPC in NVMC_OBSTAT0_EFT/NVMC_OBSTAT0_MDF */
#define OBSTAT1_DATA_OFFSET                         ((uint32_t)0x00000010U)       /*!< bit offset of DATA in NVMC_OBSTAT1_EFT/NVMC_OBSTAT1_MDF */
#define OBREPSTADDR_STARTADDR_OFFSET                ((uint32_t)0x00000005U)       /*!< bit offset of STARTADDR in NVMC_OBREPSTADDR_EFT_STARTADDR/NVMC_OBREPSTADDR_MDF_STARTADDR */
#define OBREPSTADDR_ENDADDR_OFFSET                  ((uint32_t)0x00000005U)       /*!< bit offset of ENDADDR in NVMC_OBREPSTADDR_EFT_STARTADDR/NVMC_OBREPSTADDR_MDF_STARTADDR */
#define OBBBADDRX_OBBBADDRVLD_OFFSET                ((uint32_t)0x00000000U)       /*!< bit offset of OBBBADDRVLD in NVMC_OBBBADDRX_EFT/NVMC_OBBBADDRX_MDF */
#define OBBBADDRX_OBBBADDR_OFFSET                   ((uint32_t)0x00000005U)       /*!< bit offset of OBBBADDR in NVMC_OBBBADDRX_EFT/NVMC_OBBBADDRX_MDF */
#define OBREPCTL_COPYSZ_OFFSET                      ((uint32_t)0x0000001AU)       /*!< bit offset of COPYSZ in NVMC_OBREPCTL_EFT/NVMC_OBREPCTL_MDF */
#define OBREPCTL_ERR1CNTTH_OFFSET                   ((uint32_t)0x00000010U)       /*!< bit offset of ERR1CNTTH in NVMC_OBREPCTL_EFT/NVMC_OBREPCTL_MDF */
#define OBREPCTL_ERR2CNTTH_OFFSET                   ((uint32_t)0x00000015U)       /*!< bit offset of ERR2CNTTH in NVMC_OBREPCTL_EFT/NVMC_OBREPCTL_MDF */
#define OBREPCTL_SYNSZ_OFFSET                       ((uint32_t)0x00000008U)       /*!< bit offset of SYNSZ in NVMC_OBREPCTL_EFT/NVMC_OBREPCTL_MDF */

#define ROBBADDRX_ROBBADDRSTAT_OFFSET               ((uint32_t)0x00000000U)       /*!< bit offset of ROBBADDRSTAT in NVMC_ENVM_ROBBADDRX */
#define ROBBADDRX_ROBBADDR_OFFSET                   ((uint32_t)0x00000005U)       /*!< bit offset of ROBBADDR in NVMC_ENVM_ROBBADDRX */
#define AUTOERR1ADDRX_AUTOERR1ADDRSTAT_OFFSET       ((uint32_t)0x00000000U)       /*!< bit offset of AUTOERR1ADDRSTAT in NVMC_ENVM_AUTOERR1ADDRX */
#define AUTOERR1ADDRX_AUTOERR1ADDR_OFFSET           ((uint32_t)0x00000005U)       /*!< bit offset of AUTOERR1ADDR in NVMC_ENVM_AUTOERR1ADDRX */
#define AUTOERR2ADDRX_AUTOERR2ADDRSTAT_OFFSET       ((uint32_t)0x00000000U)       /*!< bit offset of AUTOERR2ADDRSTAT in NVMC_ENVM_AUTOERR2ADDRX */
#define AUTOERR2ADDRX_AUTOERR2ADDR_OFFSET           ((uint32_t)0x00000005U)       /*!< bit offset of AUTOERR2ADDR in NVMC_ENVM_AUTOERR2ADDRX */
#define UNREPECCADDRX_ERRADDRSTAT_OFFSET            ((uint32_t)0x00000000U)       /*!< bit offset of ERRADDRSTAT in NVMC_ENVM_UNREPECCADDRX */
#define UNREPECCADDRX_ERRADDR_OFFSET                ((uint32_t)0x00000005U)       /*!< bit offset of ERRADDR in NVMC_ENVM_UNREPECCADDRX */

#define ENVM_REPCS_ERR2CNT_OFFSET                   ((uint32_t)0x0000001BU)       /*!< bit offset of ERR2CNT in NVMC_ENVM_REPCS */
#define ENVM_REPCS_ERR1CNT_OFFSET                   ((uint32_t)0x00000016U)       /*!< bit offset of ERR1CNT in NVMC_ENVM_REPCS */
#define ENVM_REPCS_ROBBCNT_OFFSET                   ((uint32_t)0x00000010U)       /*!< bit offset of ROBBCNT in NVMC_ENVM_REPCS */
#define ENVM_REPCS_REPERRSTAT_OFFSET                ((uint32_t)0x00000008U)       /*!< bit offset of REPERRSTAT in NVMC_ENVM_REPCS */

#define WP1_FLASH_OFFSET                            ((uint32_t)0x00000020U)       /*!< bit offset of WP1_FLASH in OB_FL_WP_ALL */

/* option byte factory value */
#define OB_OBSTAT0_FACTORY_VALUE                    ((uint32_t)0x5FC6AAD0U)       /*!< the factory value of option byte in NVMC_OBSTAT0_EFT/NVMC_OBSTAT0_MDF */
#define OB_OBSTAT1_FACTORY_VALUE                    ((uint32_t)0xFFFF0000U)       /*!< the factory value of option byte in NVMC_OBSTAT1_EFT/NVMC_OBSTAT1_MDF */
#define OB_BTADDR_FACTORY_VALUE                     ((uint32_t)0x1FEF0800U)       /*!< the factory value of option byte in NVMC_BTADDR_EFT/NVMC_BTADDR_MDF */
#define OB_DCRPADDR_FACTORY_VALUE                   ((uint32_t)0x000001FFU)       /*!< the factory value of option byte in NVMC_DCRPADDR_EFT/NVMC_DCRPADDR_MDF */
#define OB_SCRADDR_FACTORY_VALUE                    ((uint32_t)0x000001FFU)       /*!< the factory value of option byte in NVMC_SCRADDR_EFT/NVMC_SCRADDR_MDF */
#define OB_WP_ENVM_FACTORY_VALUE                    ((uint32_t)0x0000FFFFU)       /*!< the factory value of option byte in NVMC_WP_ENVM_EFT/NVMC_WP_ENVM_MDF */
#define OB_WP0_FLASH_FACTORY_VALUE                  ((uint32_t)0xFFFFFFFFU)       /*!< the factory value of option byte in NVMC_WP0_FLASH_EFT/NVMC_WP0_FLASH_MDF */
#define OB_WP1_FLASH_FACTORY_VALUE                  ((uint32_t)0x0FFFFFFFU)       /*!< the factory value of option byte in NVMC_WP1_FLASH_EFT/NVMC_WP1_FLASH_MDF */
#define OB_OBREPCTL_FACTORY_VALUE                   ((uint32_t)0xFE10DF00U)       /*!< the factory value of option byte in NVMC_OBREPCTL_EFT/NVMC_OBREPCTL_MDF */
#define OB_OBREPSTADDR_FACTORY_VALUE                ((uint32_t)0x08000000U)       /*!< the factory value of option byte in NVMC_OBREPSTADDR_EFT/NVMC_OBREPSTADDR_MDF */
#define OB_OBREPENDADDR_FACTORY_VALUE               ((uint32_t)0x08000000U)       /*!< the factory value of option byte in NVMC_OBREPENDADDR_EFT/NVMC_OBREPENDADDR_MDF */
#define OB_OBBBADDRX_FACTORY_VALUE                  ((uint32_t)0x08000000U)       /*!< the factory value of option byte in NVMC_OBBBADDRX_EFT/NVMC_OBBBADDRX_MDF */

/* NVMC parameter mask */
#define DCRP_EN_MASK                                ((uint32_t)0x0000E000U)        /*!< DCRP area enable bits mask */
#define DCRP_EREN_MASK                              ((uint32_t)0xE0000000U)        /*!< DCRP area erase enable bits mask */
#define SCR_EN_MASK                                 ((uint32_t)0x0000E000U)        /*!< SCR area enable bits mask */
#define SCR_EREN_MASK                               ((uint32_t)0xE0000000U)        /*!< SCR area erase enable bits mask */
#define LOW_32BITS_MASK                             ((uint32_t)0xFFFFFFFFU)        /*!< the 0-31 bits mask of a word */

#define NVMC_DCRPADDR_MDF_DCRP_EREN_29_31 (NVMC_DCRPADDR_MDF_FLB1_DCRP_EREN | NVMC_DCRPADDR_MDF_FLB0_DCRP_EREN | NVMC_DCRPADDR_MDF_ENVM_DCRP_EREN)
#define NVMC_SCRADDR_MDF_SCR_EREN_29_31 (NVMC_SCRADDR_MDF_FLB1_SCR_EREN | NVMC_SCRADDR_MDF_FLB0_SCR_EREN | NVMC_SCRADDR_MDF_ENVM_SCR_EREN)

/* get ENVM state */
static envm_state_enum nvmc_envm_state_get(void);
/* check ENVM ready or not */
static envm_state_enum nvmc_envm_ready_wait(uint32_t timeout);
/* get ENVM crc state */
static envm_state_enum nvmc_envm_crc_state_get(void);
/* check ENVM crc ready or not */
static envm_state_enum nvmc_envm_crc_ready_wait(uint32_t timeout);
/* get ENVM repair state */
static envm_state_enum nvmc_envm_repair_state_get(void);
/* check ENVM repair ready or not */
static envm_state_enum nvmc_envm_repair_ready_wait(uint32_t timeout);
/* get flash state */
static flash_state_enum nvmc_flash_state_get(void);
/* check flash ready or not */
static flash_state_enum nvmc_flash_ready_wait(uint32_t timeout);
/* get OTP state */
static otp_state_enum nvmc_otp_state_get(void);
/* check OTP ready or not */
static otp_state_enum nvmc_otp_ready_wait(uint32_t timeout);
/* get option byte state */
static nvmc_ob_state_enum nvmc_ob_state_get(void);
/* check option byte ready or not */
static nvmc_ob_state_enum nvmc_ob_ready_wait(uint32_t timeout);

/*!
    \brief      unlock register NVMC_ENVM_CTL (API_ID(0x0001U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void nvmc_envm_unlock(void)
{
    if((RESET != (NVMC_ENVM_CTL & NVMC_ENVM_CTL_LK))) {
        /* write the ENVM key */
        NVMC_ENVM_KEY = UNLOCK_KEY0;
        NVMC_ENVM_KEY = UNLOCK_KEY1;
    }
}

/*!
    \brief      lock register NVMC_ENVM_CTL (API_ID(0x0002U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void nvmc_envm_lock(void)
{
    /* set the LK bit*/
    NVMC_ENVM_CTL |= NVMC_ENVM_CTL_LK;
}

/*!
    \brief      set the ENVM wait state counter value (API_ID(0x0003U))
    \param[in]  wscnt: wait state counter value
                only one parameter can be selected which is shown as below:
      \arg        PERFCTL_WSCNT_1: 1 wait state added
      \arg        PERFCTL_WSCNT_2: 2 wait state added
      \arg        PERFCTL_WSCNT_3: 3 wait state added
      \arg        PERFCTL_WSCNT_4: 4 wait state added
      \arg        PERFCTL_WSCNT_5: 5 wait state added
      \arg        PERFCTL_WSCNT_6: 6 wait state added
      \arg        PERFCTL_WSCNT_7: 7 wait state added
      \arg        PERFCTL_WSCNT_8: 8 wait state added
      \arg        PERFCTL_WSCNT_9: 9 wait state added
      \arg        PERFCTL_WSCNT_10: 10 wait state added
      \arg        PERFCTL_WSCNT_11: 11 wait state added
      \arg        PERFCTL_WSCNT_12: 12 wait state added
      \arg        PERFCTL_WSCNT_13: 13 wait state added
      \arg        PERFCTL_WSCNT_14: 14 wait state added
      \arg        PERFCTL_WSCNT_15: 15 wait state added
    \param[out] none
    \retval     none
*/
void nvmc_envm_wscnt_set(uint8_t wscnt)
{
    uint32_t reg;
    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_ENVM_PERFCTL_WSCNT(wscnt)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x0003U), ERR_PARAM_INVALID);
    } else
#endif
    {
        reg = NVMC_PERFCTL;
        /* set the wait state counter value */
        reg &= ~NVMC_PERFCTL_WSCNT;
        NVMC_PERFCTL = (reg | wscnt);
    }
}

/*!
    \brief      empty ENVM buffer(general buffer and CPU buffer) (API_ID(0x0004U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void nvmc_envm_buf_empty(void)
{
    NVMC_PERFCTL |= NVMC_PERFCTL_EMPTY_ENVM_BUF;
}

/*!
    \brief      configure flash buffer (API_ID(0x0005U))
    \param[in]  buf_configure: flash buffer prefetch control
                only one parameter can be selected which is shown as below:
      \arg        FLASH_NO_PREFETCH: flash no prefetch
      \arg        FLASH_PREFETCH_1_LINE: flash prefetch 1 line
      \arg        FLASH_PREFETCH_3_LINE: flash prefetch 3 line
    \param[out] none
    \retval     none
*/
void nvmc_flash_buffer_configure(uint32_t buf_configure)
{
    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_FLASH_PREFETCH(buf_configure)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x0005U), ERR_PARAM_INVALID);
    } else
#endif
    {
        NVMC_PERFCTL &= ~NVMC_PERFCTL_FLASH_BUF_PFETCH;
        NVMC_PERFCTL |= buf_configure;
    }
}

/*!
    \brief      configure ENVM general buffer size (API_ID(0x0006U))
    \param[in]  size: general buffer size
                only one parameter can be selected which is shown as below:
      \arg        ENVM_GP_BUF_SIZE1: ENVM general buffer size is 1x32 bytes
      \arg        ENVM_GP_BUF_SIZE2: ENVM general buffer size is 2x32 bytes
      \arg        ENVM_GP_BUF_SIZE4: ENVM general buffer size is 4x32 bytes
      \arg        ENVM_GP_BUF_SIZE8: ENVM general buffer size is 8x32 bytes
    \param[out] none
    \retval     none
*/
void nvmc_envm_gp_buffer_configure(uint32_t size)
{
    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_ENVM_GP_BUF_SIZE(size)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x0006U), ERR_PARAM_INVALID);
    } else
#endif
    {
        NVMC_PERFCTL &= ~NVMC_PERFCTL_ENVM_GP_BUFSZ;
        NVMC_PERFCTL |= size;
    }
}

/*!
    \brief      configure ENVM CPU buffer size (API_ID(0x0007U))
    \param[in]  size: CPU buffer size
                only one parameter can be selected which is shown as below:
      \arg        ENVM_CPU_BUF_SIZE1: ENVM CPU buffer size is 1x32 bytes
      \arg        ENVM_CPU_BUF_SIZE2: ENVM CPU buffer size is 2x32 bytes
      \arg        ENVM_CPU_BUF_SIZE4: ENVM CPU buffer size is 4x32 bytes
      \arg        ENVM_CPU_BUF_SIZE8: ENVM CPU buffer size is 8x32 bytes
    \param[out] none
    \retval     none
*/
void nvmc_envm_cpu_buffer_configure(uint32_t size)
{
    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_ENVM_CPU_BUF_SIZE(size)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x0007U), ERR_PARAM_INVALID);
    } else
#endif
    {
        NVMC_PERFCTL &= ~NVMC_PERFCTL_ENVM_CPU_BUFSZ;
        NVMC_PERFCTL |= size;
    }
}

/*!
    \brief      program 32 bytes at the corresponding address (API_ID(0x0008U))
    \param[in]  address: address to program, the address is in the range of ENVM block
    \param[in]  data: 32 bytes to program
    \param[out] none
    \retval     ErrStatus: ERROR or SUCCESS
*/
ErrStatus nvmc_envm_32bytes_program(uint32_t address, uint64_t data[])
{
    uint32_t index;
    ErrStatus status;
    envm_state_enum envm_state = ENVM_READY;
    flash_state_enum flash_state = FLASH_READY;

    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_ENVM_VALID_ADDRESS(address)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x0008U), ERR_PARAM_OUT_OF_RANGE);
    } else if(NOT_VALID_POINTER(data)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x0008U), ERR_PARAM_POINTER);
    } else
#endif
    {
        /* wait for ENVM ready */
        envm_state = nvmc_envm_ready_wait(ENVM_TIMEOUT_COUNT);

        uint32_t sync_area_size = nvmc_ob_sync_area_size_get();
        if((nvmc_ob_sync_mode_get() == SET) && ((address - ENVM_BASE_ADDRESS) < sync_area_size)) {
            /* wait for flash ready */
            flash_state = nvmc_flash_ready_wait(FLASH_TIMEOUT_COUNT);

            if((ENVM_READY == envm_state) && (FLASH_READY == flash_state)) {
                /* set the PG bit to start program */
                NVMC_ENVM_CTL |= NVMC_ENVM_CTL_PG;

                __ISB();
                __DSB();

                for(index = 0U; index < 4U; index++) {
                    REG64(address) = data[index];
                    __DSB();
                    address += 8U;
                }

                __ISB();
                __DSB();

                /* wait for the ENVM ready */
                envm_state = nvmc_envm_ready_wait(ENVM_TIMEOUT_COUNT);
                /* wait for the flash ready */
                flash_state = nvmc_flash_ready_wait(FLASH_TIMEOUT_COUNT);
                /* reset the PG bit */
                NVMC_ENVM_CTL &= ~NVMC_ENVM_CTL_PG;

                if((FLASH_READY == flash_state) && (ENVM_READY == envm_state)) {
                    status = SUCCESS;
                } else {
                    status = ERROR;
                }
            } else {
                status = ERROR;
            }
        } else {
            if(ENVM_READY == envm_state) {
                /* set the PG bit to start program */
                NVMC_ENVM_CTL |= NVMC_ENVM_CTL_PG;

                __ISB();
                __DSB();

                for(index = 0U; index < 4U; index++) {
                    REG64(address) = data[index];
                    __DSB();
                    address += 8U;
                }

                __ISB();
                __DSB();

                /* wait for the ENVM ready */
                envm_state = nvmc_envm_ready_wait(ENVM_TIMEOUT_COUNT);
                /* reset the PG bit */
                NVMC_ENVM_CTL &= ~NVMC_ENVM_CTL_PG;

                if(ENVM_READY == envm_state) {
                    status = SUCCESS;
                } else {
                    status = ERROR;
                }
            } else {
                status = ERROR;
            }
        }
    }
    return status;
}

/*!
    \brief      erase sector of ENVM (API_ID(0x0009U))
    \param[in]  address: sector address to erase
    \param[out] none
    \retval     state of ENVM
      \arg        ENVM_READY: ENVM operation has been completed
      \arg        ENVM_BUSY: ENVM operation is in progress
      \arg        ENVM_WPERR: ENVM erase/program protection error
      \arg        ENVM_PGSERR: ENVM program sequence error
      \arg        ENVM_RPERR: ENVM read protection error
      \arg        ENVM_RSERR: ENVM read secure error
      \arg        ENVM_TOERR: ENVM timeout error
*/
envm_state_enum nvmc_envm_sector_erase(uint32_t address)
{
    envm_state_enum envm_state = ENVM_READY;

    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_ENVM_VALID_ADDRESS(address)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x0009U), ERR_PARAM_OUT_OF_RANGE);
    } else
#endif
    {
        /* wait for the ENVM ready */
        envm_state = nvmc_envm_ready_wait(ENVM_TIMEOUT_COUNT);

        if(ENVM_READY == envm_state) {
            NVMC_ENVM_CTL |= NVMC_ENVM_CTL_SER;
            /* write the sector address */
            NVMC_ENVM_ADDR = address;
            /* start sector erase */
            NVMC_ENVM_CTL |= NVMC_ENVM_CTL_START;
            /* wait for the ENVM ready */
            envm_state = nvmc_envm_ready_wait(ENVM_TIMEOUT_COUNT);
            /* reset the SER bit */
            NVMC_ENVM_CTL &= (~NVMC_ENVM_CTL_SER);
        }
    }
    /* return the ENVM state */
    return envm_state;
}

#ifndef NVM_VER_CASE4
/*!
    \brief      erase whole ENVM (API_ID(0x000AU))
    \param[in]  none
    \param[out] none
    \retval     state of ENVM
      \arg        ENVM_READY: ENVM operation has been completed
      \arg        ENVM_BUSY: ENVM operation is in progress
      \arg        ENVM_WPERR: ENVM erase/program protection error
      \arg        ENVM_PGSERR: ENVM program sequence error
      \arg        ENVM_RPERR: ENVM read protection error
      \arg        ENVM_RSERR: ENVM read secure error
      \arg        ENVM_TOERR: ENVM timeout error
*/
envm_state_enum nvmc_envm_mass_erase(void)
{
    envm_state_enum envm_state = ENVM_READY;

    /* wait for the ENVM ready */
    envm_state = nvmc_envm_ready_wait(ENVM_TIMEOUT_COUNT);

    if(ENVM_READY == envm_state) {
        /* enable mass erase operation */
        NVMC_ENVM_CTL |= NVMC_ENVM_CTL_MER;
        /* start whole ENVM */
        NVMC_ENVM_CTL |= NVMC_ENVM_CTL_START;
        /* wait for the ENVM ready */
        envm_state = nvmc_envm_ready_wait(ENVM_TIMEOUT_COUNT);
        /* reset the MER bit */
        NVMC_ENVM_CTL &= ~NVMC_ENVM_CTL_MER;
    }

    /* return the ENVM state */
    return envm_state;
}

/*!
    \brief      erase whole chip, including ENVM and flash (API_ID(0x000BU))
    \param[in]  none
    \param[out] none
    \retval     ErrStatus: ERROR or SUCCESS
*/
ErrStatus nvmc_typical_chip_erase(void)
{
    ErrStatus status;
    envm_state_enum envm_state = ENVM_READY;
    flash_state_enum flash_state = FLASH_READY;

    /* wait for the ENVM ready */
    envm_state = nvmc_envm_ready_wait(ENVM_TIMEOUT_COUNT);

    /* wait for the ENVM ready */
    flash_state = nvmc_flash_ready_wait(FLASH_TIMEOUT_COUNT);

    if((ENVM_READY == envm_state) && (FLASH_READY == flash_state)) {
        /* enable chip erase operation */
        NVMC_ENVM_CTL |= NVMC_ENVM_CTL_CER;
        /* start whole chip erase */
        NVMC_ENVM_CTL |= NVMC_ENVM_CTL_START;
        /* wait for the ENVM ready */
        envm_state = nvmc_envm_ready_wait(ENVM_TIMEOUT_COUNT);
        flash_state = nvmc_flash_ready_wait(FLASH_TIMEOUT_COUNT);
        /* reset the CER bit */
        NVMC_ENVM_CTL &= ~NVMC_ENVM_CTL_CER;

        if((FLASH_READY == flash_state) && (ENVM_READY == envm_state)) {
            status = SUCCESS;
        } else {
            status = ERROR;
        }
    } else {
        status = ERROR;
    }

    /* return the NVMC state */
    return status;
}

/*!
    \brief      erase whole chip, including ENVM and flash, the secure area and dcrp area will also be erased (API_ID(0x000CU))
    \param[in]  none
    \param[out] none
    \retval     ErrStatus: ERROR or SUCCESS
*/
ErrStatus nvmc_protection_removed_chip_erase(void)
{
    ErrStatus status;
    envm_state_enum envm_state = ENVM_READY;
    flash_state_enum flash_state = FLASH_READY;

    /* wait for the ENVM ready */
    envm_state = nvmc_envm_ready_wait(ENVM_TIMEOUT_COUNT);
    /* wait for the Flash ready */
    flash_state = nvmc_flash_ready_wait(FLASH_TIMEOUT_COUNT);

    if((ENVM_READY == envm_state) && (FLASH_READY == flash_state)) {
        NVMC_FLASH_CTL |= NVMC_FLASH_CTL_CHIP_REMOVE_EN;
        /* remove DCRP area */
        NVMC_DCRPADDR_MDF = NVMC_DCRPADDR_MDF_DCRP_EREN_29_31 | INVALID_DCRP_START_ADDR;
        /* remove secure-access area */
        NVMC_SCRADDR_MDF = NVMC_SCRADDR_MDF_SCR_EREN_29_31 | INVALID_SCR_START_ADDR;
        /* disable all sectors' erase/program protection */
        NVMC_WP_ENVM_MDF = OB_ENVM_WP_ALL;
        NVMC_WP0_FLASH_MDF = (uint32_t)(OB_FL_WP_ALL & LOW_32BITS_MASK);

#if defined(NVM_VER_CASE1)
        NVMC_WP1_FLASH_MDF = (uint32_t)(OB_FL_WP_ALL >> WP1_FLASH_OFFSET);
#endif /* NVM_VER_CASE1 */

        /* enable chip erase operation */
        NVMC_ENVM_CTL |= NVMC_ENVM_CTL_CER;
        /* start whole chip erase */
        NVMC_ENVM_CTL |= NVMC_ENVM_CTL_START;
        /* wait for the ENVM ready */
        envm_state = nvmc_envm_ready_wait(ENVM_TIMEOUT_COUNT);
        /* wait for the Flash ready */
        flash_state = nvmc_flash_ready_wait(FLASH_TIMEOUT_COUNT);
        /* reset the CER bit */
        NVMC_ENVM_CTL &= ~NVMC_ENVM_CTL_CER;

        if((FLASH_READY == flash_state) && (ENVM_READY == envm_state)) {
            status = SUCCESS;
        } else {
            status = ERROR;
        }
    } else {
        status = ERROR;
    }

    return status;
}
#else

/*!
    \brief      erase whole chip (API_ID(0x000BU))
    \param[in]  none
    \param[out] none
    \retval     ErrStatus: ERROR or SUCCESS
*/
ErrStatus nvmc_typical_chip_erase(void)
{
    ErrStatus status;
    envm_state_enum envm_state = ENVM_READY;

    /* wait for the ENVM ready */
    envm_state = nvmc_envm_ready_wait(ENVM_TIMEOUT_COUNT);

    if(ENVM_READY == envm_state) {
        /* enable chip erase operation */
        NVMC_ENVM_CTL |= NVMC_ENVM_CTL_CER;
        /* start whole chip erase */
        NVMC_ENVM_CTL |= NVMC_ENVM_CTL_START;
        /* wait for the ENVM ready */
        envm_state = nvmc_envm_ready_wait(ENVM_TIMEOUT_COUNT);
        /* reset the CER bit */
        NVMC_ENVM_CTL &= ~NVMC_ENVM_CTL_CER;

        if(ENVM_READY == envm_state) {
            status = SUCCESS;
        } else {
            status = ERROR;
        }
    } else {
        status = ERROR;
    }

    return status;
}

/*!
    \brief      erase whole chip, including ENVM and flash, the secure area and dcrp area will also be erased (API_ID(0x000CU))
    \param[in]  none
    \param[out] none
    \retval     ErrStatus: ERROR or SUCCESS
*/
ErrStatus nvmc_protection_removed_chip_erase(void)
{
    ErrStatus status;
    envm_state_enum envm_state = ENVM_READY;

    /* wait for the ENVM ready */
    envm_state = nvmc_envm_ready_wait(ENVM_TIMEOUT_COUNT);

    if(ENVM_READY == envm_state) {
        NVMC_FLASH_CTL |= NVMC_FLASH_CTL_CHIP_REMOVE_EN;
        /* remove DCRP area */
        NVMC_DCRPADDR_MDF = NVMC_DCRPADDR_MDF_ENVM_DCRP_EREN | INVALID_DCRP_START_ADDR;
        /* remove secure-access area */
        NVMC_SCRADDR_MDF = NVMC_SCRADDR_MDF_ENVM_SCR_EREN | INVALID_SCR_START_ADDR;
        /* disable all sectors' erase/program protection */
        NVMC_WP_ENVM_MDF = OB_ENVM_WP_ALL;

        /* enable chip erase operation */
        NVMC_ENVM_CTL |= NVMC_ENVM_CTL_CER;
        /* start whole chip erase */
        NVMC_ENVM_CTL |= NVMC_ENVM_CTL_START;
        /* wait for the ENVM ready */
        envm_state = nvmc_envm_ready_wait(ENVM_TIMEOUT_COUNT);
        /* reset the CER bit */
        NVMC_ENVM_CTL &= ~NVMC_ENVM_CTL_CER;

        if(ENVM_READY == envm_state) {
            status = SUCCESS;
        } else {
            status = ERROR;
        }
    } else {
        status = ERROR;
    }

    return status;
}
#endif /* NVM_VER_CASE4 */

/*!
    \brief      CRC calculate ENVM data block (API_ID(0x000DU))
    \param[in]  none
    \param[out] crc_value: CRC calculate result
    \retval     state of ENVM
      \arg        ENVM_CRCREADY: ENVM CRC operation has been completed
      \arg        ENVM_CRCBUSY: ENVM CRC operation is in progress
      \arg        ENVM_CRCRERR: ENVM CRC read error
      \arg        ENVM_WPERR: ENVM erase/program protection error
      \arg        ENVM_PGSERR: ENVM program sequence error
      \arg        ENVM_RPERR: ENVM read protection error
      \arg        ENVM_RSERR: ENVM read secure error
      \arg        ENVM_TOERR: ENVM timeout error
*/
envm_state_enum nvmc_envm_crc_calculate(uint32_t *crc_value)
{
    envm_state_enum envm_state = ENVM_CRCREADY;

#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_VALID_POINTER(crc_value)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x000DU), ERR_PARAM_POINTER);
    } else
#endif
    {
        /* wait for the ENVM CRC ready */
        envm_state = nvmc_envm_crc_ready_wait(ENVM_TIMEOUT_COUNT);

        if(ENVM_CRCREADY == envm_state) {
            /* start CRC calculate */
            NVMC_ENVM_CTL |= NVMC_ENVM_CTL_CRCSTART;
            /* wait for the ENVM ready */
            envm_state = nvmc_envm_crc_ready_wait(ENVM_TIMEOUT_COUNT);

            if(ENVM_CRCREADY == envm_state) {
                *crc_value = NVMC_ENVM_CRCRESULT;
            }
        }
    }
    /* return the ENVM state */
    return envm_state;
}

/*!
    \brief      software trigger ENVM repair (API_ID(0x000EU))
    \param[in]  none
    \param[out] none
    \retval     state of ENVM
      \arg        ENVM_READY: ENVM operation has been completed
      \arg        ENVM_REPAIR_BUSY: ENVM repair operation is in progress
      \arg        ENVM_REPAIR_ECC1CNTIF: ENVM the number of 1 bit error unrepaired is greater than or equal to the threshold
      \arg        ENVM_REPAIR_ECC2CNTIF: ENVM the number of 2 bits error unrepaired is greater than or equal to the threshold
      \arg        ENVM_REPAIR_FLBLANK: ENVM Flash backup content is blank error
      \arg        ENVM_REPAIR_END: ENVM self-repair has been completed
      \arg        ENVM_REPAIR_REPSTRERR: ENVM self-check repair configuration error
      \arg        ENVM_TOERR: ENVM timeout error
*/
envm_state_enum nvmc_envm_repair(void)
{
    envm_state_enum envm_state = ENVM_READY;

    /* wait for the ENVM repair ready */
    envm_state = nvmc_envm_repair_ready_wait(ENVM_TIMEOUT_COUNT);

    if(ENVM_READY == envm_state) {
        /* start software trigger repair */
        NVMC_ENVM_REPCS |= NVMC_ENVM_REPCS_REPSTART;
        /* wait for the ENVM ready */
        envm_state = nvmc_envm_repair_ready_wait(ENVM_TIMEOUT_COUNT);
    }

    /* return the ENVM state */
    return envm_state;
}

/*!
    \brief      get ENVM read only bad block address that still has the error after self-repair (API_ID(0x000FU))
    \param[in]  index: number of read only bad block (index = 0 - 3)
    \param[out] bad_block_address: bad block address
    \param[out] error_stat: error status after self-check
    \retval     state of bad block address
      \arg        INVLD_ADDRESS_FLAG: the address is invalid
      \arg        VLD_ADDRESS_FLAG: the address is valid
*/
uint8_t nvmc_envm_read_only_bad_block_address_get(uint32_t index, uint32_t *bad_block_address, uint32_t *error_stat)
{
    uint8_t address_stat = INVLD_ADDRESS_FLAG;

    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_BADBLOCK_VALID_ADDRESS_INDEX(index)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x000FU), ERR_PARAM_OUT_OF_RANGE);
    } else if(NOT_VALID_POINTER(bad_block_address)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x000FU), ERR_PARAM_POINTER);
    } else if(NOT_VALID_POINTER(error_stat)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x000FU), ERR_PARAM_POINTER);
    } else
#endif
    {
        *error_stat = ((NVMC_ENVM_ROBBADDRX(index) & NVMC_ENVM_ROBBADDRX_ROBBADDRSTAT) >> ROBBADDRX_ROBBADDRSTAT_OFFSET);
        if(*error_stat) {
            *bad_block_address = NVMC_ENVM_ROBBADDRX(index) & (~NVMC_ENVM_ROBBADDRX_ROBBADDRSTAT);
            /* valid bad block address */
            address_stat = VLD_ADDRESS_FLAG;
        } else {
            *bad_block_address = 0xFFFFFFFFU;
            /* no valid bad block address */
            address_stat = INVLD_ADDRESS_FLAG;
        }
    }
    return address_stat;
}

/*!
    \brief      get ENVM flag set or reset (API_ID(0x0010U))
    \param[in]  envm_flag: ENVM flag
                only one parameter can be selected which is shown as below:
      \arg        ENVM_FLAG_BUSY: ENVM busy flag
      \arg        ENVM_FLAG_ENDF: ENVM end of operation flag
      \arg        ENVM_FLAG_WPERR: ENVM write protection error flag
      \arg        ENVM_FLAG_PGSERR: ENVM program sequence error flag
      \arg        ENVM_FLAG_OTAFAIL: ENVM OTA update, check_bootloader check failure flag
      \arg        ENVM_FLAG_OTASUC: ENVM OTA update, check_bootloader check success flag
      \arg        ENVM_FLAG_COPYSTRERR: ENVM copy startup failure error flag
      \arg        ENVM_FLAG_RPERR: ENVM read protection error flag
      \arg        ENVM_FLAG_RSERR: ENVM read secure error flag
      \arg        ENVM_FLAG_AUTOECCSERRDET: ENVM one bit detect error flag
      \arg        ENVM_FLAG_AUTOECCDERRDET: ENVM two bits detect error flag
      \arg        ENVM_FLAG_CRCCALEND: ENVM CRC end of calculation flag
      \arg        ENVM_FLAG_CRCRERR: ENVM CRC read error flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus nvmc_envm_flag_get(uint32_t envm_flag)
{
    FlagStatus reval = RESET;

    if(NVMC_ENVM_STAT & envm_flag) {
        reval = SET;
    }
    /* return the state of corresponding ENVM flag */
    return reval;
}

/*!
    \brief      clear ENVM flag (API_ID(0x0011U))
    \param[in]  envm_flag: ENVM flag
                one or more parameters can be selected which is shown as below:
      \arg        ENVM_FLAG_ENDF: ENVM end of operation flag
      \arg        ENVM_FLAG_WPERR: ENVM write protection error flag
      \arg        ENVM_FLAG_PGSERR: ENVM program sequence error flag
      \arg        ENVM_FLAG_OTAFAIL: ENVM OTA update, check_bootloader check failure flag
      \arg        ENVM_FLAG_OTASUC: ENVM OTA update, check_bootloader check success flag
      \arg        ENVM_FLAG_COPYSTRERR: ENVM copy startup failure error flag
      \arg        ENVM_FLAG_RPERR: ENVM read protection error flag
      \arg        ENVM_FLAG_RSERR: ENVM read secure error flag
      \arg        ENVM_FLAG_CRCCALEND: ENVM CRC end of calculation flag
      \arg        ENVM_FLAG_CRCRERR: ENVM CRC read error flag
    \param[out] none
    \retval     none
*/
void nvmc_envm_flag_clear(uint32_t envm_flag)
{
    uint32_t reg_value;

    reg_value = NVMC_ENVM_STAT;
    reg_value &= ~(ENVM_FLAG_ENDF | ENVM_FLAG_WPERR | ENVM_FLAG_PGSERR | ENVM_FLAG_OTAFAIL | ENVM_FLAG_OTASUC | ENVM_FLAG_COPYSTRERR | ENVM_FLAG_RPERR |
                   ENVM_FLAG_RSERR | ENVM_FLAG_CRCCALEND | ENVM_FLAG_CRCRERR);
    reg_value |= envm_flag;

    /* clear the flags */
    NVMC_ENVM_STAT = reg_value;
}

/*!
    \brief      enable ENVM interrupt (API_ID(0x0012U))
    \param[in]  envm_int: the ENVM interrupt source
                one or more parameters can be selected which is shown as below:
      \arg        ENVM_INT_ENDIE: ENVM end of operation interrupt enable bit
      \arg        ENVM_INT_WPERRIE: ENVM write protection error interrupt enable bit
      \arg        ENVM_INT_PGSERRIE: ENVM program sequence error interrupt enable bit
      \arg        ENVM_INT_RPERRIE: ENVM read protection error interrupt enable bit
      \arg        ENVM_INT_RSERRIE: ENVM read secure error interrupt enable bit
      \arg        ENVM_INT_AUTOECCSERRDETIE: ENVM one bit detect error interrupt enable bit
      \arg        ENVM_INT_AUTOECCDERRDETIE: ENVM two bits detect error interrupt enable bit
      \arg        ENVM_INT_CRCCALENDIE: ENVM CRC end of calculation interrupt enable bit
      \arg        ENVM_INT_CRCRERRIE: ENVM CRC read error interrupt enable bit
    \param[out] none
    \retval     none
*/
void nvmc_envm_interrupt_enable(uint32_t envm_int)
{
    NVMC_ENVM_CTL |= envm_int;
}

/*!
    \brief      disable ENVM interrupt (API_ID(0x0013U))
    \param[in]  envm_int: the ENVM interrupt source
                one or more parameters can be selected which is shown as below:
      \arg        ENVM_INT_ENDIE: ENVM end of operation interrupt enable bit
      \arg        ENVM_INT_WPERRIE: ENVM write protection error interrupt enable bit
      \arg        ENVM_INT_PGSERRIE: ENVM program sequence error interrupt enable bit
      \arg        ENVM_INT_RPERRIE: ENVM read protection error interrupt enable bit
      \arg        ENVM_INT_RSERRIE: ENVM read secure error interrupt enable bit
      \arg        ENVM_INT_AUTOECCSERRDETIE: ENVM one bit detect error interrupt enable bit
      \arg        ENVM_INT_AUTOECCDERRDETIE: ENVM two bits detect error interrupt enable bit
      \arg        ENVM_INT_CRCCALENDIE: ENVM CRC end of calculation interrupt enable bit
      \arg        ENVM_INT_CRCRERRIE: ENVM CRC read error interrupt enable bit
    \param[out] none
    \retval     none
*/
void nvmc_envm_interrupt_disable(uint32_t envm_int)
{
    NVMC_ENVM_CTL &= ~(uint32_t)envm_int;
}

/*!
    \brief      get ENVM interrupt flag status (API_ID(0x0014U))
    \param[in]  envm_int_flag: ENVM interrupt flag status
                only one parameter can be selected which is shown as below:
      \arg        ENVM_INT_FLAG_ENDF: ENVM end of operation interrupt flag
      \arg        ENVM_INT_FLAG_WPERR: ENVM write protection error interrupt flag
      \arg        ENVM_INT_FLAG_PGSERR: ENVM program sequence error interrupt flag
      \arg        ENVM_INT_FLAG_RPERR: ENVM read protection error interrupt flag
      \arg        ENVM_INT_FLAG_RSERR: ENVM read secure error interrupt flag
      \arg        ENVM_INT_FLAG_AUTOECCSERRDET: ENVM one bit detect error interrupt flag
      \arg        ENVM_INT_FLAG_AUTOECCDERRDET: ENVM two bits detect error interrupt flag
      \arg        ENVM_INT_FLAG_CRCCALEND: ENVM CRC end of calculation interrupt flag
      \arg        ENVM_INT_FLAG_CRCRERR: ENVM CRC read error interrupt flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus nvmc_envm_interrupt_flag_get(uint8_t envm_int_flag)
{
    FlagStatus reval = RESET;

    uint32_t reg1 = NVMC_ENVM_STAT;
    uint32_t reg2 = NVMC_ENVM_CTL;

    switch(envm_int_flag) {
    /* ENVM end of operation interrupt */
    case ENVM_INT_FLAG_ENDF:
        reg1 = reg1 & NVMC_ENVM_STAT_ENDF;
        reg2 = reg2 & NVMC_ENVM_CTL_ENDIE;
        break;
    /* ENVM write protection error interrupt */
    case ENVM_INT_FLAG_WPERR:
        reg1 = reg1 & NVMC_ENVM_STAT_WPERR;
        reg2 = reg2 & NVMC_ENVM_CTL_WPERRIE;
        break;
    /* ENVM program sequence error interrupt */
    case ENVM_INT_FLAG_PGSERR:
        reg1 = reg1 & NVMC_ENVM_STAT_PGSERR;
        reg2 = reg2 & NVMC_ENVM_CTL_PGSERRIE;
        break;
    /* ENVM read protection error interrupt */
    case ENVM_INT_FLAG_RPERR:
        reg1 = reg1 & NVMC_ENVM_STAT_RPERR;
        reg2 = reg2 & NVMC_ENVM_CTL_RPERRIE;
        break;
    /* ENVM read secure error interrupt */
    case ENVM_INT_FLAG_RSERR:
        reg1 = reg1 & NVMC_ENVM_STAT_RSERR;
        reg2 = reg2 & NVMC_ENVM_CTL_RSERRIE;
        break;
    /* ENVM one bit detect error interrupt */
    case ENVM_INT_FLAG_AUTOECCSERRDET:
        reg1 = reg1 & NVMC_ENVM_STAT_AUTOECCSERRDET;
        reg2 = reg2 & NVMC_ENVM_CTL_AUTOECCSERRDETIE;
        break;
    /* ENVM two bits detect error interrupt */
    case ENVM_INT_FLAG_AUTOECCDERRDET:
        reg1 = reg1 & NVMC_ENVM_STAT_AUTOECCDERRDET;
        reg2 = reg2 & NVMC_ENVM_CTL_AUTOECCDERRDETIE;
        break;
    /* ENVM CRC end of calculation interrupt */
    case ENVM_INT_FLAG_CRCCALEND:
        reg1 = reg1 & NVMC_ENVM_STAT_CRCENDCAL;
        reg2 = reg2 & NVMC_ENVM_CTL_CRCENDCALIE;
        break;
    /* ENVM CRC read error interrupt */
    case ENVM_INT_FLAG_CRCRERR:
        reg1 = reg1 & NVMC_ENVM_STAT_CRCRERR;
        reg2 = reg2 & NVMC_ENVM_CTL_CRCRERRIE;
        break;
    default :
        break;
    }
    /*get ENVM interrupt flag status */
    if((0U != reg1) && (0U != reg2)) {
        reval = SET;
    }

    return reval;
}

/*!
    \brief      clear ENVM interrupt flag status (API_ID(0x0015U))
    \param[in]  envm_int_flag: ENVM interrupt flag status
                only one parameter can be selected which is shown as below:
      \arg        ENVM_INT_FLAG_ENDF: ENVM end of operation interrupt flag
      \arg        ENVM_INT_FLAG_WPERR: ENVM write protection error interrupt flag
      \arg        ENVM_INT_FLAG_PGSERR: ENVM program sequence error interrupt flag
      \arg        ENVM_INT_FLAG_RPERR: ENVM read protection error interrupt flag
      \arg        ENVM_INT_FLAG_RSERR: ENVM read secure error interrupt flag
      \arg        ENVM_INT_FLAG_CRCCALEND: ENVM CRC end of calculation interrupt flag
      \arg        ENVM_INT_FLAG_CRCRERR: ENVM CRC read error interrupt flag
    \param[out] none
    \retval     none
*/
void nvmc_envm_interrupt_flag_clear(uint8_t envm_int_flag)
{

    switch(envm_int_flag) {
    /* ENVM end of operation interrupt */
    case ENVM_INT_FLAG_ENDF:
        NVMC_ENVM_STAT |= NVMC_ENVM_STAT_ENDF;
        break;
    /* ENVM write protection error interrupt */
    case ENVM_INT_FLAG_WPERR:
        NVMC_ENVM_STAT |= NVMC_ENVM_STAT_WPERR;
        break;
    /* ENVM program sequence error interrupt */
    case ENVM_INT_FLAG_PGSERR:
        NVMC_ENVM_STAT |= NVMC_ENVM_STAT_PGSERR;
        break;
    /* ENVM read protection error interrupt */
    case ENVM_INT_FLAG_RPERR:
        NVMC_ENVM_STAT |= NVMC_ENVM_STAT_RPERR;
        break;
    /* ENVM read secure error interrupt */
    case ENVM_INT_FLAG_RSERR:
        NVMC_ENVM_STAT |= NVMC_ENVM_STAT_RSERR;
        break;
    /* ENVM CRC end of calculation interrupt */
    case ENVM_INT_FLAG_CRCCALEND:
        NVMC_ENVM_STAT |= NVMC_ENVM_STAT_CRCENDCAL;
        break;
    /* ENVM CRC read error interrupt */
    case ENVM_INT_FLAG_CRCRERR:
        NVMC_ENVM_STAT |= NVMC_ENVM_STAT_CRCRERR;
        break;
    default :
        break;
    }
}

/*!
    \brief      get ENVM repair flag set or reset (API_ID(0x0016U))
    \param[in]  envm_flag: ENVM flag
                only one parameter can be selected which is shown as below:
      \arg        ENVM_FLAG_NMIERRINT: ENVM AXI read, repair failure with 3-bit error flag
      \arg        ENVM_FLAG_REPAIR_ECC1CNTIF: ENVM the number of 1 bit error unrepaired is greater than or equal to the threshold flag
      \arg        ENVM_FLAG_REPAIR_ECC2CNTIF: ENVM the number of 2 bits error unrepaired is greater than or equal to the threshold flag
      \arg        ENVM_FLAG_REPAIR_FLBLANK: ENVM Flash backup content is blank flag
      \arg        ENVM_FLAG_REPAIR_END: ENVM self-repair end flag
      \arg        ENVM_FLAG_REPAIR_REPSTRERR: ENVM self-check repair configuration error flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus nvmc_envm_repair_flag_get(uint32_t envm_repair_flag)
{
    FlagStatus reval = RESET;

    if(NVMC_ENVM_REPCS & envm_repair_flag) {
        reval = SET;
    }
    /* return the state of corresponding ENVM flag */
    return reval;
}

/*!
    \brief      clear ENVM repair flag (API_ID(0x0017U))
    \param[in]  envm_flag: ENVM flag
                one or more parameters can be selected which is shown as below:
      \arg        ENVM_FLAG_NMIERRINT: ENVM AXI read, repair failure with 3-bit error flag
      \arg        ENVM_FLAG_REPAIR_ECC1CNTIF: ENVM the number of 1 bit error unrepaired is greater than or equal to the threshold flag
      \arg        ENVM_FLAG_REPAIR_ECC2CNTIF: ENVM the number of 2 bits error unrepaired is greater than or equal to the threshold flag
      \arg        ENVM_FLAG_REPAIR_FLBLANK: ENVM Flash backup content is blank flag
      \arg        ENVM_FLAG_REPAIR_END: ENVM self-repair end flag
      \arg        ENVM_FLAG_REPAIR_REPSTRERR: ENVM self-check repair configuration error flag
    \param[out] none
    \retval     none
*/
void nvmc_envm_repair_flag_clear(uint32_t envm_repair_flag)
{
    uint32_t reg_value;

    reg_value = NVMC_ENVM_REPCS;
    reg_value &= ~(ENVM_FLAG_NMIERRINT | ENVM_FLAG_REPAIR_ECC1CNTIF | ENVM_FLAG_REPAIR_ECC2CNTIF | ENVM_FLAG_REPAIR_FLBLANK | ENVM_FLAG_REPAIR_END |
                   ENVM_FLAG_REPAIR_REPSTRERR);
    reg_value |= envm_repair_flag;

    /* clear the flags */
    NVMC_ENVM_REPCS = reg_value;
}

/*!
    \brief      enable ENVM repair interrupt (API_ID(0x0018U))
    \param[in]  envm_repair_int: the ENVM repair interrupt source
                one or more parameters can be selected which is shown as below:
      \arg        ENVM_INT_REPAIR_ECC1CNTIE: ENVM the number of 1 bit error unrepaired is greater than or equal to the threshold interrupt enable bit
      \arg        ENVM_INT_REPAIR_ECC2CNTIE: ENVM the number of 2 bits error unrepaired is greater than or equal to the threshold interrupt enable bit
      \arg        ENVM_INT_REPAIR_FLBLANKIE: ENVM Flash backup content is blank interrupt enable bit
      \arg        ENVM_INT_REPAIR_DONEIE: ENVM self-repair done interrupt enable bit
    \param[out] none
    \retval     none
*/
void nvmc_envm_repair_interrupt_enable(uint32_t envm_repair_int)
{
    uint32_t reg;

    reg = NVMC_ENVM_REPCS;
    reg &= ~(NVMC_ENVM_REPCS_ECC2CNTIF | NVMC_ENVM_REPCS_ECC1CNTIF | NVMC_ENVM_REPCS_FLBLANK | NVMC_ENVM_REPCS_NMIERRINT | NVMC_ENVM_REPCS_REPEND |
             NVMC_ENVM_REPCS_REPSTRERR);
    reg |= (uint32_t)envm_repair_int;
    NVMC_ENVM_REPCS = reg;
}

/*!
    \brief      disable ENVM repair interrupt (API_ID(0x0019U))
    \param[in]  envm_repair_int: the ENVM repair interrupt source
                one or more parameters can be selected which is shown as below:
      \arg        ENVM_INT_REPAIR_ECC1CNTIE: ENVM the number of 1 bit error unrepaired is greater than or equal to the threshold interrupt enable bit
      \arg        ENVM_INT_REPAIR_ECC2CNTIE: ENVM the number of 2 bits error unrepaired is greater than or equal to the threshold interrupt enable bit
      \arg        ENVM_INT_REPAIR_FLBLANKIE: ENVM Flash backup content is blank interrupt enable bit
      \arg        ENVM_INT_REPAIR_DONEIE: ENVM self-repair done interrupt enable bit
    \param[out] none
    \retval     none
*/
void nvmc_envm_repair_interrupt_disable(uint32_t envm_repair_int)
{
    uint32_t reg;

    reg = NVMC_ENVM_REPCS;
    reg &= ~(NVMC_ENVM_REPCS_ECC2CNTIF | NVMC_ENVM_REPCS_ECC1CNTIF | NVMC_ENVM_REPCS_FLBLANK | NVMC_ENVM_REPCS_NMIERRINT | NVMC_ENVM_REPCS_REPEND |
             NVMC_ENVM_REPCS_REPSTRERR);
    reg &= ~(uint32_t)envm_repair_int;
    NVMC_ENVM_REPCS = reg;
}

/*!
    \brief      get ENVM repair interrupt flag status (API_ID(0x001AU))
    \param[in]  envm_repair_int_flag: ENVM repair interrupt flag status
                only one parameter can be selected which is shown as below:
      \arg        ENVM_INT_FLAG_REPAIR_ECC1CNTIF: ENVM the number of 1 bit error unrepaired is greater than or equal to the threshold interrupt flag
      \arg        ENVM_INT_FLAG_REPAIR_ECC2CNTIF: ENVM the number of 2 bit error unrepaired is greater than or equal to the threshold interrupt flag
      \arg        ENVM_INT_FLAG_REPAIR_FLBLANK: ENVM Flash backup content is blank interrupt flag
      \arg        ENVM_INT_FLAG_REPAIR_END: ENVM self-repair end interrupt flag
      \arg        ENVM_INT_FLAG_REPAIR_REPSTRERR: ENVM self-check repair configuration error interrupt flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus nvmc_envm_repair_interrupt_flag_get(uint8_t envm_repair_int_flag)
{
    FlagStatus reval = RESET;

    uint32_t reg1 = NVMC_ENVM_REPCS;
    uint32_t reg2 = NVMC_ENVM_REPCS;

    switch(envm_repair_int_flag) {
    /* ENVM the number of 1 bit error unrepaired is greater than or equal to the threshold interrupt */
    case ENVM_INT_FLAG_REPAIR_ECC1CNTIF:
        reg1 = reg1 & NVMC_ENVM_REPCS_ECC1CNTIF;
        reg2 = reg2 & NVMC_ENVM_REPCS_ECC1CNTIE;
        break;
    /* ENVM the number of 2 bit error unrepaired is greater than or equal to the threshold interrupt */
    case ENVM_INT_FLAG_REPAIR_ECC2CNTIF:
        reg1 = reg1 & NVMC_ENVM_REPCS_ECC2CNTIF;
        reg2 = reg2 & NVMC_ENVM_REPCS_ECC2CNTIE;
        break;
    /* ENVM Flash backup content is blank interrupt */
    case ENVM_INT_FLAG_REPAIR_FLBLANK:
        reg1 = reg1 & NVMC_ENVM_REPCS_FLBLANK;
        reg2 = reg2 & NVMC_ENVM_REPCS_FLBLANKIE;
        break;
    /* ENVM self-repair end interrupt */
    case ENVM_INT_FLAG_REPAIR_END:
        reg1 = reg1 & NVMC_ENVM_REPCS_REPEND;
        reg2 = reg2 & NVMC_ENVM_REPCS_REPDONEIE;
        break;
    /* ENVM self-check repair configuration error interrupt */
    case ENVM_INT_FLAG_REPAIR_REPSTRERR:
        reg1 = reg1 & NVMC_ENVM_REPCS_REPSTRERR;
        reg2 = reg2 & NVMC_ENVM_REPCS_REPDONEIE;
        break;
    default :
        break;
    }
    /*get ENVM repair interrupt flag status */
    if((0U != reg1) && (0U != reg2)) {
        reval = SET;
    }

    return reval;
}

/*!
    \brief      clear ENVM repair interrupt flag status (API_ID(0x001BU))
    \param[in]  envm_repair_int_flag: ENVM repair interrupt flag status
                only one parameter can be selected which is shown as below:
      \arg        ENVM_INT_FLAG_REPAIR_ECC1CNTIF: ENVM the number of 1 bit error unrepaired is greater than or equal to the threshold interrupt flag
      \arg        ENVM_INT_FLAG_REPAIR_ECC2CNTIF: ENVM the number of 2 bit error unrepaired is greater than or equal to the threshold interrupt flag
      \arg        ENVM_INT_FLAG_REPAIR_FLBLANK: ENVM Flash backup content is blank interrupt flag
      \arg        ENVM_INT_FLAG_REPAIR_END: ENVM self-repair end interrupt flag
      \arg        ENVM_INT_FLAG_REPAIR_REPSTRERR: ENVM self-check repair configuration error interrupt flag
    \param[out] none
    \retval     none
*/
void nvmc_envm_repair_interrupt_flag_clear(uint8_t envm_repair_int_flag)
{

    switch(envm_repair_int_flag) {
    /* ENVM the number of 1 bit error unrepaired is greater than or equal to the threshold interrupt */
    case ENVM_INT_FLAG_REPAIR_ECC1CNTIF:
        NVMC_ENVM_REPCS |= NVMC_ENVM_REPCS_ECC1CNTIF;
        break;
    /* ENVM the number of 2 bit error unrepaired is greater than or equal to the threshold interrupt */
    case ENVM_INT_FLAG_REPAIR_ECC2CNTIF:
        NVMC_ENVM_REPCS |= NVMC_ENVM_REPCS_ECC2CNTIF;
        break;
    /* ENVM Flash backup content is blank interrupt */
    case ENVM_INT_FLAG_REPAIR_FLBLANK:
        NVMC_ENVM_REPCS |= NVMC_ENVM_REPCS_FLBLANK;
        break;
    /* ENVM self-repair end interrupt */
    case ENVM_INT_FLAG_REPAIR_END:
        NVMC_ENVM_REPCS |= NVMC_ENVM_REPCS_REPEND;
        break;
    /* ENVM self-check repair configuration error interrupt */
    case ENVM_INT_FLAG_REPAIR_REPSTRERR:
        NVMC_ENVM_REPCS |= NVMC_ENVM_REPCS_REPSTRERR;
        break;
    default :
        break;
    }

}

/*!
    \brief      get the number of 2 bit error occurrences without repair during the self-check (API_ID(0x001CU))
    \param[in]  none
    \param[out] none
    \retval     the number of 2 bit error
*/
uint8_t nvmc_envm_ecc2_cnt_without_repair_get(void)
{
    return (uint8_t)((NVMC_ENVM_REPCS & NVMC_ENVM_REPCS_ERR2CNT) >> ENVM_REPCS_ERR2CNT_OFFSET);
}

/*!
    \brief      get the number of 1 bit error occurrences without repair during the self-check (API_ID(0x001DU))
    \param[in]  none
    \param[out] none
    \retval     the number of 1 bit error
*/
uint8_t nvmc_envm_ecc1_cnt_without_repair_get(void)
{
    return (uint8_t)((NVMC_ENVM_REPCS & NVMC_ENVM_REPCS_ERR1CNT) >> ENVM_REPCS_ERR1CNT_OFFSET);
}

/*!
    \brief      get the number of error count fail to repair during self-check (API_ID(0x001EU))
    \param[in]  none
    \param[out] none
    \retval     the number of error count fail to repair
*/
uint8_t nvmc_envm_robbcnt_get(void)
{
    return (uint8_t)((NVMC_ENVM_REPCS & NVMC_ENVM_REPCS_ROBBCNT) >> ENVM_REPCS_ROBBCNT_OFFSET);
}

/*!
    \brief      get the error status during self-check (API_ID(0x001FU))
    \param[in]  none
    \param[out] none
    \retval     the error status
*/
uint8_t nvmc_envm_repair_error_status_get(void)
{
    return (uint8_t)((NVMC_ENVM_REPCS & NVMC_ENVM_REPCS_REPERRSTAT) >> ENVM_REPCS_REPERRSTAT_OFFSET);
}

/*!
    \brief      clear the number of 2 bit error occurrences without repair during self-check (API_ID(0x0020U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void nvmc_envm_ecc2_cnt_without_repair_clear(void)
{
    uint32_t reg_value;

    reg_value = NVMC_ENVM_REPCS;
    /* keep repair flag unchanged */
    reg_value &= ~(ENVM_FLAG_NMIERRINT | ENVM_FLAG_REPAIR_ECC1CNTIF | ENVM_FLAG_REPAIR_ECC2CNTIF | ENVM_FLAG_REPAIR_FLBLANK | ENVM_FLAG_REPAIR_END |
                   ENVM_FLAG_REPAIR_REPSTRERR);
    reg_value &= ~NVMC_ENVM_REPCS_ERR2CNT;

    /* clear the err2cnt */
    NVMC_ENVM_REPCS = reg_value;
}

/*!
    \brief      clear the number of 1 bit error occurrences without repair during self-check (API_ID(0x0021U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void nvmc_envm_ecc1_cnt_without_repair_clear(void)
{
    uint32_t reg_value;

    reg_value = NVMC_ENVM_REPCS;
    /* keep repair flag unchanged */
    reg_value &= ~(ENVM_FLAG_NMIERRINT | ENVM_FLAG_REPAIR_ECC1CNTIF | ENVM_FLAG_REPAIR_ECC2CNTIF | ENVM_FLAG_REPAIR_FLBLANK | ENVM_FLAG_REPAIR_END |
                   ENVM_FLAG_REPAIR_REPSTRERR);
    reg_value &= ~NVMC_ENVM_REPCS_ERR1CNT;

    /* clear the err1cnt */
    NVMC_ENVM_REPCS = reg_value;
}

/*!
    \brief      clear the number of error count fail to repair during self-check (API_ID(0x0022U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void nvmc_envm_robbcnt_clear(void)
{
    uint32_t reg_value;

    reg_value = NVMC_ENVM_REPCS;
    /* keep repair flag unchanged */
    reg_value &= ~(ENVM_FLAG_NMIERRINT | ENVM_FLAG_REPAIR_ECC1CNTIF | ENVM_FLAG_REPAIR_ECC2CNTIF | ENVM_FLAG_REPAIR_FLBLANK | ENVM_FLAG_REPAIR_END |
                   ENVM_FLAG_REPAIR_REPSTRERR);
    reg_value &= ~NVMC_ENVM_REPCS_ROBBCNT;

    /* clear the robbcnt */
    NVMC_ENVM_REPCS = reg_value;
}

/*!
    \brief      clear error status during self-check (API_ID(0x0023U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void nvmc_envm_repair_error_status_clear(void)
{
    uint32_t reg_value;

    reg_value = NVMC_ENVM_REPCS;
    /* keep repair flag unchanged */
    reg_value &= ~(ENVM_FLAG_NMIERRINT | ENVM_FLAG_REPAIR_ECC1CNTIF | ENVM_FLAG_REPAIR_ECC2CNTIF | ENVM_FLAG_REPAIR_FLBLANK | ENVM_FLAG_REPAIR_END |
                   ENVM_FLAG_REPAIR_REPSTRERR);
    reg_value &= ~NVMC_ENVM_REPCS_REPERRSTAT;

    /* clear the repair status */
    NVMC_ENVM_REPCS = reg_value;
}

/*!
    \brief      clear the result of repair during self-check, including counter, error address, and status (API_ID(0x0024U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void nvmc_envm_self_check_repair_flag_clear(void)
{
    uint32_t reg_value;

    reg_value = NVMC_ENVM_CTL;
    reg_value |= NVMC_ENVM_CTL_REPRESC;

    /* clear the flags */
    NVMC_ENVM_CTL = reg_value;
}

/*!
    \brief      clear the result of repair during ENVM axi read, including counter, error address, and status (API_ID(0x0025U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void nvmc_envm_axi_read_repair_flag_clear(void)
{
    uint32_t reg_value;

    reg_value = NVMC_ENVM_CTL;
    reg_value |= NVMC_ENVM_CTL_AUTOREPRESC;

    /* clear the flags */
    NVMC_ENVM_CTL = reg_value;
}

/*!
    \brief      get ENVM state (exclude ENVM crc operation) (API_ID(0x0026U))
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
    \brief      check whether ENVM is ready or not (exclude ENVM crc operation) (API_ID(0x0027U))
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
static envm_state_enum nvmc_envm_ready_wait(uint32_t timeout)
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
    \brief      get ENVM crc state (API_ID(0x0028U))
    \param[in]  none
    \param[out] none
    \retval     state of ENVM
      \arg        ENVM_CRCREADY: ENVM CRC operation has been completed
      \arg        ENVM_CRCBUSY: ENVM CRC operation is in progress
      \arg        ENVM_CRCRERR: ENVM CRC read error
      \arg        ENVM_WPERR: ENVM erase/program protection error
      \arg        ENVM_PGSERR: ENVM program sequence error
      \arg        ENVM_RPERR: ENVM read protection error
      \arg        ENVM_RSERR: ENVM read secure error
*/
static envm_state_enum nvmc_envm_crc_state_get(void)
{
    envm_state_enum envm_state = ENVM_CRCREADY;

    if(ENVM_FLAG_CRCBUSY == (NVMC_ENVM_STAT & ENVM_FLAG_CRCBUSY)) {
        envm_state = ENVM_CRCBUSY;
    } else if(RESET != (NVMC_ENVM_STAT & ENVM_FLAG_WPERR)) {
        envm_state = ENVM_WPERR;
    } else if(RESET != (NVMC_ENVM_STAT & ENVM_FLAG_RSERR)) {
        envm_state = ENVM_RSERR;
    } else if(RESET != (NVMC_ENVM_STAT & ENVM_FLAG_RPERR)) {
        envm_state = ENVM_RPERR;
    } else if(RESET != (NVMC_ENVM_STAT & ENVM_FLAG_PGSERR)) {
        envm_state = ENVM_PGSERR;
    } else if(RESET != (NVMC_ENVM_STAT & ENVM_FLAG_CRCRERR)) {
        envm_state = ENVM_CRCRERR;
    } else {
        envm_state = ENVM_CRCREADY;
    }

    /* return the ENVM state */
    return envm_state;
}

/*!
    \brief      check whether ENVM crc is ready or not (API_ID(0x0029U))
    \param[in]  timeout: timeout count (0x00000000 - 0xFFFFFFFF)
    \param[out] none
    \retval     state of ENVM
      \arg        ENVM_CRCREADY: ENVM CRC operation has been completed
      \arg        ENVM_CRCBUSY: ENVM CRC operation is in progress
      \arg        ENVM_CRCRERR: ENVM CRC read error
      \arg        ENVM_WPERR: ENVM erase/program protection error
      \arg        ENVM_PGSERR: ENVM program sequence error
      \arg        ENVM_RPERR: ENVM read protection error
      \arg        ENVM_RSERR: ENVM read secure error
      \arg        ENVM_TOERR: ENVM timeout error
    \note       This function contains scenarios leading to an infinite loop.
                Modify according to the user's actual usage scenarios.
*/
static envm_state_enum nvmc_envm_crc_ready_wait(uint32_t timeout)
{
    envm_state_enum envm_state = ENVM_CRCBUSY;

    /* wait for ENVM ready */
    do {
        /* get ENVM state */
        envm_state = nvmc_envm_crc_state_get();
        timeout--;
    } while((ENVM_CRCBUSY == envm_state) && (0U != timeout));

    if(ENVM_CRCBUSY == envm_state) {
        envm_state = ENVM_TOERR;
    }
    /* return the ENVM state */
    return envm_state;
}

/*!
    \brief      get ENVM repair state (API_ID(0x002AU))
    \param[in]  none
    \param[out] none
    \retval     state of ENVM
      \arg        ENVM_READY: ENVM operation has been completed
      \arg        ENVM_REPAIR_BUSY: ENVM repair operation is in progress
      \arg        ENVM_REPAIR_ECC1CNTIF: ENVM the number of 1 bit error unrepaired is greater than or equal to the threshold
      \arg        ENVM_REPAIR_ECC2CNTIF: ENVM the number of 2 bits error unrepaired is greater than or equal to the threshold
      \arg        ENVM_REPAIR_FLBLANK: ENVM Flash backup content is blank error
      \arg        ENVM_REPAIR_END: ENVM self-repair has been completed
      \arg        ENVM_REPAIR_REPSTRERR: ENVM self-check repair configuration error
*/
static envm_state_enum nvmc_envm_repair_state_get(void)
{
    envm_state_enum envm_state = ENVM_READY;

    if(RESET != (NVMC_ENVM_REPCS & NVMC_ENVM_REPCS_REPSTART)) {
        envm_state = ENVM_REPAIR_BUSY;
    } else if(RESET != (NVMC_ENVM_REPCS & ENVM_FLAG_REPAIR_ECC1CNTIF)) {
        envm_state = ENVM_REPAIR_ECC1CNTIF;
    } else if(RESET != (NVMC_ENVM_REPCS & ENVM_FLAG_REPAIR_ECC2CNTIF)) {
        envm_state = ENVM_REPAIR_ECC2CNTIF;
    } else if(RESET != (NVMC_ENVM_REPCS & ENVM_FLAG_REPAIR_FLBLANK)) {
        envm_state = ENVM_REPAIR_FLBLANK;
    } else if(RESET != (NVMC_ENVM_REPCS & ENVM_FLAG_REPAIR_REPSTRERR)) {
        envm_state = ENVM_REPAIR_REPSTRERR;
    } else {
        envm_state = ENVM_READY;
    }

    /* return the ENVM state */
    return envm_state;
}

/*!
    \brief      check whether ENVM repair is ready or not (API_ID(0x002BU))
    \param[in]  timeout: timeout count (0x00000000 - 0xFFFFFFFF)
    \param[out] none
    \retval     state of ENVM
      \arg        ENVM_READY: ENVM operation has been completed
      \arg        ENVM_REPAIR_BUSY: ENVM repair operation is in progress
      \arg        ENVM_REPAIR_ECC1CNTIF: ENVM the number of 1 bit error unrepaired is greater than or equal to the threshold
      \arg        ENVM_REPAIR_ECC2CNTIF: ENVM the number of 2 bits error unrepaired is greater than or equal to the threshold
      \arg        ENVM_REPAIR_FLBLANK: ENVM Flash backup content is blank error
      \arg        ENVM_REPAIR_END: ENVM self-repair has been completed
      \arg        ENVM_REPAIR_REPSTRERR: ENVM self-check repair configuration error
      \arg        ENVM_TOERR: ENVM timeout error
    \note       This function contains scenarios leading to an infinite loop.
                Modify according to the user's actual usage scenarios.
*/
static envm_state_enum nvmc_envm_repair_ready_wait(uint32_t timeout)
{
    envm_state_enum envm_state = ENVM_REPAIR_BUSY;

    /* wait for ENVM ready */
    do {
        /* get ENVM state */
        envm_state = nvmc_envm_repair_state_get();
        timeout--;
    } while((ENVM_REPAIR_BUSY == envm_state) && (0U != timeout));

    if(ENVM_REPAIR_BUSY == envm_state) {
        envm_state = ENVM_TOERR;
    }
    /* return the ENVM state */
    return envm_state;
}

/*!
    \brief      unlock register NVMC_FLASH_CTL (API_ID(0x002CU))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void nvmc_flash_unlock(void)
{
    if((RESET != (NVMC_FLASH_CTL & NVMC_FLASH_CTL_LK))) {
        /* write the flash key */
        NVMC_FLASH_KEY = UNLOCK_KEY0;
        NVMC_FLASH_KEY = UNLOCK_KEY1;
    }
}

/*!
    \brief      lock register NVMC_FLASH_CTL (API_ID(0x002DU))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void nvmc_flash_lock(void)
{
    /* set the LK bit*/
    NVMC_FLASH_CTL |= NVMC_FLASH_CTL_LK;
}

/*!
    \brief      program a double-word at the corresponding address (API_ID(0x002EU))
    \param[in]  address: address to program (case1: 0x08200000U - 0x0897FFFF, case2 and case3:0x08200000U - 0x0857FFFF)
    \param[in]  data: double word to program
    \param[out] none
    \retval     state of flash
      \arg        FLASH_READY: flash operation has been completed
      \arg        FLASH_BUSY: flash operation is in progress
      \arg        FLASH_WPERR: flash erase/program protection error
      \arg        FLASH_PGSERR: flash program sequence error
      \arg        FLASH_RPERR: flash read protection error
      \arg        FLASH_RSERR: flash read secure error
      \arg        FLASH_TOERR: flash timeout error
*/
flash_state_enum nvmc_flash_doubleword_program(uint32_t address, uint64_t data)
{
    flash_state_enum flash_state = FLASH_READY;

    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_FLASH_VALID_ADDRESS(address)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x002EU), ERR_PARAM_OUT_OF_RANGE);
    } else
#endif
    {
        /* wait for the flash ready */
        flash_state = nvmc_flash_ready_wait(FLASH_TIMEOUT_COUNT);

        if(FLASH_READY == flash_state) {
            /* set flash program size */
            NVMC_PERFCTL &= ~NVMC_PERFCTL_FLASH_PG_SZ;
            NVMC_PERFCTL |= FLASH_PROG_SIZE_8BYTES;
            /* set the PG bit to start program */
            NVMC_FLASH_CTL |= NVMC_FLASH_CTL_PG;

            __ISB();
            __DSB();

            REG64(address) = data;

            __ISB();
            __DSB();

            /* wait for the flash ready */
            flash_state = nvmc_flash_ready_wait(FLASH_TIMEOUT_COUNT);
            /* reset the PG bit */
            NVMC_FLASH_CTL &= ~NVMC_FLASH_CTL_PG;
        }
    }
    /* return the flash state */
    return flash_state;
}

/*!
    \brief      program 32 bytes at the corresponding flash address (API_ID(0x002FU))
    \param[in]  address: address to program (case1: 0x08200000U - 0x0897FFFF, case2 and case3:0x08200000U - 0x0857FFFF)
    \param[in]  data: data buffer to program
    \param[out] none
    \retval     state of flash
      \arg        FLASH_READY: flash operation has been completed
      \arg        FLASH_BUSY: flash operation is in progress
      \arg        FLASH_WPERR: flash erase/program protection error
      \arg        FLASH_PGSERR: flash program sequence error
      \arg        FLASH_RPERR: flash read protection error
      \arg        FLASH_RSERR: flash read secure error
      \arg        FLASH_TOERR: flash timeout error
*/
flash_state_enum nvmc_flash_32bytes_program(uint32_t address, uint64_t data[])
{
    uint32_t index;
    flash_state_enum flash_state = FLASH_READY;

    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_FLASH_VALID_ADDRESS(address)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x002FU), ERR_PARAM_OUT_OF_RANGE);
    } else if(NOT_VALID_POINTER(data)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x002FU), ERR_PARAM_POINTER);
    } else
#endif
    {
        /* wait for the flash ready */
        flash_state = nvmc_flash_ready_wait(FLASH_TIMEOUT_COUNT);

        if(FLASH_READY == flash_state) {
            /* set flash program size */
            NVMC_PERFCTL &= ~NVMC_PERFCTL_FLASH_PG_SZ;
            NVMC_PERFCTL |= FLASH_PROG_SIZE_32BYTES;
            /* set the PG bit to start program */
            NVMC_FLASH_CTL |= NVMC_FLASH_CTL_PG;

            __ISB();
            __DSB();

            for(index = 0U; index < 4U; index++) {
                REG64(address) = data[index];
                __DSB();
                address += 8U;
            }

            __ISB();
            __DSB();

            /* wait for the flash ready */
            flash_state = nvmc_flash_ready_wait(FLASH_TIMEOUT_COUNT);
            /* reset the PG bit */
            NVMC_FLASH_CTL &= ~NVMC_FLASH_CTL_PG;
        }
    }
    /* return the flash state */
    return flash_state;
}

/*!
    \brief      erase page of flash (API_ID(0x0030U))
    \param[in]  address: page address to erase (case1: 0x08200000U - 0x0897FFFF, case2 and case3:0x08200000U - 0x0857FFFF)
    \param[out] none
    \retval     state of flash
      \arg        FLASH_READY: flash operation has been completed
      \arg        FLASH_BUSY: flash operation is in progress
      \arg        FLASH_WPERR: flash erase/program protection error
      \arg        FLASH_PGSERR: flash program sequence error
      \arg        FLASH_RPERR: flash read protection error
      \arg        FLASH_RSERR: flash read secure error
      \arg        FLASH_TOERR: flash timeout error
*/
flash_state_enum nvmc_flash_page_erase(uint32_t address)
{
    flash_state_enum flash_state = FLASH_READY;

    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_FLASH_VALID_ADDRESS(address)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x0030U), ERR_PARAM_OUT_OF_RANGE);
    } else
#endif
    {
        /* wait for the flash ready */
        flash_state = nvmc_flash_ready_wait(FLASH_TIMEOUT_COUNT);

        if(FLASH_READY == flash_state) {
            NVMC_FLASH_CTL |= NVMC_FLASH_CTL_PER;
            /* write the page address */
            NVMC_FLASH_ADDR = address;
            /* start sector erase */
            NVMC_FLASH_CTL |= NVMC_FLASH_CTL_START;
            /* wait for the flash ready */
            flash_state = nvmc_flash_ready_wait(FLASH_TIMEOUT_COUNT);
            /* reset the SER bit */
            NVMC_FLASH_CTL &= (~NVMC_FLASH_CTL_PER);
        }
    }
    /* return the flash state */
    return flash_state;
}

/*!
    \brief      erase sector of flash (API_ID(0x0031U))
    \param[in]  address: sector address to erase (case1: 0x08200000U - 0x0897FFFF, case2 and case3:0x08200000U - 0x0857FFFF)
    \param[out] none
    \retval     state of flash
      \arg        FLASH_READY: flash operation has been completed
      \arg        FLASH_BUSY: flash operation is in progress
      \arg        FLASH_WPERR: flash erase/program protection error
      \arg        FLASH_PGSERR: flash program sequence error
      \arg        FLASH_RPERR: flash read protection error
      \arg        FLASH_RSERR: flash read secure error
      \arg        FLASH_TOERR: flash timeout error
*/
flash_state_enum nvmc_flash_sector_erase(uint32_t address)
{
    flash_state_enum flash_state = FLASH_READY;

    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_FLASH_VALID_ADDRESS(address)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x0031U), ERR_PARAM_OUT_OF_RANGE);
    } else
#endif
    {
        /* wait for the flash ready */
        flash_state = nvmc_flash_ready_wait(FLASH_TIMEOUT_COUNT);

        if(FLASH_READY == flash_state) {
            NVMC_FLASH_CTL |= NVMC_FLASH_CTL_SER;
            /* write the sector address */
            NVMC_FLASH_ADDR = address;
            /* start sector erase */
            NVMC_FLASH_CTL |= NVMC_FLASH_CTL_START;
            /* wait for the flash ready */
            flash_state = nvmc_flash_ready_wait(FLASH_TIMEOUT_COUNT);
            /* reset the SER bit */
            NVMC_FLASH_CTL &= (~NVMC_FLASH_CTL_SER);
        }
    }
    /* return the flash state */
    return flash_state;
}

/*!
    \brief      erase whole flash (API_ID(0x0032U))
    \param[in]  none
    \param[out] none
    \retval     state of flash
      \arg        FLASH_READY: flash operation has been completed
      \arg        FLASH_BUSY: flash operation is in progress
      \arg        FLASH_WPERR: flash erase/program protection error
      \arg        FLASH_PGSERR: flash program sequence error
      \arg        FLASH_RPERR: flash read protection error
      \arg        FLASH_RSERR: flash read secure error
      \arg        FLASH_TOERR: flash timeout error
*/
flash_state_enum nvmc_flash_mass_erase(void)
{
    flash_state_enum flash_state = FLASH_READY;

    /* wait for the flash ready */
    flash_state = nvmc_flash_ready_wait(FLASH_TIMEOUT_COUNT);

    if(FLASH_READY == flash_state) {
        /* enable mass erase operation */
        NVMC_FLASH_CTL |= NVMC_FLASH_CTL_MER;
        /* start whole chip erase */
        NVMC_FLASH_CTL |= NVMC_FLASH_CTL_START;
        /* wait for the flash ready */
        flash_state = nvmc_flash_ready_wait(FLASH_TIMEOUT_COUNT);
        /* reset the MER bit */
        NVMC_FLASH_CTL &= ~NVMC_FLASH_CTL_MER;
    }

    /* return the flash state */
    return flash_state;
}

/*!
   \brief      get flash flag set or reset (API_ID(0x0033U))
   \param[in]  flash_flag: check flash flag
               only one parameter can be selected which is shown as below:
     \arg        FLASH_FLAG_BUSY: flash busy flag
     \arg        FLASH_FLAG_ENDF: flash end of operation flag
     \arg        FLASH_FLAG_WPERR: flash write protection error flag
     \arg        FLASH_FLAG_PGSERR: flash program sequence error flag
     \arg        FLASH_FLAG_RPERR: flash read protection error flag
     \arg        FLASH_FLAG_RSERR: flash read secure error flag
     \arg        FLASH_FLAG_ECCDET: flash two bits detect error flag
   \param[out] none
   \retval     FlagStatus: SET or RESET
*/
FlagStatus nvmc_flash_flag_get(uint32_t flash_flag)
{
    FlagStatus flag = RESET;

    if(NVMC_FLASH_STAT & flash_flag) {
        flag = SET;
    }
    /* return the state of corresponding flash flag */
    return flag;
}

/*!
    \brief      clear the flash pending flag (API_ID(0x0034U))
    \param[in]  flash_flag: clear flash flag
                one or more parameters can be selected which is shown as below:
      \arg        FLASH_FLAG_ENDF: flash end of operation flag
      \arg        FLASH_FLAG_WPERR: flash write protection error flag
      \arg        FLASH_FLAG_PGSERR: flash program sequence error flag
      \arg        FLASH_FLAG_RPERR: flash read protection error flag
      \arg        FLASH_FLAG_RSERR: flash read secure error flag
      \arg        FLASH_FLAG_ECCDET: flash two bits detect error flag
    \param[out] none
    \retval     none
*/
void nvmc_flash_flag_clear(uint32_t flash_flag)
{
    /* clear the flags */
    NVMC_FLASH_STAT = flash_flag;
}

/*!
    \brief      enable flash interrupt (API_ID(0x0035U))
    \param[in]  flash_int: the flash interrupt source
                one or more parameters can be selected which is shown as below:
      \arg        FLASH_INT_ENDIE: flash end of program interrupt
      \arg        FLASH_INT_WPERRIE: write protection error interrupt
      \arg        FLASH_INT_PGSERRIE: program sequence error interrupt
      \arg        FLASH_INT_RPERRIE: read protection error interrupt
      \arg        FLASH_INT_RSERRIE: read secure error interrupt
      \arg        FLASH_INT_ECCDETIE: two bits detect error interrupt
    \param[out] none
    \retval     none
*/
void nvmc_flash_interrupt_enable(uint32_t flash_int)
{
    NVMC_FLASH_CTL |= flash_int;
}

/*!
    \brief      disable flash interrupt (API_ID(0x0036U))
    \param[in]  flash_int: the flash interrupt source
                one or more parameters can be selected which is shown as below:
      \arg        FLASH_INT_ENDIE: end of program interrupt
      \arg        FLASH_INT_WPERRIE: write protection error interrupt
      \arg        FLASH_INT_PGSERRIE: program sequence error interrupt
      \arg        FLASH_INT_RPERRIE: read protection error interrupt
      \arg        FLASH_INT_RSERRIE: secure error interrupt
      \arg        FLASH_INT_ECCDETIE: two bits detect error interrupt
    \param[out] none
    \retval     none
*/
void nvmc_flash_interrupt_disable(uint32_t flash_int)
{
    NVMC_FLASH_CTL &= ~(uint32_t)flash_int;
}

/*!
    \brief      get flash interrupt flag status (API_ID(0x0037U))
    \param[in]  flash_int_flag: flash interrupt flag status
                only one parameter can be selected which is shown as below:
      \arg        FLASH_INT_FLAG_ENDF: flash end of operation interrupt flag
      \arg        FLASH_INT_FLAG_WPERR: flash write protection error interrupt flag
      \arg        FLASH_INT_FLAG_PGSERR: flash program sequence error interrupt flag
      \arg        FLASH_INT_FLAG_RPERR: flash read protection error interrupt flag
      \arg        FLASH_INT_FLAG_RSERR: flash read secure error interrupt flag
      \arg        FLASH_INT_FLAG_ECCDET: flash two bits detect error interrupt flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus nvmc_flash_interrupt_flag_get(uint8_t flash_int_flag)
{
    FlagStatus reval = RESET;

    uint32_t reg1 = NVMC_FLASH_STAT;
    uint32_t reg2 = NVMC_FLASH_CTL;

    switch(flash_int_flag) {
    /* flash end of operation interrupt */
    case FLASH_INT_FLAG_ENDF:
        reg1 = reg1 & NVMC_FLASH_STAT_ENDF;
        reg2 = reg2 & NVMC_FLASH_CTL_ENDIE;
        break;
    /* flash write protection error interrupt */
    case FLASH_INT_FLAG_WPERR:
        reg1 = reg1 & NVMC_FLASH_STAT_WPERR;
        reg2 = reg2 & NVMC_FLASH_CTL_WPERRIE;
        break;
    /* flash program sequence error interrupt */
    case FLASH_INT_FLAG_PGSERR:
        reg1 = reg1 & NVMC_FLASH_STAT_PGSERR;
        reg2 = reg2 & NVMC_FLASH_CTL_PGSERRIE;
        break;
    /* flash read protection error interrupt */
    case FLASH_INT_FLAG_RPERR:
        reg1 = reg1 & NVMC_FLASH_STAT_RPERR;
        reg2 = reg2 & NVMC_FLASH_CTL_RPERRIE;
        break;
    /* flash read secure error interrupt */
    case FLASH_INT_FLAG_RSERR:
        reg1 = reg1 & NVMC_FLASH_STAT_RSERR;
        reg2 = reg2 & NVMC_FLASH_CTL_RSERRIE;
        break;
    /* flash two bits detect error interrupt */
    case FLASH_INT_FLAG_ECCDET:
        reg1 = reg1 & NVMC_FLASH_STAT_ECCDET;
        reg2 = reg2 & NVMC_FLASH_CTL_ECCDETIE;
        break;
    default :
        break;
    }
    /*get flash interrupt flag status */
    if((0U != reg1) && (0U != reg2)) {
        reval = SET;
    }

    return reval;
}

/*!
    \brief      clear flash interrupt flag status (API_ID(0x0038U))
    \param[in]  flash_int_flag: flash interrupt flag status
                only one parameter can be selected which is shown as below:
      \arg        FLASH_INT_FLAG_ENDF: flash end of operation interrupt flag
      \arg        FLASH_INT_FLAG_WPERR: flash write protection error interrupt flag
      \arg        FLASH_INT_FLAG_PGSERR: flash program sequence error interrupt flag
      \arg        FLASH_INT_FLAG_RPERR: flash read protection error interrupt flag
      \arg        FLASH_INT_FLAG_RSERR: flash read secure error interrupt flag
      \arg        FLASH_INT_FLAG_ECCDET: flash two bits detect error interrupt flag
    \param[out] none
    \retval     none
*/
void nvmc_flash_interrupt_flag_clear(uint8_t flash_int_flag)
{

    switch(flash_int_flag) {
    /* flash end of operation interrupt */
    case FLASH_INT_FLAG_ENDF:
        NVMC_FLASH_STAT |= NVMC_FLASH_STAT_ENDF;
        break;
    /* flash write protection error interrupt */
    case FLASH_INT_FLAG_WPERR:
        NVMC_FLASH_STAT |= NVMC_FLASH_STAT_WPERR;
        break;
    /* flash program sequence error interrupt */
    case FLASH_INT_FLAG_PGSERR:
        NVMC_FLASH_STAT |= NVMC_FLASH_STAT_PGSERR;
        break;
    /* flash read protection error interrupt */
    case FLASH_INT_FLAG_RPERR:
        NVMC_FLASH_STAT |= NVMC_FLASH_STAT_RPERR;
        break;
    /* flash read secure error interrupt */
    case FLASH_INT_FLAG_RSERR:
        NVMC_FLASH_STAT |= NVMC_FLASH_STAT_RSERR;
        break;
    /* flash two bits detect error interrupt */
    case FLASH_INT_FLAG_ECCDET:
        NVMC_FLASH_STAT |= NVMC_FLASH_STAT_ECCDET;
        break;
    default :
        break;
    }

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
static flash_state_enum nvmc_flash_ready_wait(uint32_t timeout)
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
    \brief      program a half-word at the OTP corresponding address (API_ID(0x003BU))
    \param[in]  address: address to program (0x1FFF0000 + 0x20 * x (x = 0 ... 207))
    \param[in]  data: half-word to program
    \param[out] none
    \retval     state of OTP
      \arg        OTP_READY: OTP operation has been completed
      \arg        OTP_BUSY: OTP operation is in progress
      \arg        OTP_DATAERR: OTP Data error
      \arg        OTP_PGLKERR: OTP Program at lock state error
      \arg        OTP_MULTI_DATAERR: OTP Multi-redundant data error
      \arg        OTP_PGSERR: OTP program sequence error
      \arg        OTP_TOERR: OTP ENVM timeout error
*/
otp_state_enum nvmc_otp_halfword_program(uint32_t address, uint16_t data)
{
    otp_state_enum otp_state = OTP_READY;
    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_OTP_VALID_ADDRESS(address)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x003BU), ERR_PARAM_OUT_OF_RANGE);
    } else
#endif
    {
        /* wait for the OTP ready */
        otp_state = nvmc_otp_ready_wait(OTP_TIMEOUT_COUNT);

        if(OTP_READY == otp_state) {
            NVMC_ENVM_CTL |= NVMC_ENVM_CTL_PG;
            REG16(address) = data;
            __DSB();
            otp_state = nvmc_otp_ready_wait(OTP_TIMEOUT_COUNT);
            NVMC_ENVM_CTL &= ~NVMC_ENVM_CTL_PG;
        }
    }
    return otp_state;
}

/*!
    \brief     program all AES KEY parameter (API_ID(0x003CU))
    \param[in]  buf: data buffer to write
    \param[out] none
    \retval     state of OTP
      \arg        OTP_READY: OTP operation has been completed
      \arg        OTP_BUSY: OTP operation is in progress
      \arg        OTP_DATAERR: OTP Data error
      \arg        OTP_PGLKERR: OTP Program at lock state error
      \arg        OTP_MULTI_DATAERR: OTP Multi-redundant data error
      \arg        OTP_PGSERR: OTP program sequence error
      \arg        OTP_TOERR: OTP ENVM timeout error
*/
otp_state_enum nvmc_otp_aes_key_all_program(uint16_t buf[])
{
    uint32_t index;
    uint32_t OTP_HALFWORD_CNT = (uint32_t)AES_KEY_SIZE / 2U;
    otp_state_enum otp_state = OTP_READY;
    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_VALID_POINTER(buf)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x003CU), ERR_PARAM_POINTER);
    } else
#endif
    {
        for(index = 0U; index < OTP_HALFWORD_CNT; index++) {
            nvmc_otp_flag_clear(OTP_FLAG_ENDF | OTP_FLAG_DATAERR | OTP_FLAG_PGLKERR | OTP_FLAG_PGSERR | OTP_FLAG_MULTI_DATAERR);
            otp_state = nvmc_otp_halfword_program(OTP_AES_KEY_AXI_ADDR + index * 0x20U, buf[index]);
            if(OTP_READY != otp_state) {
                break;
            }
        }
    }

    return otp_state;
}

/*!
    \brief      program all debug password parameter (API_ID(0x003DU))
    \param[in]  buf: data buffer to write
    \param[out] none
    \retval     state of OTP
      \arg        OTP_READY: OTP operation has been completed
      \arg        OTP_BUSY: OTP operation is in progress
      \arg        OTP_DATAERR: OTP Data error
      \arg        OTP_PGLKERR: OTP Program at lock state error
      \arg        OTP_MULTI_DATAERR: OTP Multi-redundant data error
      \arg        OTP_PGSERR: OTP program sequence error
      \arg        OTP_TOERR: OTP ENVM timeout error
*/
otp_state_enum nvmc_otp_dp_all_program(uint16_t buf[])
{
    uint32_t index;
    uint32_t OTP_HALFWORD_CNT = (uint32_t)USER_DP_SIZE / 2U;
    otp_state_enum otp_state = OTP_READY;
    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_VALID_POINTER(buf)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x003DU), ERR_PARAM_POINTER);
    } else
#endif
    {
        for(index = 0U; index < OTP_HALFWORD_CNT; index++) {
            nvmc_otp_flag_clear(OTP_FLAG_ENDF | OTP_FLAG_DATAERR | OTP_FLAG_PGLKERR | OTP_FLAG_PGSERR | OTP_FLAG_MULTI_DATAERR);
            otp_state = nvmc_otp_halfword_program(OTP_USER_DBGPSWD_DATA_AXI_ADDR + index * 0x20U, buf[index]);
            if(OTP_READY != otp_state) {
                break;
            }
        }
    }
    return otp_state;
}

/*!
    \brief      program all user data parameter (API_ID(0x003EU))
    \param[in]  buf: data buffer to write
    \param[out] none
    \retval     state of OTP
      \arg        OTP_READY: OTP operation has been completed
      \arg        OTP_BUSY: OTP operation is in progress
      \arg        OTP_DATAERR: OTP Data error
      \arg        OTP_PGLKERR: OTP Program at lock state error
      \arg        OTP_MULTI_DATAERR: OTP Multi-redundant data error
      \arg        OTP_PGSERR: OTP program sequence error
      \arg        OTP_TOERR: OTP ENVM timeout error
*/
otp_state_enum nvmc_otp_user_data_all_program(uint16_t buf[])
{
    uint32_t index;
    uint32_t OTP_HALFWORD_CNT = (uint32_t)USER_DATA_SIZE / 2U;
    otp_state_enum otp_state = OTP_READY;
    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_VALID_POINTER(buf)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x003EU), ERR_PARAM_POINTER);
    } else
#endif
    {
        for(index = 0U; index < OTP_HALFWORD_CNT; index++) {
            nvmc_otp_flag_clear(OTP_FLAG_ENDF | OTP_FLAG_DATAERR | OTP_FLAG_PGLKERR | OTP_FLAG_PGSERR | OTP_FLAG_MULTI_DATAERR);
            otp_state = nvmc_otp_halfword_program(OTP_USER_DATA_AXI_ADDR + index * 0x20U, buf[index]);
            if(OTP_READY != otp_state) {
                break;
            }
        }
    }
    return otp_state;
}

/*!
    \brief      repair OTP data (API_ID(0x003FU))
    \param[in]  none
    \param[out] none
    \retval     state of OTP
      \arg        OTP_READY: OTP operation has been completed
      \arg        OTP_BUSY: OTP operation is in progress
      \arg        OTP_DATAERR: OTP Data error
      \arg        OTP_PGLKERR: OTP Program at lock state error
      \arg        OTP_MULTI_DATAERR: OTP Multi-redundant data error
      \arg        OTP_PGSERR: OTP program sequence error
      \arg        OTP_TOERR: OTP ENVM timeout error
*/
otp_state_enum nvmc_otp_repair(void)
{
    otp_state_enum otp_state = OTP_READY;

    /* wait for the OTP ready */
    otp_state = nvmc_otp_ready_wait(OTP_TIMEOUT_COUNT);

    if(OTP_READY == otp_state) {
        NVMC_OTP_CTL |= NVMC_OTP_CTL_OTP_REPSTART;
        otp_state = nvmc_otp_ready_wait(OTP_TIMEOUT_COUNT);
    }

    return otp_state;
}

/*!
    \brief      get OTP flag set or reset (API_ID(0x0040U))
    \param[in]  otp_flag: check OTP flag
                only one parameter can be selected which is shown as below:
      \arg        OTP_FLAG_BUSY: OTP busy flag
      \arg        OTP_FLAG_ENDF: OTP end of operation flag
      \arg        OTP_FLAG_DATAERR: OTP Data error flag
      \arg        OTP_FLAG_PGLKERR: OTP Program at lock state error flag
      \arg        OTP_FLAG_PGSERR: OTP program sequence error flag
      \arg        OTP_FLAG_MULTI_DATAERR: OTP Multi-redundant data error flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus nvmc_otp_flag_get(uint32_t otp_flag)
{
    FlagStatus flag = RESET;

    if(NVMC_OTP_STAT & (uint32_t)otp_flag) {
        flag = SET;
    }
    /* return the state of corresponding OTP flag */
    return flag;
}

/*!
    \brief      clear OTP flag (API_ID(0x0041U))
    \param[in]  otp_flag: check OTP flag
                one or more parameters can be selected which is shown as below:
      \arg        OTP_FLAG_ENDF: OTP end of operation flag
      \arg        OTP_FLAG_DATAERR: OTP Data error flag
      \arg        OTP_FLAG_PGLKERR: OTP Program at lock state error flag
      \arg        OTP_FLAG_PGSERR: OTP program sequence error flag
      \arg        OTP_FLAG_MULTI_DATAERR: OTP Multi-redundant data error flag
    \param[out] none
    \retval     none
*/
void nvmc_otp_flag_clear(uint32_t otp_flag)
{
    /* clear the flags */
    NVMC_OTP_STAT = (uint32_t)otp_flag;
}

/*!
    \brief      enable OTP interrupt (API_ID(0x0042U))
    \param[in]  otp_int: the OTP interrupt source
                one or more parameters can be selected which is shown as below:
      \arg        OTP_INT_ENDIE: OTP program/repair end interrupt enable bit
      \arg        OTP_INT_DATAERRIE: OTP Data error interrupt enable bit
      \arg        OTP_INT_PGLKERRIE: OTP Program at lock state error interrupt enable bit
      \arg        OTP_INT_PGSERRIE: OTP program sequence error enable bit
      \arg        OTP_INT_MULTI_DATAERRIE: OTP Multi-redundant data error interrupt enable bit
    \param[out] none
    \retval     none
*/
void nvmc_otp_interrupt_enable(uint32_t otp_int)
{
    NVMC_OTP_CTL |= (uint32_t)otp_int;
}

/*!
    \brief      disable OTP interrupt (API_ID(0x0043U))
    \param[in]  otp_int: the OTP interrupt source
                one or more parameters can be selected which is shown as below:
      \arg        OTP_INT_ENDIE: OTP program/repair end interrupt enable bit
      \arg        OTP_INT_DATAERRIE: OTP Data error interrupt enable bit
      \arg        OTP_INT_PGLKERRIE: OTP Program at lock state error interrupt enable bit
      \arg        OTP_INT_PGSERRIE: OTP program sequence error enable bit
      \arg        OTP_INT_MULTI_DATAERRIE: OTP Multi-redundant data error interrupt enable bit
    \param[out] none
    \retval     none
*/
void nvmc_otp_interrupt_disable(uint32_t otp_int)
{
    NVMC_OTP_CTL &= ~(uint32_t)otp_int;
}

/*!
    \brief      get OTP interrupt flag status (API_ID(0x0044U))
    \param[in]  otp_int_flag: OTP interrupt flag status
                only one parameter can be selected which is shown as below:
      \arg        OTP_INT_FLAG_ENDF: OTP end of operation interrupt flag
      \arg        OTP_INT_FLAG_DATAERR: OTP data error interrupt flag
      \arg        OTP_INT_FLAG_PGLKERR: OTP program at lock state error interrupt flag
      \arg        OTP_INT_FLAG_PGSERR: OTP program sequence error interrupt flag
      \arg        OTP_INT_FLAG_MULTI_DATAERR: OTP multi-redundant data error interrupt flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus nvmc_otp_interrupt_flag_get(uint8_t otp_int_flag)
{
    FlagStatus reval = RESET;

    uint32_t reg1 = NVMC_OTP_STAT;
    uint32_t reg2 = NVMC_OTP_CTL;

    switch(otp_int_flag) {
    /* OTP end of operation interrupt */
    case OTP_INT_FLAG_ENDF:
        reg1 = reg1 & NVMC_OTP_STAT_ENDF;
        reg2 = reg2 & NVMC_OTP_CTL_ENDIE;
        break;
    /* OTP data error interrupt */
    case OTP_INT_FLAG_DATAERR:
        reg1 = reg1 & NVMC_OTP_STAT_DATAERRF;
        reg2 = reg2 & NVMC_OTP_CTL_DATAERRIE;
        break;
    /* OTP program at lock state error interrupt */
    case OTP_INT_FLAG_PGLKERR:
        reg1 = reg1 & NVMC_OTP_STAT_PGLKERRF;
        reg2 = reg2 & NVMC_OTP_CTL_PGLKERRIE;
        break;
    /* OTP program sequence error interrupt */
    case OTP_INT_FLAG_PGSERR:
        reg1 = reg1 & NVMC_OTP_STAT_PGSERR;
        reg2 = reg2 & NVMC_OTP_CTL_PGSERRIE;
        break;
    /* OTP multi-redundant data error interrupt */
    case OTP_INT_FLAG_MULTI_DATAERR:
        reg1 = reg1 & NVMC_OTP_STAT_MULTI_DATAERRF;
        reg2 = reg2 & NVMC_OTP_CTL_MULTI_DATAERRIE;
        break;
    default :
        break;
    }
    /*get OTP interrupt flag status */
    if((0U != reg1) && (0U != reg2)) {
        reval = SET;
    }

    return reval;
}

/*!
    \brief      clear OTP interrupt flag status (API_ID(0x0045U))
    \param[in]  otp_int_flag: OTP interrupt flag status
                only one parameter can be selected which is shown as below:
      \arg        OTP_INT_FLAG_ENDF: OTP end of operation interrupt flag
      \arg        OTP_INT_FLAG_DATAERR: OTP data error interrupt flag
      \arg        OTP_INT_FLAG_PGLKERR: OTP program at lock state error interrupt flag
      \arg        OTP_INT_FLAG_PGSERR: OTP program sequence error interrupt flag
      \arg        OTP_INT_FLAG_MULTI_DATAERR: OTP multi-redundant data error interrupt flag
    \param[out] none
    \retval     none
*/
void nvmc_otp_interrupt_flag_clear(uint8_t otp_int_flag)
{

    switch(otp_int_flag) {
    /* OTP end of operation interrupt */
    case OTP_INT_FLAG_ENDF:
        NVMC_OTP_STAT |= NVMC_OTP_STAT_ENDF;
        break;
    /* OTP data error interrupt */
    case OTP_INT_FLAG_DATAERR:
        NVMC_OTP_STAT |= NVMC_OTP_STAT_DATAERRF;
        break;
    /* OTP program at lock state error interrupt */
    case OTP_INT_FLAG_PGLKERR:
        NVMC_OTP_STAT |= NVMC_OTP_STAT_PGLKERRF;
        break;
    /* OTP program sequence error interrupt */
    case OTP_INT_FLAG_PGSERR:
        NVMC_OTP_STAT |= NVMC_OTP_STAT_PGSERR;
        break;
    /* OTP multi-redundant data error interrupt */
    case OTP_INT_FLAG_MULTI_DATAERR:
        NVMC_OTP_STAT |= NVMC_OTP_STAT_MULTI_DATAERRF;
        break;
    default :
        break;
    }

}

/*!
    \brief      get the OTP state (API_ID(0x0046U))
    \param[in]  none
    \param[out] none
    \retval     state of OTP
      \arg        OTP_READY: OTP operation has been completed
      \arg        OTP_BUSY: OTP operation is in progress
      \arg        OTP_DATAERR: OTP Data error
      \arg        OTP_PGLKERR: OTP Program at lock state error
      \arg        OTP_MULTI_DATAERR: OTP Multi-redundant data error
      \arg        OTP_PGSERR: OTP program sequence error
*/
static otp_state_enum nvmc_otp_state_get(void)
{
    otp_state_enum otp_state = OTP_READY;

    if(OTP_FLAG_BUSY == (NVMC_OTP_STAT & NVMC_OTP_STAT_BUSY)) {
        otp_state = OTP_BUSY;
    } else if(RESET != (NVMC_OTP_STAT & NVMC_OTP_STAT_MULTI_DATAERRF)) {
        otp_state = OTP_MULTI_DATAERR;
    } else if(RESET != (NVMC_OTP_STAT & NVMC_OTP_STAT_PGLKERRF)) {
        otp_state = OTP_PGLKERR;
    } else if(RESET != (NVMC_OTP_STAT & NVMC_OTP_STAT_DATAERRF)) {
        otp_state = OTP_DATAERR;
    } else if(RESET != (NVMC_OTP_STAT & NVMC_OTP_STAT_PGSERR)) {
        otp_state = OTP_PGSERR;
    } else {
        otp_state = OTP_READY;
    }

    /* return the OTP state */
    return otp_state;
}

/*!
    \brief      check whether OTP is ready or not (API_ID(0x0047U))
    \param[in]  timeout: timeout count (0x00000000 - 0xFFFFFFFF)
    \param[out] none
    \retval     state of OTP
      \arg        OTP_READY: OTP operation has been completed
      \arg        OTP_BUSY: OTP operation is in progress
      \arg        OTP_DATAERR: OTP Data error
      \arg        OTP_PGLKERR: OTP Program at lock state error
      \arg        OTP_MULTI_DATAERR: OTP Multi-redundant data error
      \arg        OTP_PGSERR: OTP program sequence error
      \arg        OTP_TOERR: OTP ENVM timeout error
    \note       This function contains scenarios leading to an infinite loop.
                Modify according to the user's actual usage scenarios.
*/
static otp_state_enum nvmc_otp_ready_wait(uint32_t timeout)
{
    otp_state_enum otp_state = OTP_BUSY;

    /* wait for OTP ready */
    do {
        /* get OTP state */
        otp_state = nvmc_otp_state_get();
        timeout--;
    } while((OTP_BUSY == otp_state) && (0U != timeout));

    if(OTP_BUSY == otp_state) {
        otp_state = OTP_TOERR;
    }

    /* return the OTP state */
    return otp_state;
}

/*!
    \brief      unlock the option byte operation (API_ID(0x0048U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void nvmc_ob_unlock(void)
{
    if(RESET != (NVMC_OBCTL & NVMC_OBCTL_OBLK)) {
        /* write the NVMC key */
        NVMC_OBKEY = OB_UNLOCK_KEY0;
        NVMC_OBKEY = OB_UNLOCK_KEY1;
    }
}

/*!
    \brief      lock the option byte operation (API_ID(0x0049U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void nvmc_ob_lock(void)
{
    /* reset the OB_LK bit */
    NVMC_OBCTL |= NVMC_OBCTL_OBLK;
}

/*!
    \brief      send option byte change command (API_ID(0x004AU))
    \param[in]  none
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
*/
nvmc_ob_state_enum nvmc_ob_start(void)
{
    nvmc_ob_state_enum ob_state = NVMC_OB_READY;

    /* set the OB_START bit in OBCTL register */
    NVMC_OBCTL |= NVMC_OBCTL_OBSTART;
    ob_state = nvmc_ob_ready_wait(FLASH_TIMEOUT_COUNT);

    return ob_state;
}

/*!
    \brief      modify option byte to factory value (API_ID(0x004BU))
    \param[in]  none
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
      \arg        NVMC_OB_DCRP_SCR_PROT: DCRP/SCR protection is enabled, should not remove protection by this function.
*/
nvmc_ob_state_enum nvmc_ob_factory_value_config(void)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;
    uint32_t obstat0_eft_spc = NVMC_OBSTAT0_EFT & NVMC_OBSTAT0_EFT_SPC;
    uint32_t otp_user_ctl0_spc_h = NVMC_OTP_USER_CTL0 & NVMC_OTP_USER_CTL0_SPC_H;
    uint32_t dcrp_area_end = (NVMC_DCRPADDR_EFT & NVMC_DCRPADDR_EFT_DCRP_AREA_END) >> DCRPADDR_DCRP_AREA_END_OFFSET;
    uint32_t dcrp_area_start = (NVMC_DCRPADDR_EFT & NVMC_DCRPADDR_EFT_DCRP_AREA_START) >> DCRPADDR_DCRP_AREA_START_OFFSET;
    uint32_t scr_area_end = (NVMC_SCRADDR_EFT & NVMC_SCRADDR_EFT_SCR_AREA_END) >> SCRADDR_SCR_AREA_END_OFFSET;
    uint32_t scr_area_start = (NVMC_SCRADDR_EFT & NVMC_SCRADDR_EFT_SCR_AREA_START) >> SCRADDR_SCR_AREA_START_OFFSET;

    /* check the option bytes security protection value */
    if((OB_HSPC == obstat0_eft_spc) || otp_user_ctl0_spc_h) {
        nvmc_ob_state = NVMC_HSPC;
    } else if((dcrp_area_end >= dcrp_area_start) || (scr_area_end >= scr_area_start)) {
        nvmc_ob_state = NVMC_OB_DCRP_SCR_PROT;
    } else {
        /* wait for the NVMC ready */
        nvmc_ob_state = nvmc_ob_ready_wait(FLASH_TIMEOUT_COUNT);

        if(NVMC_OB_READY == nvmc_ob_state) {
            /* write NVMC_OBSTAT0_MDF */
            NVMC_OBSTAT0_MDF = OB_OBSTAT0_FACTORY_VALUE;
            /* write NVMC_BTADDR_MDF */
            NVMC_BTADDR_MDF = OB_BTADDR_FACTORY_VALUE;
            /* write NVMC_OBSTAT1_MDF */
            NVMC_OBSTAT1_MDF = OB_OBSTAT1_FACTORY_VALUE;
            /* write NVMC_WP0_FLASH_MDF */
            NVMC_WP0_FLASH_MDF = OB_WP0_FLASH_FACTORY_VALUE;
            /* write NVMC_WP1_FLASH_MDF */
            NVMC_WP1_FLASH_MDF = OB_WP1_FLASH_FACTORY_VALUE;
            NVMC_OBREPCTL_MDF = OB_OBREPCTL_FACTORY_VALUE;
            NVMC_OBREPSTADDR_MDF = OB_OBREPSTADDR_FACTORY_VALUE;
            NVMC_OBREPENDADDR_MDF = OB_OBREPENDADDR_FACTORY_VALUE;
            NVMC_OBBBADDRX_MDF(0U) = OB_OBBBADDRX_FACTORY_VALUE;
            NVMC_OBBBADDRX_MDF(1U) = OB_OBBBADDRX_FACTORY_VALUE;
            NVMC_OBBBADDRX_MDF(2U) = OB_OBBBADDRX_FACTORY_VALUE;
            NVMC_OBBBADDRX_MDF(3U) = OB_OBBBADDRX_FACTORY_VALUE;
            /* write NVMC_WP_ENVM_MDF */
            NVMC_WP_ENVM_MDF = OB_WP_ENVM_FACTORY_VALUE;
            nvmc_ob_state = nvmc_ob_start();
        }

    }
    return nvmc_ob_state;
}

/*!
    \brief      program the option bytes OBSTAT0 (API_ID(0x004CU))
    \param[in]  ob_obstat0: obstat0 option bytes. When write one paramter, the corresponding mask should be set.
                one or more parameters can be selected which are shown as below:
      \arg        OB_LICENCE_FW_INSTALL_ENABLE/OB_LICENCE_FW_INSTALL_DISABLE: Enable/disable icensed firmware install
      \arg        OB_INFO_PRINT_ENABLE/OB_INFO_PRINT_DISABLE: Enable/disable information print during secure boot
      \arg        OB_BOR_TH_VALUE3/OB_BOR_TH_VALUE2/OB_BOR_TH_VALUE1/OB_BOR_TH_OFF: BOR threshold value
      \arg        OB_FWDGT_HW/OB_FWDGT_SW: hardware or software free watchdog timer
      \arg        OB_FW_IMAGE_VERIFICATION_DISABLE/OB_FW_IMAGE_VERIFICATION_ENABLE: Firmware image verification
      \arg        OB_DEEPSLEEP_RST/OB_DEEPSLEEP_NRST: reset or not entering deep sleep mode
      \arg        OB_STDBY_RST/OB_STDBY_NRST: reset or not entering standby mode
      \arg        OB_NSPC/OB_LSPC/OB_HSPC: no protection/protection level low/protection level high
      \arg        OB_OTA_CHECK_DISABLE/OB_OTA_CHECK_ENABLE: disable/enable firmware check
      \arg        OB_DPSLP_FWDGT_SUSPEND/OB_DPSLP_FWDGT_RUN: free watchdog is suspended or running in deep-sleep mode
      \arg        OB_STDBY_FWDGT_SUSPEND/OB_STDBY_FWDGT_RUN: free watchdog is suspended or running in standby mode
      \arg        OB_FW_UPDATE_DISABLE/OB_FW_UPDATE_ENABLE: disable/enable hardware copies flash backup data to ENVM
      \arg        OB_ENVM_BANK0_BACKUP/OB_ENVM_BANK1_BACKUP: ENVM content is consistent with flash bank0/bank1 backup
      \arg        OB_SECURE_MODE_DISABLE/OB_SECURE_MODE_ENABLE: secure mode disable/enable
      \arg        OB_ITCM_ECC_DISABLE/OB_ITCM_ECC_ENABLE: disable/enable CPU ITCM ECC function
      \arg        OB_DTCM0_ECC_DISABLE/OB_DTCM0_ECC_ENABLE: disable/enable CPU DTCM0 ECC function
      \arg        OB_DTCM1_ECC_DISABLE/OB_DTCM1_ECC_ENABLE: disable/enable CPU DTCM1 ECC function
      \arg        OB_R2_SRAM_ECC_DISABLE/OB_R2_SRAM_ECC_ENABLE: disable/enable Region 2 SRAM ECC function
      \arg        OB_R1_SRAM_ECC_DISABLE/OB_R1_SRAM_ECC_ENABLE: disable/enable Region 1 SRAM ECC function
      \arg        OB_R0_SRAM_ECC_DISABLE/OB_R0_SRAM_ECC_ENABLE: disable/enable Region 0 SRAM ECC function
      \arg        OB_ENVM_ECC_CONTROL_BY_ENVM_ECC_MODE/OB_ENVM_ECC_ENABLE: disable/enable ENVM ECC function
      \arg        OB_IOSPDOPEN_DISABLE/OB_IOSPDOPEN_ENABLE: high-speed at low-voltage disable/enable
      \arg        OB_TIMING_FRIENDLY_MODE/OB_PERFORMANCE_FRIENDLY_MODE: Timing friendly mode/Performance-first mode
      \arg        OB_FIRMWARE_IMG_VERIFY_DISABLE/OB_FIRMWARE_IMG_VERIFY_ENABLE: firmware image verification disable/enable
    \param[in]  ob_obstat0_mask: obstat0 bits mask. They correspond to the above paramter value one by one.
                one or more parameters can be selected which are shown as below:
      \arg        NVMC_OBSTAT0_MDF_DISLFI: Licensed firmware install (LFI) disable bit mask
      \arg        NVMC_OBSTAT0_MDF_NOPRINT: no information is printed during secure boot mask
      \arg        NVMC_OBSTAT0_MDF_BOR_TH: brownout reset threshold value mask
      \arg        NVMC_OBSTAT0_MDF_NWDG_HW: watchdog value mask
      \arg        NVMC_OBSTAT0_MDF_VFALL: Firmware image verification enable bit mask
      \arg        NVMC_OBSTAT0_MDF_NRST_DPSLP: deepsleep reset value mask
      \arg        NVMC_OBSTAT0_MDF_NRST_STDBY: standby reset value mask
      \arg        NVMC_OBSTAT0_MDF_SPC: security protection code mask
      \arg        NVMC_OBSTAT0_MDF_OTA_CHECK: OTA check status bit mask
      \arg        NVMC_OBSTAT0_MDF_FWDGSPD_DPSLP: watchdog suspend status in deepsleep mode mask
      \arg        NVMC_OBSTAT0_MDF_FWDGSPD_STDBY: watchdog suspend status in standby mode mask
      \arg        NVMC_OBSTAT0_MDF_FW_UPDATE: Firmware update status bit mask
      \arg        NVMC_OBSTAT0_MDF_SWAP: SWAP status bit mask
      \arg        NVMC_OBSTAT0_MDF_SCR: security access mode status bit mask
      \arg        NVMC_OBSTAT0_MDF_ITCMECCEN: ITCM ECC function enable status bit mask
      \arg        NVMC_OBSTAT0_MDF_DTCM0ECCEN: DTCM0 ECC function enable status bit mask
      \arg        NVMC_OBSTAT0_MDF_DTCM1ECCEN: DTCM1 ECC function enable status bit mask
      \arg        NVMC_OBSTAT0_MDF_R2_SRAM_ECCEN: Region 2 SRAM ECC enable status bit mask
      \arg        NVMC_OBSTAT0_MDF_R1_SRAM_ECCEN: Region 1 SRAM ECC enable status bit mask
      \arg        NVMC_OBSTAT0_MDF_R0_SRAM_ECCEN: Region 0 SRAM ECC enable status bit mask
      \arg        NVMC_OBSTAT0_MDF_ENVM_ECC_OVRD: ENVM ECC enable status bit mask
      \arg        NVMC_OBSTAT0_MDF_IOSPDOPEN: I/O speed optimization, high-speed at low-voltage enable status bit mask
      \arg        NVMC_OBSTAT0_MDF_NOPIPE_MODE: pipeline mode bit mask
      \arg        NVMC_OBSTAT0_MDF_VFIMG: Firmware image verification enable bit mask
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_obstat0_config(uint32_t ob_obstat0, uint32_t ob_obstat0_mask)
{
    uint32_t obstat0_reg;
    uint32_t obstat0_eft_spc = NVMC_OBSTAT0_EFT & NVMC_OBSTAT0_EFT_SPC;
    uint32_t otp_user_ctl0_spc_h = NVMC_OTP_USER_CTL0 & NVMC_OTP_USER_CTL0_SPC_H;
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    /* check the option bytes security protection value */
    if((OB_HSPC == obstat0_eft_spc) || otp_user_ctl0_spc_h) {
        nvmc_ob_state = NVMC_HSPC;
    } else {
        /* wait for the nvmc ready */
        nvmc_ob_state = nvmc_ob_ready_wait(NVMC_TIMEOUT_COUNT);

        if(NVMC_OB_READY == nvmc_ob_state) {
            obstat0_reg = NVMC_OBSTAT0_EFT;

            NVMC_OBSTAT0_MDF = (obstat0_reg & (~ob_obstat0_mask)) | (ob_obstat0 & ob_obstat0_mask);
            /* set the OBSTART bit */
            NVMC_OBCTL |= NVMC_OBCTL_OBSTART;
            /* wait for the nvmc ready */
            nvmc_ob_state = nvmc_ob_ready_wait(NVMC_TIMEOUT_COUNT);
        }
    }
    return nvmc_ob_state;
}

/*!
    \brief      enable secure access mode (API_ID(0x004DU))
                For independent functions that complete option byte configuration by calling nvmc_ob_obstat0_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  none
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_secure_access_mode_enable(void)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    nvmc_ob_state = nvmc_ob_obstat0_config(OB_SECURE_MODE_ENABLE, NVMC_OBSTAT0_MDF_SCR);

    return nvmc_ob_state;
}

/*!
    \brief      disable secure access mode (API_ID(0x004EU))
                For independent functions that complete option byte configuration by calling nvmc_ob_obstat0_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  none
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_secure_access_mode_disable(void)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    nvmc_ob_state = nvmc_ob_obstat0_config(OB_SECURE_MODE_DISABLE, NVMC_OBSTAT0_MDF_SCR);

    return nvmc_ob_state;
}

/*!
    \brief      configure the option byte security protection level (API_ID(0x004FU))
                For independent functions that complete option byte configuration by calling nvmc_ob_obstat0_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  ob_spc: specify security protection level
                only one parameter can be selected which is shown as below:
      \arg        OB_NSPC: no read protection
      \arg        OB_LSPC: low read protection
      \arg        OB_HSPC: high read protection
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_security_protection_config(uint32_t ob_spc)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;
    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_NVMC_SPC_VALUE(ob_spc)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x004FU), ERR_PARAM_INVALID);
    } else
#endif
    {
        nvmc_ob_state = nvmc_ob_obstat0_config(ob_spc, NVMC_OBSTAT0_MDF_SPC);
    }
    return nvmc_ob_state;
}

/*!
    \brief      program low power related option byte configuration (API_ID(0x0050U))
                For independent functions that complete option byte configuration by calling nvmc_ob_obstat0_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  ob_deepsleep: option byte deepsleep reset value
                only one parameter can be selected which is shown as below:
      \arg        OB_DEEPSLEEP_NRST: no reset when entering deepsleep mode
      \arg        OB_DEEPSLEEP_RST: generate a reset instead of entering deepsleep mode
    \param[in]  ob_stdby:option byte standby reset value
                only one parameter can be selected which is shown as below:
      \arg        OB_STDBY_NRST: no reset when entering standby mode
      \arg        OB_STDBY_RST: generate a reset instead of entering standby mode
    \param[in]  ob_fwdg_suspend_deepsleep: option byte FWDG suspend status in deep-sleep mode
                only one parameter can be selected which is shown as below:
      \arg        OB_DPSLP_FWDGT_SUSPEND: free watchdog is suspended in deep-sleep mode
      \arg        OB_DPSLP_FWDGT_RUN: free watchdog is running in deep-sleep mode
    \param[in]  ob_fwdg_suspend_standby: option byte FWDG suspend status in standby mode
                only one parameter can be selected which is shown as below:
      \arg        OB_STDBY_FWDGT_SUSPEND: free watchdog is suspended in standby mode
      \arg        OB_STDBY_FWDGT_RUN: free watchdog is running in standby mode
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_low_power_write(uint32_t ob_deepsleep, uint32_t ob_stdby, uint32_t ob_fwdg_suspend_deepsleep,
        uint32_t ob_fwdg_suspend_standby)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    nvmc_ob_state = nvmc_ob_obstat0_config(ob_deepsleep | ob_stdby | ob_fwdg_suspend_deepsleep | ob_fwdg_suspend_standby,
                                           NVMC_OBSTAT0_MDF_NRST_DPSLP | NVMC_OBSTAT0_MDF_NRST_STDBY | NVMC_OBSTAT0_MDF_FWDGSPD_DPSLP | NVMC_OBSTAT0_MDF_FWDGSPD_STDBY);

    return nvmc_ob_state;
}

/*!
    \brief      program the option byte BOR threshold value (API_ID(0x0051U))
                For independent functions that complete option byte configuration by calling nvmc_ob_obstat0_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  ob_bor_th: user option byte
                only one parameter can be selected which is shown as below:
      \arg        OB_BOR_TH_VALUE3: BOR threshold value 3
      \arg        OB_BOR_TH_VALUE2: BOR threshold value 2
      \arg        OB_BOR_TH_VALUE1: BOR threshold value 1
      \arg        OB_BOR_TH_OFF: no BOR function
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_bor_threshold_config(uint32_t ob_bor_th)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    nvmc_ob_state = nvmc_ob_obstat0_config(ob_bor_th, NVMC_OBSTAT0_MDF_BOR_TH);

    return nvmc_ob_state;
}

/*!
    \brief      enable hardware watchdog (API_ID(0x0052U))
                For independent functions that complete option byte configuration by calling nvmc_ob_obstat0_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  none
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_hardware_watchdog_enable(void)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    nvmc_ob_state = nvmc_ob_obstat0_config(OB_FWDGT_HW, NVMC_OBSTAT0_MDF_NWDG_HW);

    return nvmc_ob_state;
}

/*!
    \brief      disable hardware watchdog (API_ID(0x0053U))
                For independent functions that complete option byte configuration by calling nvmc_ob_obstat0_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  none
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_hardware_watchdog_disable(void)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    nvmc_ob_state = nvmc_ob_obstat0_config(OB_FWDGT_SW, NVMC_OBSTAT0_MDF_NWDG_HW);

    return nvmc_ob_state;
}

/*!
    \brief      enable OTA check (API_ID(0x0054U))
                For independent functions that complete option byte configuration by calling nvmc_ob_obstat0_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  none
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_ota_check_enable(void)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    nvmc_ob_state = nvmc_ob_obstat0_config(OB_OTA_CHECK_ENABLE, NVMC_OBSTAT0_MDF_OTA_CHECK);

    return nvmc_ob_state;
}

/*!
    \brief      disable OTA check (API_ID(0x0055U))
                For independent functions that complete option byte configuration by calling nvmc_ob_obstat0_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  none
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_ota_check_disable(void)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    nvmc_ob_state = nvmc_ob_obstat0_config(OB_OTA_CHECK_DISABLE, NVMC_OBSTAT0_MDF_OTA_CHECK);

    return nvmc_ob_state;
}

/*!
    \brief      enable firmware update (API_ID(0x0056U))
                For independent functions that complete option byte configuration by calling nvmc_ob_obstat0_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  none
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_firmware_update_enable(void)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    nvmc_ob_state = nvmc_ob_obstat0_config(OB_FW_UPDATE_ENABLE, NVMC_OBSTAT0_MDF_FW_UPDATE);

    return nvmc_ob_state;
}

/*!
    \brief      disable firmware update (API_ID(0x0057U))
                For independent functions that complete option byte configuration by calling nvmc_ob_obstat0_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  none
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_firmware_update_disable(void)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    nvmc_ob_state = nvmc_ob_obstat0_config(OB_FW_UPDATE_DISABLE, NVMC_OBSTAT0_MDF_FW_UPDATE);

    return nvmc_ob_state;
}

/*!
    \brief      configure option byte bank0 as backup of ENVM (API_ID(0x0058U))
                For independent functions that complete option byte configuration by calling nvmc_ob_obstat0_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  none
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_flash_bank0_backup_envm(void)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    nvmc_ob_state = nvmc_ob_obstat0_config(OB_ENVM_BANK0_BACKUP, NVMC_OBSTAT0_MDF_SWAP);

    return nvmc_ob_state;
}

/*!
    \brief      configure option byte bank1 as backup of ENVM (API_ID(0x0059U))
                For independent functions that complete option byte configuration by calling nvmc_ob_obstat0_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  none
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_flash_bank1_backup_envm(void)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    nvmc_ob_state = nvmc_ob_obstat0_config(OB_ENVM_BANK1_BACKUP, NVMC_OBSTAT0_MDF_SWAP);

    return nvmc_ob_state;
}

/*!
    \brief      enable ECC of appointed region (API_ID(0x005AU))
                For independent functions that complete option byte configuration by calling nvmc_ob_obstat0_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  ecc_type: ecc type
                only one parameter can be selected which is shown as below:
      \arg        ITCM_ECC: ITCM ECC function
      \arg        DTCM0_ECC: DTCM0 ECC region
      \arg        DTCM1_ECC: DTCM1 ECC region
      \arg        R2_SRAM_ECC: Region 2 SRAM ECC region
      \arg        R1_SRAM_ECC: Region 1 SRAM ECC region
      \arg        R0_SRAM_ECC: Region 0 SRAM ECC region
      \arg        ENVM_ECC: ENVM ECC region
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_ecc_enable(uint32_t ecc_type)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    switch(ecc_type) {
    /* enable ITCM ECC */
    case ITCM_ECC:
        nvmc_ob_state = nvmc_ob_obstat0_config(OB_ITCM_ECC_ENABLE, NVMC_OBSTAT0_MDF_ITCMECCEN);
        break;
    /* enable DTCM0 ECC */
    case DTCM0_ECC:
        nvmc_ob_state = nvmc_ob_obstat0_config(OB_DTCM0_ECC_ENABLE, NVMC_OBSTAT0_MDF_DTCM0ECCEN);
        break;
    /* enable DTCM1 ECC */
    case DTCM1_ECC:
        nvmc_ob_state = nvmc_ob_obstat0_config(OB_DTCM1_ECC_ENABLE, NVMC_OBSTAT0_MDF_DTCM1ECCEN);
        break;
    /* enable Region 2 SRAM ECC */
    case R2_SRAM_ECC:
        nvmc_ob_state = nvmc_ob_obstat0_config(OB_R2_SRAM_ECC_ENABLE, NVMC_OBSTAT0_MDF_R2_SRAM_ECCEN);
        break;
    /* enable Region 1 SRAM ECC */
    case R1_SRAM_ECC:
        nvmc_ob_state = nvmc_ob_obstat0_config(OB_R1_SRAM_ECC_ENABLE, NVMC_OBSTAT0_MDF_R1_SRAM_ECCEN);
        break;
    /* enable Region 0 SRAM ECC */
    case R0_SRAM_ECC:
        nvmc_ob_state = nvmc_ob_obstat0_config(OB_R0_SRAM_ECC_ENABLE, NVMC_OBSTAT0_MDF_R0_SRAM_ECCEN);
        break;
    /* enable ENVM ECC */
    case ENVM_ECC:
        nvmc_ob_state = nvmc_ob_obstat0_config(OB_ENVM_ECC_ENABLE, NVMC_OBSTAT0_MDF_ENVM_ECC_OVRD);
        break;
    default:
        /* should not be here */
        break;
    }

    return nvmc_ob_state;
}

/*!
    \brief      disable ECC of appointed region (API_ID(0x005BU))
                For independent functions that complete option byte configuration by calling nvmc_ob_obstat0_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  ecc_type: ecc type
                only one parameter can be selected which is shown as below:
      \arg        ITCM_ECC: ITCM ECC region
      \arg        DTCM0_ECC: DTCM0 ECC region
      \arg        DTCM1_ECC: DTCM1 ECC region
      \arg        R2_SRAM_ECC: Region 2 SRAM ECC region
      \arg        R1_SRAM_ECC: Region 1 SRAM ECC region
      \arg        R0_SRAM_ECC: Region 0 SRAM ECC region
      \arg        ENVM_ECC: ENVM ECC region
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_ecc_disable(uint32_t ecc_type)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    switch(ecc_type) {
    /* enable ITCM ECC */
    case ITCM_ECC:
        nvmc_ob_state = nvmc_ob_obstat0_config(OB_ITCM_ECC_DISABLE, NVMC_OBSTAT0_MDF_ITCMECCEN);
        break;
    /* enable DTCM0 ECC */
    case DTCM0_ECC:
        nvmc_ob_state = nvmc_ob_obstat0_config(OB_DTCM0_ECC_DISABLE, NVMC_OBSTAT0_MDF_DTCM0ECCEN);
        break;
    /* enable DTCM1 ECC */
    case DTCM1_ECC:
        nvmc_ob_state = nvmc_ob_obstat0_config(OB_DTCM1_ECC_DISABLE, NVMC_OBSTAT0_MDF_DTCM1ECCEN);
        break;
    /* enable Region 2 SRAM ECC */
    case R2_SRAM_ECC:
        nvmc_ob_state = nvmc_ob_obstat0_config(OB_R2_SRAM_ECC_DISABLE, NVMC_OBSTAT0_MDF_R2_SRAM_ECCEN);
        break;
    /* enable Region 1 SRAM ECC */
    case R1_SRAM_ECC:
        nvmc_ob_state = nvmc_ob_obstat0_config(OB_R1_SRAM_ECC_DISABLE, NVMC_OBSTAT0_MDF_R1_SRAM_ECCEN);
        break;
    /* enable Region 0 SRAM ECC */
    case R0_SRAM_ECC:
        nvmc_ob_state = nvmc_ob_obstat0_config(OB_R0_SRAM_ECC_DISABLE, NVMC_OBSTAT0_MDF_R0_SRAM_ECCEN);
        break;
    /* enable ENVM ECC */
    case ENVM_ECC:
        nvmc_ob_state = nvmc_ob_obstat0_config(OB_ENVM_ECC_CONTROL_BY_ENVM_ECC_MODE, NVMC_OBSTAT0_MDF_ENVM_ECC_OVRD);
        break;
    default:
        /* should not be here */
        break;
    }

    return nvmc_ob_state;
}

/*!
    \brief      enable pipeline mode (API_ID(0x005CU))
                For independent functions that complete option byte configuration by calling nvmc_ob_obstat0_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  none
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_pipemode_enable(void)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    nvmc_ob_state = nvmc_ob_obstat0_config(OB_PERFORMANCE_FRIENDLY_MODE, NVMC_OBSTAT0_MDF_NOPIPE_MODE);

    return nvmc_ob_state;
}

/*!
    \brief      disable pipeline mode (API_ID(0x005DU))
                For independent functions that complete option byte configuration by calling nvmc_ob_obstat0_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  none
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_pipemode_disable(void)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    nvmc_ob_state = nvmc_ob_obstat0_config(OB_TIMING_FRIENDLY_MODE, NVMC_OBSTAT0_MDF_NOPIPE_MODE);

    return nvmc_ob_state;
}

/*!
    \brief      program the option bytes OBSTAT1 (API_ID(0x005EU))
    \param[in]  ob_obstat1: obstat1 option bytes. When write one paramter, the corresponding mask should be set.
                one or more  parameters can be selected which are shown as below:
      \arg        OB_ITCM_DTCM_SHARED_ITCM_0KB/OB_ITCM_DTCM_SHARED_ITCM_64KB/OB_ITCM_DTCM_SHARED_ITCM_128KB/OB_ITCM_DTCM_SHARED_ITCM_192KB/OB_ITCM_DTCM_SHARED_ITCM_256KB: ITCM DTCM shared RAM ITCM size
      \arg        OB_ITCM_AXI_SHARED_ITCM_0KB/OB_ITCM_AXI_SHARED_ITCM_64KB/OB_ITCM_AXI_SHARED_ITCM_128KB/OB_ITCM_AXI_SHARED_ITCM_192KB/OB_ITCM_AXI_SHARED_ITCM_256KB: ITCM AXI shared RAM ITCM size
      \arg        OB_SYSCFG_CPU_BUS_TIMEOUT_RESET_DISABLE/OB_SYSCFG_CPU_BUS_TIMEOUT_RESET_ENABLE: disable/enable CPU bus timeout
    \param[in]  ob_obstat1_mask: obstat1 bits mask. They correspond to the above paramter value one by one.
                  one or more parameters can be selected which are shown as below:
      \arg        NVMC_OBSTAT1_MDF_ITCM_AXI_SHARED: ITCM and AXI RAM sharing status bit mask
      \arg        NVMC_OBSTAT1_MDF_ITCM_DTCM_SHARED: ITCM and DTCM RAM sharing status bit mask
      \arg        NVMC_OBSTAT1_MDF_CPUBUSTO: CPU bus timeout status bit mask
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_obstat1_config(uint32_t ob_obstat1, uint32_t ob_obstat1_mask)
{
    uint32_t obstat1_reg;
    uint32_t obstat0_eft_spc = NVMC_OBSTAT0_EFT & NVMC_OBSTAT0_EFT_SPC;
    uint32_t otp_user_ctl0_spc_h = NVMC_OTP_USER_CTL0 & NVMC_OTP_USER_CTL0_SPC_H;
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    /* check the option bytes security protection value */
    if((OB_HSPC == obstat0_eft_spc) || otp_user_ctl0_spc_h) {
        nvmc_ob_state = NVMC_HSPC;
    } else {
        /* wait for the nvmc ready */
        nvmc_ob_state = nvmc_ob_ready_wait(NVMC_TIMEOUT_COUNT);

        if(NVMC_OB_READY == nvmc_ob_state) {
            obstat1_reg = NVMC_OBSTAT1_EFT;

            NVMC_OBSTAT1_MDF = (obstat1_reg & (~ob_obstat1_mask)) | (ob_obstat1 & ob_obstat1_mask);
            /* set the OBSTART bit */
            NVMC_OBCTL |= NVMC_OBCTL_OBSTART;
            /* wait for the nvmc ready */
            nvmc_ob_state = nvmc_ob_ready_wait(NVMC_TIMEOUT_COUNT);
        }
    }
    return nvmc_ob_state;
}

/*!
    \brief      program the option bytes user data (API_ID(0x005FU))
                For independent functions that complete option byte configuration by calling nvmc_ob_obstat1_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  ob_data: user option bytes (0x00000000 - 0x00007FFF)
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_data_config(uint32_t user_data)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;
    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_OB_DATA_RANGE(user_data)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x005FU), ERR_PARAM_OUT_OF_RANGE);
    } else
#endif
    {
        nvmc_ob_state = nvmc_ob_obstat1_config(user_data << OBSTAT1_DATA_OFFSET, NVMC_OBSTAT1_MDF_DATA);
    }
    return nvmc_ob_state;
}

/*!
    \brief      configure the ITCM size in ITCM/DTCM shared RAM (API_ID(0x0060U))
                ITCM RAM + DTCM RAM size should not exceed 512KB
                For independent functions that complete option byte configuration by calling nvmc_ob_obstat1_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  itcm_shared_ram_size: ITCM shared RAM size
                only one parameter can be selected which is shown as below:
      \arg        OB_ITCM_DTCM_SHARED_ITCM_0KB: ITCM size is 0Kbyte in ITCM DTCM shared RAM
      \arg        OB_ITCM_DTCM_SHARED_ITCM_64KB: ITCM size is 64-Kbyte in ITCM DTCM shared RAM
      \arg        OB_ITCM_DTCM_SHARED_ITCM_128KB: ITCM size is 128-Kbyte in ITCM DTCM shared RAM
      \arg        OB_ITCM_DTCM_SHARED_ITCM_192KB: ITCM size is 192-Kbyte in ITCM DTCM shared RAM
      \arg        OB_ITCM_DTCM_SHARED_ITCM_256KB: ITCM size is 256-Kbyte in ITCM DTCM shared RAM
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_itcm_dtcm_shared_ram_config(uint32_t itcm_size)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;
    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_OB_ITCM_DTCM_SHARED_ITCM_SIZE(itcm_size)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x0060U), ERR_PARAM_OUT_OF_RANGE);
    } else
#endif
    {
        nvmc_ob_state = nvmc_ob_obstat1_config(itcm_size, NVMC_OBSTAT1_MDF_ITCM_DTCM_SHARED);
    }
    return nvmc_ob_state;
}

/*!
    \brief      configure the ITCM size in ITCM/AXI shared RAM (API_ID(0x0061U))
                ITCM RAM + DTCM RAM size should not exceed 512KB
                For independent functions that complete option byte configuration by calling nvmc_ob_obstat1_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  itcm_shared_ram_size: ITCM shared RAM size
                only one parameter can be selected which is shown as below:
      \arg        OB_ITCM_AXI_SHARED_ITCM_0KB: ITCM size is 0-Kbyte ITCM AXI shared RAM
      \arg        OB_ITCM_AXI_SHARED_ITCM_64KB: ITCM size is 64-Kbyte ITCM AXI shared RAM
      \arg        OB_ITCM_AXI_SHARED_ITCM_128KB: ITCM size is 128-KbyteITCM AXI shared RAM
      \arg        OB_ITCM_AXI_SHARED_ITCM_192KB: ITCM size is 192-KbyteITCM AXI shared RAM
      \arg        OB_ITCM_AXI_SHARED_ITCM_256KB: ITCM size is 256-KbyteITCM AXI shared RAM
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_itcm_axi_shared_ram_config(uint32_t itcm_size)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;
    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_OB_ITCM_AXI_SHARED_ITCM_SIZE(itcm_size)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x0061U), ERR_PARAM_OUT_OF_RANGE);
    } else
#endif
    {
        nvmc_ob_state = nvmc_ob_obstat1_config(itcm_size, NVMC_OBSTAT1_MDF_ITCM_AXI_SHARED);
    }
    return nvmc_ob_state;
}

/*!
    \brief      configure the reset value of the CPUBUSTO bit in SYSCFG_BUSTO (API_ID(0x0062U))
                For independent functions that complete option byte configuration by calling nvmc_ob_obstat1_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  cpubusto_value: the reset value of the CPUBUSTO bit
                only one parameter can be selected which is shown as below:
      \arg        OB_SYSCFG_CPU_BUS_TIMEOUT_RESET_DISABLE: disable CPU bus timeout
      \arg        OB_SYSCFG_CPU_BUS_TIMEOUT_RESET_ENABLE: enable CPU bus timeout
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_cpubusto_reset_value_config(uint32_t cpubusto_value)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;
    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_OB_CPUBUSTO_RESET_VALUE(cpubusto_value)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x0062U), ERR_PARAM_INVALID);
    } else
#endif
    {
        nvmc_ob_state = nvmc_ob_obstat1_config(cpubusto_value, NVMC_OBSTAT1_MDF_CPUBUSTO);
    }
    return nvmc_ob_state;
}

/*!
    \brief      configure the option byte boot address value (API_ID(0x0063U))
    \param[in]  boot_pin: boot pin configuration
                only one parameter can be selected which is shown as below:
      \arg        BOOT_PIN_0: boot pin value is '0'
      \arg        BOOT_PIN_1: boot pin value is '1'
    \param[in]  boot_address: specify the MSB of boot address
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_boot_address_config(uint32_t boot_pin, uint16_t boot_address)
{
    uint32_t btaddr_reg;
    uint32_t obstat0_eft_spc = NVMC_OBSTAT0_EFT & NVMC_OBSTAT0_EFT_SPC;
    uint32_t otp_user_ctl0_spc_h = NVMC_OTP_USER_CTL0 & NVMC_OTP_USER_CTL0_SPC_H;
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_NVMC_BOOT_PIN_VALUE(boot_pin)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x0063U), ERR_PARAM_INVALID);
    } else
#endif
    {
        /* check the option bytes security protection value */
        if((OB_HSPC == obstat0_eft_spc) || otp_user_ctl0_spc_h) {
            nvmc_ob_state = NVMC_HSPC;
        } else {
            /* wait for the nvmc ready */
            nvmc_ob_state = nvmc_ob_ready_wait(NVMC_TIMEOUT_COUNT);

            if(NVMC_OB_READY == nvmc_ob_state) {
                btaddr_reg = NVMC_BTADDR_EFT;

                if(BOOT_PIN_0 == boot_pin) {
                    /* set boot address 0 */
                    btaddr_reg &= ~NVMC_BTADDR_MDF_BOOT_ADDR0;
                    btaddr_reg |= (uint32_t)((uint32_t)boot_address << BTADDR_BOOT_ADDR0_OFFSET);
                } else {
                    /* set boot address 1 */
                    btaddr_reg &= ~NVMC_BTADDR_MDF_BOOT_ADDR1;
                    btaddr_reg |= (uint32_t)((uint32_t)boot_address << BTADDR_BOOT_ADDR1_OFFSET);
                }
                NVMC_BTADDR_MDF = btaddr_reg;

                NVMC_OBCTL |= NVMC_OBCTL_OBSTART;

                /* wait for the nvmc ready */
                nvmc_ob_state = nvmc_ob_ready_wait(NVMC_TIMEOUT_COUNT);
            }
        }
    }
    return nvmc_ob_state;
}

/*!
    \brief      configure the option byte DCRP area (API_ID(0x0064U))
    \param[in]  dcrp_en: DCRP area enable bit
                one or more parameters can be selected which are shown as below:
      \arg        OB_FL_BANK1_DCRP_AREA_DISABLE: FLB1 DCRP area disable
      \arg        OB_FL_BANK1_DCRP_AREA_ENABLE: FLB1 DCRP area enable
      \arg        OB_FL_BANK0_DCRP_AREA_DISABLE: FLB0 DCRP area disable
      \arg        OB_FL_BANK0_DCRP_AREA_ENABLE: FLB0 DCRP area enable
      \arg        OB_ENVM_DCRP_AREA_DISABLE: ENVM DCRP area disable
      \arg        OB_ENVM_DCRP_AREA_ENABLE: ENVM DCRP area enable
    \param[in]  dcrp_eren: DCRP area erase enable bit
                one or more parameters can be selected which are shown as below:
      \arg        OB_FL_BANK1_DCRP_AREA_ERASE_DISABLE: FLB1 DCRP area erase disable
      \arg        OB_FL_BANK1_DCRP_AREA_ERASE_ENABLE: FLB1 DCRP area erase enable
      \arg        OB_FL_BANK0_DCRP_AREA_ERASE_DISABLE: FLB0 DCRP area erase disable
      \arg        OB_FL_BANK0_DCRP_AREA_ERASE_ENABLE: FLB0 DCRP area erase enable
      \arg        OB_ENVM_DCRP_AREA_ERASE_DISABLE: ENVM DCRP area erase disable
      \arg        OB_ENVM_DCRP_AREA_ERASE_ENABLE: ENVM DCRP area erase enable
    \param[in]  dcrp_start: DCRP area start block number, contain the first 4K-byte block of the DCRP area.(case1 and case2: 0 - 0x1FF, case3: 0 - 0x0FF, case4: 0 - 0x19F)
    \param[in]  dcrp_end: DCRP area end block number, contain the last 4K-byte block of the DCRP area.(case1 and case2: 0 - 0x1FF, case3: 0 - 0x0FF, case4: 0 - 0x19F)
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_dcrp_area_config(uint32_t dcrp_en, uint32_t dcrp_eren, uint32_t dcrp_start, uint32_t dcrp_end)
{
    uint32_t obstat0_eft_spc = NVMC_OBSTAT0_EFT & NVMC_OBSTAT0_EFT_SPC;
    uint32_t otp_user_ctl0_spc_h = NVMC_OTP_USER_CTL0 & NVMC_OTP_USER_CTL0_SPC_H;
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_NVMC_DCRP_AREA_START(dcrp_start)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x0064U), ERR_PARAM_OUT_OF_RANGE);
    } else if(NOT_NVMC_DCRP_AREA_END(dcrp_end)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x0064U), ERR_PARAM_OUT_OF_RANGE);
    } else
#endif
    {
        /* check the option bytes security protection value */
        if((OB_HSPC == obstat0_eft_spc) || otp_user_ctl0_spc_h) {
            nvmc_ob_state = NVMC_HSPC;
        } else {

            /* wait for the nvmc ready */
            nvmc_ob_state = nvmc_ob_ready_wait(NVMC_TIMEOUT_COUNT);

            if(NVMC_OB_READY == nvmc_ob_state) {
                NVMC_DCRPADDR_MDF = (dcrp_eren & DCRP_EREN_MASK) | (dcrp_en & DCRP_EN_MASK) | (dcrp_end << DCRPADDR_DCRP_AREA_END_OFFSET) | dcrp_start;
                /* set the OBSTART bit */
                NVMC_OBCTL |= NVMC_OBCTL_OBSTART;
                /* wait for the nvmc ready */
                nvmc_ob_state = nvmc_ob_ready_wait(NVMC_TIMEOUT_COUNT);
            }
        }
    }
    return nvmc_ob_state;
}

/*!
    \brief      configure the option byte secure area (API_ID(0x0065U))
    \param[in]  scr_en: DCRP area enable bit
                one or more parameters can be selected which are shown as below:
      \arg        OB_FL_BANK1_SECURE_AREA_DISABLE: FLB1 SCR area disable
      \arg        OB_FL_BANK1_SECURE_AREA_ENABLE: FLB1 SCR area enable
      \arg        OB_FL_BANK0_SECURE_AREA_DISABLE: FLB0 SCR area disable
      \arg        OB_FL_BANK0_SECURE_AREA_ENABLE: FLB0 SCR area enable
      \arg        OB_ENVM_SECURE_AREA_DISABLE: ENVM SCR area disable
      \arg        OB_ENVM_SECURE_AREA_ENABLE: ENVM SCR area enable
    \param[in]  scr_eren: DCRP area erase enable bit
                one or more parameters can be selected which are shown as below:
      \arg        OB_FL_BANK1_SECURE_AREA_ERASE_DISABLE: FLB1 SCR area erase disable
      \arg        OB_FL_BANK1_SECURE_AREA_ERASE_ENABLE: FLB1 SCR area erase enable
      \arg        OB_FL_BANK0_SECURE_AREA_ERASE_DISABLE: FLB0 SCR area erase disable
      \arg        OB_FL_BANK0_SECURE_AREA_ERASE_ENABLE: FLB0 SCR area erase enable
      \arg        OB_ENVM_SECURE_AREA_ERASE_DISABLE: ENVM SCR area erase disable
      \arg        OB_ENVM_SECURE_AREA_ERASE_ENABLE: ENVM SCR area erase enable
    \param[in]  scr_start: secure area start block number, contain the first 4K-byte block of the secure-access area.(case1 and case2: 0 - 0x1FF, case3: 0 - 0x0FF, case4: 0 - 0x19F)
    \param[in]  scr_end: secure area end block number, contain the last 4K-byte block of the secure-access area.(case1 and case2: 0 - 0x1FF, case3: 0 - 0x0FF, case4: 0 - 0x19F)
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_secure_area_config(uint32_t scr_en, uint32_t scr_eren, uint32_t scr_start, uint32_t scr_end)
{
    uint32_t obstat0_eft_spc = NVMC_OBSTAT0_EFT & NVMC_OBSTAT0_EFT_SPC;
    uint32_t otp_user_ctl0_spc_h = NVMC_OTP_USER_CTL0 & NVMC_OTP_USER_CTL0_SPC_H;
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;
    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_NVMC_SCR_AREA_START(scr_start)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x0065U), ERR_PARAM_OUT_OF_RANGE);
    } else if(NOT_NVMC_SCR_AREA_END(scr_end)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x0065U), ERR_PARAM_OUT_OF_RANGE);
    } else
#endif
    {
        /* check the option bytes security protection value */
        if((OB_HSPC == obstat0_eft_spc) || otp_user_ctl0_spc_h) {
            nvmc_ob_state = NVMC_HSPC;
        } else {
            /* wait for the nvmc ready */
            nvmc_ob_state = nvmc_ob_ready_wait(NVMC_TIMEOUT_COUNT);

            if(NVMC_OB_READY == nvmc_ob_state) {
                NVMC_SCRADDR_MDF = (scr_eren & SCR_EREN_MASK) | (scr_en & SCR_EN_MASK) | (scr_end << SCRADDR_SCR_AREA_END_OFFSET) | scr_start;

                NVMC_OBCTL |= NVMC_OBCTL_OBSTART;
                /* wait for the nvmc ready */
                nvmc_ob_state = nvmc_ob_ready_wait(NVMC_TIMEOUT_COUNT);
            }
        }
    }
    return nvmc_ob_state;
}

/*!
    \brief      enable erase/program protection (API_ID(0x0066U))
    \param[in]  wp_area: erase/program protection area, when case4 is selected, only WP_AREA_ENVM can be selected, WP_AREA_FLASH can not be selected.
                only one parameter can be selected which is shown as below:
      \arg        WP_AREA_FLASH: main flash erase/program protection area (in case1, case2 and case3, WP_AREA_FLASH can be selected, in case4, WP_AREA_FLASH can not be selected)
      \arg        WP_AREA_ENVM: ENVM erase/program protection area (in case1, case2, case3 and case4, WP_AREA_ENVM can be selected)
    \param[in]  ob_wp: specify sector to be erase/program protected, when wp_area = WP_AREA_ENVM, only OB_ENVM_WP_x and OB_ENVM_WP_ALL can be selected
                       when wp_area = WP_AREA_FLASH, only OB_FL_WP_x and OB_FL_WP_ALL can be selected
                one or more parameters can be selected which are shown as below:
      \arg        OB_ENVM_WP_x(when wp_area = WP_AREA_ENVM): write protect sector (x*2) ~ sector (x*2+1)(case1 and case2: x = 0,1...15, case3: x = 0,1...7, case4: x = 0,1...12) of ENVM
      \arg        OB_ENVM_WP_ALL(when wp_area = WP_AREA_ENVM): write protect all sectors of ENVM
      \arg        OB_FL_WP_x(when wp_area = WP_AREA_FLASH): write protect sector (x*2 + 32) ~ sector (x*2+33)(case1: x = 0,1...59, case2 and case3: x = 0,1...27, case4: OB_FL_WP_x is invalid) of main flash
      \arg        OB_FL_WP_ALL(when wp_area = WP_AREA_FLASH): write protect all sectors of main flash
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_write_protection_enable(wp_area_enum wp_area, uint64_t ob_wp)
{
    uint32_t wp_reg;
    uint32_t obstat0_eft_spc = NVMC_OBSTAT0_EFT & NVMC_OBSTAT0_EFT_SPC;
    uint32_t otp_user_ctl0_spc_h = NVMC_OTP_USER_CTL0 & NVMC_OTP_USER_CTL0_SPC_H;
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    /* check the option bytes security protection value */
    if((OB_HSPC == obstat0_eft_spc) || otp_user_ctl0_spc_h) {
        nvmc_ob_state = NVMC_HSPC;
    } else {
        /* wait for the nvmc ready */
        nvmc_ob_state = nvmc_ob_ready_wait(NVMC_TIMEOUT_COUNT);

        if(NVMC_OB_READY == nvmc_ob_state) {
#ifndef NVM_VER_CASE4
            if(WP_AREA_FLASH == wp_area) {
                /* configure the enable erase/program protection bit in NVMC_WP0_FLASH_EFT register */
                wp_reg = NVMC_WP0_FLASH_EFT;
                wp_reg &= ~(uint32_t)(ob_wp & NVMC_WP0_FLASH_EFT_WP);
                NVMC_WP0_FLASH_MDF = wp_reg;

#if defined(NVM_VER_CASE1)
                /* configure the enable erase/program protection bit in NVMC_WP1_FLASH_EFT register */
                wp_reg = NVMC_WP1_FLASH_EFT;
                wp_reg &= ~(uint32_t)((ob_wp >> WP1_FLASH_OFFSET) & (uint32_t)(OB_FL_WP_ALL >> WP1_FLASH_OFFSET));
                NVMC_WP1_FLASH_MDF = wp_reg;
#endif /* NVM_VER_CASE1 */
                /* set the OBSTART bit */
                NVMC_OBCTL |= NVMC_OBCTL_OBSTART;

            } else
#endif /* NVM_VER_CASE4 */
            {
                if(WP_AREA_ENVM == wp_area) {
                    /* configure the enable erase/program protection bit in NVMC_WP_ENVM_MDF register */
                    wp_reg = NVMC_WP_ENVM_EFT;
                    wp_reg &= ~((uint32_t)ob_wp & OB_ENVM_WP_ALL);
                    NVMC_WP_ENVM_MDF = wp_reg;
                    /* set the OBSTART bit */
                    NVMC_OBCTL |= NVMC_OBCTL_OBSTART;
                } else {
                    /* illegal parameters */
                }
            }

            /* wait for the nvmc ready */
            nvmc_ob_state = nvmc_ob_ready_wait(NVMC_TIMEOUT_COUNT);
        }
    }

    return nvmc_ob_state;
}

/*!
    \brief      disable erase/program protection (API_ID(0x0067U))
    \param[in]  wp_area: erase/program protection area, when case4 is selected, only WP_AREA_ENVM can be selected, WP_AREA_FLASH can not be selected.
                only one parameter can be selected which is shown as below:
      \arg        WP_AREA_FLASH: main flash erase/program protection area (in case1, case2 and case3, WP_AREA_FLASH can be selected, in case4, WP_AREA_FLASH can not be selected)
      \arg        WP_AREA_ENVM: ENVM erase/program protection area (in case1, case2, case3 and case4, WP_AREA_ENVM can be selected)
    \param[in]  ob_wp: specify sector to be erase/program protected, when wp_area = WP_AREA_ENVM, only OB_ENVM_WP_x and OB_ENVM_WP_ALL can be selected
                       when wp_area = WP_AREA_FLASH, only OB_FL_WP_x and OB_FL_WP_ALL can be selected
                one or more parameters can be selected which are shown as below:
      \arg        OB_ENVM_WP_x(when wp_area = WP_AREA_ENVM): write protect sector (x*2) ~ sector (x*2+1)(case1 and case2: x = 0,1...15, case3: x = 0,1...7, case4: x = 0,1...12) of ENVM
      \arg        OB_ENVM_WP_ALL(when wp_area = WP_AREA_ENVM): write protect all sectors of ENVM
      \arg        OB_FL_WP_x(when wp_area = WP_AREA_FLASH): write protect sector (x*2 + 32) ~ sector (x*2+33)(case1: x = 0,1...59, case2 and case3: x = 0,1...27, case4: OB_FL_WP_x is invalid) of main flash
      \arg        OB_FL_WP_ALL(when wp_area = WP_AREA_FLASH): write protect all sectors of main flash
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_write_protection_disable(wp_area_enum wp_area, uint64_t ob_wp)
{
    uint32_t wp_reg;
    uint32_t obstat0_eft_spc = NVMC_OBSTAT0_EFT & NVMC_OBSTAT0_EFT_SPC;
    uint32_t otp_user_ctl0_spc_h = NVMC_OTP_USER_CTL0 & NVMC_OTP_USER_CTL0_SPC_H;
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    /* check the option bytes security protection value */
    if((OB_HSPC == obstat0_eft_spc) || otp_user_ctl0_spc_h) {
        nvmc_ob_state = NVMC_HSPC;
    } else {
        /* wait for the nvmc ready */
        nvmc_ob_state = nvmc_ob_ready_wait(NVMC_TIMEOUT_COUNT);

        if(NVMC_OB_READY == nvmc_ob_state) {
#ifndef NVM_VER_CASE4
            if(WP_AREA_FLASH == wp_area) {
                /* configure the disable erase/program protection bit in NVMC_WP0_FLASH_EFT register */
                wp_reg = NVMC_WP0_FLASH_EFT;
                wp_reg |= (uint32_t)(ob_wp & NVMC_WP0_FLASH_EFT_WP);
                NVMC_WP0_FLASH_MDF = wp_reg;

#if defined(NVM_VER_CASE1)
                /* configure the disable erase/program protection bit in NVMC_WP1_FLASH_EFT register */
                wp_reg = NVMC_WP1_FLASH_EFT;
                wp_reg |= (uint32_t)((ob_wp >> WP1_FLASH_OFFSET) & (uint32_t)(OB_FL_WP_ALL >> WP1_FLASH_OFFSET));
                NVMC_WP1_FLASH_MDF = wp_reg;
#endif /* NVM_VER_CASE1 */
                /* set the OBSTART bit */
                NVMC_OBCTL |= NVMC_OBCTL_OBSTART;
            } else
#endif /* NVM_VER_CASE4 */
            {
                if(WP_AREA_ENVM == wp_area) {
                    /* configure the disable erase/program protection bit in NVMC_WP_ENVM_MDF register */
                    wp_reg = NVMC_WP_ENVM_EFT;
                    wp_reg |= (uint32_t)ob_wp & OB_ENVM_WP_ALL;
                    NVMC_WP_ENVM_MDF = wp_reg;
                    /* set the OBSTART bit */
                    NVMC_OBCTL |= NVMC_OBCTL_OBSTART;
                } else {
                    /* illegal parameters */
                }
            }

            /* wait for the nvmc ready */
            nvmc_ob_state = nvmc_ob_ready_wait(NVMC_TIMEOUT_COUNT);
        }
    }
    return nvmc_ob_state;
}

/*!
    \brief      program the option bytes OBREPCTL (API_ID(0x0068U))
    \param[in]  ob_repctl: repair control option bytes. When write one paramter, the corresponding mask should be set.
                one or more parameters can be selected which are shown as below:
      \arg        POR_RESET_REPAIR_DISABLE/POR_RESET_REPAIR_ENABLE: disable/enable power-on reset self-check repair function
      \arg        SYS_RESET_REPAIR_DISABLE/SYS_RESET_REPAIR_ENABLE: disable/enable system reset self-check repair function
      \arg        REPAIR_FAIL_FOR_ANY_ERROR/REPAIR_FAIL_FOR_MORE_THAN_ERR1: After repair, any error occurs(errors greater than 1 bit) as repair failure
      \arg        ECC2_AUTO_REPAIR_DISABLE/ECC2_AUTO_REPAIR_ENABLE: disable/enable 2 bits error automatic repair
      \arg        ECC1_AUTO_REPAIR_DISABLE/ECC1_AUTO_REPAIR_ENABLE: disable/enable 1 bit error automatic repair
      \arg        BAD_ENVM_ADDR_REPLACE_DISABLE/BAD_ENVM_ADDR_REPLACE_ENABLE: disable/enable bad data replacement in ENVM with flash content
      \arg        TWO_BIT_ERR_REPAIR_DISABLE/TWO_BIT_ERR_REPAIR_ENABLE: disable/enable fix address with two bit error
      \arg        ONE_BIT_ERR_REPAIR_DISABLE/ONE_BIT_ERR_REPAIR_ENABLE: disable/enable fix address with one bit error
      \arg        BOOTLOADER_CHECK_DISABLE/BOOTLOADER_CHECK_ENABLE: disable/enable bootloader self-check
      \arg        OB_HW_SELF_CHECK_DISABLE/OB_HW_SELF_CHECK_ENABLE: hardware not do self-check mode/hardware self-check mode
      \arg        OB_SYNC_MODE_DISABLE/OB_SYNC_MODE_ENABLE: disable/enable synchronization mode
    \param[in]  ob_repctl_mask: repair control bits mask. They correspond to the above paramter value one by one.
                  one or more parameters can be selected which are shown as below:
      \arg        NVMC_OBREPCTL_MDF_PORRSTREPDIS: The power-on reset self-check repair function enable status bit mask
      \arg        NVMC_OBREPCTL_MDF_SYSRSTREPDIS: The system reset self-check repair function enable status bit mask
      \arg        NVMC_OBREPCTL_MDF_BBMODE: Mode of determining whether the repair is successful after the repair bit mask
      \arg        NVMC_OBREPCTL_MDF_ERR2AUTOREPEN: 2 bits error automatic repair enable status bit mask
      \arg        NVMC_OBREPCTL_MDF_ERR1AUTOREPEN: 1 bit error automatic repair enable status bit mask
      \arg        NVMC_OBREPCTL_MDF_BBREPLEN: Bad block address replace status bit mask
      \arg        NVMC_OBREPCTL_MDF_ERR2REPEN: Two bit error repair enable status bit mask
      \arg        NVMC_OBREPCTL_MDF_ERR1REPEN: One bit error repair enable status bit mask
      \arg        NVMC_OBREPCTL_MDF_BLREPMOD: Bootloader self-check mode status bit mask
      \arg        NVMC_OBREPCTL_MDF_HWREPMOD: Hardware self-check mode status bit mask
      \arg        NVMC_OBREPCTL_MDF_SYNEN: The synchronization mode enable status bit mask
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_repair_control_config(uint32_t ob_repctl, uint32_t ob_repctl_mask)
{
    uint32_t obrepctl_reg;
    uint32_t obstat0_eft_spc = NVMC_OBSTAT0_EFT & NVMC_OBSTAT0_EFT_SPC;
    uint32_t otp_user_ctl0_spc_h = NVMC_OTP_USER_CTL0 & NVMC_OTP_USER_CTL0_SPC_H;
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    /* check the option bytes security protection value */
    if((OB_HSPC == obstat0_eft_spc) || otp_user_ctl0_spc_h) {
        nvmc_ob_state = NVMC_HSPC;
    } else {
        /* wait for the nvmc ready */
        nvmc_ob_state = nvmc_ob_ready_wait(NVMC_TIMEOUT_COUNT);

        if(NVMC_OB_READY == nvmc_ob_state) {
            obrepctl_reg = NVMC_OBREPCTL_EFT;

            NVMC_OBREPCTL_MDF = (obrepctl_reg & (~ob_repctl_mask)) | (ob_repctl & ob_repctl_mask);
            NVMC_OBCTL |= NVMC_OBCTL_OBSTART;
            /* wait for the nvmc ready */
            nvmc_ob_state = nvmc_ob_ready_wait(NVMC_TIMEOUT_COUNT);
        }
    }
    return nvmc_ob_state;
}

/*!
    \brief      configure copy size during OTA update (API_ID(0x0069U))
                For independent functions that complete option byte configuration by calling nvmc_ob_repair_control_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  value: copy size during OTA update with granularity 32KB. (value:0x00 - 0x3F, copy size = (value + 1) * 32KB)
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_copysz_config(uint32_t value)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;
    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_NVMC_OB_COPY_SIZE(value)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x0069U), ERR_PARAM_OUT_OF_RANGE);
    } else
#endif
    {
        nvmc_ob_state = nvmc_ob_repair_control_config(value << OBREPCTL_COPYSZ_OFFSET, NVMC_OBREPCTL_MDF_COPYSZ);
    }
    return nvmc_ob_state;
}

/*!
    \brief      configure two bit error counter interrupt threshold during self-check (API_ID(0x006AU))
                For independent functions that complete option byte configuration by calling nvmc_ob_repair_control_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  err2_cnt_threshold: two bit error counter threshold value.(0x00 - 0x1F)
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_ecc2cntth_config(uint32_t err2_cnt_threshold)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;
    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_NVMC_OB_ERR2CNT_THRESHOLD(err2_cnt_threshold)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x006AU), ERR_PARAM_OUT_OF_RANGE);
    } else
#endif
    {
        nvmc_ob_state = nvmc_ob_repair_control_config(err2_cnt_threshold << OBREPCTL_ERR2CNTTH_OFFSET, NVMC_OBREPCTL_MDF_ERR2CNTTH);
    }
    return nvmc_ob_state;
}

/*!
    \brief      configure one bit error counter interrupt threshold during self-check (API_ID(0x006BU))
                For independent functions that complete option byte configuration by calling nvmc_ob_repair_control_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  err1_cnt_threshold: one bit error counter threshold value. (0x00 - 0x1F)
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_ecc1cntth_config(uint32_t err1_cnt_threshold)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;
    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_NVMC_OB_ERR1CNT_THRESHOLD(err1_cnt_threshold)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x006BU), ERR_PARAM_OUT_OF_RANGE);
    } else
#endif
    {
        nvmc_ob_state = nvmc_ob_repair_control_config(err1_cnt_threshold << OBREPCTL_ERR1CNTTH_OFFSET, NVMC_OBREPCTL_MDF_ERR1CNTTH);
    }
    return nvmc_ob_state;
}

/*!
    \brief      configure sync area size (API_ID(0x006CU))
                For independent functions that complete option byte configuration by calling nvmc_ob_repair_control_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  value: sync area size with granularity 64KB. (value: 0x00 - 0x1F, sync area size = (value + 1) * 64KB)
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_sync_area_size_config(uint32_t value)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;
    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_NVMC_OB_SYNC_SIZE(value)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x006CU), ERR_PARAM_OUT_OF_RANGE);
    } else
#endif
    {
        nvmc_ob_state = nvmc_ob_repair_control_config(value << OBREPCTL_SYNSZ_OFFSET, NVMC_OBREPCTL_MDF_SYNSZ);
    }
    return nvmc_ob_state;
}

/*!
    \brief      enable power-on reset self-check repair (API_ID(0x006DU))
                For independent functions that complete option byte configuration by calling nvmc_ob_repair_control_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  none
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_power_on_self_check_enable(void)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    nvmc_ob_state = nvmc_ob_repair_control_config(POR_RESET_REPAIR_ENABLE, NVMC_OBREPCTL_MDF_PORRSTREPDIS);

    return nvmc_ob_state;
}

/*!
    \brief      disable power-on reset self-check repair (API_ID(0x006EU))
                For independent functions that complete option byte configuration by calling nvmc_ob_repair_control_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  none
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_power_on_self_check_disable(void)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    nvmc_ob_state = nvmc_ob_repair_control_config(POR_RESET_REPAIR_DISABLE, NVMC_OBREPCTL_MDF_PORRSTREPDIS);

    return nvmc_ob_state;
}

/*!
    \brief      enable system reset self-check repair (API_ID(0x006FU))
                For independent functions that complete option byte configuration by calling nvmc_ob_repair_control_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  none
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_system_reset_check_enable(void)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    nvmc_ob_state = nvmc_ob_repair_control_config(SYS_RESET_REPAIR_ENABLE, NVMC_OBREPCTL_MDF_SYSRSTREPDIS);

    return nvmc_ob_state;
}

/*!
    \brief      disable system reset self-check repair (API_ID(0x0070U))
                For independent functions that complete option byte configuration by calling nvmc_ob_repair_control_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  none
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_system_reset_check_disable(void)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    nvmc_ob_state = nvmc_ob_repair_control_config(SYS_RESET_REPAIR_DISABLE, NVMC_OBREPCTL_MDF_SYSRSTREPDIS);

    return nvmc_ob_state;
}

/*!
    \brief      enable 2 bits error automatic repair (API_ID(0x0071U))
                For independent functions that complete option byte configuration by calling nvmc_ob_repair_control_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  none
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_ecc2_auto_repair_enable(void)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    nvmc_ob_state = nvmc_ob_repair_control_config(ECC2_AUTO_REPAIR_ENABLE, NVMC_OBREPCTL_MDF_ERR2AUTOREPEN);

    return nvmc_ob_state;
}

/*!
    \brief      disable 2 bits error automatic repair (API_ID(0x0072U))
                For independent functions that complete option byte configuration by calling nvmc_ob_repair_control_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  none
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_ecc2_auto_repair_disable(void)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    nvmc_ob_state = nvmc_ob_repair_control_config(ECC2_AUTO_REPAIR_DISABLE, NVMC_OBREPCTL_MDF_ERR2AUTOREPEN);

    return nvmc_ob_state;
}

/*!
    \brief      enable 1 bit error automatic repair (API_ID(0x0073U))
                For independent functions that complete option byte configuration by calling nvmc_ob_repair_control_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  none
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_ecc1_auto_repair_enable(void)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    nvmc_ob_state = nvmc_ob_repair_control_config(ECC1_AUTO_REPAIR_ENABLE, NVMC_OBREPCTL_MDF_ERR1AUTOREPEN);

    return nvmc_ob_state;
}

/*!
    \brief      disable 1 bit error automatic repair (API_ID(0x0074U))
                For independent functions that complete option byte configuration by calling nvmc_ob_repair_control_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  none
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_ecc1_auto_repair_disable(void)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    nvmc_ob_state = nvmc_ob_repair_control_config(ECC1_AUTO_REPAIR_DISABLE, NVMC_OBREPCTL_MDF_ERR1AUTOREPEN);

    return nvmc_ob_state;
}

/*!
    \brief      enable bad data replacement in ENVM with flash content (API_ID(0x0075U))
                For independent functions that complete option byte configuration by calling nvmc_ob_repair_control_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  none
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_envm_bad_data_replace_enable(void)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    nvmc_ob_state = nvmc_ob_repair_control_config(BAD_ENVM_ADDR_REPLACE_ENABLE, NVMC_OBREPCTL_MDF_BBREPLEN);

    return nvmc_ob_state;
}

/*!
    \brief      disable bad data replacement in ENVM with flash content (API_ID(0x0076U))
                For independent functions that complete option byte configuration by calling nvmc_ob_repair_control_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  none
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_envm_bad_data_replace_disable(void)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    nvmc_ob_state = nvmc_ob_repair_control_config(BAD_ENVM_ADDR_REPLACE_DISABLE, NVMC_OBREPCTL_MDF_BBREPLEN);

    return nvmc_ob_state;
}

/*!
    \brief      enable repair data with two bits error during self check (API_ID(0x0077U))
                For independent functions that complete option byte configuration by calling nvmc_ob_repair_control_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  none
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_two_bits_error_repair_enable(void)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    nvmc_ob_state = nvmc_ob_repair_control_config(TWO_BIT_ERR_REPAIR_ENABLE, NVMC_OBREPCTL_MDF_ERR2REPEN);

    return nvmc_ob_state;
}

/*!
    \brief      disable repair data with two bits error during self check (API_ID(0x0078U))
                For independent functions that complete option byte configuration by calling nvmc_ob_repair_control_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  none
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_two_bits_error_repair_disable(void)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    nvmc_ob_state = nvmc_ob_repair_control_config(TWO_BIT_ERR_REPAIR_DISABLE, NVMC_OBREPCTL_MDF_ERR2REPEN);

    return nvmc_ob_state;
}

/*!
    \brief      enable repair data with one bit error during self check (API_ID(0x0079U))
                For independent functions that complete option byte configuration by calling nvmc_ob_repair_control_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  none
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_one_bit_error_repair_enable(void)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    nvmc_ob_state = nvmc_ob_repair_control_config(ONE_BIT_ERR_REPAIR_ENABLE, NVMC_OBREPCTL_MDF_ERR1REPEN);

    return nvmc_ob_state;
}

/*!
    \brief      disable repair data with one bit error during self check (API_ID(0x007AU))
                For independent functions that complete option byte configuration by calling nvmc_ob_repair_control_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  none
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_one_bit_error_repair_disable(void)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    nvmc_ob_state = nvmc_ob_repair_control_config(ONE_BIT_ERR_REPAIR_DISABLE, NVMC_OBREPCTL_MDF_ERR1REPEN);

    return nvmc_ob_state;
}

/*!
    \brief      enable bootloader self-check (API_ID(0x007BU))
                For independent functions that complete option byte configuration by calling nvmc_ob_repair_control_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  none
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_bootloader_self_check_enable(void)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    nvmc_ob_state = nvmc_ob_repair_control_config(BOOTLOADER_CHECK_ENABLE, NVMC_OBREPCTL_MDF_BLREPMOD);

    return nvmc_ob_state;
}

/*!
    \brief      disable bootloader self-check (API_ID(0x0007CU))
                For independent functions that complete option byte configuration by calling nvmc_ob_repair_control_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  none
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_bootloader_self_check_disable(void)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    nvmc_ob_state = nvmc_ob_repair_control_config(BOOTLOADER_CHECK_DISABLE, NVMC_OBREPCTL_MDF_BLREPMOD);

    return nvmc_ob_state;
}

/*!
    \brief      enable synchronization mode (API_ID(0x007DU))
                For independent functions that complete option byte configuration by calling nvmc_ob_repair_control_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  none
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_sync_mode_enable(void)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    nvmc_ob_state = nvmc_ob_repair_control_config(OB_SYNC_MODE_ENABLE, NVMC_OBREPCTL_MDF_SYNEN);

    return nvmc_ob_state;
}

/*!
    \brief      disable synchronization mode (API_ID(0x007EU))
                For independent functions that complete option byte configuration by calling nvmc_ob_repair_control_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  none
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_sync_mode_disable(void)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    nvmc_ob_state = nvmc_ob_repair_control_config(OB_SYNC_MODE_DISABLE, NVMC_OBREPCTL_MDF_SYNEN);

    return nvmc_ob_state;
}

/*!
    \brief      enable hardware self-check mode (API_ID(0x007FU))
                For independent functions that complete option byte configuration by calling nvmc_ob_repair_control_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  none
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_hardware_self_check_enable(void)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    nvmc_ob_state = nvmc_ob_repair_control_config(OB_HW_SELF_CHECK_ENABLE, NVMC_OBREPCTL_MDF_HWREPMOD);

    return nvmc_ob_state;
}

/*!
    \brief      disable hardware self-check mode (API_ID(0x0080U))
                For independent functions that complete option byte configuration by calling nvmc_ob_repair_control_config, only one such independent function 
                can be called at a time to complete the reset and activation of the option byte configuration. When multiple independent functions are called,
                only the configuration of the option byte by the last independent function takes effect after the reset.
    \param[in]  none
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_hardware_self_check_disable(void)
{
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;

    nvmc_ob_state = nvmc_ob_repair_control_config(OB_HW_SELF_CHECK_DISABLE, NVMC_OBREPCTL_MDF_HWREPMOD);

    return nvmc_ob_state;
}

/*!
    \brief      configure repair address (API_ID(0x0081U))
    \param[in]  repair_start: self-check repair start offset address(case1 and case2: 0 - 0xFFFF, case3: 0 - 0x7FFF, case4: 0 - 0xCFFF, granularity is 256bit)
    \param[in]  repair_end: self-check repair end offset address(case1 and case2: 0 - 0xFFFF, case3: 0 - 0x7FFF, case4: 0 - 0xCFFF, granularity is 256bit)
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_repair_address_config(uint32_t repair_start, uint32_t repair_end)
{
    uint32_t obstat0_eft_spc = NVMC_OBSTAT0_EFT & NVMC_OBSTAT0_EFT_SPC;
    uint32_t otp_user_ctl0_spc_h = NVMC_OTP_USER_CTL0 & NVMC_OTP_USER_CTL0_SPC_H;
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;
    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_NVMC_REPAIR_AREA_START(repair_start)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x0081U), ERR_PARAM_OUT_OF_RANGE);
    } else if(NOT_NVMC_REPAIR_AREA_END(repair_end)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x0081U), ERR_PARAM_OUT_OF_RANGE);
    } else
#endif
    {
        /* check the option bytes security protection value */
        if((OB_HSPC == obstat0_eft_spc) || otp_user_ctl0_spc_h) {
            nvmc_ob_state = NVMC_HSPC;
        } else {
            /* wait for the nvmc ready */
            nvmc_ob_state = nvmc_ob_ready_wait(NVMC_TIMEOUT_COUNT);

            if(NVMC_OB_READY == nvmc_ob_state) {
                NVMC_OBREPSTADDR_MDF = (repair_start << OBREPSTADDR_STARTADDR_OFFSET) & NVMC_OBREPSTADDR_MDF_STARTADDR;
                NVMC_OBREPENDADDR_MDF = (repair_end << OBREPSTADDR_ENDADDR_OFFSET) & NVMC_OBREPENDADDR_MDF_ENDADDR;
                /* set the OBSTART bit */
                NVMC_OBCTL |= NVMC_OBCTL_OBSTART;
                /* wait for the nvmc ready */
                nvmc_ob_state = nvmc_ob_ready_wait(NVMC_TIMEOUT_COUNT);
            }
        }
    }
    return nvmc_ob_state;
}

/*!
    \brief      configure bad block address (API_ID(0x0082U))
    \param[in]  index: bad block index in NVMC_OBBBADDRX_MDF (index = 0 - 3)
    \param[in]  bad_block_valid: the bad block addres valid status (0: invalid, 1: valid)
    \param[in]  bad_block_address: bad block address (case1 and case2: 0 - 0xFFFF, case3: 0 - 0x7FFF, case4: 0 - 0xCFFF, granularity is 256bit)
    \param[out] none
    \retval     state of option byte modification
      \arg        NVMC_OB_READY: NVMC operation has been completed
      \arg        NVMC_OB_BUSY: NVMC operation is in progress
      \arg        NVMC_OB_OBMERR: option byte modified error
      \arg        NVMC_OB_TOERR: option byte modified timeout error
      \arg        NVMC_HSPC: high security protection
*/
nvmc_ob_state_enum nvmc_ob_bad_block_address_config(uint32_t index, uint32_t bad_block_valid, uint32_t bad_block_address)
{
    uint32_t obstat0_eft_spc = NVMC_OBSTAT0_EFT & NVMC_OBSTAT0_EFT_SPC;
    uint32_t otp_user_ctl0_spc_h = NVMC_OTP_USER_CTL0 & NVMC_OTP_USER_CTL0_SPC_H;
    nvmc_ob_state_enum nvmc_ob_state = NVMC_OB_READY;
    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_NVMC_BAD_BLOCK_INDEX(index)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x0082U), ERR_PARAM_OUT_OF_RANGE);
    } else if(NOT_NVMC_BAD_BLOCK_VALID(bad_block_valid)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x0082U), ERR_PARAM_INVALID);
    } else if(NOT_NVMC_BAD_BLOCK_ADDR(bad_block_address)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x0082U), ERR_PARAM_OUT_OF_RANGE);
    } else
#endif
    {
        /* check the option bytes security protection value */
        if((OB_HSPC == obstat0_eft_spc) || otp_user_ctl0_spc_h) {
            nvmc_ob_state = NVMC_HSPC;
        } else {
            /* wait for the nvmc ready */
            nvmc_ob_state = nvmc_ob_ready_wait(NVMC_TIMEOUT_COUNT);

            if(NVMC_OB_READY == nvmc_ob_state) {
                NVMC_OBBBADDRX_MDF(index) = (bad_block_valid << OBBBADDRX_OBBBADDRVLD_OFFSET) | (bad_block_address << OBBBADDRX_OBBBADDR_OFFSET);
                NVMC_OBCTL |= NVMC_OBCTL_OBSTART;

                /* wait for the nvmc ready */
                nvmc_ob_state = nvmc_ob_ready_wait(NVMC_TIMEOUT_COUNT);
            }
        }
    }
    return nvmc_ob_state;
}

/*!
    \brief      get option byte hardware watchdog configuration (API_ID(0x0083U))
    \param[in]  none
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus nvmc_ob_hardware_watchdog_get(void)
{
    FlagStatus flag = RESET;

    if(OB_FWDGT_HW == (uint32_t)(NVMC_OBSTAT0_EFT & NVMC_OBSTAT0_EFT_NWDG_HW)) {
        flag = SET;
    } else {
        flag = RESET;
    }
    return flag;
}

/*!
    \brief      get option byte OTA check configuration (API_ID(0x0084U))
    \param[in]  none
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus nvmc_ob_ota_check_get(void)
{
    FlagStatus flag = RESET;

    if(OB_OTA_CHECK_ENABLE == (uint32_t)(NVMC_OBSTAT0_EFT & NVMC_OBSTAT0_EFT_OTA_CHECK)) {
        flag = SET;
    } else {
        flag = RESET;
    }
    return flag;
}

/*!
    \brief      get option byte firmware update configuration (API_ID(0x0085U))
    \param[in]  none
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus nvmc_ob_firmware_update_get(void)
{
    FlagStatus flag = RESET;

    if(OB_FW_UPDATE_ENABLE == (uint32_t)(NVMC_OBSTAT0_EFT & NVMC_OBSTAT0_EFT_FW_UPDATE)) {
        flag = SET;
    } else {
        flag = RESET;
    }
    return flag;
}

/*!
    \brief      get option byte ENVM backup bank number (API_ID(0x0086U))
    \param[in]  none
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
uint8_t nvmc_ob_envm_backup_bank_get(void)
{
    uint8_t bank_number = 1U;

    if(OB_ENVM_BANK0_BACKUP == (uint32_t)(NVMC_OBSTAT0_EFT & NVMC_OBSTAT0_EFT_SWAP)) {
        bank_number = 0U;
    } else {
        bank_number = 1U;
    }
    return bank_number;
}

/*!
    \brief      get the option byte secure mode (API_ID(0x0087U))
    \param[in]  none
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus nvmc_ob_secure_mode_get(void)
{
    FlagStatus secure_mode_state = RESET;

    if(OB_SECURE_MODE_ENABLE == (uint32_t)(NVMC_OBSTAT0_EFT & NVMC_OBSTAT0_EFT_SCR)) {
        secure_mode_state = SET;
    } else {
        secure_mode_state = RESET;
    }
    return secure_mode_state;
}

/*!
    \brief      get option byte ecc configuration (API_ID(0x0088U))
    \param[in]  ecc_type: ecc type index
                only one parameter can be selected which is shown as below:
      \arg        ITCM_ECC: ITCM ECC
      \arg        DTCM0_ECC: DTCM0 ECC
      \arg        DTCM1_ECC: DTCM1 ECC
      \arg        R2_SRAM_ECC: Region 2 SRAM ECC
      \arg        R1_SRAM_ECC: Region 1 SRAM ECC
      \arg        R0_SRAM_ECC: Region 0 SRAM ECC
      \arg        ENVM_ECC: ENVM ECC
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus nvmc_ob_ecc_get(ecc_type_enum ecc_type)
{
    FlagStatus ecc_mode;

    switch(ecc_type) {
    /* get ITCM ECC mode */
    case ITCM_ECC:
        if(OB_ITCM_ECC_ENABLE == (uint32_t)(NVMC_OBSTAT0_EFT & NVMC_OBSTAT0_EFT_ITCMECCEN)) {
            ecc_mode = SET;
        } else {
            ecc_mode = RESET;
        }
        break;
    /* get DTCM0 ECC mode */
    case DTCM0_ECC:
        if(OB_DTCM0_ECC_ENABLE == (uint32_t)(NVMC_OBSTAT0_EFT & NVMC_OBSTAT0_EFT_DTCM0ECCEN)) {
            ecc_mode = SET;
        } else {
            ecc_mode = RESET;
        }
        break;
    /* get DTCM1 ECC mode */
    case DTCM1_ECC:
        if(OB_DTCM1_ECC_ENABLE == (uint32_t)(NVMC_OBSTAT0_EFT & NVMC_OBSTAT0_EFT_DTCM1ECCEN)) {
            ecc_mode = SET;
        } else {
            ecc_mode = RESET;
        }
        break;
    /* get Region 2 SRAM ECC mode */
    case R2_SRAM_ECC:
        if(OB_R2_SRAM_ECC_ENABLE == (uint32_t)(NVMC_OBSTAT0_EFT & NVMC_OBSTAT0_EFT_R2_SRAM_ECCEN)) {
            ecc_mode = SET;
        } else {
            ecc_mode = RESET;
        }
        break;
    /* get Region 1 SRAM ECC mode */
    case R1_SRAM_ECC:
        if(OB_R1_SRAM_ECC_ENABLE == (uint32_t)(NVMC_OBSTAT0_EFT & NVMC_OBSTAT0_EFT_R1_SRAM_ECCEN)) {
            ecc_mode = SET;
        } else {
            ecc_mode = RESET;
        }
        break;
    /* get Region 0 SRAM ECC mode */
    case R0_SRAM_ECC:
        if(OB_R0_SRAM_ECC_ENABLE == (uint32_t)(NVMC_OBSTAT0_EFT & NVMC_OBSTAT0_EFT_R0_SRAM_ECCEN)) {
            ecc_mode = SET;
        } else {
            ecc_mode = RESET;
        }
        break;
    /* get ENVM ECC mode */
    case ENVM_ECC:
        if(OB_ENVM_ECC_ENABLE == (uint32_t)(NVMC_OBSTAT0_EFT & NVMC_OBSTAT0_EFT_ENVM_ECC_OVRD)) {
            ecc_mode = SET;
        } else {
            ecc_mode = RESET;
        }
        break;
    default:
        /* should not be here */
        break;
    }

    return ecc_mode;
}

/*!
    \brief      get the option byte security protection level (API_ID(0x0089U))
    \param[in]  none
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus nvmc_ob_security_protection_flag_get(void)
{
    FlagStatus spc_state = RESET;

    if(((NVMC_OBSTAT0_EFT & NVMC_OBSTAT0_EFT_SPC)) != OB_NSPC) {
        spc_state = SET;
    } else {
        spc_state = RESET;
    }
    return spc_state;
}

/*!
    \brief      get the option byte BOR threshold value (API_ID(0x008AU))
    \param[in]  none
    \param[out] none
    \retval     the BOR threshold value:
      \arg        OB_BOR_TH_VALUE3: BOR threshold value 3
      \arg        OB_BOR_TH_VALUE2: BOR threshold value 2
      \arg        OB_BOR_TH_VALUE1: BOR threshold value 1
      \arg        OB_BOR_TH_OFF: no BOR function
*/
uint32_t nvmc_ob_bor_threshold_get(void)
{
    return (uint32_t)((uint8_t)NVMC_OBSTAT0_EFT & NVMC_OBSTAT0_EFT_BOR_TH);
}

/*!
    \brief      get ITCM size in ITCM/AXI shared RAM (API_ID(0x008BU))
    \param[in]  none
    \param[out] none
    \retval     itcm size: ITCM size in KB unit
*/
uint32_t nvmc_ob_itcm_axi_shared_ram_itcm_size_get(void)
{
    uint32_t itcm_size_ob_value;
    uint32_t itcm_kb_size;

    itcm_size_ob_value = (uint32_t)((uint32_t)NVMC_OBSTAT1_EFT & NVMC_OBSTAT1_EFT_ITCM_AXI_SHARED);

    switch(itcm_size_ob_value) {
    case OB_ITCM_AXI_SHARED_ITCM_64KB:
        /* ITCM size is 64KB */
        itcm_kb_size = 64U;
        break;
    case OB_ITCM_AXI_SHARED_ITCM_128KB:
        /* ITCM size is 128KB */
        itcm_kb_size = 128U;
        break;
    case OB_ITCM_AXI_SHARED_ITCM_192KB:
        /* ITCM size is 192KB */
        itcm_kb_size = 192U;
        break;
    case OB_ITCM_AXI_SHARED_ITCM_256KB:
        /* ITCM size is 256KB */
        itcm_kb_size = 256U;
        break;
    default:
        /* ITCM size is 0KB */
        itcm_kb_size = 0U;
        break;
    }

    return itcm_kb_size;
}

/*!
    \brief      get ITCM size in ITCM/DTCM shared RAM (API_ID(0x008CU))
    \param[in]  none
    \retval     itcm size: ITCM size in KB unit
*/
uint32_t nvmc_ob_itcm_dtcm_shared_ram_itcm_size_get(void)
{
    uint32_t itcm_size_ob_value;
    uint32_t itcm_kb_size;

    itcm_size_ob_value = (uint32_t)((uint32_t)NVMC_OBSTAT1_EFT & NVMC_OBSTAT1_EFT_ITCM_DTCM_SHARED);

    switch(itcm_size_ob_value) {
    case OB_ITCM_DTCM_SHARED_ITCM_64KB:
        /* ITCM size is 64KB */
        itcm_kb_size = 64U;
        break;
    case OB_ITCM_DTCM_SHARED_ITCM_128KB:
        /* ITCM size is 128KB */
        itcm_kb_size = 128U;
        break;
    case OB_ITCM_DTCM_SHARED_ITCM_192KB:
        /* ITCM size is 192KB */
        itcm_kb_size = 192U;
        break;
    case OB_ITCM_DTCM_SHARED_ITCM_256KB:
        /* ITCM size is 256KB */
        itcm_kb_size = 256U;
        break;
    default:
        /* ITCM size is 0KB */
        itcm_kb_size = 0U;
        break;
    }

    return itcm_kb_size;
}

/*!
    \brief      get ENVM DCRP area configuration (API_ID(0x008DU))
    \param[in]  none
    \param[out] dcrp_erase_option: DCRP area erase option
    \param[out] dcrp_area_en: DCRP area enable option
    \param[out] dcrp_start_addr: DCRP area start address
    \param[out] dcrp_end_addr: DCRP area end address
    \retval     state of DCRP area address
      \arg        INVLD_ADDRESS_FLAG: the area address is invalid
      \arg        VLD_ADDRESS_FLAG: the area address is valid
*/
uint8_t nvmc_ob_envm_dcrp_area_get(uint32_t *dcrp_erase_option, uint32_t *dcrp_area_en, uint32_t *dcrp_area_start_addr, uint32_t *dcrp_area_end_addr)
{
    uint8_t address_stat = VLD_ADDRESS_FLAG;

    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_VALID_POINTER(dcrp_erase_option)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x008DU), ERR_PARAM_POINTER);
    } else if(NOT_VALID_POINTER(dcrp_area_en)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x008DU), ERR_PARAM_POINTER);
    } else if(NOT_VALID_POINTER(dcrp_area_start_addr)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x008DU), ERR_PARAM_POINTER);
    } else if(NOT_VALID_POINTER(dcrp_area_end_addr)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x008DU), ERR_PARAM_POINTER);
    } else
#endif
    {
        uint32_t dcrpaddr_reg;
        dcrpaddr_reg = NVMC_DCRPADDR_EFT;
        *dcrp_erase_option = dcrpaddr_reg & NVMC_DCRPADDR_MDF_ENVM_DCRP_EREN;
        *dcrp_area_en = dcrpaddr_reg & NVMC_DCRPADDR_MDF_ENVM_DCRP_EN;

        *dcrp_area_start_addr = ((uint32_t)(dcrpaddr_reg & NVMC_DCRPADDR_EFT_DCRP_AREA_START)) >> DCRPADDR_DCRP_AREA_START_OFFSET;
        *dcrp_area_end_addr = ((uint32_t)(dcrpaddr_reg & NVMC_DCRPADDR_EFT_DCRP_AREA_END)) >> DCRPADDR_DCRP_AREA_END_OFFSET;

        if(*dcrp_area_en == 0U) {
            /* no valid DCRP area */
            address_stat = INVLD_ADDRESS_FLAG;
        } else {
            if((*dcrp_area_start_addr) == (*dcrp_area_end_addr)) {
                /* the whole ENVM memory is DCRP area */
                *dcrp_area_start_addr = ENVM_BASE_ADDRESS;
                *dcrp_area_end_addr = ENVM_BASE_ADDRESS + ENVM_SIZE - 1U;
                address_stat = VLD_ADDRESS_FLAG;
            } else if((*dcrp_area_start_addr) < (*dcrp_area_end_addr)) {
                /* get ENVM DCRP area start address */
                *dcrp_area_start_addr = (*dcrp_area_start_addr) * DCRP_SIZE_UNIT;
                *dcrp_area_start_addr += ENVM_BASE_ADDRESS;

                /* get ENVM DCRP area end address */
                *dcrp_area_end_addr = ((*dcrp_area_end_addr) + 1U) * DCRP_SIZE_UNIT - 1U;
                *dcrp_area_end_addr += ENVM_BASE_ADDRESS;
                address_stat = VLD_ADDRESS_FLAG;
            } else {
                /* no valid DCRP area */
                address_stat = INVLD_ADDRESS_FLAG;
            }
        }
    }
    return address_stat;
}

#ifndef NVM_VER_CASE4
/*!
    \brief      get flash bank0 DCRP area configuration (API_ID(0x008EU))
    \param[in]  none
    \param[out] dcrp_erase_option: DCRP area erase option
    \param[out] dcrp_area_en: DCRP area enable option
    \param[out] dcrp_area_start_addr: DCRP area start address
    \param[out] dcrp_area_end_addr: DCRP area end address
    \retval     state of DCRP area address
      \arg        INVLD_ADDRESS_FLAG: the area address is invalid
      \arg        VLD_ADDRESS_FLAG: the area address is valid
*/
uint8_t nvmc_ob_fb0_dcrp_area_get(uint32_t *dcrp_erase_option, uint32_t *dcrp_area_en, uint32_t *dcrp_area_start_addr, uint32_t *dcrp_area_end_addr)
{
    uint8_t address_stat = VLD_ADDRESS_FLAG;

    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_VALID_POINTER(dcrp_erase_option)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x008EU), ERR_PARAM_POINTER);
    } else if(NOT_VALID_POINTER(dcrp_area_en)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x008EU), ERR_PARAM_POINTER);
    } else if(NOT_VALID_POINTER(dcrp_area_start_addr)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x008EU), ERR_PARAM_POINTER);
    } else if(NOT_VALID_POINTER(dcrp_area_end_addr)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x008EU), ERR_PARAM_POINTER);
    } else
#endif
    {
        uint32_t dcrpaddr_reg;
        dcrpaddr_reg = NVMC_DCRPADDR_EFT;
        *dcrp_erase_option = dcrpaddr_reg & NVMC_DCRPADDR_MDF_FLB0_DCRP_EREN;
        *dcrp_area_en = dcrpaddr_reg & NVMC_DCRPADDR_MDF_FLB0_DCRP_EN;

        *dcrp_area_start_addr = ((uint32_t)(dcrpaddr_reg & NVMC_DCRPADDR_EFT_DCRP_AREA_START)) >> DCRPADDR_DCRP_AREA_START_OFFSET;
        *dcrp_area_end_addr = ((uint32_t)(dcrpaddr_reg & NVMC_DCRPADDR_EFT_DCRP_AREA_END)) >> DCRPADDR_DCRP_AREA_END_OFFSET;

        if(*dcrp_area_en == 0U) {
            /* no valid DCRP area */
            address_stat = INVLD_ADDRESS_FLAG;
        } else {
            if((*dcrp_area_start_addr) == (*dcrp_area_end_addr)) {
                /* the whole flash bank0 memory is DCRP area */
                *dcrp_area_start_addr = MAIN_FLASH_BANK0_BASE_ADDRESS;
                *dcrp_area_end_addr = MAIN_FLASH_BANK0_BASE_ADDRESS + MAIN_FLASH_BANK0_SIZE - 1U;
                address_stat = VLD_ADDRESS_FLAG;

            } else if((*dcrp_area_start_addr) < (*dcrp_area_end_addr)) {
                /* get flash bank0 DCRP area start address */
                *dcrp_area_start_addr = (*dcrp_area_start_addr) * DCRP_SIZE_UNIT;
                *dcrp_area_start_addr += MAIN_FLASH_BANK0_BASE_ADDRESS;

                /* get flash bank0 DCRP area end address */
                *dcrp_area_end_addr = ((*dcrp_area_end_addr) + 1U) * DCRP_SIZE_UNIT - 1U;
                *dcrp_area_end_addr += MAIN_FLASH_BANK0_BASE_ADDRESS;
                address_stat = VLD_ADDRESS_FLAG;
            } else {
                /* no valid DCRP area */
                address_stat = INVLD_ADDRESS_FLAG;
            }
        }
    }
    return address_stat;
}

#ifndef NVM_VER_CASE2
/*!
    \brief      get flash bank1 DCRP area configuration (API_ID(0x008FU))
    \param[in]  none
    \param[out] dcrp_erase_option: DCRP area erase option
    \param[out] dcrp_area_en: DCRP area enable option
    \param[out] dcrp_area_start_addr: DCRP area start address
    \param[out] dcrp_area_end_addr: DCRP area end address
    \retval     state of DCRP area address
      \arg        INVLD_ADDRESS_FLAG: the area address is invalid
      \arg        VLD_ADDRESS_FLAG: the area address is valid
*/
uint8_t nvmc_ob_fb1_dcrp_area_get(uint32_t *dcrp_erase_option, uint32_t *dcrp_area_en, uint32_t *dcrp_area_start_addr, uint32_t *dcrp_area_end_addr)
{
    uint8_t address_stat = VLD_ADDRESS_FLAG;

    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_VALID_POINTER(dcrp_erase_option)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x008FU), ERR_PARAM_POINTER);
    } else if(NOT_VALID_POINTER(dcrp_area_en)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x008FU), ERR_PARAM_POINTER);
    } else if(NOT_VALID_POINTER(dcrp_area_start_addr)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x008FU), ERR_PARAM_POINTER);
    } else if(NOT_VALID_POINTER(dcrp_area_end_addr)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x008FU), ERR_PARAM_POINTER);
    } else
#endif
    {
        uint32_t dcrpaddr_reg;
        dcrpaddr_reg = NVMC_DCRPADDR_EFT;
        *dcrp_erase_option = dcrpaddr_reg & NVMC_DCRPADDR_MDF_FLB1_DCRP_EREN;
        *dcrp_area_en = dcrpaddr_reg & NVMC_DCRPADDR_MDF_FLB1_DCRP_EN;

        *dcrp_area_start_addr = ((uint32_t)(dcrpaddr_reg & NVMC_DCRPADDR_EFT_DCRP_AREA_START)) >> DCRPADDR_DCRP_AREA_START_OFFSET;
        *dcrp_area_end_addr = ((uint32_t)(dcrpaddr_reg & NVMC_DCRPADDR_EFT_DCRP_AREA_END)) >> DCRPADDR_DCRP_AREA_END_OFFSET;

        if(*dcrp_area_en == 0U) {
            /* no valid DCRP area */
            address_stat = INVLD_ADDRESS_FLAG;
        } else {
            if((*dcrp_area_start_addr) == (*dcrp_area_end_addr)) {
                /* the whole flash bank1 memory is DCRP area */
                *dcrp_area_start_addr = MAIN_FLASH_BANK1_BASE_ADDRESS;
                *dcrp_area_end_addr = MAIN_FLASH_BANK1_BASE_ADDRESS + MAIN_FLASH_BANK1_SIZE - 1U;
                address_stat = VLD_ADDRESS_FLAG;
            } else if((*dcrp_area_start_addr) < (*dcrp_area_end_addr)) {
                /* get flash bank1 DCRP area start address */
                *dcrp_area_start_addr = (*dcrp_area_start_addr) * DCRP_SIZE_UNIT;
                *dcrp_area_start_addr += MAIN_FLASH_BANK1_BASE_ADDRESS;

                /* get flash bank1 DCRP area end address */
                *dcrp_area_end_addr = ((*dcrp_area_end_addr) + 1U) * DCRP_SIZE_UNIT - 1U;
                *dcrp_area_end_addr += MAIN_FLASH_BANK1_BASE_ADDRESS;
                address_stat = VLD_ADDRESS_FLAG;
            } else {
                /* no valid DCRP area */
                address_stat = INVLD_ADDRESS_FLAG;
            }
        }
    }
    return address_stat;
}
#endif /* NVM_VER_CASE2 */
#endif /* NVM_VER_CASE4 */

/*!
    \brief      get ENVM secure area configuration (API_ID(0x0090U))
    \param[in]  none
    \param[out] secure_erase_option: secure area erase option
    \param[out] scr_area_en: secure area enable option
    \param[out] scr_area_start_addr: secure area start address
    \param[out] scr_area_end_addr: secure area end address
    \retval     state of secure area address
      \arg        INVLD_ADDRESS_FLAG: the area address is invalid
      \arg        VLD_ADDRESS_FLAG: the area address is valid
*/
uint8_t nvmc_ob_envm_secure_area_get(uint32_t *secure_erase_option, uint32_t *scr_area_en, uint32_t *scr_area_start_addr, uint32_t *scr_area_end_addr)
{
    uint8_t address_stat = VLD_ADDRESS_FLAG;

    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_VALID_POINTER(secure_erase_option)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x0090U), ERR_PARAM_POINTER);
    } else if(NOT_VALID_POINTER(scr_area_en)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x0090U), ERR_PARAM_POINTER);
    } else if(NOT_VALID_POINTER(scr_area_start_addr)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x0090U), ERR_PARAM_POINTER);
    } else if(NOT_VALID_POINTER(scr_area_end_addr)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x0090U), ERR_PARAM_POINTER);
    } else
#endif
    {
        uint32_t scraddr_reg;
        scraddr_reg = NVMC_SCRADDR_EFT;
        *secure_erase_option = scraddr_reg & NVMC_SCRADDR_MDF_ENVM_SCR_EREN;
        *scr_area_en = scraddr_reg & NVMC_SCRADDR_MDF_ENVM_SCR_EN;

        *scr_area_start_addr = ((uint32_t)(scraddr_reg & NVMC_SCRADDR_EFT_SCR_AREA_START)) >> SCRADDR_SCR_AREA_START_OFFSET;
        *scr_area_end_addr = ((uint32_t)(scraddr_reg & NVMC_SCRADDR_EFT_SCR_AREA_END)) >> SCRADDR_SCR_AREA_END_OFFSET;

        if(*scr_area_en == 0U) {
            /* no valid secure area */
            address_stat = INVLD_ADDRESS_FLAG;
        } else {
            if((*scr_area_start_addr) == (*scr_area_end_addr)) {
                /* the whole ENVM memory is secure area */
                *scr_area_start_addr = ENVM_BASE_ADDRESS;
                *scr_area_end_addr = ENVM_BASE_ADDRESS + ENVM_SIZE - 1U;
                address_stat = VLD_ADDRESS_FLAG;
            } else if((*scr_area_start_addr) < (*scr_area_end_addr)) {
                /* get secure area start address */
                *scr_area_start_addr = (*scr_area_start_addr) * SCR_SIZE_UNIT;
                *scr_area_start_addr += ENVM_BASE_ADDRESS;

                /* get secure area end address */
                *scr_area_end_addr = ((*scr_area_end_addr) + 1U) * SCR_SIZE_UNIT - 1U;
                *scr_area_end_addr += ENVM_BASE_ADDRESS;
                address_stat = VLD_ADDRESS_FLAG;
            } else {
                /* no valid secure area */
                address_stat = INVLD_ADDRESS_FLAG;
            }
        }
    }
    return address_stat;
}

#ifndef NVM_VER_CASE4
/*!
    \brief      get flash bank0 secure area configuration (API_ID(0x0091U))
    \param[in]  none
    \param[out] secure_erase_option: secure area erase option
    \param[out] scr_area_en: secure area enable option
    \param[out] scr_area_start_addr: secure area start address
    \param[out] scr_area_end_addr: secure area end address
    \retval     state of secure area address
      \arg        INVLD_ADDRESS_FLAG: the area address is invalid
      \arg        VLD_ADDRESS_FLAG: the area address is valid
*/
uint8_t nvmc_ob_fb0_secure_area_get(uint32_t *secure_erase_option, uint32_t *scr_area_en, uint32_t *scr_area_start_addr, uint32_t *scr_area_end_addr)
{
    uint8_t address_stat = VLD_ADDRESS_FLAG;

    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_VALID_POINTER(secure_erase_option)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x0091U), ERR_PARAM_POINTER);
    } else if(NOT_VALID_POINTER(scr_area_en)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x0091U), ERR_PARAM_POINTER);
    } else if(NOT_VALID_POINTER(scr_area_start_addr)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x0091U), ERR_PARAM_POINTER);
    } else if(NOT_VALID_POINTER(scr_area_end_addr)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x0091U), ERR_PARAM_POINTER);
    } else
#endif
    {
        uint32_t scraddr_reg;
        scraddr_reg = NVMC_SCRADDR_EFT;
        *secure_erase_option = scraddr_reg & NVMC_SCRADDR_MDF_FLB0_SCR_EREN;
        *scr_area_en = scraddr_reg & NVMC_SCRADDR_MDF_FLB0_SCR_EN;

        *scr_area_start_addr = ((uint32_t)(scraddr_reg & NVMC_SCRADDR_EFT_SCR_AREA_START)) >> SCRADDR_SCR_AREA_START_OFFSET;
        *scr_area_end_addr = ((uint32_t)(scraddr_reg & NVMC_SCRADDR_EFT_SCR_AREA_END)) >> SCRADDR_SCR_AREA_END_OFFSET;

        if(*scr_area_en == 0U) {
            /* no valid secure area */
            address_stat = INVLD_ADDRESS_FLAG;
        } else {
            if((*scr_area_start_addr) == (*scr_area_end_addr)) {
                /* the whole flash bank0 memory is secure area */
                *scr_area_start_addr = MAIN_FLASH_BANK0_BASE_ADDRESS;
                *scr_area_end_addr = MAIN_FLASH_BANK0_BASE_ADDRESS + MAIN_FLASH_BANK0_SIZE - 1U;
                address_stat = VLD_ADDRESS_FLAG;
            } else if((*scr_area_start_addr) < (*scr_area_end_addr)) {
                /* get secure area start address */
                *scr_area_start_addr = (*scr_area_start_addr) * SCR_SIZE_UNIT;
                *scr_area_start_addr += MAIN_FLASH_BANK0_BASE_ADDRESS;

                /* get secure area end address */
                *scr_area_end_addr = ((*scr_area_end_addr) + 1U) * SCR_SIZE_UNIT - 1U;
                *scr_area_end_addr += MAIN_FLASH_BANK0_BASE_ADDRESS;
                address_stat = VLD_ADDRESS_FLAG;
            } else {
                /* no valid secure area */
                address_stat = INVLD_ADDRESS_FLAG;
            }
        }
    }
    return address_stat;
}

#ifndef NVM_VER_CASE2
/*!
    \brief      get flash bank1 secure area configuration (API_ID(0x0092U))
    \param[in]  none
    \param[out] secure_erase_option: secure area erase option
    \param[out] scr_area_en: secure area enable option
    \param[out] scr_area_start_addr: secure area start address
    \param[out] scr_area_end_addr: secure area end address
    \retval     state of secure area address
      \arg        INVLD_ADDRESS_FLAG: the area address is invalid
      \arg        VLD_ADDRESS_FLAG: the area address is valid
*/
uint8_t nvmc_ob_fb1_secure_area_get(uint32_t *secure_erase_option, uint32_t *scr_area_en, uint32_t *scr_area_start_addr, uint32_t *scr_area_end_addr)
{
    uint8_t address_stat = VLD_ADDRESS_FLAG;

    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_VALID_POINTER(secure_erase_option)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x0092U), ERR_PARAM_POINTER);
    } else if(NOT_VALID_POINTER(scr_area_en)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x0092U), ERR_PARAM_POINTER);
    } else if(NOT_VALID_POINTER(scr_area_start_addr)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x0092U), ERR_PARAM_POINTER);
    } else if(NOT_VALID_POINTER(scr_area_end_addr)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x0092U), ERR_PARAM_POINTER);
    } else
#endif
    {
        uint32_t scraddr_reg;
        scraddr_reg = NVMC_SCRADDR_EFT;
        *secure_erase_option = scraddr_reg & NVMC_SCRADDR_MDF_FLB1_SCR_EREN;
        *scr_area_en = scraddr_reg & NVMC_SCRADDR_MDF_FLB1_SCR_EN;

        *scr_area_start_addr = ((uint32_t)(scraddr_reg & NVMC_SCRADDR_EFT_SCR_AREA_START)) >> SCRADDR_SCR_AREA_START_OFFSET;
        *scr_area_end_addr = ((uint32_t)(scraddr_reg & NVMC_SCRADDR_EFT_SCR_AREA_END)) >> SCRADDR_SCR_AREA_END_OFFSET;

        if(*scr_area_en == 0U) {
            /* no valid secure area */
            address_stat = INVLD_ADDRESS_FLAG;
        } else {
            if((*scr_area_start_addr) == (*scr_area_end_addr)) {
                /* the whole flash bank1 memory is secure area */
                *scr_area_start_addr = MAIN_FLASH_BANK1_BASE_ADDRESS;
                *scr_area_end_addr = MAIN_FLASH_BANK1_BASE_ADDRESS + MAIN_FLASH_BANK1_SIZE - 1U;
                address_stat = VLD_ADDRESS_FLAG;
            } else if((*scr_area_start_addr) < (*scr_area_end_addr)) {
                /* get secure area start address */
                *scr_area_start_addr = (*scr_area_start_addr) * SCR_SIZE_UNIT;
                *scr_area_start_addr += MAIN_FLASH_BANK1_BASE_ADDRESS;

                /* get secure area end address */
                *scr_area_end_addr = ((*scr_area_end_addr) + 1U) * SCR_SIZE_UNIT - 1U;
                *scr_area_end_addr += MAIN_FLASH_BANK1_BASE_ADDRESS;
                address_stat = VLD_ADDRESS_FLAG;
            } else {
                /* no valid secure area */
                address_stat = INVLD_ADDRESS_FLAG;
            }
        }
    }
    return address_stat;
}
#endif /* NVM_VER_CASE2 */
#endif /* NVM_VER_CASE4 */

/*!
    \brief      get option byte write protection (API_ID(0x0093U))
    \param[in]  wp_area: write protection area
                only one parameter can be selected which is shown as below:
      \arg        WP_AREA_FLASH: main flash write protection area (only in case1, case2, case3, WP_AREA_FLASH can be selected )
      \arg        WP_AREA_ENVM: ENVM write protection area (for case1-4, WP_AREA_ENVM can be selected )
    \param[out] none
    \retval     the write protection option byte value(when wp_area = WP_AREA_FLASH, case1:0 - 0x0FFFFFFFFFFFFFFFU, case2 and case3:0 - 0x000000000FFFFFFFU)
                (when wp_area = WP_AREA_ENVM, case1 and case2:0 - 0x0000FFFFU, case3:0 - 0x000000FFU, case4:0 - 0x00001FFFU)
*/
uint64_t nvmc_ob_write_protection_get(wp_area_enum wp_area)
{
    uint64_t wp_cfg;

#ifndef NVM_VER_CASE4
    /* return write protection option byte value */
    if(WP_AREA_FLASH == wp_area) {
        /* flash write protection */
        uint32_t temp = NVMC_WP0_FLASH_EFT & NVMC_WP0_FLASH_EFT_WP;
        wp_cfg = (uint64_t)temp;
#ifdef NVM_VER_CASE1
        uint32_t wp1_val = NVMC_WP1_FLASH_EFT & NVMC_WP1_FLASH_EFT_WP;
        wp_cfg |= (uint64_t)wp1_val << WP1_FLASH_OFFSET;
#endif /* NVM_VER_CASE1 */
    } else
#endif /* NVM_VER_CASE4 */
    {
        if(WP_AREA_ENVM == wp_area) {
            /* ENVM write protection */
            wp_cfg = (uint64_t)(NVMC_WP_ENVM_EFT);
        } else {
            /* report error */
            wp_cfg = 0xFFFFFFFFFFFFFFFFU;
        }
    }
    return wp_cfg;
}

/*!
    \brief      get boot address (API_ID(0x0094U))
    \param[in]  none
    \param[out] boot_address_0: MSB of boot address 0
    \param[out] boot_address_1: MSB of boot address 1
    \retval     none
*/
void nvmc_ob_boot_address_get(uint32_t *boot_address_0, uint32_t *boot_address_1)
{
    uint32_t btaddr_reg;

    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_VALID_POINTER(boot_address_0)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x0094U), ERR_PARAM_POINTER);
    } else if(NOT_VALID_POINTER(boot_address_1)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x0094U), ERR_PARAM_POINTER);
    } else
#endif
    {
        btaddr_reg = NVMC_BTADDR_EFT;
        *boot_address_0 = (uint32_t)((btaddr_reg & NVMC_BTADDR_EFT_BOOT_ADDR0) >> BTADDR_BOOT_ADDR0_OFFSET);
        *boot_address_1 = (uint32_t)((btaddr_reg & NVMC_BTADDR_EFT_BOOT_ADDR1) >> BTADDR_BOOT_ADDR1_OFFSET);
    }
}

/*!
    \brief      get option byte one bit error unrepaired counter interrupt threshold value (API_ID(0x0095U))
    \param[in]  none
    \param[out] none
    \retval     one bit error threshold value
*/
uint32_t nvmc_ob_err1_cnt_threshold_get(void)
{
    return (uint32_t)((NVMC_OBREPCTL_MDF & NVMC_OBREPCTL_MDF_ERR1CNTTH) >> OBREPCTL_ERR1CNTTH_OFFSET);
}

/*!
    \brief      get option byte two bit error unrepaired counter interrupt threshold value (API_ID(0x0096U))
    \param[in]  none
    \param[out] none
    \retval     two bit error threshold value
*/
uint32_t nvmc_ob_err2_cnt_threshold_get(void)
{
    return (uint32_t)((NVMC_OBREPCTL_MDF & NVMC_OBREPCTL_MDF_ERR2CNTTH) >> OBREPCTL_ERR2CNTTH_OFFSET);
}

/*!
    \brief      get option byte power-on reset self-check repair configuration (API_ID(0x0097U))
    \param[in]  none
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus nvmc_ob_power_on_self_check_get(void)
{
    FlagStatus flag = RESET;

    if(POR_RESET_REPAIR_ENABLE == (uint32_t)(NVMC_OBREPCTL_EFT & NVMC_OBREPCTL_EFT_PORRSTREPDIS)) {
        flag = SET;
    } else {
        flag = RESET;
    }
    return flag;
}

/*!
    \brief      get option byte system reset self-check repair configuration (API_ID(0x0098U))
    \param[in]  none
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus nvmc_ob_system_reset_check_get(void)
{
    FlagStatus flag = RESET;

    if(SYS_RESET_REPAIR_ENABLE == (uint32_t)(NVMC_OBREPCTL_EFT & NVMC_OBREPCTL_EFT_SYSRSTREPDIS)) {
        flag = SET;
    } else {
        flag = RESET;
    }
    return flag;
}

/*!
    \brief      get option byte 2 bits error automatic repair (API_ID(0x0099U))
    \param[in]  none
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus nvmc_ob_ecc2_auto_repair_get(void)
{
    FlagStatus flag = RESET;

    if(ECC2_AUTO_REPAIR_ENABLE == (uint32_t)(NVMC_OBREPCTL_MDF & NVMC_OBREPCTL_MDF_ERR2AUTOREPEN)) {
        flag = SET;
    } else {
        flag = RESET;
    }
    return flag;
}

/*!
    \brief      get option byte 1 bit errors automatic repair (API_ID(0x009AU))
    \param[in]  none
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus nvmc_ob_ecc1_auto_repair_get(void)
{
    FlagStatus flag = RESET;

    if(ECC1_AUTO_REPAIR_ENABLE == (uint32_t)(NVMC_OBREPCTL_MDF & NVMC_OBREPCTL_MDF_ERR1AUTOREPEN)) {
        flag = SET;
    } else {
        flag = RESET;
    }
    return flag;
}

/*!
    \brief      get option byte bad data replacement in ENVM with flash content (API_ID(0x009BU))
    \param[in]  none
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus nvmc_ob_envm_bad_data_replace_get(void)
{
    FlagStatus flag = RESET;

    if(BAD_ENVM_ADDR_REPLACE_ENABLE == (uint32_t)(NVMC_OBREPCTL_MDF & NVMC_OBREPCTL_MDF_BBREPLEN)) {
        flag = SET;
    } else {
        flag = RESET;
    }
    return flag;
}

/*!
    \brief      get option byte repair data with two bits error during self check (API_ID(0x009CU))
    \param[in]  none
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus nvmc_ob_two_bits_error_repair_get(void)
{
    FlagStatus flag = RESET;

    if(TWO_BIT_ERR_REPAIR_ENABLE == (uint32_t)(NVMC_OBREPCTL_EFT & NVMC_OBREPCTL_EFT_ERR2REPEN)) {
        flag = SET;
    } else {
        flag = RESET;
    }
    return flag;
}

/*!
    \brief      get option byte repair data with one bit error during self check (API_ID(0x009DU))
    \param[in]  none
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus nvmc_ob_one_bit_error_repair_get(void)
{
    FlagStatus flag = RESET;

    if(ONE_BIT_ERR_REPAIR_ENABLE == (uint32_t)(NVMC_OBREPCTL_EFT & NVMC_OBREPCTL_EFT_ERR1REPEN)) {
        flag = SET;
    } else {
        flag = RESET;
    }
    return flag;
}

/*!
    \brief      get option byte bootloader self-check (API_ID(0x009EU))
    \param[in]  none
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus nvmc_ob_bootloader_self_check_get(void)
{
    FlagStatus flag = RESET;

    if(BOOTLOADER_CHECK_ENABLE == (uint32_t)(NVMC_OBREPCTL_MDF & NVMC_OBREPCTL_MDF_BLREPMOD)) {
        flag = SET;
    } else {
        flag = RESET;
    }
    return flag;
}

/*!
    \brief      get option byte synchronization mode (API_ID(0x009FU))
    \param[in]  none
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus nvmc_ob_sync_mode_get(void)
{
    FlagStatus flag = RESET;

    if(OB_SYNC_MODE_ENABLE == (uint32_t)(NVMC_OBREPCTL_EFT & NVMC_OBREPCTL_EFT_SYNEN)) {
        flag = SET;
    } else {
        flag = RESET;
    }
    return flag;
}

/*!
    \brief      get option byte synchronization area size in byte unit (API_ID(0x00A0U))
    \param[in]  none
    \param[out] none
    \retval     sync_area_size: synchronization area size
*/
uint32_t nvmc_ob_sync_area_size_get(void)
{
    uint32_t sync_area_size = 0U;

    if(OB_SYNC_MODE_ENABLE == (uint32_t)(NVMC_OBREPCTL_EFT & NVMC_OBREPCTL_EFT_SYNEN)) {
        sync_area_size = (((((uint32_t)(NVMC_OBREPCTL_EFT & NVMC_OBREPCTL_EFT_SYNSZ)) >> OBREPCTL_SYNSZ_OFFSET) + 1U) * SYNC_SIZE_UNIT);
    }
    return sync_area_size;
}

/*!
    \brief      get option byte hardware self-check mode (API_ID(0x00A1U))
    \param[in]  none
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus nvmc_ob_hardware_self_check_get(void)
{
    FlagStatus flag = RESET;

    if(OB_HW_SELF_CHECK_ENABLE == (uint32_t)(NVMC_OBREPCTL_MDF & NVMC_OBREPCTL_MDF_HWREPMOD)) {
        flag = SET;
    }

    return flag;
}

/*!
    \brief      get option byte repair address (API_ID(0x00A2U))
    \param[in]  none
    \param[out] repair_start_addr: repair start address
    \param[out] repair_end_addr: repair end address
    \retval     state of repair address
      \arg        INVLD_ADDRESS_FLAG: the repair address is invalid
      \arg        VLD_ADDRESS_FLAG: the repair address is valid
*/
uint8_t nvmc_ob_repair_address_get(uint32_t *repair_start_addr, uint32_t *repair_end_addr)
{
    uint8_t address_stat = INVLD_ADDRESS_FLAG;

    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_VALID_POINTER(repair_start_addr)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x00A2U), ERR_PARAM_POINTER);
    } else if(NOT_VALID_POINTER(repair_end_addr)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x00A2U), ERR_PARAM_POINTER);
    } else
#endif
    {
        *repair_start_addr = ((uint32_t)(NVMC_OBREPSTADDR_EFT & NVMC_OBREPSTADDR_EFT_STARTADDR)) >> OBREPSTADDR_STARTADDR_OFFSET;
        *repair_end_addr = ((uint32_t)(NVMC_OBREPENDADDR_MDF & NVMC_OBREPENDADDR_MDF_ENDADDR)) >> OBREPSTADDR_ENDADDR_OFFSET;

        if((*repair_start_addr) <= (*repair_end_addr)) {
            *repair_start_addr = ENVM_BASE_ADDRESS + (*repair_start_addr) * REPAIR_ADDRESS_UNIT;
            *repair_end_addr = ENVM_BASE_ADDRESS + (*repair_end_addr + 1U) * REPAIR_ADDRESS_UNIT - 1U;
            address_stat = VLD_ADDRESS_FLAG;
        } else {
            /* no valid repair area */
            address_stat = INVLD_ADDRESS_FLAG;
        }
    }
    return address_stat;
}

/*!
    \brief      get option byte bad block address (API_ID(0x00A3U))
    \param[in]  index: number of bad block address (0 - 3)
    \param[out] bad_block_address: bad block address
    \retval     state of bad block address
      \arg        INVLD_ADDRESS_FLAG: the address is invalid
      \arg        VLD_ADDRESS_FLAG: the address is valid
*/
uint8_t nvmc_ob_bad_block_address_get(uint32_t index, uint32_t *bad_block_address)
{
    uint32_t bad_block_valid;
    uint8_t address_stat = INVLD_ADDRESS_FLAG;
    /* check parameter */
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_NVMC_BAD_BLOCK_INDEX(index)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x00A3U), ERR_PARAM_INVALID);
    } else if(NOT_VALID_POINTER(bad_block_address)) {
        fw_debug_report_err(NVMC_MODULE_ID, API_ID(0x00A3U), ERR_PARAM_POINTER);
    } else
#endif
    {
        bad_block_valid = (NVMC_OBBBADDRX_EFT(index) & NVMC_OBBBADDRX_EFT_OBBBADDRVLD) >> OBBBADDRX_OBBBADDRVLD_OFFSET;
        if(bad_block_valid) {
            *bad_block_address = NVMC_OBBBADDRX_EFT(index) & (~NVMC_OBBBADDRX_EFT_OBBBADDRVLD);
            address_stat = VLD_ADDRESS_FLAG;
        } else {
            *bad_block_address = 0xFFFFFFFFU;
            /* no valid bad block address */
            address_stat = INVLD_ADDRESS_FLAG;
        }
    }
    return address_stat;
}

/*!
    \brief      get option byte flag set or reset (API_ID(0x00A4U))
    \param[in]  ob_flag: check option byte flag
                only one parameter can be selected which is shown as below:
      \arg        NVMC_FLAG_OBMERR: option byte modified error flag bit
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus nvmc_ob_flag_get(uint32_t ob_flag)
{
    FlagStatus flag = RESET;

    if(NVMC_OBCTL & ob_flag) {
        flag = SET;
    }
    /* return the state of corresponding flash flag */
    return flag;
}

/*!
    \brief      clear the option byte pending flag (API_ID(0x00A5U))
    \param[in]  ob_flag: clear option byte flag
                only one parameter can be selected which is shown as below:
      \arg        NVMC_FLAG_OBMERR: option byte modified error flag bit
    \param[out] none
    \retval     none
*/
void nvmc_ob_flag_clear(uint32_t ob_flag)
{
    uint32_t reg_value;

    reg_value = NVMC_OBCTL;
    reg_value |= ob_flag;

    /* clear the flag */
    NVMC_OBCTL = reg_value;
}

/*!
    \brief      enable option byte interrupt (API_ID(0x00A6U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void nvmc_ob_interrupt_enable(void)
{
    NVMC_OBCTL |= NVMC_INT_OBMERRIE;
}

/*!
    \brief      disable option byte interrupt (API_ID(0x00A7U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void nvmc_ob_interrupt_disable(void)
{
    NVMC_OBCTL &= ~(uint32_t)NVMC_INT_OBMERRIE;
}

/*!
    \brief      get option byte interrupt flag status (API_ID(0x00A8U))
    \param[in]  ob_int_flag: option byte interrupt flag status
                only one parameter can be selected which is shown as below:
      \arg        NVMC_INT_FLAG_OBMERR: option byte modified error interrupt flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus nvmc_ob_interrupt_flag_get(uint8_t ob_int_flag)
{
    FlagStatus reval = RESET;

    uint32_t reg1 = NVMC_OBCTL;
    uint32_t reg2 = NVMC_OBCTL;

    switch(ob_int_flag) {
    /* option byte modified error interrupt */
    case NVMC_INT_FLAG_OBMERR:
        reg1 = reg1 & NVMC_FLAG_OBMERR;
        reg2 = reg2 & NVMC_INT_OBMERRIE;
        break;
    default :
        break;
    }
    /*get option byte interrupt flag status */
    if((0U != reg1) && (0U != reg2)) {
        reval = SET;
    }

    return reval;
}

/*!
    \brief      clear option byte interrupt flag status (API_ID(0x00A9U))
    \param[in]  ob_int_flag: option byte interrupt flag status
                only one parameter can be selected which is shown as below:
      \arg        NVMC_INT_FLAG_OBMERR: option byte modified error interrupt flag
    \param[out] none
    \retval     none
*/
void nvmc_ob_interrupt_flag_clear(uint8_t ob_int_flag)
{

    switch(ob_int_flag) {
    /* option byte modified error interrupt */
    case NVMC_INT_FLAG_OBMERR:
        NVMC_OBCTL |= NVMC_FLAG_OBMERR;
        break;
    default :
        break;
    }
}

/*!
    \brief      get the option byte state (API_ID(0x00AAU))
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

/*!
    \brief      check whether option byte is ready or not (API_ID(0x00ABU))
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
