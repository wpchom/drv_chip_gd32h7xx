/*!
    \file    gd32h77x_nvmc.h
    \brief   definitions for the NVMC

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

#ifndef GD32H77X_NVMC_H
#define GD32H77X_NVMC_H

#include "gd32h77x.h"

#if (defined(GD32H77EXW) || (GD32H77DXW) || (GD32H779XW))
#define NVM_VER_CASE1
#elif (defined(GD32H77EXP) || (GD32H77DXP) || (GD32H779XP))
#define NVM_VER_CASE2
#elif (defined(GD32H77EXN) || (GD32H77DXN) || (GD32H779XN))
#define NVM_VER_CASE3
#elif (defined(GD32H77EXI) || (GD32H77DXI) || (GD32H779XI))
#define NVM_VER_CASE4
#endif

/* NVMC definition */
#define NVMC                                            NVMC_BASE                                           /*!< NVMC register base address */

/* ENVM registers definitions */
#define NVMC_ENVM_KEY                                   REG32((NVMC) + 0x00000004U)                         /*!< ENVM unlock key register */
#define NVMC_ENVM_CTL                                   REG32((NVMC) + 0x0000000CU)                         /*!< ENVM control register */
#define NVMC_ENVM_STAT                                  REG32((NVMC) + 0x00000010U)                         /*!< ENVM status register */
#define NVMC_ENVM_ADDR                                  REG32((NVMC) + 0x00000014U)                         /*!< ENVM address register */
#define NVMC_ENVM_CRCRESULT                             REG32((NVMC) + 0x0000005CU)                         /*!< ENVM CRC result register */
#define NVMC_ENVM_SW_REP_FATAL_ADDR                     REG32((NVMC) + 0x00000060U)                         /*!< ENVM software repair fatal address register */
#define NVMC_ENVM_REPCS                                 REG32((NVMC) + 0x000000C8U)                         /*!< ENVM repair function control and status register */
#define NVMC_ENVM_ROBBADDR0                             REG32((NVMC) + 0x000000CCU)                         /*!< ENVM read only bad block 0 address that still has the error after repair during self-check */
#define NVMC_ENVM_ROBBADDR1                             REG32((NVMC) + 0x000000D0U)                         /*!< ENVM read only bad block 1 address that still has the error after repair during self-check */
#define NVMC_ENVM_ROBBADDR2                             REG32((NVMC) + 0x000000D4U)                         /*!< ENVM read only bad block 2 address that still has the error after repair during self-check */
#define NVMC_ENVM_ROBBADDR3                             REG32((NVMC) + 0x000000D8U)                         /*!< ENVM read only bad block 3 address that still has the error after repair during self-check */
#define NVMC_ENVM_ROBBADDRX(x)                          REG32((NVMC) + 0x000000CCU + (uint32_t)((x) * 4U))  /*!< ENVM read only bad block 3 address that still has the error after repair during self-check */
#define NVMC_ENVM_AUTOERR1ADDR0                         REG32((NVMC) + 0x000000DCU)                         /*!< The address 0 where 1 bit error occurs when the bus reads ENVM */
#define NVMC_ENVM_AUTOERR1ADDR1                         REG32((NVMC) + 0x000000E0U)                         /*!< The address 1 where 1 bit error occurs when the bus reads ENVM */
#define NVMC_ENVM_AUTOERR1ADDR2                         REG32((NVMC) + 0x000000E4U)                         /*!< The address 2 where 1 bit error occurs when the bus reads ENVM */
#define NVMC_ENVM_AUTOERR1ADDR3                         REG32((NVMC) + 0x000000E8U)                         /*!< The address 3 where 1 bit error occurs when the bus reads ENVM */
#define NVMC_ENVM_AUTOERR1ADDRX(x)                      REG32((NVMC) + 0x000000DCU + (uint32_t)((x) * 4U))  /*!< The address x(x=0,1,2,3) where 1 bit error occurs when the bus reads ENVM */
#define NVMC_ENVM_AUTOERR2ADDR0                         REG32((NVMC) + 0x000000ECU)                         /*!< The address 0 where 2 bits error occurs when the bus reads ENVM */
#define NVMC_ENVM_AUTOERR2ADDR1                         REG32((NVMC) + 0x000000F0U)                         /*!< The address 1 where 2 bits error occurs when the bus reads ENVM */
#define NVMC_ENVM_AUTOERR2ADDR2                         REG32((NVMC) + 0x000000F4U)                         /*!< The address 2 where 2 bits error occurs when the bus reads ENVM */
#define NVMC_ENVM_AUTOERR2ADDR3                         REG32((NVMC) + 0x000000F8U)                         /*!< The address 3 where 2 bits error occurs when the bus reads ENVM */
#define NVMC_ENVM_AUTOERR2ADDRX(x)                      REG32((NVMC) + 0x000000ECU + (uint32_t)((x) * 4U))  /*!< The address x(x=0,1,2,3) where 2 bits error occurs when the bus reads ENVM */
#define NVMC_ENVM_UNREPECCADDR0                         REG32((NVMC) + 0x00000110U)                         /*!< ENVM self-check unrepaired ECC error address 0 register */
#define NVMC_ENVM_UNREPECCADDR1                         REG32((NVMC) + 0x00000114U)                         /*!< ENVM self-check unrepaired ECC error address 1 register */
#define NVMC_ENVM_UNREPECCADDR2                         REG32((NVMC) + 0x00000118U)                         /*!< ENVM self-check unrepaired ECC error address 2 register */
#define NVMC_ENVM_UNREPECCADDR3                         REG32((NVMC) + 0x0000011CU)                         /*!< ENVM self-check unrepaired ECC error address 3 register */
#define NVMC_ENVM_UNREPECCADDR4                         REG32((NVMC) + 0x00000120U)                         /*!< ENVM self-check unrepaired ECC error address 4 register */
#define NVMC_ENVM_UNREPECCADDR5                         REG32((NVMC) + 0x00000124U)                         /*!< ENVM self-check unrepaired ECC error address 5 register */
#define NVMC_ENVM_UNREPECCADDRX(x)                      REG32((NVMC) + 0x00000110U + (uint32_t)((x) * 4U))  /*!< ENVM self-check unrepaired ECC error address x(x=0,1,2,3,4,5) register */
#define NVMC_ENVM_OTAFAILADDR                           REG32((NVMC) + 0x00000128U)                         /*!< ENVM OTA fail address register */
#define NVMC_ENVM_PFINJ_KEY                             REG32((NVMC) + 0x000001F0U)                         /*!< ENVM program fault inject unlock key register */
#define NVMC_ENVM_PFINJ                                 REG32((NVMC) + 0x000001F4U)                         /*!< ENVM program fault inject register */

/* flash registers definitions */
#define NVMC_FLASH_KEY                                  REG32((NVMC) + 0x00000150U)                         /*!< NVMC_FLASH_CTL unlock key register */
#define NVMC_FLASH_CTL                                  REG32((NVMC) + 0x00000154U)                         /*!< flash control register */
#define NVMC_FLASH_STAT                                 REG32((NVMC) + 0x00000158U)                         /*!< flash status register */
#define NVMC_FLASH_ADDR                                 REG32((NVMC) + 0x0000015CU)                         /*!< flash address register */
#define NVMC_FLASH_ECCADDR                              REG32((NVMC) + 0x00000160U)                         /*!< flash ECC error address register */

/* NVMC registers definitions */
#define NVMC_PERFCTL                                    REG32((NVMC) + 0x00000000U)                         /*!< NVMC performance control register */
#define NVMC_OBKEY                                      REG32((NVMC) + 0x00000008U)                         /*!< NVMC option byte unlock key register */
#define NVMC_OBCTL                                      REG32((NVMC) + 0x00000018U)                         /*!< NVMC option byte control register */
#define NVMC_OBSTAT0_EFT                                REG32((NVMC) + 0x0000001CU)                         /*!< NVMC effective option byte status0 register */
#define NVMC_OBSTAT0_MDF                                REG32((NVMC) + 0x00000020U)                         /*!< NVMC modified option byte status0 register */
#define NVMC_DCRPADDR_EFT                               REG32((NVMC) + 0x00000028U)                         /*!< NVMC effective DCRP address register */
#define NVMC_DCRPADDR_MDF                               REG32((NVMC) + 0x0000002CU)                         /*!< NVMC modified DCRP address register */
#define NVMC_SCRADDR_EFT                                REG32((NVMC) + 0x00000030U)                         /*!< NVMC effective secure address register */
#define NVMC_SCRADDR_MDF                                REG32((NVMC) + 0x00000034U)                         /*!< NVMC modified secure address register */
#define NVMC_WP_ENVM_EFT                                REG32((NVMC) + 0x00000038U)                         /*!< ENVM effective erase/program protection register */
#define NVMC_WP_ENVM_MDF                                REG32((NVMC) + 0x0000003CU)                         /*!< ENVM modified erase/program protection register */
#define NVMC_BTADDR_EFT                                 REG32((NVMC) + 0x00000040U)                         /*!< NVMC effective boot address register */
#define NVMC_BTADDR_MDF                                 REG32((NVMC) + 0x00000044U)                         /*!< NVMC modified boot address register */
#define NVMC_OBSTAT1_EFT                                REG32((NVMC) + 0x00000050U)                         /*!< NVMC effective option byte status1 register */
#define NVMC_OBSTAT1_MDF                                REG32((NVMC) + 0x00000054U)                         /*!< NVMC modified option byte status1 register */
#define NVMC_WP0_FLASH_EFT                              REG32((NVMC) + 0x00000080U)                         /*!< NVMC flash effective erase/program protection register 0 */
#define NVMC_WP1_FLASH_EFT                              REG32((NVMC) + 0x00000084U)                         /*!< NVMC flash effective erase/program protection register 1 */
#define NVMC_WP0_FLASH_MDF                              REG32((NVMC) + 0x00000088U)                         /*!< NVMC flash modified erase/program protection register 0 */
#define NVMC_WP1_FLASH_MDF                              REG32((NVMC) + 0x0000008CU)                         /*!< NVMC flash modified erase/program protection register 1 */
#define NVMC_OBREPCTL_EFT                               REG32((NVMC) + 0x00000090U)                         /*!< NVMC effective option byte repair control register */
#define NVMC_OBREPCTL_MDF                               REG32((NVMC) + 0x00000094U)                         /*!< NVMC modified option byte repair control register */
#define NVMC_OBREPSTADDR_EFT                            REG32((NVMC) + 0x00000098U)                         /*!< NVMC effective option byte repair start address register */
#define NVMC_OBREPENDADDR_EFT                           REG32((NVMC) + 0x0000009CU)                         /*!< NVMC effective option byte repair end address register */
#define NVMC_OBREPSTADDR_MDF                            REG32((NVMC) + 0x000000A0U)                         /*!< NVMC modified option byte repair start address register */
#define NVMC_OBREPENDADDR_MDF                           REG32((NVMC) + 0x000000A4U)                         /*!< NVMC modified option byte repair end address register */
#define NVMC_OBBBADDRX_EFT(x)                           REG32((NVMC) + 0x000000A8U + (uint32_t)((x) * 4U))  /*!< NVMC effective option byte bad block x(x=0,1,2,3) address register */
#define NVMC_OBBBADDRX_MDF(x)                           REG32((NVMC) + 0x000000B8U + (uint32_t)((x) * 4U))  /*!< NVMC modified option byte bad block x(x=0,1,2,3) address register */
#define NVMC_BLANKADDR                                  REG32((NVMC) + 0x000000FCU)                         /*!< NVMC blank address register  */
#define NVMC_PID0                                       REG32((NVMC) + 0x00000100U)                         /*!< product ID register 0 */
#define NVMC_PID1                                       REG32((NVMC) + 0x00000104U)                         /*!< product ID register 1 */
#define NVMC_NMIERRADDR                                 REG32((NVMC) + 0x00000108U)                         /*!< NVMC not maskable interrupt error address register  */

/* OTP registers definitions */
#define NVMC_OTP_CTL                                    REG32((NVMC) + 0x00000200U)                         /*!< OTP contro register */
#define NVMC_OTP_STAT                                   REG32((NVMC) + 0x00000204U)                         /*!< OTP status register */
#define NVMC_OTP_USER_CTL0                              REG32((NVMC) + 0x0000020CU)                         /*!< OTP user control 0 register */
#define NVMC_OTP_USER_CTL1                              REG32((NVMC) + 0x00000210U)                         /*!< OTP user control 1 register */
#define NVMC_OTP_USER_CTL2                              REG32((NVMC) + 0x00000214U)                         /*!< OTP user control 2 register */
#define NVMC_OTP_USER_CTL3                              REG32((NVMC) + 0x00000218U)                         /*!< OTP user control 3 register */
#define NVMC_OTP_USER_CTL4                              REG32((NVMC) + 0x0000021CU)                         /*!< OTP user control 4 register */
#define NVMC_OTP_USER_CTL5                              REG32((NVMC) + 0x00000220U)                         /*!< OTP user control 5 register */

/* bits definitions */
/* ENVM bits definitions */
/* NVMC_ENVM_KEY */
#define NVMC_ENVM_KEY_KEY                               BITS(0,31)                                          /*!< NVMC_ENVM_CTL unlock register bits */

/* NVMC_ENVM_CTL */
#define NVMC_ENVM_CTL_REPRESC                           BIT(31)                                             /*!< the result of the repair function clear bit */
#define NVMC_ENVM_CTL_AUTOREPRESC                       BIT(30)                                             /*!< the result of automatic repair function clear bit */
#define NVMC_ENVM_CTL_CRCRERRIE                         BIT(28)                                             /*!< CRC read error interrupt enable bit */
#define NVMC_ENVM_CTL_CRCENDCALIE                       BIT(27)                                             /*!< CRC end of calculation interrupt enable bit */
#define NVMC_ENVM_CTL_AUTOECCDERRDETIE                  BIT(26)                                             /*!< ENVM ECC two bit errors detect interrupt enable bit */
#define NVMC_ENVM_CTL_AUTOECCSERRDETIE                  BIT(25)                                             /*!< ENVM ECC one bit error detect interrupt enable bit */
#define NVMC_ENVM_CTL_RSERRIE                           BIT(24)                                             /*!< Read secure error interrupt enable bit */
#define NVMC_ENVM_CTL_RPERRIE                           BIT(23)                                             /*!< Read protection error interrupt enable bit */
#define NVMC_ENVM_CTL_OTAFAILCNT                        BITS(19,20)                                         /*!< OTA fail times count */
#define NVMC_ENVM_CTL_PGSERRIE                          BIT(18)                                             /*!< Program sequence error interrupt enable bit */
#define NVMC_ENVM_CTL_WPERRIE                           BIT(17)                                             /*!< Erase / program protection error interrupt enable bit */
#define NVMC_ENVM_CTL_ENDIE                             BIT(16)                                             /*!< End of operation interrupt enable bit */
#define NVMC_ENVM_CTL_CRCSTART                          BIT(15)                                             /*!< crc check for repair area enable bit */
#define NVMC_ENVM_CTL_ENVM_ECC_LK                       BIT(10)                                             /*!< ECC test function lock bit */
#define NVMC_ENVM_CTL_ENVM_ECC_MODE                     BITS(8,9)                                           /*!< send erase command to NVMC */
#define NVMC_ENVM_CTL_START                             BIT(7)                                              /*!< ENVM ECC mode configuration bit */
#define NVMC_ENVM_CTL_CER                               BIT(4)                                              /*!< chip erase command bit */
#define NVMC_ENVM_CTL_MER                               BIT(3)                                              /*!< ENVM mass erase command bit */
#define NVMC_ENVM_CTL_SER                               BIT(2)                                              /*!< ENVM sector erase command bit */
#define NVMC_ENVM_CTL_PG                                BIT(1)                                              /*!< ENVM program command bit */
#define NVMC_ENVM_CTL_LK                                BIT(0)                                              /*!< NVMC_ENVM_CTL lock bit */

/* NVMC_ENVM_STAT */
#define NVMC_ENVM_STAT_ENVMECCF                         BIT(31)                                             /*!< ENVM ECC function flag bit */
#define NVMC_ENVM_STAT_BOOT_AT_CHECK_SYS_EFT            BIT(30)                                             /*!< Flag generated by the option byte EFT, indicating boot from the check_bootLoader */
#define NVMC_ENVM_STAT_BOOT_AT_CHECK_SYS_MDF            BIT(29)                                             /*!< Flag generated by the option byte MDF, indicating boot from the check_bootLoader */
#define NVMC_ENVM_STAT_CRCRERR                          BIT(28)                                             /*!< CRC read error flag bit */
#define NVMC_ENVM_STAT_CRCENDCAL                        BIT(27)                                             /*!< CRC end of calculation flag bit */
#define NVMC_ENVM_STAT_AUTOECCDERRDET                   BIT(26)                                             /*!< Two bit errors detect flag bit */
#define NVMC_ENVM_STAT_AUTOECCSERRDET                   BIT(25)                                             /*!< One bit error detected and correct flag bit */
#define NVMC_ENVM_STAT_RSERR                            BIT(24)                                             /*!< Read secure error flag bit */
#define NVMC_ENVM_STAT_RPERR                            BIT(23)                                             /*!< Read protection error flag bit */
#define NVMC_ENVM_STAT_COPYSTRERR                       BIT(22)                                             /*!< Copy startup failure error flag bit */
#define NVMC_ENVM_STAT_FLENDF                           BIT(21)                                             /*!< End of flash operation flag bit */
#define NVMC_ENVM_STAT_OTASUC                           BIT(20)                                             /*!< OTA upgrade, bootloader check success flag bit */
#define NVMC_ENVM_STAT_OTAFAIL                          BIT(19)                                             /*!< OTA upgrade, bootloader check failure flag bit */
#define NVMC_ENVM_STAT_PGSERR                           BIT(18)                                             /*!< Program sequence error flag bit */
#define NVMC_ENVM_STAT_WPERR                            BIT(17)                                             /*!< Erase / program protection error flag bit */
#define NVMC_ENVM_STAT_ENDF                             BIT(16)                                             /*!< End of operation flag bit */
#define NVMC_ENVM_STAT_NVMVER                           BITS(14,15)                                         /*!< NVM version bits */
#define NVMC_ENVM_STAT_SW_REP_FLBLANK                   BIT(10)                                             /*!< Software self-check repair flash backup content blank flag */
#define NVMC_ENVM_STAT_SW_REP_FATAL                     BIT(9)                                              /*!< CRC busy flag bit */
#define NVMC_ENVM_STAT_SW_REP_ERR                       BIT(8)                                              /*!< Software self-check repair fatal error flag  */
#define NVMC_ENVM_STAT_CRCBUSY                          BIT(3)                                              /*!< Software self-check repair error flag */
#define NVMC_ENVM_STAT_BUSY                             BIT(0)                                              /*!< The ENVM is busy bit */

/* NVMC_ENVM_ADDR */
#define NVMC_ENVM_ADDR_ADDR                             BITS(0,31)                                          /*!< ENVM erase command address bits */

/* NVMC_ENVM_CRCRESULT */
#define NVMC_ENVM_CRCRESULT_CRC_RESULT                  BITS(0,31)                                          /*!< ENVM CRC result bits */

/* NVMC_ENVM_SW_REP_FATAL_ADDR */
#define NVMC_ENVM_SW_REP_FATAL_ADDR_SW_REP_FATAL_ADDR   BITS(0,31)                                          /*!< The first blank address of the flash backup area or fatal address detected by software self-check bits */

/* NVMC_ENVM_REPCS */
#define NVMC_ENVM_REPCS_ERR2CNT                         BITS(27,31)                                         /*!< the number of 2 bit error occurrences without repair during the self-check */
#define NVMC_ENVM_REPCS_ERR1CNT                         BITS(22,26)                                         /*!< the number of 1 bit error occurrences without repair during the self-check */
#define NVMC_ENVM_REPCS_BLANKMODE                       BIT(20)                                             /*!< Flash backup content blank mode */
#define NVMC_ENVM_REPCS_ROBBCNT                         BITS(16,19)                                         /*!< the number of repair failures during the self-check */
#define NVMC_ENVM_REPCS_ECC2CNTIE                       BIT(15)                                             /*!< the number of 2 bit errors is greater than or equal to the threshold interrupt enable */
#define NVMC_ENVM_REPCS_ECC1CNTIE                       BIT(14)                                             /*!< the number of 1 bit error is greater than or equal to the threshold Interrupt enable */
#define NVMC_ENVM_REPCS_ECC2CNTIF                       BIT(13)                                             /*!< flag generated when the number of 2 bit errors is greater than or equal to the threshold */
#define NVMC_ENVM_REPCS_ECC1CNTIF                       BIT(12)                                             /*!< flag generated when the number of 1 bit error is greater than or equal to the threshold */
#define NVMC_ENVM_REPCS_REPERRSTAT                      BITS(8,11)                                          /*!< self-check result, error status */
#define NVMC_ENVM_REPCS_BLREPDONE                       BIT(7)                                              /*!< bootloader writes the register to release the CPU after the bootloader self-check is done */
#define NVMC_ENVM_REPCS_FLBLANKIE                       BIT(6)                                              /*!< the flash backup content blank interruption was enabled */
#define NVMC_ENVM_REPCS_FLBLANK                         BIT(5)                                              /*!< flash backup content is blank flag */
#define NVMC_ENVM_REPCS_NMIERRINT                       BIT(4)                                              /*!< AXI read, repair failure with 3-bit error */
#define NVMC_ENVM_REPCS_REPDONEIE                       BIT(3)                                              /*!< self-repair done interrupt enable */
#define NVMC_ENVM_REPCS_REPEND                          BIT(2)                                              /*!< self-repair end flag */
#define NVMC_ENVM_REPCS_REPSTRERR                       BIT(1)                                              /*!< self-check repair configuration error bit */
#define NVMC_ENVM_REPCS_REPSTART                        BIT(0)                                              /*!< repair start bit */

/* NVMC_ENVM_ROBBADDRX */
#define NVMC_ENVM_ROBBADDRX_ROBBADDR                    BITS(5,20)                                          /*!< the address that still has the error after repair during the self-check process */
#define NVMC_ENVM_ROBBADDRX_ROBBADDRSTAT                BITS(0,1)                                           /*!< state of ROBBADDR */

/* NVMC_ENVM_AUTOERR1ADDRX */
#define NVMC_ENVM_AUTOERR1ADDRX_AUTOERR1ADDR            BITS(5,31)                                          /*!< the address where 1 bit error occurs when the bus reads ENVM */
#define NVMC_ENVM_AUTOERR1ADDRX_AUTOERR1ADDRSTAT        BIT(0)                                              /*!< state of AUTOERR1ADDR */

/* NVMC_ENVM_AUTOERR2ADDRX */
#define NVMC_ENVM_AUTOERR2ADDRX_AUTOERR2ADDR            BITS(5,31)                                          /*!< the address where 2 bit error occurs when the bus reads ENVM */
#define NVMC_ENVM_AUTOERR2ADDRX_AUTOERR2ADDRSTAT        BIT(0)                                              /*!< state of AUTOERR2ADDR */

/* NVMC_ENVM_UNREPECCADDRX */
#define NVMC_ENVM_UNREPECCADDRX_ERRADDR                 BITS(5,20)                                          /*!< the 1/2 bit error address that occurs if it is not repaired during the self-check process */
#define NVMC_ENVM_UNREPECCADDRX_ERRADDRSTAT             BITS(0,1)                                           /*!< state of ERRADDR */

/* NVMC_ENVM_OTAFAILADDR */
#define NVMC_ENVM_OTAFAILADDR_OTAFAILADDR               BITS(0,20)                                          /*!< the 1/2 bit error address that occurs if it is not repaired during the self-check process */

/* NVMC_ENVM_PFINJ_KEY */
#define NVMC_ENVM_PFINJ_KEY_PFINJ_KEY                   BITS(0,31)                                          /*!< NVMC_ENVM_PFINJ unlock register bit */

/* NVMC_ENVM_PFINJ */
#define NVMC_ENVM_PFINJ_ENVM0_PFINJ_DATA                BITS(4,6)                                           /*!< when PFINJEN[1:0] is set to 0x3, ENVM0_PFINJ_DATA[*] is set 1, the corresponding position of ENVM0_DATAIN[*] is reversed */
#define NVMC_ENVM_PFINJ_ENVM1_PFINJ_DATA                BITS(7,9)                                           /*!< when PFINJEN[1:0] is set to 0x3, ENVM1_PFINJ_DATA[*] is set 1, the corresponding position of ENVM1_DATAIN[*] is reversed */
#define NVMC_ENVM_PFINJ_PFINJEN                         BITS(12,13)                                         /*!< program fault inject enable bit */

/* flash bits definitions */
/* NVMC_FLASH_KEY */
#define NVMC_FLASH_KEY_KEY                              BITS(0,31)                                          /*!< NVMC_FLASH_CTL unlock key bits */

/* NVMC_FLASH_CTL */
#define NVMC_FLASH_CTL_LK                               BIT(0)                                              /*!< NVMC_FLASH_CTL lock bit */
#define NVMC_FLASH_CTL_PG                               BIT(1)                                              /*!< main flash program command bit */
#define NVMC_FLASH_CTL_SER                              BIT(2)                                              /*!< main flash sector erase command bit */
#define NVMC_FLASH_CTL_MER                              BIT(3)                                              /*!< main flash mass erase command bit */
#define NVMC_FLASH_CTL_PGCHEN                           BIT(4)                                              /*!< check programming area enable bit */
#define NVMC_FLASH_CTL_PER                              BIT(6)                                              /*!< page erase command bit */
#define NVMC_FLASH_CTL_START                            BIT(7)                                              /*!< send erase command to flash bit */
#define NVMC_FLASH_CTL_ENDIE                            BIT(16)                                             /*!< end of operation interrupt enable bit */
#define NVMC_FLASH_CTL_WPERRIE                          BIT(17)                                             /*!< write protection error interrupt enable bit */
#define NVMC_FLASH_CTL_PGSERRIE                         BIT(18)                                             /*!< program sequence error interrupt enable bit */
#define NVMC_FLASH_CTL_RPERRIE                          BIT(23)                                             /*!< read protection error interrupt enable bit */
#define NVMC_FLASH_CTL_RSERRIE                          BIT(24)                                             /*!< read secure error interrupt enable bit */
#define NVMC_FLASH_CTL_ECCDETIE                         BIT(26)                                             /*!< two bits detect error interrupt enable bit */
#define NVMC_FLASH_CTL_CHIP_REMOVE_EN                   BIT(31)                                             /*!< protection-removed chip erase enable bit */

/* NVMC_FLASH_STAT */
#define NVMC_FLASH_STAT_BUSY                            BIT(0)                                              /*!< flash busy flag bit */
#define NVMC_FLASH_STAT_ENDF                            BIT(16)                                             /*!< end of operation flag bit */
#define NVMC_FLASH_STAT_WPERR                           BIT(17)                                             /*!< write protection error flag bit */
#define NVMC_FLASH_STAT_PGSERR                          BIT(18)                                             /*!< program sequence error flag bit */
#define NVMC_FLASH_STAT_RPERR                           BIT(23)                                             /*!< read protection error flag bit */
#define NVMC_FLASH_STAT_RSERR                           BIT(24)                                             /*!< read secure error flag bit */
#define NVMC_FLASH_STAT_ECCDET                          BIT(26)                                             /*!< two bits detect error flag bit */
#define NVMC_FLASH_STAT_FECCF                           BIT(31)                                             /*!< flash ECC function flag */

/* NVMC_FLASH_ADDR */
#define NVMC_FLASH_ADDR_ADDR                            BITS(0,31)                                          /*!< flash erase command address bits */

/* NVMC_FLASH_ECCADDR */
#define NVMC_FLASH_ECCADDR_ECCADDR                      BITS(0,31)                                          /*!< address of byte where an ECC error is detected */

/* NVMC bits definitions */
/* NVMC_PERFCTL */
#define NVMC_PERFCTL_EMPTY_ENVM_BUF                     BIT(31)                                             /*!< empty ENVM buffer (general buffer and CPU buffer) function */
#define NVMC_PERFCTL_ENVM_BUF_MODE                      BIT(24)                                             /*!< ENVM buffer mode */
#define NVMC_PERFCTL_FLASH_BUF_PFETCH                   BITS(22,23)                                         /*!< flash buffer prefetch control */
#define NVMC_PERFCTL_FLASH_PG_SZ                        BITS(20,21)                                         /*!< flash program size */
#define NVMC_PERFCTL_ENVM_GP_BUFSZ                      BITS(18,19)                                         /*!< ENVM general buffer size (for other masters) */
#define NVMC_PERFCTL_ENVM_CPU_BUFSZ                     BITS(16,17)                                         /*!< ENVM cpu buffer size, the buffer size takes effect after a reset */
#define NVMC_PERFCTL_DTREN                              BIT(15)                                             /*!< flash DTR mode enable bit */
#define NVMC_PERFCTL_WSCNT                              BITS(0,3)                                           /*!< wait state counter register */

/* NVMC_OBKEY */
#define NVMC_OBKEY_OBKEY                                BITS(0,31)                                          /*!< option byte unlock key bits */

/* NVMC_OBCTL */
#define NVMC_OBCTL_OBLK                                 BIT(0)                                              /*!< NVMC_OBCTL lock bit */
#define NVMC_OBCTL_OBSTART                              BIT(1)                                              /*!< send option byte change command to flash */
#define NVMC_OBCTL_OBMERRIE                             BIT(30)                                             /*!< option byte modified error interrupt enable bit */
#define NVMC_OBCTL_OBMERR                               BIT(31)                                             /*!< option byte modified error flag */

/* NVMC_OBSTAT0_EFT */
#define NVMC_OBSTAT0_EFT_DISLFI                         BIT(0)                                              /*!< effective option byte Licensed firmware install (LFI) disable bit */
#define NVMC_OBSTAT0_EFT_NOPRINT                        BIT(1)                                              /*!< effective option byte no information is printed during secure boot */
#define NVMC_OBSTAT0_EFT_BOR_TH                         BITS(2,3)                                           /*!< effective option byte brownout reset threshold value */
#define NVMC_OBSTAT0_EFT_NWDG_HW                        BIT(4)                                              /*!< effective option byte watchdog value */
#define NVMC_OBSTAT0_EFT_VFALL                          BIT(5)                                              /*!< effective option byte Firmware image verification enable bit */
#define NVMC_OBSTAT0_EFT_NRST_DPSLP                     BIT(6)                                              /*!< effective option byte deepsleep reset value */
#define NVMC_OBSTAT0_EFT_NRST_STDBY                     BIT(7)                                              /*!< effective option byte standby reset value */
#define NVMC_OBSTAT0_EFT_SPC                            BITS(8,15)                                          /*!< effective option byte security protection code */
#define NVMC_OBSTAT0_EFT_OTA_CHECK                      BIT(16)                                             /*!< effective option byte OTA check status bit */
#define NVMC_OBSTAT0_EFT_FWDGSPD_DPSLP                  BIT(17)                                             /*!< effective option byte watchdog suspend status in deepsleep mode */
#define NVMC_OBSTAT0_EFT_FWDGSPD_STDBY                  BIT(18)                                             /*!< effective option byte watchdog suspend status in standby mode */
#define NVMC_OBSTAT0_EFT_FW_UPDATE                      BIT(19)                                             /*!< effective option byte Firmware update status bit */
#define NVMC_OBSTAT0_EFT_SWAP                           BIT(20)                                             /*!< effective option byte SWAP status bit */
#define NVMC_OBSTAT0_EFT_SCR                            BIT(21)                                             /*!< effective option byte security access mode status bit */
#define NVMC_OBSTAT0_EFT_ITCMECCEN                      BIT(22)                                             /*!< effective option byte ITCM ECC function enable status bit */
#define NVMC_OBSTAT0_EFT_DTCM0ECCEN                     BIT(23)                                             /*!< effective option byte DTCM0 ECC function enable status bit */
#define NVMC_OBSTAT0_EFT_DTCM1ECCEN                     BIT(24)                                             /*!< effective option byte DTCM1 ECC function enable status bit */
#define NVMC_OBSTAT0_EFT_R2_SRAM_ECCEN                  BIT(25)                                             /*!< effective option byte Region 2 SRAM ECC enable status bit */
#define NVMC_OBSTAT0_EFT_R1_SRAM_ECCEN                  BIT(26)                                             /*!< effective option byte Region 1 SRAM ECC enable status bit */
#define NVMC_OBSTAT0_EFT_R0_SRAM_ECCEN                  BIT(27)                                             /*!< effective option byte Region 0 SRAM ECC enable status bit */
#define NVMC_OBSTAT0_EFT_ENVM_ECC_OVRD                  BIT(28)                                             /*!< effective option byte ENVM ECC override enable status bit */
#define NVMC_OBSTAT0_EFT_IOSPDOPEN                      BIT(29)                                             /*!< effective option byte I/O speed optimization, high-speed at low-voltage enable status bit */
#define NVMC_OBSTAT0_EFT_NOPIPE_MODE                    BIT(30)                                             /*!< effective option byte pipeline mode bit */
#define NVMC_OBSTAT0_EFT_VFIMG                          BIT(31)                                             /*!< effective option byte Firmware image verification enable bit */

/* NVMC_OBSTAT0_MDF */
#define NVMC_OBSTAT0_MDF_DISLFI                         BIT(0)                                              /*!< modified option byte Licensed firmware install (LFI) disable bit */
#define NVMC_OBSTAT0_MDF_NOPRINT                        BIT(1)                                              /*!< modified option byte no information is printed during secure boot */
#define NVMC_OBSTAT0_MDF_BOR_TH                         BITS(2,3)                                           /*!< modified option byte brownout reset threshold value */
#define NVMC_OBSTAT0_MDF_NWDG_HW                        BIT(4)                                              /*!< modified option byte watchdog value */
#define NVMC_OBSTAT0_MDF_VFALL                          BIT(5)                                              /*!< modified option byte Firmware image verification enable bit */
#define NVMC_OBSTAT0_MDF_NRST_DPSLP                     BIT(6)                                              /*!< modified option byte deepsleep reset value */
#define NVMC_OBSTAT0_MDF_NRST_STDBY                     BIT(7)                                              /*!< modified option byte standby reset value */
#define NVMC_OBSTAT0_MDF_SPC                            BITS(8,15)                                          /*!< modified option byte security protection code */
#define NVMC_OBSTAT0_MDF_OTA_CHECK                      BIT(16)                                             /*!< modified option byte OTA check status bit */
#define NVMC_OBSTAT0_MDF_FWDGSPD_DPSLP                  BIT(17)                                             /*!< modified option byte watchdog suspend status in deepsleep mode */
#define NVMC_OBSTAT0_MDF_FWDGSPD_STDBY                  BIT(18)                                             /*!< modified option byte watchdog suspend status in standby mode */
#define NVMC_OBSTAT0_MDF_FW_UPDATE                      BIT(19)                                             /*!< modified option byte Firmware update status bit */
#define NVMC_OBSTAT0_MDF_SWAP                           BIT(20)                                             /*!< modified option byte SWAP status bit */
#define NVMC_OBSTAT0_MDF_SCR                            BIT(21)                                             /*!< modified option byte security access mode status bit */
#define NVMC_OBSTAT0_MDF_ITCMECCEN                      BIT(22)                                             /*!< modified option byte ITCM ECC function enable status bit */
#define NVMC_OBSTAT0_MDF_DTCM0ECCEN                     BIT(23)                                             /*!< modified option byte DTCM0 ECC function enable status bit */
#define NVMC_OBSTAT0_MDF_DTCM1ECCEN                     BIT(24)                                             /*!< modified option byte DTCM1 ECC function enable status bit */
#define NVMC_OBSTAT0_MDF_R2_SRAM_ECCEN                  BIT(25)                                             /*!< modified option byte Region 2 SRAM ECC enable status bit */
#define NVMC_OBSTAT0_MDF_R1_SRAM_ECCEN                  BIT(26)                                             /*!< modified option byte Region 1 SRAM ECC enable status bit */
#define NVMC_OBSTAT0_MDF_R0_SRAM_ECCEN                  BIT(27)                                             /*!< modified option byte Region 0 SRAM ECC enable status bit */
#define NVMC_OBSTAT0_MDF_ENVM_ECC_OVRD                  BIT(28)                                             /*!< modified option byte ENVM ECC override enable status bit */
#define NVMC_OBSTAT0_MDF_IOSPDOPEN                      BIT(29)                                             /*!< modified option byte I/O speed optimization status bit */
#define NVMC_OBSTAT0_MDF_NOPIPE_MODE                    BIT(30)                                             /*!< modified option byte pipeline mode bit */
#define NVMC_OBSTAT0_MDF_VFIMG                          BIT(31)                                             /*!< modified option byte Firmware image verification enable bit */

/* NVMC_DCRPADDR_EFT */
#define NVMC_DCRPADDR_EFT_DCRP_AREA_START               BITS(0,8)                                           /*!< effective option byte DCRP area start address status bits */
#define NVMC_DCRPADDR_EFT_FLB1_DCRP_EN                  BIT(13)                                             /*!< effective option byte Flash bank1 region DCRP function enable status bits */
#define NVMC_DCRPADDR_EFT_FLB0_DCRP_EN                  BIT(14)                                             /*!< effective option byte Flash bank0 region DCRP function enable status bits */
#define NVMC_DCRPADDR_EFT_ENVM_DCRP_EN                  BIT(15)                                             /*!< effective option byte ENVM region DCRP function enable status bits */
#define NVMC_DCRPADDR_EFT_DCRP_AREA_END                 BITS(16,24)                                         /*!< effective option byte DCRP area end address status bits */
#define NVMC_DCRPADDR_EFT_FLB1_DCRP_EREN                BIT(29)                                             /*!< effective option byte Flash bank1 DCRP area erase enable status bit */
#define NVMC_DCRPADDR_EFT_FLB0_DCRP_EREN                BIT(30)                                             /*!< effective option byte Flash bank0 DCRP area erase enable status bit */
#define NVMC_DCRPADDR_EFT_ENVM_DCRP_EREN                BIT(31)                                             /*!< effective option byte ENVM DCRP area erase enable status bit */

/* NVMC_DCRPADDR_MDF */
#define NVMC_DCRPADDR_MDF_DCRP_AREA_START               BITS(0,8)                                           /*!< modified option byte DCRP area start address status bits */
#define NVMC_DCRPADDR_MDF_FLB1_DCRP_EN                  BIT(13)                                             /*!< modified option byte Flash bank1 region DCRP function enable status bits */
#define NVMC_DCRPADDR_MDF_FLB0_DCRP_EN                  BIT(14)                                             /*!< modified option byte Flash bank0 region DCRP function enable status bits */
#define NVMC_DCRPADDR_MDF_ENVM_DCRP_EN                  BIT(15)                                             /*!< modified option byte ENVM region DCRP function enable status bits */
#define NVMC_DCRPADDR_MDF_DCRP_AREA_END                 BITS(16,24)                                         /*!< modified option byte DCRP area end address status bits */
#define NVMC_DCRPADDR_MDF_FLB1_DCRP_EREN                BIT(29)                                             /*!< modified option byte Flash bank1 DCRP area erase enable status bit */
#define NVMC_DCRPADDR_MDF_FLB0_DCRP_EREN                BIT(30)                                             /*!< modified option byte Flash bank0 DCRP area erase enable status bit */
#define NVMC_DCRPADDR_MDF_ENVM_DCRP_EREN                BIT(31)                                             /*!< modified option byte ENVM DCRP area erase enable status bit */

/* NVMC_SCRADDR_EFT */
#define NVMC_SCRADDR_EFT_SCR_AREA_START                 BITS(0,8)                                           /*!< effective option byte secure area start status bits */
#define NVMC_SCRADDR_EFT_FLB1_SCR_EN                    BIT(13)                                             /*!< effective option byte Flash bank1 region SCR function enable status bits */
#define NVMC_SCRADDR_EFT_FLB0_SCR_EN                    BIT(14)                                             /*!< effective option byte Flash bank0 region SCR function enable status bits */
#define NVMC_SCRADDR_EFT_ENVM_SCR_EN                    BIT(15)                                             /*!< effective option byte ENVM region SCR function enable status bits */
#define NVMC_SCRADDR_EFT_SCR_AREA_END                   BITS(16,24)                                         /*!< effective option byte secure area end status bits */
#define NVMC_SCRADDR_EFT_FLB1_SCR_EREN                  BIT(29)                                             /*!< effective option byte Flash bank1 secure area erase enable status bit */
#define NVMC_SCRADDR_EFT_FLB0_SCR_EREN                  BIT(30)                                             /*!< effective option byte Flash bank0 secure area erase enable status bit */
#define NVMC_SCRADDR_EFT_ENVM_SCR_EREN                  BIT(31)                                             /*!< effective option byte ENVM secure area erase enable status bit */

/* NVMC_SCRADDR_MDF */
#define NVMC_SCRADDR_MDF_SCR_AREA_START                 BITS(0,8)                                           /*!< modified option byte secure area start status bits */
#define NVMC_SCRADDR_MDF_FLB1_SCR_EN                    BIT(13)                                             /*!< modified option byte Flash bank1 region SCR function enable status bits */
#define NVMC_SCRADDR_MDF_FLB0_SCR_EN                    BIT(14)                                             /*!< modified option byte Flash bank0 region SCR function enable status bits */
#define NVMC_SCRADDR_MDF_ENVM_SCR_EN                    BIT(15)                                             /*!< modified option byte ENVM region SCR function enable status bits */
#define NVMC_SCRADDR_MDF_SCR_AREA_END                   BITS(16,24)                                         /*!< modified option byte secure area end status bits */
#define NVMC_SCRADDR_MDF_FLB1_SCR_EREN                  BIT(29)                                             /*!< modified option byte Flash bank1 secure area erase enable status bit */
#define NVMC_SCRADDR_MDF_FLB0_SCR_EREN                  BIT(30)                                             /*!< modified option byte Flash bank0 secure area erase enable status bit */
#define NVMC_SCRADDR_MDF_ENVM_SCR_EREN                  BIT(31)                                             /*!< modified option byte ENVM secure area erase enable status bit */

/* NVMC_BTADDR_EFT */
#define NVMC_BTADDR_EFT_BOOT_ADDR0                      BITS(0,15)                                          /*!< effective option byte boot address 0 */
#define NVMC_BTADDR_EFT_BOOT_ADDR1                      BITS(16,31)                                         /*!< effective option byte boot address 1 */

/* NVMC_BTADDR_MDF */
#define NVMC_BTADDR_MDF_BOOT_ADDR0                      BITS(0,15)                                          /*!< modified option byte boot address 0 */
#define NVMC_BTADDR_MDF_BOOT_ADDR1                      BITS(16,31)                                         /*!< modified option byte boot address 1 */

/* NVMC_OBSTAT1_EFT */
#define NVMC_OBSTAT1_EFT_ITCM_AXI_SHARED                BITS(0,2)                                           /*!< effective option byte ITCM and AXI RAM sharing status bit */
#define NVMC_OBSTAT1_EFT_ITCM_DTCM_SHARED               BITS(4,6)                                           /*!< effective option byte ITCM and DTCM RAM sharing status bit */
#define NVMC_OBSTAT1_EFT_DATA                           BITS(16,30)                                         /*!< effective option byte User defined option byte data status value */
#define NVMC_OBSTAT1_EFT_CPUBUSTO                       BIT(31)                                             /*!< effective option byte CPU bus timeout status bit */

/* NVMC_OBSTAT1_MDF */
#define NVMC_OBSTAT1_MDF_ITCM_AXI_SHARED                BITS(0,2)                                           /*!< modified option byte ITCM and AXI RAM sharing status bit */
#define NVMC_OBSTAT1_MDF_ITCM_DTCM_SHARED               BITS(4,6)                                           /*!< modified option byte ITCM and DTCM RAM sharing status bit */
#define NVMC_OBSTAT1_MDF_DATA                           BITS(16,30)                                         /*!< modified option byte User defined option byte data status value */
#define NVMC_OBSTAT1_MDF_CPUBUSTO                       BIT(31)                                             /*!< modified option byte CPU bus timeout status bit */

#if defined(NVM_VER_CASE2)
/* case2: 3.5MB flash memory and 2MB ENVM */
/* NVMC_WP_ENVM_EFT */
#define NVMC_WP_ENVM_EFT_WP                             BITS(0,15)                                          /*!< effective option byte ENVM sector erase/program protection option status bit */

/* NVMC_WP_ENVM_EFT */
#define NVMC_WP_ENVM_MDF_WP                             BITS(0,15)                                          /*!< modified option byte ENVM sector erase/program protection option status bit */

/* NVMC_WP0_FLASH_EFT */
#define NVMC_WP0_FLASH_EFT_WP                           BITS(0,27)                                          /*!< effective option byte Flash sector erase / program protection option status bit */

/* NVMC_WP0_FLASH_MDF */
#define NVMC_WP0_FLASH_MDF_WP                           BITS(0,27)                                          /*!< modified option byte Flash sector erase / program protection option status bit */

#elif defined(NVM_VER_CASE3)
/* case3: 3.5MB flash memory and 1MB ENVM */
/* NVMC_WP_ENVM_EFT */
#define NVMC_WP_ENVM_EFT_WP                             BITS(0,7)                                           /*!< effective option byte ENVM sector erase/program protection option status bit */

/* NVMC_WP_ENVM_EFT */
#define NVMC_WP_ENVM_MDF_WP                             BITS(0,7)                                           /*!< modified option byte ENVM sector erase/program protection option status bit */

/* NVMC_WP0_FLASH_EFT */
#define NVMC_WP0_FLASH_EFT_WP                           BITS(0,27)                                          /*!< effective option byte Flash sector erase / program protection option status bit */

/* NVMC_WP0_FLASH_MDF */
#define NVMC_WP0_FLASH_MDF_WP                           BITS(0,27)                                          /*!< modified option byte Flash sector erase / program protection option status bit */

#elif defined(NVM_VER_CASE4)
/* case4: 0MB flash memory and 1.625MB ENVM */
/* NVMC_WP_ENVM_EFT */
#define NVMC_WP_ENVM_EFT_WP                             BITS(0,12)                                          /*!< effective option byte ENVM sector erase/program protection option status bit */

/* NVMC_WP_ENVM_EFT */
#define NVMC_WP_ENVM_MDF_WP                             BITS(0,12)                                          /*!< modified option byte ENVM sector erase/program protection option status bit */

#else
/* case1: 7.5MB flash memory and 2MB ENVM */
/* NVMC_WP_ENVM_EFT */
#define NVMC_WP_ENVM_EFT_WP                             BITS(0,15)                                          /*!< effective option byte ENVM sector erase/program protection option status bit */

/* NVMC_WP_ENVM_EFT */
#define NVMC_WP_ENVM_MDF_WP                             BITS(0,15)                                          /*!< modified option byte ENVM sector erase/program protection option status bit */

/* NVMC_WP0_FLASH_EFT */
#define NVMC_WP0_FLASH_EFT_WP                           BITS(0,31)                                          /*!< effective option byte Flash sector erase / program protection option status bit */

/* NVMC_WP0_FLASH_MDF */
#define NVMC_WP0_FLASH_MDF_WP                           BITS(0,31)                                          /*!< modified option byte Flash sector erase / program protection option status bit */

/* NVMC_WP1_FLASH_EFT */
#define NVMC_WP1_FLASH_EFT_WP                           BITS(0,27)                                          /*!< effective option byte Flash sector erase / program protection option status bit */

/* NVMC_WP1_FLASH_MDF */
#define NVMC_WP1_FLASH_MDF_WP                           BITS(0,27)                                          /*!< modified option byte Flash sector erase / program protection option status bit */
#endif /* NVM_VER_CASEx */

/* NVMC_OBREPCTL_EFT */
#define NVMC_OBREPCTL_EFT_SYNEN                         BIT(0)                                              /*!< effective option byte The synchronization mode enable status bit */
#define NVMC_OBREPCTL_EFT_HWREPMOD                      BIT(1)                                              /*!< effective option byte Hardware self-check mode status bit */
#define NVMC_OBREPCTL_EFT_BLREPMOD                      BIT(2)                                              /*!< effective option byte Bootloader self-check mode status bit */
#define NVMC_OBREPCTL_EFT_ERR1REPEN                     BIT(3)                                              /*!< effective option byte One bit error repair in self-repair function status bit */
#define NVMC_OBREPCTL_EFT_ERR2REPEN                     BIT(4)                                              /*!< effective option byte Two bit error repair in self-repair function status bit */
#define NVMC_OBREPCTL_EFT_BBREPLEN                      BIT(5)                                              /*!< effective option byte Bad block address replace status bit */
#define NVMC_OBREPCTL_EFT_ERR1AUTOREPEN                 BIT(6)                                              /*!< effective option byte 1 bit error automatic repair enable status bit */
#define NVMC_OBREPCTL_EFT_ERR2AUTOREPEN                 BIT(7)                                              /*!< effective option byte 2 bits error automatic repair enable status bit */
#define NVMC_OBREPCTL_EFT_SYNSZ                         BITS(8,12)                                          /*!< effective option byte Option Byte synchronize size status bit */
#define NVMC_OBREPCTL_EFT_BBMODE                        BIT(13)                                             /*!< effective option byte Mode of determining whether the repair is successful after the repair bit */
#define NVMC_OBREPCTL_EFT_SYSRSTREPDIS                  BIT(14)                                             /*!< effective option byte The system reset self-check repair function enable status bit */
#define NVMC_OBREPCTL_EFT_PORRSTREPDIS                  BIT(15)                                             /*!< effective option byte The power-on reset self-check repair function enable status bit */
#define NVMC_OBREPCTL_EFT_ERR1CNTTH                     BITS(16,20)                                         /*!< effective option byte One bit error not repaired counter interrupt threshold bits */
#define NVMC_OBREPCTL_EFT_ERR2CNTTH                     BITS(21,25)                                         /*!< effective option byte Two bit error not repaired counter interrupt threshold bits */
#define NVMC_OBREPCTL_EFT_COPYSZ                        BITS(26,31)                                         /*!< effective option byte copy size during OTA upgrade */

/* NVMC_OBREPCTL_MDF */
#define NVMC_OBREPCTL_MDF_SYNEN                         BIT(0)                                              /*!< modified option byte The synchronization mode enable status bit */
#define NVMC_OBREPCTL_MDF_HWREPMOD                      BIT(1)                                              /*!< modified option byte Hardware self-check mode status bit */
#define NVMC_OBREPCTL_MDF_BLREPMOD                      BIT(2)                                              /*!< modified option byte Bootloader self-check mode status bit */
#define NVMC_OBREPCTL_MDF_ERR1REPEN                     BIT(3)                                              /*!< modified option byte One bit error repair enable status bit */
#define NVMC_OBREPCTL_MDF_ERR2REPEN                     BIT(4)                                              /*!< modified option byte Two bit error repair enable status bit */
#define NVMC_OBREPCTL_MDF_BBREPLEN                      BIT(5)                                              /*!< modified option byte Bad block address replace status bit */
#define NVMC_OBREPCTL_MDF_ERR1AUTOREPEN                 BIT(6)                                              /*!< modified option byte 1 bit error automatic repair enable status bit */
#define NVMC_OBREPCTL_MDF_ERR2AUTOREPEN                 BIT(7)                                              /*!< modified option byte 2 bits error automatic repair enable status bit */
#define NVMC_OBREPCTL_MDF_SYNSZ                         BITS(8,12)                                          /*!< modified option byte Option Byte synchronize size bit */
#define NVMC_OBREPCTL_MDF_BBMODE                        BIT(13)                                             /*!< modified option byte Mode of determining whether the repair is successful after the repair bit */
#define NVMC_OBREPCTL_MDF_SYSRSTREPDIS                  BIT(14)                                             /*!< modified option byte The system reset self-check repair function enable status bit */
#define NVMC_OBREPCTL_MDF_PORRSTREPDIS                  BIT(15)                                             /*!< modified option byte The power-on reset self-check repair function enable status bit */
#define NVMC_OBREPCTL_MDF_ERR1CNTTH                     BITS(16,20)                                         /*!< modified option byte One bit error not repaired counter interrupt threshold bits */
#define NVMC_OBREPCTL_MDF_ERR2CNTTH                     BITS(21,25)                                         /*!< modified option byte Two bit error not repaired counter interrupt threshold bits */
#define NVMC_OBREPCTL_MDF_COPYSZ                        BITS(26,31)                                         /*!< modified option byte copy size during OTA upgrade */

/* NVMC_OBREPSTADDR_EFT */
#define NVMC_OBREPSTADDR_EFT_STARTADDR                  BITS(5,20)                                          /*!< effective option byte Self-check repair start offset address status bit */

/* NVMC_OBREPENDADDR_EFT */
#define NVMC_OBREPENDADDR_EFT_ENDADDR                   BITS(5,20)                                          /*!< effective option byte Self-check repair end offset address status bit */

/* NVMC_OBREPSTADDR_MDF */
#define NVMC_OBREPSTADDR_MDF_STARTADDR                  BITS(5,20)                                          /*!< modified option byte Self-check repair start offset address status bit */

/* NVMC_OBREPENDADDR_EFT */
#define NVMC_OBREPENDADDR_MDF_ENDADDR                   BITS(5,20)                                          /*!< modified option byte Self-check repair end offset address status bit */

/* NVMC_OBBBADDRX_EFT */
#define NVMC_OBBBADDRX_EFT_OBBBADDRVLD                  BIT(0)                                              /*!< effective option byte The bad block address recorded by the option byte valid status bit */
#define NVMC_OBBBADDRX_EFT_OBBBADDR                     BITS(5,20)                                          /*!< effective option byte Bad block address bit */

/* NVMC_OBBBADDRX_MDF */
#define NVMC_OBBBADDRX_MDF_OBBBADDRVLD                  BIT(0)                                              /*!< modified option byte The bad block address recorded by the option byte valid status bit */
#define NVMC_OBBBADDRX_MDF_OBBBADDR                     BITS(5,20)                                          /*!< modified option byte Bad block address status bit */

/* NVMC_BLANKADDR */
#define NVMC_BLANKADDR_FLBLANKADDR                      BITS(5,20)                                          /*!< flash backup blank address */

/* NVMC_PID0 */
#define NVMC_PID0_PID                                   BITS(0,31)                                          /*!< product ID 0 */

/* NVMC_PID1 */
#define NVMC_PID1_PID                                   BITS(0,31)                                          /*!< product ID 1 */

/* NVMC_NMIERRADDR */
#define NVMC_NMIERRADDR_NMIERRADDR                      BITS(5,20)                                          /*!< AXI read with uncorrectable error interrupt address */

/* OTP bits definitions */
/* NVMC_OTP_CTL */
#define NVMC_OTP_CTL_AES_KEY_CRC                        BITS(24,31)                                         /*!< 8-bits CRC calculation result value of AES key bits */
#define NVMC_OTP_CTL_OTP_REPSTART                       BIT(16)                                             /*!< OTP repair start signal */
#define NVMC_OTP_CTL_DP_CRC                             BITS(8,15)                                          /*!< 8-bits CRC calculation result value of debug password data */
#define NVMC_OTP_CTL_ENDIE                              BIT(4)                                              /*!< OTP program/repair end flag interrupt enable */
#define NVMC_OTP_CTL_PGSERRIE                           BIT(3)                                              /*!< OTP program sequence error interrupt enable bit */
#define NVMC_OTP_CTL_MULTI_DATAERRIE                    BIT(2)                                              /*!< OTP Multi-redundant data error interrupt enable bit */
#define NVMC_OTP_CTL_DATAERRIE                          BIT(1)                                              /*!< OTP data error interrupt enable bit */
#define NVMC_OTP_CTL_PGLKERRIE                          BIT(0)                                              /*!< OTP program at lock state error interrupt enable bit */

/* NVMC_OTP_STAT */
#define NVMC_OTP_STAT_PGLKERR_ADDR                      BITS(16,28)                                         /*!< low 13 bits of the logical address corresponding to the PGLKERRF */
#define NVMC_OTP_STAT_BUSY                              BIT(5)                                              /*!< OTP busy flag */
#define NVMC_OTP_STAT_ENDF                              BIT(4)                                              /*!< OTP program/repair end flag */
#define NVMC_OTP_STAT_PGSERR                            BIT(3)                                              /*!< OTP program sequence error flag */
#define NVMC_OTP_STAT_MULTI_DATAERRF                    BIT(2)                                              /*!< OTP Multi-redundant data error flag */
#define NVMC_OTP_STAT_DATAERRF                          BIT(1)                                              /*!< OTP Data error flag */
#define NVMC_OTP_STAT_PGLKERRF                          BIT(0)                                              /*!< OTP Program at lock state error flag */

/* NVMC_OTP_USER_CTL0 */
#define NVMC_OTP_USER_CTL0_DCRP_EFFCTL_LK               BIT(31)                                             /*!< DCRP_EFFCTL lock bit */
#define NVMC_OTP_USER_CTL0_SCR_EFFCTL_LK                BIT(30)                                             /*!< SCR_EFFCTL lock bit */
#define NVMC_OTP_USER_CTL0_SCR_LK                       BIT(29)                                             /*!< SCR lock bit */
#define NVMC_OTP_USER_CTL0_SPC_H_LK                     BIT(28)                                             /*!< SPC_H lock bit */
#define NVMC_OTP_USER_CTL0_SPC_L_LK                     BIT(27)                                             /*!< SPC_L lock bit */
#define NVMC_OTP_USER_CTL0_JTAGNSW_LK                   BIT(26)                                             /*!< JTAGNSW lock bit */
#define NVMC_OTP_USER_CTL0_NDBG_LK                      BITS(24,25)                                         /*!< NDBG lock bit */
#define NVMC_OTP_USER_CTL0_DCRP_EFFCTL                  BIT(23)                                             /*!< DCRP area effective control bit */
#define NVMC_OTP_USER_CTL0_SCR_EFFCTL                   BIT(22)                                             /*!< SCR area effective control bit */
#define NVMC_OTP_USER_CTL0_SCR                          BIT(21)                                             /*!< secure mode enable */
#define NVMC_OTP_USER_CTL0_SPC_H                        BIT(20)                                             /*!< configure security protection to level high */
#define NVMC_OTP_USER_CTL0_SPC_L                        BIT(19)                                             /*!< configure security protection to level low */
#define NVMC_OTP_USER_CTL0_JTAGNSW                      BIT(18)                                             /*!< SW or JTAG debugger select */
#define NVMC_OTP_USER_CTL0_NDBG                         BITS(16,17)                                         /*!< debugging permission setting */
#define NVMC_OTP_USER_CTL0_VFALL_LK                     BIT(10)                                             /*!< VFALL lock bit */
#define NVMC_OTP_USER_CTL0_VFIMG_LK                     BIT(9)                                              /*!< VFIMG lock bit */
#define NVMC_OTP_USER_CTL0_DISLFI_LK                    BIT(8)                                              /*!< DISLFI lock bit */
#define NVMC_OTP_USER_CTL0_VFALL                        BIT(2)                                              /*!< firmware image (Image must contain a certificate) verification enable bit */
#define NVMC_OTP_USER_CTL0_VFIMG                        BIT(1)                                              /*!< firmware image verification enable bit */
#define NVMC_OTP_USER_CTL0_DISLFI                       BIT(0)                                              /*!< licensed firmware install (LFI) disable */

/* NVMC_OTP_USER_CTL1 */
#define NVMC_OTP_USER_CTL1_FLBAK_DISABLE_LK             BIT(14)                                             /*!< FLBAK_DISABLE lock bit */
#define NVMC_OTP_USER_CTL1_BBREPL_DISABLE_LK            BIT(13)                                             /*!< BBREPL_DISABLE lock bit */
#define NVMC_OTP_USER_CTL1_SWAP_DISABLE_LK              BIT(12)                                             /*!< SWAP_DISABLE lock bit */
#define NVMC_OTP_USER_CTL1_SYN_MODE_LOCK_LK             BIT(11)                                             /*!< SYN_MODE_LOCK lock bit */
#define NVMC_OTP_USER_CTL1_SYN_EN_LK                    BIT(10)                                             /*!< SYN_EN lock bit */
#define NVMC_OTP_USER_CTL1_REPAIR_DISABLE_LK            BIT(9)                                              /*!< REPAIR_DISABLE lock bit */
#define NVMC_OTP_USER_CTL1_COPY_DISABLE_LK              BIT(8)                                              /*!< COPY_DISABLE lock bit */
#define NVMC_OTP_USER_CTL1_FLBAK_DISABLE                BIT(6)                                              /*!< Disable the flash backup function */
#define NVMC_OTP_USER_CTL1_BBREPL_DISABLE               BIT(5)                                              /*!< Force disable the bad block replacement function */
#define NVMC_OTP_USER_CTL1_SWAP_DISABLE                 BIT(4)                                              /*!< Disable the backup area swap function */
#define NVMC_OTP_USER_CTL1_SYN_MODE_LOCK                BIT(3)                                              /*!< Lock the current synchronization mode configuration */
#define NVMC_OTP_USER_CTL1_SYN_EN                       BIT(2)                                              /*!< Force enable the synchronization mode */
#define NVMC_OTP_USER_CTL1_REPAIR_DISABLE               BIT(1)                                              /*!< Disable self-check and repair function */
#define NVMC_OTP_USER_CTL1_COPY_DISABLE                 BIT(0)                                              /*!< Disable OTA upgrade copy function */

/* NVMC_OTP_USER_CTL2 */
#define NVMC_OTP_USER_CTL2_SCR_AREA_END                 BITS(16,24)                                         /*!< secure user area end address bits */
#define NVMC_OTP_USER_CTL2_ENVM_SCR_EN                  BIT(15)                                             /*!< ENVM region SCR function enable configuration bits */
#define NVMC_OTP_USER_CTL2_FLB0_SCR_EN                  BIT(14)                                             /*!< flash bank0 region SCR function enable configuration bits */
#define NVMC_OTP_USER_CTL2_FLB1_SCR_EN                  BIT(13)                                             /*!< flash bank1 region SCR function enable configuration bits */
#define NVMC_OTP_USER_CTL2_SCR_AREA_START               BITS(0,8)                                           /*!< secure user area start address bits */

/* NVMC_OTP_USER_CTL3 */
#define NVMC_OTP_USER_CTL3_DCRP_AREA_END                BITS(16,24)                                         /*!< DCRP area end address bits */
#define NVMC_OTP_USER_CTL3_ENVM_DCRP_EN                 BIT(15)                                             /*!< ENVM region DCRP function enable status bits */
#define NVMC_OTP_USER_CTL3_FLB0_DCRP_EN                 BIT(14)                                             /*!< flash bank0 region DCRP function enable status bits */
#define NVMC_OTP_USER_CTL3_FLB1_DCRP_EN                 BIT(13)                                             /*!< flash bank1 region DCRP function enable status bits */
#define NVMC_OTP_USER_CTL3_DCRP_AREA_START              BITS(0,8)                                           /*!< DCRP area start address bits */

/* NVMC_OTP_USER_CTL4 */
#define NVMC_OTP_USER_CTL4_ENVM_DCRP_ENLK               BIT(12)                                             /*!< ENVM_DCRP_EN of NVMC_OTP_USER_CTL2 register lock bit */
#define NVMC_OTP_USER_CTL4_FLB0_DCRP_ENLK               BIT(11)                                             /*!< FLB0_DCRP_EN of NVMC_OTP_USER_CTL2 register lock bit */
#define NVMC_OTP_USER_CTL4_FLB1_DCRP_ENLK               BIT(10)                                             /*!< FLB1_DCRP_EN of NVMC_OTP_USER_CTL2 register lock bit */
#define NVMC_OTP_USER_CTL4_ENVM_SCR_ENLK                BIT(6)                                              /*!< ENVM_SCR_EN of NVMC_OTP_USER_CTL1 register lock bit */
#define NVMC_OTP_USER_CTL4_FLB0_SCR_ENLK                BIT(5)                                              /*!< FLB0_SCR_EN of NVMC_OTP_USER_CTL1 register lock bit */
#define NVMC_OTP_USER_CTL4_FLB1_SCR_ENLK                BIT(4)                                              /*!< FLB1_SCR_EN of NVMC_OTP_USER_CTL1 register lock bit */
#define NVMC_OTP_USER_CTL4_AES_KEY_ON                   BIT(3)                                              /*!< lock OTP_AES_KEY register and enable AES decrypt function */
#define NVMC_OTP_USER_CTL4_DCRP_AREALK                  BIT(2)                                              /*!< DCRP area address lock bit */
#define NVMC_OTP_USER_CTL4_SCR_AREALK                   BIT(1)                                              /*!< Secure user area address lock bit */
#define NVMC_OTP_USER_CTL4_DPLK                         BIT(0)                                              /*!< DBGPSWD_DATA lock bit */

/* NVMC_OTP_USER_CTL5 */
#define NVMC_OTP_USER_CTL5_DUMMY1_LK                    BITS(24,31)                                         /*!< DUMMY1 lock bit */
#define NVMC_OTP_USER_CTL5_DUMMY1                       BITS(16,23)                                         /*!< reserve space for other operations */
#define NVMC_OTP_USER_CTL5_DUMMY0_LK                    BITS(8,15)                                          /*!< DUMMY0 lock bit */
#define NVMC_OTP_USER_CTL5_DUMMY0                       BITS(0,7)                                           /*!< reserve space for other operations */

/* OTP AXI program NVMC_OTP_USER_CTL0 related parameters */
#define OTP_AXI_PROGRAM_DATA_DCRP_EFFCTL_LK             BIT(15)                                             /*!< DCRP_EFFCTL lock bit */
#define OTP_AXI_PROGRAM_DATA_SCR_EFFCTL_LK              BIT(14)                                             /*!< SCR_EFFCTL lock bit */
#define OTP_AXI_PROGRAM_DATA_SCR_LK                     BIT(13)                                             /*!< SCR lock bit */
#define OTP_AXI_PROGRAM_DATA_SPC_H_LK                   BIT(12)                                             /*!< SPC_H lock bit */
#define OTP_AXI_PROGRAM_DATA_SPC_L_LK                   BIT(11)                                             /*!< SPC_L lock bit */
#define OTP_AXI_PROGRAM_DATA_JTAGNSW_LK                 BIT(10)                                             /*!< JTAGNSW lock bit */
#define OTP_AXI_PROGRAM_DATA_NDBG_LK                    BITS(8,9)                                           /*!< NDBG lock bit */
#define OTP_AXI_PROGRAM_DATA_DCRP_EFFCTL                BIT(7)                                              /*!< DCRP area effective control bit */
#define OTP_AXI_PROGRAM_DATA_SCR_EFFCTL                 BIT(6)                                              /*!< SCR area effective control bit */
#define OTP_AXI_PROGRAM_DATA_SCR                        BIT(5)                                              /*!< secure mode enable */
#define OTP_AXI_PROGRAM_DATA_SPC_H                      BIT(4)                                              /*!< configure security protection to level high */
#define OTP_AXI_PROGRAM_DATA_SPC_L                      BIT(3)                                              /*!< configure security protection to level low */
#define OTP_AXI_PROGRAM_DATA_JTAGNSW                    BIT(2)                                              /*!< SW or JTAG debugger select */
#define OTP_AXI_PROGRAM_DATA_NDBG                       BITS(0,1)                                           /*!< debugging permission setting */
#define OTP_AXI_PROGRAM_DATA_VFALL_LK                   BIT(10)                                             /*!< VFALL lock bit  */
#define OTP_AXI_PROGRAM_DATA_VFIMG_LK                   BIT(9)                                              /*!< VFIMG lock bit */
#define OTP_AXI_PROGRAM_DATA_DISLFI_LK                  BIT(8)                                              /*!< DISLFI lock bit */
#define OTP_AXI_PROGRAM_DATA_VFALL                      BIT(2)                                              /*!< Firmware image verification enable bit  */
#define OTP_AXI_PROGRAM_DATA_VFIMG                      BIT(1)                                              /*!< firmware image verification enable bit */
#define OTP_AXI_PROGRAM_DATA_DISLFI                     BIT(0)                                              /*!< licensed firmware install (LFI) disable */

/* OTP AXI program NVMC_OTP_USER_CTL1 related parameters */
#define OTP_AXI_PROGRAM_DATA_BBREPL_DISABLE_LK          BIT(13)                                             /*!< BBREPL_DISABLE lock bit */
#define OTP_AXI_PROGRAM_DATA_SWAP_DISABLE_LK            BIT(12)                                             /*!< SWAP_DISABLE lock bit */
#define OTP_AXI_PROGRAM_DATA_SYN_MODE_LOCK_LK           BIT(11)                                             /*!< SYN_MODE_LOCK lock bit */
#define OTP_AXI_PROGRAM_DATA_SYN_EN_LK                  BIT(10)                                             /*!< SYN_EN lock bit */
#define OTP_AXI_PROGRAM_DATA_REPAIR_DISABLE_LK          BIT(9)                                              /*!< REPAIR_DISABLE lock bit */
#define OTP_AXI_PROGRAM_DATA_COPY_DISABLE_LK            BIT(8)                                              /*!< COPY_DISABLE lock bit */
#define OTP_AXI_PROGRAM_DATA_BBREPL_DISABLE             BIT(5)                                              /*!< Force disable the bad block replacement function */
#define OTP_AXI_PROGRAM_DATA_SWAP_DISABLE               BIT(4)                                              /*!< Disable the backup area swap function */
#define OTP_AXI_PROGRAM_DATA_SYN_MODE_LOCK              BIT(3)                                              /*!< Lock the current synchronization mode configuration */
#define OTP_AXI_PROGRAM_DATA_SYN_EN                     BIT(2)                                              /*!< Force enable the synchronization mode */
#define OTP_AXI_PROGRAM_DATA_REPAIR_DISABLE             BIT(1)                                              /*!< Disable self-check and repair function */
#define OTP_AXI_PROGRAM_DATA_COPY_DISABLE               BIT(0)                                              /*!< Disable OTA upgrade copy function */

/* OTP AXI program NVMC_OTP_USER_CTL2 related parameters */
#define OTP_AXI_PROGRAM_DATA_SCR_AREA_END               BITS(0,8)                                           /*!< secure user area end address bits */
#define OTP_AXI_PROGRAM_DATA_ENVM_SCR_EN                BIT(15)                                             /*!< ENVM region SCR function enable configuration bits */
#define OTP_AXI_PROGRAM_DATA_FLB0_SCR_EN                BIT(14)                                             /*!< flash bank0 region SCR function enable configuration bits */
#define OTP_AXI_PROGRAM_DATA_FLB1_SCR_EN                BIT(13)                                             /*!< flash bank1 region SCR function enable configuration bits */
#define OTP_AXI_PROGRAM_DATA_SCR_AREA_START             BITS(0,8)                                           /*!< secure user area start address bits */

/* OTP AXI program NVMC_OTP_USER_CTL3 related parameters */
#define OTP_AXI_PROGRAM_DATA_DCRP_AREA_END              BITS(0,8)                                           /*!< DCRP area end address bits */
#define OTP_AXI_PROGRAM_DATA_ENVM_DCRP_EN               BIT(15)                                             /*!< ENVM region DCRP function enable status bits */
#define OTP_AXI_PROGRAM_DATA_FLB0_DCRP_EN               BIT(14)                                             /*!< flash bank0 region DCRP function enable status bits */
#define OTP_AXI_PROGRAM_DATA_FLB1_DCRP_EN               BIT(13)                                             /*!< flash bank1 region DCRP function enable status bits */
#define OTP_AXI_PROGRAM_DATA_DCRP_AREA_START            BITS(0,8)                                           /*!< DCRP area start address bits */

/* OTP AXI program NVMC_OTP_USER_CTL4 related parameters */
#define OTP_AXI_PROGRAM_DATA_ENVM_DCRP_ENLK             BIT(12)                                             /*!< ENVM_DCRP_EN of NVMC_OTP_USER_CTL2 register lock bit */
#define OTP_AXI_PROGRAM_DATA_FLB0_DCRP_ENLK             BIT(11)                                             /*!< FLB0_DCRP_EN of NVMC_OTP_USER_CTL2 register lock bit */
#define OTP_AXI_PROGRAM_DATA_FLB1_DCRP_ENLK             BIT(10)                                             /*!< FLB1_DCRP_EN of NVMC_OTP_USER_CTL2 register lock bit */
#define OTP_AXI_PROGRAM_DATA_ENVM_SCR_ENLK              BIT(6)                                              /*!< ENVM_SCR_EN of NVMC_OTP_USER_CTL1 register lock bit */
#define OTP_AXI_PROGRAM_DATA_FLB0_SCR_ENLK              BIT(5)                                              /*!< FLB0_SCR_EN of NVMC_OTP_USER_CTL1 register lock bit */
#define OTP_AXI_PROGRAM_DATA_FLB1_SCR_ENLK              BIT(4)                                              /*!< FLB1_SCR_EN of NVMC_OTP_USER_CTL1 register lock bit */
#define OTP_AXI_PROGRAM_DATA_AES_KEY_ON                 BIT(3)                                              /*!< lock OTP_AES_KEY register and enable AES decrypt function */
#define OTP_AXI_PROGRAM_DATA_DCRP_AREALK                BIT(2)                                              /*!< DCRP area address lock bit */
#define OTP_AXI_PROGRAM_DATA_SCR_AREALK                 BIT(1)                                              /*!< Secure user area address lock bit */
#define OTP_AXI_PROGRAM_DATA_DPLK                       BIT(0)                                              /*!< DBGPSWD_DATA lock bit */

/* NVMC_OTP_USER_CTL5 */
#define OTP_AXI_PROGRAM_DATA_DUMMY1_LK                  BITS(8,15)                                          /*!< DUMMY1 lock bit */
#define OTP_AXI_PROGRAM_DATA_DUMMY1                     BITS(0,7)                                           /*!< reserve space for other operations */
#define OTP_AXI_PROGRAM_DATA_DUMMY0_LK                  BITS(8,15)                                          /*!< DUMMY0 lock bit */
#define OTP_AXI_PROGRAM_DATA_DUMMY0                     BITS(0,7)                                           /*!< reserve space for other operations */

/* unlock key */
#define UNLOCK_KEY0                                     ((uint32_t)0x45670123U)                             /*!< unlock key 0 */
#define UNLOCK_KEY1                                     ((uint32_t)0xCDEF89ABU)                             /*!< unlock key 1 */

#define OB_UNLOCK_KEY0                                  ((uint32_t)0x08192A3BU)                             /*!< option byte unlock key 0 */
#define OB_UNLOCK_KEY1                                  ((uint32_t)0x4C5D6E7FU)                             /*!< option byte unlock key 1 */

#define PFINJ_UNLOCK_KEY                                ((uint32_t)0xA55A55AAU)                             /*!< ENVM program fault inject unlock key */

/* FMC wait state counter value */
#define PERFCTL_WSCNT(regval)                           (BITS(0,3) & ((uint32_t)(regval) << 0))
#define PERFCTL_WSCNT_1                                 PERFCTL_WSCNT(1)                                    /*!< 1 wait state added */
#define PERFCTL_WSCNT_2                                 PERFCTL_WSCNT(2)                                    /*!< 2 wait state added */
#define PERFCTL_WSCNT_3                                 PERFCTL_WSCNT(3)                                    /*!< 3 wait state added */
#define PERFCTL_WSCNT_4                                 PERFCTL_WSCNT(4)                                    /*!< 4 wait state added */
#define PERFCTL_WSCNT_5                                 PERFCTL_WSCNT(5)                                    /*!< 5 wait state added */
#define PERFCTL_WSCNT_6                                 PERFCTL_WSCNT(6)                                    /*!< 6 wait state added */
#define PERFCTL_WSCNT_7                                 PERFCTL_WSCNT(7)                                    /*!< 7 wait state added */
#define PERFCTL_WSCNT_8                                 PERFCTL_WSCNT(8)                                    /*!< 8 wait state added */
#define PERFCTL_WSCNT_9                                 PERFCTL_WSCNT(9)                                    /*!< 9 wait state added */
#define PERFCTL_WSCNT_10                                PERFCTL_WSCNT(10)                                   /*!< 10 wait state added */
#define PERFCTL_WSCNT_11                                PERFCTL_WSCNT(11)                                   /*!< 11 wait state added */
#define PERFCTL_WSCNT_12                                PERFCTL_WSCNT(12)                                   /*!< 12 wait state added */
#define PERFCTL_WSCNT_13                                PERFCTL_WSCNT(13)                                   /*!< 13 wait state added */
#define PERFCTL_WSCNT_14                                PERFCTL_WSCNT(14)                                   /*!< 14 wait state added */
#define PERFCTL_WSCNT_15                                PERFCTL_WSCNT(15)                                   /*!< 15 wait state added */

/* Flash buffer prefetch control */
#define PERFCTL_FLASH_BUF_PFETCH(regval)                (BITS(22,23) & ((uint32_t)(regval) << 22U))
#define FLASH_NO_PREFETCH                               PERFCTL_FLASH_BUF_PFETCH(0)                         /*!< flash no prefetch  */
#define FLASH_PREFETCH_1_LINE                           PERFCTL_FLASH_BUF_PFETCH(1)                         /*!< flash prefetch 1 line */
#define FLASH_PREFETCH_3_LINE                           PERFCTL_FLASH_BUF_PFETCH(2)                         /*!< flash prefetch 3 line */

/* Flash program size */
#define PERFCTL_FLASH_PG_SZ(regval)                     (BITS(20,21) & ((uint32_t)(regval) << 20U))
#define FLASH_PROG_SIZE_8BYTES                          PERFCTL_FLASH_PG_SZ(0)                              /*!< flash program size is 8 bytes */
#define FLASH_PROG_SIZE_32BYTES                         PERFCTL_FLASH_PG_SZ(1)                              /*!< flash program size is 32 bytes */

/* ENVM general buffer size value */
#define PERFCTL_ENVM_GP_BUFSZ(regval)                   (BITS(18,19) & ((uint32_t)(regval) << 18U))
#define ENVM_GP_BUF_SIZE1                               PERFCTL_ENVM_GP_BUFSZ(0)                            /*!< ENVM general buffer size is 1x32 bytes */
#define ENVM_GP_BUF_SIZE2                               PERFCTL_ENVM_GP_BUFSZ(1)                            /*!< ENVM general buffer size is 2x32 bytes */
#define ENVM_GP_BUF_SIZE4                               PERFCTL_ENVM_GP_BUFSZ(2)                            /*!< ENVM general buffer size is 4x32 bytes */
#define ENVM_GP_BUF_SIZE8                               PERFCTL_ENVM_GP_BUFSZ(3)                            /*!< ENVM general buffer size is 8x32 bytes */

/* ENVM CPU buffer size value */
#define PERFCTL_ENVM_CPU_BUFSZ(regval)                  (BITS(16,17) & ((uint32_t)(regval) << 16U))
#define ENVM_CPU_BUF_SIZE1                              PERFCTL_ENVM_CPU_BUFSZ(0)                           /*!< ENVM CPU buffer size is 1x32 bytes */
#define ENVM_CPU_BUF_SIZE2                              PERFCTL_ENVM_CPU_BUFSZ(1)                           /*!< ENVM CPU buffer size is 2x32 bytes */
#define ENVM_CPU_BUF_SIZE4                              PERFCTL_ENVM_CPU_BUFSZ(2)                           /*!< ENVM CPU buffer size is 4x32 bytes */
#define ENVM_CPU_BUF_SIZE8                              PERFCTL_ENVM_CPU_BUFSZ(3)                           /*!< ENVM CPU buffer size is 8x32 bytes */

/* flash DTR mode value */
#define PERFCTL_DTREN(regval)                           (BIT(15) & ((uint32_t)(regval) << 15U))
#define FLASH_DTR_MODE_ENABLE                           PERFCTL_DTREN(0)                                    /*!< Disable flash DTR mode */
#define FLASH_DTR_MODE_DISABLE                          PERFCTL_DTREN(1)                                    /*!< Enable flash DTR mode */

/* option byte Licensed firmware install configure */
#define OBSTAT0_DISLFI(regval)                          (BIT(0) & ((uint32_t)(regval) << 0U))
#define OB_LICENCE_FW_INSTALL_ENABLE                    OBSTAT0_DISLFI(0)                                   /*!< Enable licensed firmware install */
#define OB_LICENCE_FW_INSTALL_DISABLE                   OBSTAT0_DISLFI(1)                                   /*!< Disable licensed firmware install */

/* option byte information print configure */
#define OBSTAT0_NOPRINT(regval)                         (BIT(1) & ((uint32_t)(regval) << 1U))
#define OB_INFO_PRINT_DISABLE                           OBSTAT0_NOPRINT(0)                                  /*!< Disable information print during secure boot */
#define OB_INFO_PRINT_ENABLE                            OBSTAT0_NOPRINT(1)                                  /*!< Enable information print during secure boot */

/* option byte BOR threshold value */
#define OBSTAT0_BOR_TH(regval)                          (BITS(2,3) & ((uint32_t)(regval) << 2U))
#define OB_BOR_TH_OFF                                   OBSTAT0_BOR_TH(0)                                   /*!< no BOR function */
#define OB_BOR_TH_VALUE1                                OBSTAT0_BOR_TH(1)                                   /*!< BOR threshold value 1 */
#define OB_BOR_TH_VALUE2                                OBSTAT0_BOR_TH(2)                                   /*!< BOR threshold value 2 */
#define OB_BOR_TH_VALUE3                                OBSTAT0_BOR_TH(3)                                   /*!< BOR threshold value 3 */

/* option byte software/hardware free watchdog timer */
#define OBSTAT0_NWDG_HW(regval)                         (BIT(4) & ((uint32_t)(regval) << 4U))
#define OB_FWDGT_HW                                     OBSTAT0_NWDG_HW(0)                                  /*!< hardware free watchdog */
#define OB_FWDGT_SW                                     OBSTAT0_NWDG_HW(1)                                  /*!< software free watchdog */

/* option byte Firmware image (image must contain a certificate) verification */
#define OBSTAT0_VFALL(regval)                           (BIT(5) & ((uint32_t)(regval) << 5U))
#define OB_FW_IMAGE_VERIFICATION_DISABLE                OBSTAT0_VFALL(0)                                    /*!< Disable firmware image verification */
#define OB_FW_IMAGE_VERIFICATION_ENABLE                 OBSTAT0_VFALL(1)                                    /*!< Enable firmware image verification */

/* option byte reset or not entering deep sleep mode */
#define OBSTAT0_NRST_DPSLP(regval)                      (BIT(6) & ((uint32_t)(regval) << 6U))
#define OB_DEEPSLEEP_RST                                OBSTAT0_NRST_DPSLP(0)                               /*!< generate a reset instead of entering deepsleep mode */
#define OB_DEEPSLEEP_NRST                               OBSTAT0_NRST_DPSLP(1)                               /*!< no reset when entering deepsleep mode */

/* option byte reset or not entering standby mode */
#define OBSTAT0_NRST_STDBY(regval)                      (BIT(7) & ((uint32_t)(regval) << 7U))
#define OB_STDBY_RST                                    OBSTAT0_NRST_STDBY(0)                               /*!< generate a reset instead of entering standby mode */
#define OB_STDBY_NRST                                   OBSTAT0_NRST_STDBY(1)                               /*!< no reset when entering deepsleep mode */

/* option byte security protection configuration */
#define OBSTAT0_SPC(regval)                             (BITS(8,15) & ((uint32_t)(regval) << 8U))
#define OB_NSPC                                         OBSTAT0_SPC(0xAAU)                                  /*!< no protection */
#define OB_LSPC                                         OBSTAT0_SPC(0xBBU)                                  /*!< protection level low */
#define OB_HSPC                                         OBSTAT0_SPC(0xCCU)                                  /*!< protection level high */

/* option byte OTA check */
#define OBSTAT0_OTA_CHECK(regval)                       (BIT(16) & ((uint32_t)(regval) << 16U))
#define OB_OTA_CHECK_DISABLE                            OBSTAT0_OTA_CHECK(0)                                /*!< disable firmware check */
#define OB_OTA_CHECK_ENABLE                             OBSTAT0_OTA_CHECK(1)                                /*!< enable firmware check */

/* option byte FWDG status in deep-sleep mode */
#define OBSTAT0_FWDGSPD_DPSLP(regval)                   (BIT(17) & ((uint32_t)(regval)<< 17U))
#define OB_DPSLP_FWDGT_SUSPEND                          OBSTAT0_FWDGSPD_DPSLP(0)                            /*!< free watchdog is suspended in deep-sleep mode */
#define OB_DPSLP_FWDGT_RUN                              OBSTAT0_FWDGSPD_DPSLP(1)                            /*!< free watchdog is running in deep-sleep mode */

/* option byte FWDG status in standby mode */
#define OBSTAT0_FWDGSPD_STDBY(regval)                   (BIT(18) & ((uint32_t)(regval) << 18U))
#define OB_STDBY_FWDGT_SUSPEND                          OBSTAT0_FWDGSPD_STDBY(0)                            /*!< free watchdog is suspended in standby mode */
#define OB_STDBY_FWDGT_RUN                              OBSTAT0_FWDGSPD_STDBY(1)                            /*!< free watchdog is running in standby mode */

/* option byte firmware update */
#define OBSTAT0_FW_UPDATE(regval)                       (BIT(19) & ((uint32_t)(regval) << 19U))
#define OB_FW_UPDATE_DISABLE                            OBSTAT0_FW_UPDATE(0)                                /*!< disable hardware copies flash backup data to ENVM */
#define OB_FW_UPDATE_ENABLE                             OBSTAT0_FW_UPDATE(1)                                /*!< enable hardware copies flash backup data to ENVM */

/* option byte swap status */
#define OBSTAT0_SWAP(regval)                            (BIT(20) & ((uint32_t)(regval) << 20U))
#define OB_ENVM_BANK0_BACKUP                            OBSTAT0_SWAP(0)                                     /*!< ENVM content is consistent with flash bank0 backup */
#define OB_ENVM_BANK1_BACKUP                            OBSTAT0_SWAP(1)                                     /*!< ENVM content is consistent with flash bank1 backup */

/* option byte security access mode configuration */
#define OBSTAT0_SCR(regval)                             (BIT(21) & ((uint32_t)(regval) << 21U))
#define OB_SECURE_MODE_DISABLE                          OBSTAT0_SCR(0)                                      /*!< secure mode disable */
#define OB_SECURE_MODE_ENABLE                           OBSTAT0_SCR(1)                                      /*!< secure mode enable */

#define OBSTAT0_ITCMECCEN(regval)                       (BIT(22) & ((uint32_t)(regval) << 22U))
#define OB_ITCM_ECC_DISABLE                             OBSTAT0_ITCMECCEN(0)                                /*!< disable CPU ITCM ECC function */
#define OB_ITCM_ECC_ENABLE                              OBSTAT0_ITCMECCEN(1)                                /*!< enable CPU ITCM ECC function */

#define OBSTAT0_DTCM0ECCEN(regval)                      (BIT(23) & ((uint32_t)(regval) << 23U))
#define OB_DTCM0_ECC_DISABLE                            OBSTAT0_DTCM0ECCEN(0)                               /*!< disable CPU DTCM0 ECC function */
#define OB_DTCM0_ECC_ENABLE                             OBSTAT0_DTCM0ECCEN(1)                               /*!< enable CPU DTCM0 ECC function */

#define OBSTAT0_DTCM1ECCEN(regval)                      (BIT(24) & ((uint32_t)(regval) << 24U))
#define OB_DTCM1_ECC_DISABLE                            OBSTAT0_DTCM1ECCEN(0)                               /*!< disable CPU DTCM1 ECC function */
#define OB_DTCM1_ECC_ENABLE                             OBSTAT0_DTCM1ECCEN(1)                               /*!< enable CPU DTCM0 ECC function */

#define OBSTAT0_R2_SRAM_ECCEN(regval)                   (BIT(25) & ((uint32_t)(regval) << 25U))
#define OB_R2_SRAM_ECC_DISABLE                          OBSTAT0_R2_SRAM_ECCEN(0)                            /*!< disable Region 2 SRAM ECC function */
#define OB_R2_SRAM_ECC_ENABLE                           OBSTAT0_R2_SRAM_ECCEN(1)                            /*!< enable Region 2 SRAM ECC function */

#define OBSTAT0_R1_SRAM_ECCEN(regval)                   (BIT(26) & ((uint32_t)(regval) << 26U))
#define OB_R1_SRAM_ECC_DISABLE                          OBSTAT0_R1_SRAM_ECCEN(0)                            /*!< disable Region 1 SRAM ECC function */
#define OB_R1_SRAM_ECC_ENABLE                           OBSTAT0_R1_SRAM_ECCEN(1)                            /*!< enable Region 1 SRAM ECC function */

#define OBSTAT0_R0_SRAM_ECCEN(regval)                   (BIT(27) & ((uint32_t)(regval) << 27U))
#define OB_R0_SRAM_ECC_DISABLE                          OBSTAT0_R0_SRAM_ECCEN(0)                            /*!< disable Region 0 SRAM ECC function */
#define OB_R0_SRAM_ECC_ENABLE                           OBSTAT0_R0_SRAM_ECCEN(1)                            /*!< enable Region 0 SRAM ECC function */

#define OBSTAT0_ENVM_ECC_OVRD(regval)                   (BIT(28) & ((uint32_t)(regval) << 28U))
#define OB_ENVM_ECC_CONTROL_BY_ENVM_ECC_MODE            OBSTAT0_ENVM_ECC_OVRD(0)                            /*!< disable ENVM ECC function */
#define OB_ENVM_ECC_ENABLE                              OBSTAT0_ENVM_ECC_OVRD(1)                            /*!< enable ENVM ECC function */

/* option byte I/O speed optimization configuration */
#define OBSTAT0_IOSPDOPEN(regval)                       (BIT(29) & ((uint32_t)(regval) << 29U))
#define OB_IOSPDOPEN_DISABLE                            OBSTAT0_IOSPDOPEN(0)                                /*!< high-speed at low-voltage diable */
#define OB_IOSPDOPEN_ENABLE                             OBSTAT0_IOSPDOPEN(1)                                /*!< high-speed at low-voltage enable */

/* option byte pipe mode configuration */
#define OBSTAT0_NOPIPE_MODE(regval)                     (BIT(30) & ((uint32_t)(regval) << 30U))
#define OB_TIMING_FRIENDLY_MODE                         OBSTAT0_NOPIPE_MODE(0)                              /*!< Timing friendly mode */
#define OB_PERFORMANCE_FRIENDLY_MODE                    OBSTAT0_NOPIPE_MODE(1)                              /*!< Performance-first mode */

/* option byte I/O speed optimization configuration */
#define OBSTAT0_VFIMG(regval)                           (BIT(31) & ((uint32_t)(regval) << 31U))
#define OB_FIRMWARE_IMG_VERIFY_DISABLE                  OBSTAT0_VFIMG(0)                                    /*!< Disable firmware image verification */
#define OB_FIRMWARE_IMG_VERIFY_ENABLE                   OBSTAT0_VFIMG(1)                                    /*!< Enable firmware image verification */

/* option byte flash bank1 DCRP function enable configuration */
#define OB_DCRPADDR_FLB1_DCRP_EN(regval)                (BIT(13) & ((uint32_t)(regval) << 13U))
#define OB_FL_BANK1_DCRP_AREA_DISABLE                   OB_DCRPADDR_FLB1_DCRP_EN(0)                         /*!< flash bank1 DCRP function disable */
#define OB_FL_BANK1_DCRP_AREA_ENABLE                    OB_DCRPADDR_FLB1_DCRP_EN(1)                         /*!< flash bank1 DCRP function enable */

/* option byte flash bank0 DCRP function enable configuration */
#define OB_DCRPADDR_FLB0_DCRP_EN(regval)                (BIT(14) & ((uint32_t)(regval) << 14U))
#define OB_FL_BANK0_DCRP_AREA_DISABLE                   OB_DCRPADDR_FLB0_DCRP_EN(0)                         /*!< flash bank0 DCRP function disable */
#define OB_FL_BANK0_DCRP_AREA_ENABLE                    OB_DCRPADDR_FLB0_DCRP_EN(1)                         /*!< flash bank0 DCRP function enable */

/* option byte ENVM DCRP function enable configuration */
#define OB_DCRPADDR_ENVM_DCRP_EN(regval)                (BIT(15) & ((uint32_t)(regval) << 15U))
#define OB_ENVM_DCRP_AREA_DISABLE                       OB_DCRPADDR_ENVM_DCRP_EN(0)                         /*!< ENVM region DCRP function disable */
#define OB_ENVM_DCRP_AREA_ENABLE                        OB_DCRPADDR_ENVM_DCRP_EN(1)                         /*!< ENVM region DCRP function enable */

/* option byte flash bank1 DCRP erase enable configuration */
#define OB_DCRPADDR_FLB1_DCRP_EREN(regval)              (BIT(29) & ((uint32_t)(regval) << 29U))
#define OB_FL_BANK1_DCRP_AREA_ERASE_DISABLE             OB_DCRPADDR_FLB1_DCRP_EREN(0)                       /*!< flash bank1 region DCRP area erase disable */
#define OB_FL_BANK1_DCRP_AREA_ERASE_ENABLE              OB_DCRPADDR_FLB1_DCRP_EREN(1)                       /*!< flash bank1 region DCRP area erase enable */

/* option byte flash bank0 DCRP erase enable configuration */
#define OB_DCRPADDR_FLB0_DCRP_EREN(regval)              (BIT(30) & ((uint32_t)(regval) << 30U))
#define OB_FL_BANK0_DCRP_AREA_ERASE_DISABLE             OB_DCRPADDR_FLB0_DCRP_EREN(0)                       /*!< flash bank0 region DCRP area erase disable */
#define OB_FL_BANK0_DCRP_AREA_ERASE_ENABLE              OB_DCRPADDR_FLB0_DCRP_EREN(1)                       /*!< flash bank0 region DCRP area erase enable */

/* option byte ENVM DCRP erase enable configuration */
#define OB_DCRPADDR_ENVM_DCRP_EREN(regval)              (BIT(31) & ((uint32_t)(regval) << 31U))
#define OB_ENVM_DCRP_AREA_ERASE_DISABLE                 OB_DCRPADDR_ENVM_DCRP_EREN(0)                       /*!< ENVM region DCRP area erase disable */
#define OB_ENVM_DCRP_AREA_ERASE_ENABLE                  OB_DCRPADDR_ENVM_DCRP_EREN(1)                       /*!< ENVM region DCRP area erase enable */

/* option byte flash bank1 secure function enable configuration */
#define OB_SCRADDR_FLB1_SECURE_EN(regval)               (BIT(13) & ((uint32_t)(regval) << 13U))
#define OB_FL_BANK1_SECURE_AREA_DISABLE                 OB_SCRADDR_FLB1_SECURE_EN(0)                        /*!< flash bank1 region secure function disable */
#define OB_FL_BANK1_SECURE_AREA_ENABLE                  OB_SCRADDR_FLB1_SECURE_EN(1)                        /*!< flash bank1 region secure function enable */

/* option byte flash bank0 secure function enable configuration */
#define OB_SCRADDR_FLB0_SECURE_EN(regval)               (BIT(14) & ((uint32_t)(regval) << 14U))
#define OB_FL_BANK0_SECURE_AREA_DISABLE                 OB_SCRADDR_FLB0_SECURE_EN(0)                        /*!< flash bank0 region secure function disable */
#define OB_FL_BANK0_SECURE_AREA_ENABLE                  OB_SCRADDR_FLB0_SECURE_EN(1)                        /*!< flash bank0 region secure function enable */

/* option byte ENVM secure function enable configuration */
#define OB_SCRADDR_ENVM_SECURE_EN(regval)               (BIT(15) & ((uint32_t)(regval) << 15U))
#define OB_ENVM_SECURE_AREA_DISABLE                     OB_SCRADDR_ENVM_SECURE_EN(0)                        /*!< ENVM region SCR function disable */
#define OB_ENVM_SECURE_AREA_ENABLE                      OB_SCRADDR_ENVM_SECURE_EN(1)                        /*!< ENVM region SCR function enable */

/* option byte flash bank1 secure erase enable configuration */
#define OB_SCRADDR_FLB1_SECURE_EREN(regval)             (BIT(29) & ((uint32_t)(regval) << 29U))
#define OB_FL_BANK1_SECURE_AREA_ERASE_DISABLE           OB_SCRADDR_FLB1_SECURE_EREN(0)                      /*!< flash bank1 region secure area erase disable */
#define OB_FL_BANK1_SECURE_AREA_ERASE_ENABLE            OB_SCRADDR_FLB1_SECURE_EREN(1)                      /*!< flash bank1 region secure area erase enable */

/* option byte flash bank0 secure erase enable configuration */
#define OB_SCRADDR_FLB0_SECURE_EREN(regval)             (BIT(30) & ((uint32_t)(regval) << 30U))
#define OB_FL_BANK0_SECURE_AREA_ERASE_DISABLE           OB_SCRADDR_FLB0_SECURE_EREN(0)                      /*!< flash bank0 region secure area erase disable */
#define OB_FL_BANK0_SECURE_AREA_ERASE_ENABLE            OB_SCRADDR_FLB0_SECURE_EREN(1)                      /*!< flash bank0 region secure area erase enable */

/* option byte flash bank0 secure erase enable configuration */
#define OB_SCRADDR_ENVM_SECURE_EREN(regval)             (BIT(31) & ((uint32_t)(regval) << 31U))
#define OB_ENVM_SECURE_AREA_ERASE_DISABLE               OB_SCRADDR_ENVM_SECURE_EREN(0)                      /*!< ENVM region secure area erase disable */
#define OB_ENVM_SECURE_AREA_ERASE_ENABLE                OB_SCRADDR_ENVM_SECURE_EREN(1)                      /*!< ENVM region secure area erase enable */

/* option byte ENVM write protection */
#define OB_ENVM_WP_0                                    ((uint32_t)0x00000001U)                             /*!< ENVM erase/program protection of write protection sector 0~1 */
#define OB_ENVM_WP_1                                    ((uint32_t)0x00000002U)                             /*!< ENVM erase/program protection of write protection sector 2~3 */
#define OB_ENVM_WP_2                                    ((uint32_t)0x00000004U)                             /*!< ENVM erase/program protection of write protection sector 4~5 */
#define OB_ENVM_WP_3                                    ((uint32_t)0x00000008U)                             /*!< ENVM erase/program protection of write protection sector 6~7 */
#define OB_ENVM_WP_4                                    ((uint32_t)0x00000010U)                             /*!< ENVM erase/program protection of write protection sector 8~9 */
#define OB_ENVM_WP_5                                    ((uint32_t)0x00000020U)                             /*!< ENVM erase/program protection of write protection sector 10~11 */
#define OB_ENVM_WP_6                                    ((uint32_t)0x00000040U)                             /*!< ENVM erase/program protection of write protection sector 12~13 */
#define OB_ENVM_WP_7                                    ((uint32_t)0x00000080U)                             /*!< ENVM erase/program protection of write protection sector 14~15 */
#define OB_ENVM_WP_8                                    ((uint32_t)0x00000100U)                             /*!< ENVM erase/program protection of write protection sector 16~17 */
#define OB_ENVM_WP_9                                    ((uint32_t)0x00000200U)                             /*!< ENVM erase/program protection of write protection sector 18~19 */
#define OB_ENVM_WP_10                                   ((uint32_t)0x00000400U)                             /*!< ENVM erase/program protection of write protection sector 20~21 */
#define OB_ENVM_WP_11                                   ((uint32_t)0x00000800U)                             /*!< ENVM erase/program protection of write protection sector 22~23 */
#define OB_ENVM_WP_12                                   ((uint32_t)0x00001000U)                             /*!< ENVM erase/program protection of write protection sector 24~25 */
#define OB_ENVM_WP_13                                   ((uint32_t)0x00002000U)                             /*!< ENVM erase/program protection of write protection sector 26~27 */
#define OB_ENVM_WP_14                                   ((uint32_t)0x00004000U)                             /*!< ENVM erase/program protection of write protection sector 28~29 */
#define OB_ENVM_WP_15                                   ((uint32_t)0x00008000U)                             /*!< ENVM erase/program protection of write protection sector 30~31 */
#if defined(NVM_VER_CASE2)
#define OB_ENVM_WP_ALL                                  ((uint32_t)0x0000FFFFU)                             /*!< ENVM erase/program protection of all sectors */
#elif defined(NVM_VER_CASE3)
#define OB_ENVM_WP_ALL                                  ((uint32_t)0x000000FFU)                             /*!< ENVM erase/program protection of all sectors */
#elif defined(NVM_VER_CASE4)
#define OB_ENVM_WP_ALL                                  ((uint32_t)0x00001FFFU)                             /*!< ENVM erase/program protection of all sectors */
#else
#define OB_ENVM_WP_ALL                                  ((uint32_t)0x0000FFFFU)                             /*!< ENVM erase/program protection of all sectors */
#endif /* NVM_VER_CASEx */

/* option byte flash write protection */
#define OB_FL_WP_0                                      ((uint64_t)0x0000000000000001U)                     /*!< flash erase/program protection of write protection sector 32~33 */
#define OB_FL_WP_1                                      ((uint64_t)0x0000000000000002U)                     /*!< flash erase/program protection of write protection sector 34~35 */
#define OB_FL_WP_2                                      ((uint64_t)0x0000000000000004U)                     /*!< flash erase/program protection of write protection sector 36~37 */
#define OB_FL_WP_3                                      ((uint64_t)0x0000000000000008U)                     /*!< flash erase/program protection of write protection sector 38~39 */
#define OB_FL_WP_4                                      ((uint64_t)0x0000000000000010U)                     /*!< flash erase/program protection of write protection sector 40~41 */
#define OB_FL_WP_5                                      ((uint64_t)0x0000000000000020U)                     /*!< flash erase/program protection of write protection sector 42~43 */
#define OB_FL_WP_6                                      ((uint64_t)0x0000000000000040U)                     /*!< flash erase/program protection of write protection sector 44~45 */
#define OB_FL_WP_7                                      ((uint64_t)0x0000000000000080U)                     /*!< flash erase/program protection of write protection sector 46~47 */
#define OB_FL_WP_8                                      ((uint64_t)0x0000000000000100U)                     /*!< flash erase/program protection of write protection sector 48~49 */
#define OB_FL_WP_9                                      ((uint64_t)0x0000000000000200U)                     /*!< flash erase/program protection of write protection sector 50~51 */
#define OB_FL_WP_10                                     ((uint64_t)0x0000000000000400U)                     /*!< flash erase/program protection of write protection sector 52~53 */
#define OB_FL_WP_11                                     ((uint64_t)0x0000000000000800U)                     /*!< flash erase/program protection of write protection sector 54~55 */
#define OB_FL_WP_12                                     ((uint64_t)0x0000000000001000U)                     /*!< flash erase/program protection of write protection sector 56~57 */
#define OB_FL_WP_13                                     ((uint64_t)0x0000000000002000U)                     /*!< flash erase/program protection of write protection sector 58~59 */
#define OB_FL_WP_14                                     ((uint64_t)0x0000000000004000U)                     /*!< flash erase/program protection of write protection sector 60~61 */
#define OB_FL_WP_15                                     ((uint64_t)0x0000000000008000U)                     /*!< flash erase/program protection of write protection sector 62~63 */
#define OB_FL_WP_16                                     ((uint64_t)0x0000000000010000U)                     /*!< flash erase/program protection of write protection sector 64~65 */
#define OB_FL_WP_17                                     ((uint64_t)0x0000000000020000U)                     /*!< flash erase/program protection of write protection sector 66~67 */
#define OB_FL_WP_18                                     ((uint64_t)0x0000000000040000U)                     /*!< flash erase/program protection of write protection sector 68~69 */
#define OB_FL_WP_19                                     ((uint64_t)0x0000000000080000U)                     /*!< flash erase/program protection of write protection sector 70~71 */
#define OB_FL_WP_20                                     ((uint64_t)0x0000000000100000U)                     /*!< flash erase/program protection of write protection sector 72~73 */
#define OB_FL_WP_21                                     ((uint64_t)0x0000000000200000U)                     /*!< flash erase/program protection of write protection sector 74~75 */
#define OB_FL_WP_22                                     ((uint64_t)0x0000000000400000U)                     /*!< flash erase/program protection of write protection sector 76~77 */
#define OB_FL_WP_23                                     ((uint64_t)0x0000000000800000U)                     /*!< flash erase/program protection of write protection sector 78~79 */
#define OB_FL_WP_24                                     ((uint64_t)0x0000000001000000U)                     /*!< flash erase/program protection of write protection sector 80~81 */
#define OB_FL_WP_25                                     ((uint64_t)0x0000000002000000U)                     /*!< flash erase/program protection of write protection sector 82~83 */
#define OB_FL_WP_26                                     ((uint64_t)0x0000000004000000U)                     /*!< flash erase/program protection of write protection sector 84~85 */
#define OB_FL_WP_27                                     ((uint64_t)0x0000000008000000U)                     /*!< flash erase/program protection of write protection sector 86~87 */
#define OB_FL_WP_28                                     ((uint64_t)0x0000000010000000U)                     /*!< flash erase/program protection of write protection sector 88~89 */
#define OB_FL_WP_29                                     ((uint64_t)0x0000000020000000U)                     /*!< flash erase/program protection of write protection sector 90~91 */
#define OB_FL_WP_30                                     ((uint64_t)0x0000000040000000U)                     /*!< flash erase/program protection of write protection sector 92~93 */
#define OB_FL_WP_31                                     ((uint64_t)0x0000000080000000U)                     /*!< flash erase/program protection of write protection sector 94~95 */
#define OB_FL_WP_32                                     ((uint64_t)0x0000000100000000U)                     /*!< flash erase/program protection of write protection sector 96~97 */
#define OB_FL_WP_33                                     ((uint64_t)0x0000000200000000U)                     /*!< flash erase/program protection of write protection sector 98~99 */
#define OB_FL_WP_34                                     ((uint64_t)0x0000000400000000U)                     /*!< flash erase/program protection of write protection sector 100~101 */
#define OB_FL_WP_35                                     ((uint64_t)0x0000000800000000U)                     /*!< flash erase/program protection of write protection sector 102~103 */
#define OB_FL_WP_36                                     ((uint64_t)0x0000001000000000U)                     /*!< flash erase/program protection of write protection sector 104~105 */
#define OB_FL_WP_37                                     ((uint64_t)0x0000002000000000U)                     /*!< flash erase/program protection of write protection sector 106~107 */
#define OB_FL_WP_38                                     ((uint64_t)0x0000004000000000U)                     /*!< flash erase/program protection of write protection sector 108~109 */
#define OB_FL_WP_39                                     ((uint64_t)0x0000008000000000U)                     /*!< flash erase/program protection of write protection sector 110~111 */
#define OB_FL_WP_40                                     ((uint64_t)0x0000010000000000U)                     /*!< flash erase/program protection of write protection sector 112~113 */
#define OB_FL_WP_41                                     ((uint64_t)0x0000020000000000U)                     /*!< flash erase/program protection of write protection sector 114~115 */
#define OB_FL_WP_42                                     ((uint64_t)0x0000040000000000U)                     /*!< flash erase/program protection of write protection sector 116~117 */
#define OB_FL_WP_43                                     ((uint64_t)0x0000080000000000U)                     /*!< flash erase/program protection of write protection sector 118~119 */
#define OB_FL_WP_44                                     ((uint64_t)0x0000100000000000U)                     /*!< flash erase/program protection of write protection sector 120~121 */
#define OB_FL_WP_45                                     ((uint64_t)0x0000200000000000U)                     /*!< flash erase/program protection of write protection sector 122~123 */
#define OB_FL_WP_46                                     ((uint64_t)0x0000400000000000U)                     /*!< flash erase/program protection of write protection sector 124~125 */
#define OB_FL_WP_47                                     ((uint64_t)0x0000800000000000U)                     /*!< flash erase/program protection of write protection sector 126~127 */
#define OB_FL_WP_48                                     ((uint64_t)0x0001000000000000U)                     /*!< flash erase/program protection of write protection sector 128~129 */
#define OB_FL_WP_49                                     ((uint64_t)0x0002000000000000U)                     /*!< flash erase/program protection of write protection sector 130~131 */
#define OB_FL_WP_50                                     ((uint64_t)0x0004000000000000U)                     /*!< flash erase/program protection of write protection sector 132~133 */
#define OB_FL_WP_51                                     ((uint64_t)0x0008000000000000U)                     /*!< flash erase/program protection of write protection sector 134~135 */
#define OB_FL_WP_52                                     ((uint64_t)0x0010000000000000U)                     /*!< flash erase/program protection of write protection sector 136~137 */
#define OB_FL_WP_53                                     ((uint64_t)0x0020000000000000U)                     /*!< flash erase/program protection of write protection sector 138~139 */
#define OB_FL_WP_54                                     ((uint64_t)0x0040000000000000U)                     /*!< flash erase/program protection of write protection sector 140~141 */
#define OB_FL_WP_55                                     ((uint64_t)0x0080000000000000U)                     /*!< flash erase/program protection of write protection sector 142~143 */
#define OB_FL_WP_56                                     ((uint64_t)0x0100000000000000U)                     /*!< flash erase/program protection of write protection sector 144~145 */
#define OB_FL_WP_57                                     ((uint64_t)0x0200000000000000U)                     /*!< flash erase/program protection of write protection sector 146~147 */
#define OB_FL_WP_58                                     ((uint64_t)0x0400000000000000U)                     /*!< flash erase/program protection of write protection sector 148~149 */
#define OB_FL_WP_59                                     ((uint64_t)0x0800000000000000U)                     /*!< flash erase/program protection of write protection sector 150~151 */
#ifndef NVM_VER_CASE4
#if defined(NVM_VER_CASE2)
#define OB_FL_WP_ALL                                    ((uint64_t)0x000000000FFFFFFFU)                     /*!< flash erase/program protection of all sectors */
#elif defined(NVM_VER_CASE3)
#define OB_FL_WP_ALL                                    ((uint64_t)0x000000000FFFFFFFU)                     /*!< flash erase/program protection of all sectors */
#else
#define OB_FL_WP_ALL                                    ((uint64_t)0x0FFFFFFFFFFFFFFFU)                     /*!< flash erase/program protection of all sectors */
#endif /* NVM_VER_CASEx */
#endif /* NVM_VER_CASE4 */

/* option byte The power-on reset self-check repair function enable status bit */
#define OBREPCTL_PORRSTREPDIS(regval)                   (BIT(15) & ((uint32_t)(regval) << 15U))
#define POR_RESET_REPAIR_DISABLE                        OBREPCTL_PORRSTREPDIS(1)                            /*!< disable power-on reset self-check repair function */
#define POR_RESET_REPAIR_ENABLE                         OBREPCTL_PORRSTREPDIS(0)                            /*!< enable power-on reset self-check repair function */

/* option byte The system reset self-check repair function enable status bit */
#define OBREPCTL_SYSRSTREPDIS(regval)                   (BIT(14) & ((uint32_t)(regval) << 14U))
#define SYS_RESET_REPAIR_DISABLE                        OBREPCTL_SYSRSTREPDIS(1)                            /*!< disable system reset self-check repair function */
#define SYS_RESET_REPAIR_ENABLE                         OBREPCTL_SYSRSTREPDIS(0)                            /*!< enable system reset self-check repair function */

/* option byte Mode of determining whether the repair is successful after the repair */
#define OBREPCTL_BBMODE(regval)                         (BIT(13) & ((uint32_t)(regval) << 13U))
#define REPAIR_FAIL_FOR_ANY_ERROR                       OBREPCTL_BBMODE(0)                                  /*!< After repair, any error occurs as repair failure */
#define REPAIR_FAIL_FOR_MORE_THAN_ERR1                  OBREPCTL_BBMODE(1)                                  /*!< After repair, errors greater than 1 bit as repair failure */

/* option byte 2 bits error automatic repair enable status bit  */
#define OBREPCTL_ERR2AUTOREPEN(regval)                  (BIT(7) & ((uint32_t)(regval) << 7U))
#define ECC2_AUTO_REPAIR_DISABLE                        OBREPCTL_ERR2AUTOREPEN(0)                           /*!< disable 2 bits error automatic repair */
#define ECC2_AUTO_REPAIR_ENABLE                         OBREPCTL_ERR2AUTOREPEN(1)                           /*!< enable 2 bits error automatic repair */

/* option byte 1 bit error automatic repair enable status bit */
#define OBREPCTL_ERR1AUTOREPEN(regval)                  (BIT(6) & ((uint32_t)(regval) << 6U))
#define ECC1_AUTO_REPAIR_DISABLE                        OBREPCTL_ERR1AUTOREPEN(0)                           /*!< disable 1 bit error automatic repair */
#define ECC1_AUTO_REPAIR_ENABLE                         OBREPCTL_ERR1AUTOREPEN(1)                           /*!< enable 1 bit error automatic repair */

/* option byte Bad block address replace status bit */
#define OBREPCTL_BBREPLEN(regval)                       (BIT(5) & ((uint32_t)(regval) << 5U))
#define BAD_ENVM_ADDR_REPLACE_DISABLE                   OBREPCTL_BBREPLEN(0)                                /*!< the data in ENVM is not replaced with flash content when ENVM reads the bad block address */
#define BAD_ENVM_ADDR_REPLACE_ENABLE                    OBREPCTL_BBREPLEN(1)                                /*!< the data in ENVM is automatically replaced with flash content when ENVM reads the bad block address */

/* option byte Two bit error repair enable status bit */
#define OBREPCTL_ERR2REPEN(regval)                      (BIT(4) & ((uint32_t)(regval) << 4U))
#define TWO_BIT_ERR_REPAIR_DISABLE                      OBREPCTL_ERR2REPEN(0)                               /*!< disable repair address with two bit error */
#define TWO_BIT_ERR_REPAIR_ENABLE                       OBREPCTL_ERR2REPEN(1)                               /*!< enable repair address with two bit error */

/* option byte One bit error repair enable status bit */
#define OBREPCTL_ERR1REPEN(regval)                      (BIT(3) & ((uint32_t)(regval) << 3U))
#define ONE_BIT_ERR_REPAIR_DISABLE                      OBREPCTL_ERR1REPEN(0)                               /*!< disable repair address with one bit error */
#define ONE_BIT_ERR_REPAIR_ENABLE                       OBREPCTL_ERR1REPEN(1)                               /*!< enable repair address with one bit error */

/* option byte Bootloader self-check mode status bit */
#define OBREPCTL_BLREPMOD(regval)                       (BIT(2) & ((uint32_t)(regval) << 2U))
#define BOOTLOADER_CHECK_DISABLE                        OBREPCTL_BLREPMOD(0)                                /*!< boot without self-check */
#define BOOTLOADER_CHECK_ENABLE                         OBREPCTL_BLREPMOD(1)                                /*!< bootloader to do a self-check */

/* option byte Hardware self-check mode configuration */
#define OBREPCTL_HWREPMOD(regval)                       (BIT(1) & ((uint32_t)(regval) << 1U))
#define OB_HW_SELF_CHECK_DISABLE                        OBREPCTL_HWREPMOD(0)                                /*!< hardware not do self-check mode (no self-check or self-check by bootloader) */
#define OB_HW_SELF_CHECK_ENABLE                         OBREPCTL_HWREPMOD(1)                                /*!< hardware self-check mode */

/* option byte ENVM synchronization mode configuration */
#define OBREPCTL_SYNEN(regval)                          (BIT(0) & ((uint32_t)(regval) << 0U))
#define OB_SYNC_MODE_DISABLE                            OBREPCTL_SYNEN(0)                                   /*!< disable synchronization mode  */
#define OB_SYNC_MODE_ENABLE                             OBREPCTL_SYNEN(1)                                   /*!< enable synchronization mode */

/* OBSTAT1 */
/* option byte ITCM and DTCM RAM sharing status bit */
#define OBSTAT1_ITCM_DTCM_SHARED(regval)                (BITS(4,6) & ((uint32_t)(regval) << 4U))
#define OB_ITCM_DTCM_SHARED_ITCM_0KB                    OBSTAT1_ITCM_DTCM_SHARED(0)                         /*!< ITCM DTCM shared RAM ITCM size is 0Kbyte */
#define OB_ITCM_DTCM_SHARED_ITCM_64KB                   OBSTAT1_ITCM_DTCM_SHARED(1)                         /*!< ITCM DTCM shared RAM ITCM size is 64-Kbyte */
#define OB_ITCM_DTCM_SHARED_ITCM_128KB                  OBSTAT1_ITCM_DTCM_SHARED(2)                         /*!< ITCM DTCM shared RAM ITCM size is 128-Kbyte */
#define OB_ITCM_DTCM_SHARED_ITCM_192KB                  OBSTAT1_ITCM_DTCM_SHARED(3)                         /*!< ITCM DTCM shared RAM ITCM size is 192-Kbyte */
#define OB_ITCM_DTCM_SHARED_ITCM_256KB                  OBSTAT1_ITCM_DTCM_SHARED(4)                         /*!< ITCM DTCM shared RAM ITCM size is 256-Kbyte */

/* option byte ITCM and AXI RAM sharing status bit */
#define OBSTAT1_ITCM_AXI_SHARED(regval)                 (BITS(0,2) & ((uint32_t)(regval) << 0U))
#define OB_ITCM_AXI_SHARED_ITCM_0KB                     OBSTAT1_ITCM_AXI_SHARED(0)                          /*!< ITCM AXI shared RAM ITCM size is 0-Kbyte */
#define OB_ITCM_AXI_SHARED_ITCM_64KB                    OBSTAT1_ITCM_AXI_SHARED(1)                          /*!< ITCM AXI shared RAM ITCM size is 64-Kbyte */
#define OB_ITCM_AXI_SHARED_ITCM_128KB                   OBSTAT1_ITCM_AXI_SHARED(2)                          /*!< ITCM AXI shared RAM ITCM size is 128-Kbyte */
#define OB_ITCM_AXI_SHARED_ITCM_192KB                   OBSTAT1_ITCM_AXI_SHARED(3)                          /*!< ITCM AXI shared RAM ITCM size is 192-Kbyte */
#define OB_ITCM_AXI_SHARED_ITCM_256KB                   OBSTAT1_ITCM_AXI_SHARED(4)                          /*!< ITCM AXI shared RAM ITCM size is 256-Kbyte */

/* boot pin value definitions */
#define BOOT_PIN_0                                      ((uint32_t)0x00000000U)                             /*!< boot pin value is '0' */
#define BOOT_PIN_1                                      ((uint32_t)0x00000001U)                             /*!< boot pin value is '1' */

/* option byte CPU bus timeout status bit */
#define OBSTAT1_CPUBUSTO(regval)                        (BIT(31) & ((uint32_t)(regval) << 31U))
#define OB_SYSCFG_CPU_BUS_TIMEOUT_RESET_DISABLE         OBSTAT1_CPUBUSTO(0)                                 /*!<  CPU bus timeout reset value in SYSCFG_BUSTO is disable*/
#define OB_SYSCFG_CPU_BUS_TIMEOUT_RESET_ENABLE          OBSTAT1_CPUBUSTO(1)                                 /*!<  CPU bus timeout reset value in SYSCFG_BUSTO is enable*/

/* ENVM interrupt enable */
#define ENVM_INT_ENDIE                                  NVMC_ENVM_CTL_ENDIE                                 /*!< ENVM end of operation interrupt enable bit */
#define ENVM_INT_WPERRIE                                NVMC_ENVM_CTL_WPERRIE                               /*!< ENVM write protection error interrupt enable bit */
#define ENVM_INT_PGSERRIE                               NVMC_ENVM_CTL_PGSERRIE                              /*!< ENVM program sequence error interrupt enable bit */
#define ENVM_INT_RPERRIE                                NVMC_ENVM_CTL_RPERRIE                               /*!< ENVM read protection error interrupt enable bit */
#define ENVM_INT_RSERRIE                                NVMC_ENVM_CTL_RSERRIE                               /*!< ENVM read secure error interrupt enable bit */
#define ENVM_INT_AUTOECCSERRDETIE                       NVMC_ENVM_CTL_AUTOECCSERRDETIE                      /*!< ENVM one bit detect error interrupt enable bit */
#define ENVM_INT_AUTOECCDERRDETIE                       NVMC_ENVM_CTL_AUTOECCDERRDETIE                      /*!< ENVM two bits detect error interrupt enable bit */
#define ENVM_INT_CRCCALENDIE                            NVMC_ENVM_CTL_CRCENDCALIE                           /*!< ENVM CRC end of calculation interrupt enable bit */
#define ENVM_INT_CRCRERRIE                              NVMC_ENVM_CTL_CRCRERRIE                             /*!< ENVM CRC read error interrupt enable bit */
#define ENVM_INT_REPAIR_ECC1CNTIE                       NVMC_ENVM_REPCS_ECC1CNTIE                           /*!< ENVM the number of 1 bit error unrepaired is greater than or equal to the threshold interrupt enable bit  */
#define ENVM_INT_REPAIR_ECC2CNTIE                       NVMC_ENVM_REPCS_ECC2CNTIE                           /*!< ENVM the number of 2 bits error unrepaired is greater than or equal to the threshold interrupt enable bit  */
#define ENVM_INT_REPAIR_FLBLANKIE                       NVMC_ENVM_REPCS_FLBLANKIE                           /*!< ENVM Flash backup content is blank interrupt enable bit  */
#define ENVM_INT_REPAIR_DONEIE                          NVMC_ENVM_REPCS_REPDONEIE                           /*!< ENVM self-repair done interrupt enable bit  */

/* ENVM flags */
#define ENVM_FLAG_BUSY                                  NVMC_ENVM_STAT_BUSY                                 /*!< ENVM busy flag */
#define ENVM_FLAG_ENDF                                  NVMC_ENVM_STAT_ENDF                                 /*!< ENVM end of operation flag */
#define ENVM_FLAG_WPERR                                 NVMC_ENVM_STAT_WPERR                                /*!< ENVM write protection error flag */
#define ENVM_FLAG_PGSERR                                NVMC_ENVM_STAT_PGSERR                               /*!< ENVM program sequence error flag */
#define ENVM_FLAG_OTAFAIL                               NVMC_ENVM_STAT_OTAFAIL                              /*!< ENVM OTA update, check_bootloader check failure flag */
#define ENVM_FLAG_OTASUC                                NVMC_ENVM_STAT_OTASUC                               /*!< ENVM OTA update, check_bootloader check success flag */
#define ENVM_FLAG_COPYSTRERR                            NVMC_ENVM_STAT_COPYSTRERR                           /*!< ENVM copy startup failure error flag */
#define ENVM_FLAG_RPERR                                 NVMC_ENVM_STAT_RPERR                                /*!< ENVM read protection error flag */
#define ENVM_FLAG_RSERR                                 NVMC_ENVM_STAT_RSERR                                /*!< ENVM read secure error flag */
#define ENVM_FLAG_AUTOECCSERRDET                        NVMC_ENVM_STAT_AUTOECCSERRDET                       /*!< ENVM one bit detect error flag */
#define ENVM_FLAG_AUTOECCDERRDET                        NVMC_ENVM_STAT_AUTOECCDERRDET                       /*!< ENVM two bits detect error flag */
#define ENVM_FLAG_CRCBUSY                               NVMC_ENVM_STAT_CRCBUSY                              /*!< ENVM crc busy flag */
#define ENVM_FLAG_CRCCALEND                             NVMC_ENVM_STAT_CRCENDCAL                            /*!< ENVM CRC end of calculation flag */
#define ENVM_FLAG_CRCRERR                               NVMC_ENVM_STAT_CRCRERR                              /*!< ENVM CRC read error flag */
#define ENVM_FLAG_NMIERRINT                             NVMC_ENVM_REPCS_NMIERRINT                           /*!< ENVM AXI read, repair failure with 3-bit error flag */
#define ENVM_FLAG_REPAIR_ECC1CNTIF                      NVMC_ENVM_REPCS_ECC1CNTIF                           /*!< ENVM the number of 1 bit error unrepaired is greater than or equal to the threshold flag */
#define ENVM_FLAG_REPAIR_ECC2CNTIF                      NVMC_ENVM_REPCS_ECC2CNTIF                           /*!< ENVM the number of 2 bits error unrepaired is greater than or equal to the threshold flag */
#define ENVM_FLAG_REPAIR_FLBLANK                        NVMC_ENVM_REPCS_FLBLANK                             /*!< ENVM Flash backup content is blank flag */
#define ENVM_FLAG_REPAIR_END                            NVMC_ENVM_REPCS_REPEND                              /*!< ENVM self-repair end flag */
#define ENVM_FLAG_REPAIR_REPSTRERR                      NVMC_ENVM_REPCS_REPSTRERR                           /*!< ENVM self-check repair configuration error flag */

/* ENVM interrupt flags */
#define ENVM_INT_FLAG_ENDF                              ((uint8_t)0x00U)                                    /*!< ENVM end of operation interrupt flag */
#define ENVM_INT_FLAG_WPERR                             ((uint8_t)0x01U)                                    /*!< ENVM write protection error interrupt flag */
#define ENVM_INT_FLAG_PGSERR                            ((uint8_t)0x02U)                                    /*!< ENVM program sequence error interrupt flag */
#define ENVM_INT_FLAG_RPERR                             ((uint8_t)0x03U)                                    /*!< ENVM read protection error interrupt flag */
#define ENVM_INT_FLAG_RSERR                             ((uint8_t)0x04U)                                    /*!< ENVM read secure error interrupt flag */
#define ENVM_INT_FLAG_AUTOECCSERRDET                    ((uint8_t)0x05U)                                    /*!< ENVM one bit detect error interrupt flag */
#define ENVM_INT_FLAG_AUTOECCDERRDET                    ((uint8_t)0x06U)                                    /*!< ENVM two bits detect error interrupt flag */
#define ENVM_INT_FLAG_CRCCALEND                         ((uint8_t)0x07U)                                    /*!< ENVM CRC end of calculation interrupt flag */
#define ENVM_INT_FLAG_CRCRERR                           ((uint8_t)0x08U)                                    /*!< ENVM CRC read error interrupt flag */
#define ENVM_INT_FLAG_REPAIR_ECC1CNTIF                  ((uint8_t)0x09U)                                    /*!< ENVM the number of 1 bit error unrepaired is greater than or equal to the threshold interrupt flag */
#define ENVM_INT_FLAG_REPAIR_ECC2CNTIF                  ((uint8_t)0x0AU)                                    /*!< ENVM the number of 2 bits error unrepaired is greater than or equal to the threshold interrupt flag */
#define ENVM_INT_FLAG_REPAIR_FLBLANK                    ((uint8_t)0x0BU)                                    /*!< ENVM Flash backup content is blank interrupt flag */
#define ENVM_INT_FLAG_REPAIR_END                        ((uint8_t)0x0CU)                                    /*!< ENVM self-repair end interrupt flag */
#define ENVM_INT_FLAG_REPAIR_REPSTRERR                  ((uint8_t)0x0DU)                                    /*!< ENVM self-check repair configuration error interrupt flag */

/* flash interrupt enable */
#define FLASH_INT_ENDIE                                 NVMC_FLASH_CTL_ENDIE                                /*!< flash end of operation interrupt enable bit */
#define FLASH_INT_WPERRIE                               NVMC_FLASH_CTL_WPERRIE                              /*!< flash write protection error interrupt enable bit */
#define FLASH_INT_PGSERRIE                              NVMC_FLASH_CTL_PGSERRIE                             /*!< flash program sequence error interrupt enable bit */
#define FLASH_INT_RPERRIE                               NVMC_FLASH_CTL_RPERRIE                              /*!< flash read protection error interrupt enable bit */
#define FLASH_INT_RSERRIE                               NVMC_FLASH_CTL_RSERRIE                              /*!< flash read secure error interrupt enable bit */
#define FLASH_INT_ECCDETIE                              NVMC_FLASH_CTL_ECCDETIE                             /*!< flash two bits detect error interrupt enable bit */

/* flash flags */
#define FLASH_FLAG_BUSY                                 NVMC_FLASH_STAT_BUSY                                /*!< flash busy flag */
#define FLASH_FLAG_ENDF                                 NVMC_FLASH_STAT_ENDF                                /*!< flash end of operation flag */
#define FLASH_FLAG_WPERR                                NVMC_FLASH_STAT_WPERR                               /*!< flash write protection error flag */
#define FLASH_FLAG_PGSERR                               NVMC_FLASH_STAT_PGSERR                              /*!< flash program sequence error flag */
#define FLASH_FLAG_RPERR                                NVMC_FLASH_STAT_RPERR                               /*!< flash read protection error flag */
#define FLASH_FLAG_RSERR                                NVMC_FLASH_STAT_RSERR                               /*!< flash read secure error flag */
#define FLASH_FLAG_ECCDET                               NVMC_FLASH_STAT_ECCDET                              /*!< flash two bits detect error flag */

/* flash interrupt flags */
#define FLASH_INT_FLAG_ENDF                            ((uint8_t)0x00U)                                     /*!< flash end of operation interrupt flag */
#define FLASH_INT_FLAG_WPERR                           ((uint8_t)0x01U)                                     /*!< flash write protection error interrupt flag */
#define FLASH_INT_FLAG_PGSERR                          ((uint8_t)0x02U)                                     /*!< flash program sequence error interrupt flag */
#define FLASH_INT_FLAG_RPERR                           ((uint8_t)0x03U)                                     /*!< flash read protection error interrupt flag */
#define FLASH_INT_FLAG_RSERR                           ((uint8_t)0x04U)                                     /*!< flash read secure error interrupt flag */
#define FLASH_INT_FLAG_ECCDET                          ((uint8_t)0x05U)                                     /*!< flash two bits detect error interrupt flag */

/* option byte interrupt enable */
#define NVMC_INT_OBMERRIE                               NVMC_OBCTL_OBMERRIE                                 /*!< option byte modified error interrupt enable bit */

/* option byte flags */
#define NVMC_FLAG_OBMERR                                NVMC_OBCTL_OBMERR                                   /*!< option byte modified error flag */

/* option byte interrupt flags */
#define NVMC_INT_FLAG_OBMERR                           ((uint8_t)0x00U)                                    /*!< option byte modified error interrupt flag */

/* OTP interrupt enable */
#define OTP_INT_ENDIE                                   NVMC_OTP_CTL_ENDIE                                  /*!< OTP program/repair end interrupt enable bit */
#define OTP_INT_DATAERRIE                               NVMC_OTP_CTL_DATAERRIE                              /*!< OTP data error interrupt enable bit */
#define OTP_INT_PGLKERRIE                               NVMC_OTP_CTL_PGLKERRIE                              /*!< OTP program at lock state error interrupt enable bit */
#define OTP_INT_PGSERRIE                                NVMC_OTP_CTL_PGSERRIE                               /*!< OTP program sequence error enable bit */
#define OTP_INT_MULTI_DATAERRIE                         NVMC_OTP_CTL_MULTI_DATAERRIE                        /*!< OTP multi-redundant data error interrupt enable bit */

/* OTP flags */
#define OTP_FLAG_BUSY                                   NVMC_OTP_STAT_BUSY                                  /*!< OTP busy flag */
#define OTP_FLAG_ENDF                                   NVMC_OTP_STAT_ENDF                                  /*!< OTP end of operation flag */
#define OTP_FLAG_DATAERR                                NVMC_OTP_STAT_DATAERRF                              /*!< OTP data error flag */
#define OTP_FLAG_PGLKERR                                NVMC_OTP_STAT_PGLKERRF                              /*!< OTP program at lock state error flag */
#define OTP_FLAG_PGSERR                                 NVMC_OTP_STAT_PGSERR                                /*!< OTP program sequence error flag */
#define OTP_FLAG_MULTI_DATAERR                          NVMC_OTP_STAT_MULTI_DATAERRF                        /*!< OTP multi-redundant data error flag */

/* OTP interrupt flags */
#define OTP_INT_FLAG_ENDF                               ((uint8_t)0x00U)                                    /*!< OTP end of operation interrupt flag */
#define OTP_INT_FLAG_DATAERR                            ((uint8_t)0x01U)                                    /*!< OTP data error interrupt flag */
#define OTP_INT_FLAG_PGLKERR                            ((uint8_t)0x02U)                                    /*!< OTP program at lock state error interrupt flag */
#define OTP_INT_FLAG_PGSERR                             ((uint8_t)0x03U)                                    /*!< OTP program sequence error interrupt flag */
#define OTP_INT_FLAG_MULTI_DATAERR                      ((uint8_t)0x04U)                                    /*!< OTP multi-redundant data error interrupt flag */

/* address and size of NVM */
#define ENVM_BASE_ADDRESS                               ((uint32_t)0x08000000U)                             /*!< ENVM base address */
#define MAIN_FLASH_BASE_ADDRESS                         ((uint32_t)0x08200000U)                             /*!< main flash base address */
#define OTP_BASE_ADDRESS                                ((uint32_t)0x1FFF0000U)                             /*!< OTP base address */

#if defined(NVM_VER_CASE2)
/* case2: 3.5MB flash memory and 2MB ENVM */
#define MAIN_FLASH_DFLASH0_BASE_ADDRESS                 ((uint32_t)0x08200000U)                             /*!< main flash data flash0 base address */
#define MAIN_FLASH_BANK0_BASE_ADDRESS                   ((uint32_t)0x08300000U)                             /*!< main flash bank0 base address */
#define MAIN_FLASH_DFLASH1_BASE_ADDRESS                 ((uint32_t)0x08500000U)                             /*!< main flash data flash1 base address */

#define ENVM_SIZE                                       ((uint32_t)0x00200000U)                             /*!< ENVM size */
#define MAIN_FLASH_SIZE                                 ((uint32_t)0x00380000U)                             /*!< main flash size */
#define MAIN_FLASH_DFLASH0_SIZE                         ((uint32_t)0x00100000U)                             /*!< main flash data flash0 size */
#define MAIN_FLASH_BANK0_SIZE                           ((uint32_t)0x00200000U)                             /*!< main flash bank0 size */
#define MAIN_FLASH_DFLASH1_SIZE                         ((uint32_t)0x00080000U)                             /*!< main flash data flash1 size */

#elif defined(NVM_VER_CASE3)
/* case3: 3.5MB flash memory and 1MB ENVM */
#define MAIN_FLASH_DFLASH0_BASE_ADDRESS                 ((uint32_t)0x08200000U)                             /*!< main flash data flash0 base address */
#define MAIN_FLASH_BANK1_BASE_ADDRESS                   ((uint32_t)0x08300000U)                             /*!< main flash bank1 base address */
#define MAIN_FLASH_BANK0_BASE_ADDRESS                   ((uint32_t)0x08400000U)                             /*!< main flash bank0 base address */
#define MAIN_FLASH_DFLASH1_BASE_ADDRESS                 ((uint32_t)0x08500000U)                             /*!< main flash data flash1 base address */

#define ENVM_SIZE                                       ((uint32_t)0x00100000U)                             /*!< ENVM size */
#define MAIN_FLASH_SIZE                                 ((uint32_t)0x00380000U)                             /*!< main flash size */
#define MAIN_FLASH_DFLASH0_SIZE                         ((uint32_t)0x00100000U)                             /*!< main flash data flash0 size */
#define MAIN_FLASH_BANK1_SIZE                           ((uint32_t)0x00100000U)                             /*!< main flash bank1 size */
#define MAIN_FLASH_BANK0_SIZE                           ((uint32_t)0x00100000U)                             /*!< main flash bank0 size */
#define MAIN_FLASH_DFLASH1_SIZE                         ((uint32_t)0x00080000U)                             /*!< main flash data flash1 size */

#elif defined(NVM_VER_CASE4)
/* case4: 0MB flash memory and 1.625MB ENVM */
#define ENVM_SIZE                                       ((uint32_t)0x001A0000U)                             /*!< ENVM size */

#else
/* case1: 7.5MB flash memory and 2MB ENVM */
#define MAIN_FLASH_DFLASH0_BASE_ADDRESS                 ((uint32_t)0x08200000U)                             /*!< main flash data flash0 base address */
#define MAIN_FLASH_BANK1_BASE_ADDRESS                   ((uint32_t)0x08500000U)                             /*!< main flash bank1 base address */
#define MAIN_FLASH_BANK0_BASE_ADDRESS                   ((uint32_t)0x08700000U)                             /*!< main flash bank0 base address */
#define MAIN_FLASH_DFLASH1_BASE_ADDRESS                 ((uint32_t)0x08900000U)                             /*!< main flash data flash1 base address */

#define ENVM_SIZE                                       ((uint32_t)0x00200000U)                             /*!< ENVM size */
#define MAIN_FLASH_SIZE                                 ((uint32_t)0x00780000U)                             /*!< main flash size */
#define MAIN_FLASH_DFLASH0_SIZE                         ((uint32_t)0x00300000U)                             /*!< main flash data flash0 size */
#define MAIN_FLASH_BANK1_SIZE                           ((uint32_t)0x00200000U)                             /*!< main flash bank1 size */
#define MAIN_FLASH_BANK0_SIZE                           ((uint32_t)0x00200000U)                             /*!< main flash bank0 size */
#define MAIN_FLASH_DFLASH1_SIZE                         ((uint32_t)0x00080000U)                             /*!< main flash data flash1 size */
#endif /* NVM_VER_CASEx */

#define FLASH_PAGE_SIZE                                 ((uint32_t)0x00001000U)                             /*!< flash size */
#define SECTOR_SIZE                                     ((uint32_t)0x00010000U)                             /*!< sector size */
#define SYNC_SIZE_UNIT                                  ((uint32_t)0x00010000U)                             /*!< sync area granularity */
#define SCR_SIZE_UNIT                                   ((uint32_t)0x00001000U)                             /*!< secure area granularity */
#define DCRP_SIZE_UNIT                                  ((uint32_t)0x00001000U)                             /*!< DCRP area granularity */
#define REPAIR_ADDRESS_UNIT                             ((uint32_t)0x00000020U)                             /*!< repair offset address granularity */
#define BAD_BLOCK_ADDRESS_UNIT                          ((uint32_t)0x00000020U)                             /*!< bad block address granularity */

#define ENVM_TIMEOUT_COUNT                              ((uint32_t)0xFFFFFFFFU)                             /*!< count to judge of ENVM timeout */
#define FLASH_TIMEOUT_COUNT                             ((uint32_t)0xFFFFFFFFU)                             /*!< count to judge of flash timeout */
#define NVMC_TIMEOUT_COUNT                              ((uint32_t)0xFFFFFFFFU)                             /*!< count to judge of NVMC timeout */
#define OTP_TIMEOUT_COUNT                               ((uint32_t)0x00FFFFFFU)                             /*!< count to judge of OTP timeout */

#define INVLD_ADDRESS_FLAG                              ((uint8_t)0x00U)                                    /*!< the address is invalid */
#define VLD_ADDRESS_FLAG                                ((uint8_t)0x01U)                                    /*!< the address is valid */

/* OTP parameters' logical address */
#define OTP_USER_CFG_CTL0_LOW_16BITS_AXI_ADDR           ((uint32_t)0x1FFF0000U)                             /*!< AXI access address of OTP user configuration parameter in NVMC_OTP_USER_CTL0 low 16bits */
#define OTP_USER_CFG_CTL0_HIGH_16BITS_AXI_ADDR          ((uint32_t)0x1FFF0020U)                             /*!< AXI access address of OTP user configuration parameter in NVMC_OTP_USER_CTL0 high 16bits */
#define OTP_USER_CFG_CTL1_LOW_16BITS_AXI_ADDR           ((uint32_t)0x1FFF0040U)                             /*!< AXI access address of OTP user configuration parameter in NVMC_OTP_USER_CTL1 low 16bits */
#define OTP_USER_CFG_CTL2_LOW_16BITS_AXI_ADDR           ((uint32_t)0x1FFF0060U)                             /*!< AXI access address of OTP user configuration parameter in NVMC_OTP_USER_CTL2 low 16bits */
#define OTP_USER_CFG_CTL2_HIGH_16BITS_AXI_ADDR          ((uint32_t)0x1FFF0080U)                             /*!< AXI access address of OTP user configuration parameter in NVMC_OTP_USER_CTL2 high 16bits */
#define OTP_USER_CFG_CTL3_LOW_16BITS_AXI_ADDR           ((uint32_t)0x1FFF00A0U)                             /*!< AXI access address of OTP user configuration parameter in NVMC_OTP_USER_CTL3 low 16bits */
#define OTP_USER_CFG_CTL3_HIGH_16BITS_AXI_ADDR          ((uint32_t)0x1FFF00C0U)                             /*!< AXI access address of OTP user configuration parameter in NVMC_OTP_USER_CTL3 high 16bits */
#define OTP_USER_CFG_CTL4_LOW_16BITS_AXI_ADDR           ((uint32_t)0x1FFF00E0U)                             /*!< AXI access address of OTP user configuration parameter in NVMC_OTP_USER_CTL4 low 16bits */
#define OTP_USER_CFG_CTL5_LOW_16BITS_AXI_ADDR           ((uint32_t)0x1FFF0100U)                             /*!< AXI access address of OTP user configuration parameter in NVMC_OTP_USER_CTL5 low 16bits */
#define OTP_USER_CFG_CTL5_HIGH_16BITS_AXI_ADDR          ((uint32_t)0x1FFF0120U)                             /*!< AXI access address of OTP user configuration parameter in NVMC_OTP_USER_CTL5 high 16bits */
#define OTP_USER_DBGPSWD_DATA_AXI_ADDR                  ((uint32_t)0x1FFF0140U)                             /*!< AXI write access address of OTP user configuration debug password parameter */
#define OTP_AES_KEY_AXI_ADDR                            ((uint32_t)0x1FFF01C0U)                             /*!< AXI write access address of OTP user configuration aes key parameter */
#define OTP_USER_DATA_AXI_ADDR                          ((uint32_t)0x1FFF02C0U)                             /*!< AXI access address of OTP user data parameter */

/* invalid DCRP area value */
#define INVALID_DCRP_START_ADDR                         ((uint32_t)0x000000FFU)                             /*!< the start address to make DCRP area invalid */
#define INVALID_DCRP_END_ADDR                           ((uint32_t)0x00000000U)                             /*!< the end address to make DCRP area invalid */

/* invalid SCR area value */
#define INVALID_SCR_START_ADDR                          ((uint32_t)0x000000FFU)                             /*!< the start address to make SCR area invalid */
#define INVALID_SCR_END_ADDR                            ((uint32_t)0x00000000U)                             /*!< the end address to make SCR area invalid */

/* constants definitions */
/* ENVM state */
typedef enum {
    ENVM_READY = 0U,                                                                                        /*!< ENVM operation has been completed */
    ENVM_BUSY,                                                                                              /*!< ENVM operation is in progress */
    ENVM_WPERR,                                                                                             /*!< ENVM erase/program protection error */
    ENVM_PGSERR,                                                                                            /*!< ENVM program sequence error */
    ENVM_RPERR,                                                                                             /*!< ENVM read protection error */
    ENVM_RSERR,                                                                                             /*!< ENVM read secure error */
    ENVM_AUTOECCSERRDET,                                                                                    /*!< ENVM one bit detect error */
    ENVM_AUTOECCDERRDET,                                                                                    /*!< ENVM two bits detect error */
    ENVM_CRCREADY,                                                                                          /*!< ENVM CRC operation has been completed */
    ENVM_CRCBUSY,                                                                                           /*!< ENVM CRC operation is in progress */
    ENVM_CRCRERR,                                                                                           /*!< ENVM CRC read error */
    ENVM_OTAFAIL,                                                                                           /*!< ENVM OTA fail error */
    ENVM_COPYSTRERR,                                                                                        /*!< ENVM Copy startup failure error */
    ENVM_NMIERRINT,                                                                                         /*!< ENVM AXI read, repair failure with 3-bit error */
    ENVM_REPAIR_BUSY,                                                                                       /*!< ENVM repair operation is in progress */
    ENVM_REPAIR_ECC1CNTIF,                                                                                  /*!< ENVM the number of 1 bit error unrepaired is greater than or equal to the threshold */
    ENVM_REPAIR_ECC2CNTIF,                                                                                  /*!< ENVM the number of 2 bits error unrepaired is greater than or equal to the threshold */
    ENVM_REPAIR_FLBLANK,                                                                                    /*!< ENVM Flash backup content is blank error */
    ENVM_REPAIR_END,                                                                                        /*!< ENVM self-repair has been completed */
    ENVM_REPAIR_REPSTRERR,                                                                                  /*!< ENVM self-check repair configuration error */
    ENVM_TOERR                                                                                              /*!< ENVM timeout error */
} envm_state_enum;

/* flash state */
typedef enum {
    FLASH_READY = 0U,                                                                                       /*!< flash operation has been completed */
    FLASH_BUSY,                                                                                             /*!< flash operation is in progress */
    FLASH_WPERR,                                                                                            /*!< flash erase/program protection error */
    FLASH_PGSERR,                                                                                           /*!< flash program sequence error */
    FLASH_RPERR,                                                                                            /*!< flash read protection error */
    FLASH_RSERR,                                                                                            /*!< flash read secure error */
    FLASH_ECCDET,                                                                                           /*!< flash two bits detect error */
    FLASH_TOERR                                                                                             /*!< flash timeout error */
} flash_state_enum;

/* OTP state */
typedef enum {
    OTP_READY = 0U,                                                                                         /*!< OTP operation has been completed */
    OTP_BUSY,                                                                                               /*!< OTP operation is in progress */
    OTP_DATAERR,                                                                                            /*!< OTP Data error */
    OTP_PGLKERR,                                                                                            /*!< OTP Program at lock state error */
    OTP_MULTI_DATAERR,                                                                                      /*!< OTP Multi-redundant data error */
    OTP_PGSERR,                                                                                             /*!< OTP program sequence error */
    OTP_TOERR                                                                                               /*!< OTP timeout error */
} otp_state_enum;

/* NVMC state */
typedef enum {
    NVMC_OB_READY = 0U,                                                                                     /*!< option byte modification has been completed */
    NVMC_OB_BUSY,                                                                                           /*!< option byte modification is in progress */
    NVMC_OB_OBMERR,                                                                                         /*!< option byte modification error */
    NVMC_OB_REPSTRERR,                                                                                      /*!< option byte self-check repair configuration error */
    NVMC_OB_TOERR,                                                                                          /*!< option byte modification timeout error */
    NVMC_HSPC,                                                                                              /*!< high security protection */
    NVMC_OB_DCRP_SCR_PROT                                                                                   /*!< DCRP/SCR protection */
} nvmc_ob_state_enum;

/* ECC type */
typedef enum {
    ITCM_ECC = 0U,                                                                                          /*!< ITCM ECC */
    DTCM0_ECC,                                                                                              /*!< DTCM0 ECC */
    DTCM1_ECC,                                                                                              /*!< DTCM1 ECC */
    R2_SRAM_ECC,                                                                                            /*!< R2 SRAM ECC */
    R1_SRAM_ECC,                                                                                            /*!< R1 SRAM ECC */
    R0_SRAM_ECC,                                                                                            /*!< R0 SRAM ECC */
    ENVM_ECC                                                                                                /*!< ENVM ECC */
} ecc_type_enum;

/* NVMC area */
typedef enum {
    WP_AREA_ENVM  = 0U,                                                                                     /*!< ENVM area */
    WP_AREA_FLASH = 1U                                                                                      /*!< main flash area */
} wp_area_enum;

/* OTP parameter size enum, in byte unit */
typedef enum {
    BASE_USER_CFG_SIZE = 20U,                                                                               /*!< user configure size */
    USER_DP_SIZE       = 8U,                                                                                /*!< debug password size */
    AES_KEY_SIZE       = 16U,                                                                               /*!< AES key size */
    USER_DATA_SIZE     = 372U                                                                               /*!< user data size */
} otp_para_size;

/* parameter check definitions */
#ifdef FW_DEBUG_ERR_REPORT
/* check wait state counter value */
#define NOT_ENVM_PERFCTL_WSCNT(wscnt)                            (((wscnt) != PERFCTL_WSCNT_1) && ((wscnt) != PERFCTL_WSCNT_2) && ((wscnt) != PERFCTL_WSCNT_3) && \
                                                                 ((wscnt) != PERFCTL_WSCNT_4) && ((wscnt) != PERFCTL_WSCNT_5) && ((wscnt) != PERFCTL_WSCNT_6) && \
                                                                 ((wscnt) != PERFCTL_WSCNT_7) && ((wscnt) != PERFCTL_WSCNT_8) && ((wscnt) != PERFCTL_WSCNT_9) && \
                                                                 ((wscnt) != PERFCTL_WSCNT_10) && ((wscnt) != PERFCTL_WSCNT_11) && ((wscnt) != PERFCTL_WSCNT_12) && \
                                                                 ((wscnt) != PERFCTL_WSCNT_13) && ((wscnt) != PERFCTL_WSCNT_14) && ((wscnt) != PERFCTL_WSCNT_15))
/* check flash buffer prefetch */
#define NOT_FLASH_PREFETCH(line)                                 (((line) != FLASH_NO_PREFETCH) && ((line) != FLASH_PREFETCH_1_LINE) && ((line) != FLASH_PREFETCH_3_LINE))

/* check ENVM general buffer size */
#define NOT_ENVM_GP_BUF_SIZE(size)                               (((size) != ENVM_GP_BUF_SIZE1) && ((size) != ENVM_GP_BUF_SIZE2) && ((size) != ENVM_GP_BUF_SIZE4) && \
                                                                 ((size) != ENVM_GP_BUF_SIZE8))
/* check ENVM CPU buffer size */
#define NOT_ENVM_CPU_BUF_SIZE(size)                              (((size) != ENVM_CPU_BUF_SIZE1) && ((size) != ENVM_CPU_BUF_SIZE2) && ((size) != ENVM_CPU_BUF_SIZE4) && \
                                                                 ((size) != ENVM_CPU_BUF_SIZE8))
#if defined(NVM_VER_CASE2)
/* check ENVM program address */
#define NOT_ENVM_VALID_ADDRESS(address)                          (((address) < 0x08000000U) || ((address) > 0x081FFFFFU))
#elif defined(NVM_VER_CASE3)
/* check ENVM program address */
#define NOT_ENVM_VALID_ADDRESS(address)                          (((address) < 0x08000000U) || ((address) > 0x080FFFFFU))
#elif defined(NVM_VER_CASE4)
/* check ENVM program address */
#define NOT_ENVM_VALID_ADDRESS(address)                          (((address) < 0x08000000U) || ((address) > 0x0819FFFFU))
#else
/* check ENVM program address */
#define NOT_ENVM_VALID_ADDRESS(address)                          (((address) < 0x08000000U) || ((address) > 0x081FFFFFU))
#endif /* NVM_VER_CASEx */

/* check ENVM CPU buffer size */
#define NOT_BADBLOCK_VALID_ADDRESS_INDEX(index)                  (((index) != 0U) && ((index) != 1U) && ((index) != 2U) && ((index) != 3U))

#ifndef NVM_VER_CASE4
#if defined(NVM_VER_CASE2)
/* check FLASH program address */
#define NOT_FLASH_VALID_ADDRESS(address)                         (((address) < 0x08200000U) || ((address) > 0x0857FFFFU))
#elif defined(NVM_VER_CASE3)
/* check FLASH program address */
#define NOT_FLASH_VALID_ADDRESS(address)                         (((address) < 0x08200000U) || ((address) > 0x0857FFFFU))
#else
/* check FLASH program address */
#define NOT_FLASH_VALID_ADDRESS(address)                         (((address) < 0x08200000U) || ((address) > 0x0897FFFFU))
#endif /* NVM_VER_CASEx */
#endif /* NVM_VER_CASE4 */

/* check OTP program address */
#define NOT_OTP_VALID_ADDRESS(address)                           ((((address) - 0x1FFF0000U) % 0x20U != 0U) || ((address) < 0x1FFF0000U) || ((address) > (0x1FFF0000U + 0x20U * 207U)))

/* check security protection value */
#define NOT_NVMC_SPC_VALUE(ob_spc)                               (((ob_spc) != OB_NSPC) && ((ob_spc) != OB_LSPC) && ((ob_spc) != OB_HSPC))

/* check DATA value */
#define NOT_OB_DATA_RANGE(user_data)                             ((user_data) > 0x00007FFFU)

/* check ITCM size in ITCM/DTCM shared RAM */
#define NOT_OB_ITCM_DTCM_SHARED_ITCM_SIZE(itcm_size)             (((itcm_size) != OB_ITCM_DTCM_SHARED_ITCM_0KB) && ((itcm_size) != OB_ITCM_DTCM_SHARED_ITCM_64KB) && ((itcm_size) != OB_ITCM_DTCM_SHARED_ITCM_128KB) && \
                                                                 ((itcm_size) != OB_ITCM_DTCM_SHARED_ITCM_192KB) && ((itcm_size) != OB_ITCM_DTCM_SHARED_ITCM_256KB))

/* check ITCM size in ITCM/AXI shared RAM */
#define NOT_OB_ITCM_AXI_SHARED_ITCM_SIZE(itcm_size)              (((itcm_size) != OB_ITCM_AXI_SHARED_ITCM_0KB) && ((itcm_size) != OB_ITCM_AXI_SHARED_ITCM_64KB) && ((itcm_size) != OB_ITCM_AXI_SHARED_ITCM_128KB) && \
                                                                 ((itcm_size) != OB_ITCM_AXI_SHARED_ITCM_192KB) && ((itcm_size) != OB_ITCM_AXI_SHARED_ITCM_256KB))

/* check the reset value of the CPUBUSTO bit in SYSCFG_BUSTO */
#define NOT_OB_CPUBUSTO_RESET_VALUE(cpubusto_value)              (((cpubusto_value) != OB_SYSCFG_CPU_BUS_TIMEOUT_RESET_DISABLE) && ((cpubusto_value) != OB_SYSCFG_CPU_BUS_TIMEOUT_RESET_ENABLE))

/* check boot pin configuration */
#define NOT_NVMC_BOOT_PIN_VALUE(boot_pin)                        (((boot_pin) != BOOT_PIN_0) && ((boot_pin) != BOOT_PIN_1))

#if defined(NVM_VER_CASE2)
/* check DCRP area start address */
#define NOT_NVMC_DCRP_AREA_START(dcrp_start)                      ((dcrp_start) > 0x1FFU)
/* check DCRP area end address */
#define NOT_NVMC_DCRP_AREA_END(dcrp_end)                          ((dcrp_end) > 0x1FFU)
/* check SCR area start address */
#define NOT_NVMC_SCR_AREA_START(scr_start)                        ((scr_start) > 0x1FFU)
/* check SCR area end address */
#define NOT_NVMC_SCR_AREA_END(scr_end)                            ((scr_end) > 0x1FFU)
#elif defined(NVM_VER_CASE3)
/* check DCRP area start address */
#define NOT_NVMC_DCRP_AREA_START(dcrp_start)                      ((dcrp_start) > 0x0FFU)
/* check DCRP area end address */
#define NOT_NVMC_DCRP_AREA_END(dcrp_end)                          ((dcrp_end) > 0x0FFU)
/* check SCR area start address */
#define NOT_NVMC_SCR_AREA_START(scr_start)                        ((scr_start) > 0x0FFU)
/* check SCR area end address */
#define NOT_NVMC_SCR_AREA_END(scr_end)                            ((scr_end) > 0x0FFU)
#elif defined(NVM_VER_CASE4)
/* check DCRP area start address */
#define NOT_NVMC_DCRP_AREA_START(dcrp_start)                      ((dcrp_start) > 0x19FU)
/* check DCRP area end address */
#define NOT_NVMC_DCRP_AREA_END(dcrp_end)                          ((dcrp_end) > 0x19FU)
/* check SCR area start address */
#define NOT_NVMC_SCR_AREA_START(scr_start)                        ((scr_start) > 0x19FU)
/* check SCR area end address */
#define NOT_NVMC_SCR_AREA_END(scr_end)                            ((scr_end) > 0x19FU)
#else
/* check DCRP area start address */
#define NOT_NVMC_DCRP_AREA_START(dcrp_start)                      ((dcrp_start) > 0x1FFU)
/* check DCRP area end address */
#define NOT_NVMC_DCRP_AREA_END(dcrp_end)                          ((dcrp_end) > 0x1FFU)
/* check SCR area start address */
#define NOT_NVMC_SCR_AREA_START(scr_start)                        ((scr_start) > 0x1FFU)
/* check SCR area end address */
#define NOT_NVMC_SCR_AREA_END(scr_end)                            ((scr_end) > 0x1FFU)
#endif /* NVM_VER_CASEx */

/* check copy size */
#define NOT_NVMC_OB_COPY_SIZE(value)                              ((value) > 0x3FU)
/* check two bit error counter interrupt threshold during self-check */
#define NOT_NVMC_OB_ERR2CNT_THRESHOLD(err2_cnt_threshold)         ((err2_cnt_threshold) > 0x1FU)
/* check one bit error counter interrupt threshold during self-check */
#define NOT_NVMC_OB_ERR1CNT_THRESHOLD(err1_cnt_threshold)         ((err1_cnt_threshold) > 0x1FU)
/* check sync area size */
#define NOT_NVMC_OB_SYNC_SIZE(value)                              ((value) > 0x1FU)

#if defined(NVM_VER_CASE2)
/* check repair area start address */
#define NOT_NVMC_REPAIR_AREA_START(repair_start)                  ((repair_start) > 0xFFFFU)
/* check repair area end address */
#define NOT_NVMC_REPAIR_AREA_END(repair_end)                      ((repair_end) > 0xFFFFU)
/* check bad block address */
#define NOT_NVMC_BAD_BLOCK_ADDR(bad_block_address)                ((bad_block_address) > 0xFFFFU)
#elif defined(NVM_VER_CASE3)
/* check repair area start address */
#define NOT_NVMC_REPAIR_AREA_START(repair_start)                  ((repair_start) > 0x7FFFU)
/* check repair area end address */
#define NOT_NVMC_REPAIR_AREA_END(repair_end)                      ((repair_end) > 0x7FFFU)
/* check bad block address */
#define NOT_NVMC_BAD_BLOCK_ADDR(bad_block_address)                ((bad_block_address) > 0x7FFFU)
#elif defined(NVM_VER_CASE4)
/* check repair area start address */
#define NOT_NVMC_REPAIR_AREA_START(repair_start)                  ((repair_start) > 0xCFFFU)
/* check repair area end address */
#define NOT_NVMC_REPAIR_AREA_END(repair_end)                      ((repair_end) > 0xCFFFU)
/* check bad block address */
#define NOT_NVMC_BAD_BLOCK_ADDR(bad_block_address)                ((bad_block_address) > 0xCFFFU)
#else
/* check repair area start address */
#define NOT_NVMC_REPAIR_AREA_START(repair_start)                  ((repair_start) > 0xFFFFU)
/* check repair area end address */
#define NOT_NVMC_REPAIR_AREA_END(repair_end)                      ((repair_end) > 0xFFFFU)
/* check bad block address */
#define NOT_NVMC_BAD_BLOCK_ADDR(bad_block_address)                ((bad_block_address) > 0xFFFFU)
#endif /* NVM_VER_CASEx */

/* check bad block index */
#define NOT_NVMC_BAD_BLOCK_INDEX(index)                           ((index) > 0x3U)
/* check bad block addres valid status */
#define NOT_NVMC_BAD_BLOCK_VALID(bad_block_valid)                 ((bad_block_valid) > 0x1U)

#endif /* FW_DEBUG_ERR_REPORT */

/* function declarations */
/* ENVM operation functions */
/* unlock ENVM_CTL */
void nvmc_envm_unlock(void);
/* lock ENVM_CTL */
void nvmc_envm_lock(void);
/* set the ENVM wait state counter value */
void nvmc_envm_wscnt_set(uint8_t wscnt);
/* empty ENVM buffer (general buffer and CPU buffer) */
void nvmc_envm_buf_empty(void);
/* configure flash buffer */
void nvmc_flash_buffer_configure(uint32_t buf_configure);
/* configure ENVM general buffer size */
void nvmc_envm_gp_buffer_configure(uint32_t size);
/* configure ENVM CPU buffer size */
void nvmc_envm_cpu_buffer_configure(uint32_t size);
/* ENVM program 32 bytes at the corresponding address */
ErrStatus nvmc_envm_32bytes_program(uint32_t address, uint64_t data[]);
/* ENVM erase sector */
envm_state_enum nvmc_envm_sector_erase(uint32_t address);
/* erase whole ENVM */
envm_state_enum nvmc_envm_mass_erase(void);
/* erase whole chip */
ErrStatus nvmc_typical_chip_erase(void);
/*erase whole chip, including ENVM and flash*/
ErrStatus nvmc_protection_removed_chip_erase(void);
/* CRC calculate ENVM data block */
envm_state_enum nvmc_envm_crc_calculate(uint32_t *crc_value);
/* software trigger ENVM repair */
envm_state_enum nvmc_envm_repair(void);
/* get ENVM read only bad block address after self-check */
uint8_t nvmc_envm_read_only_bad_block_address_get(uint32_t index, uint32_t *bad_block_address, uint32_t *error_stat);

/* ENVM interrupts and flags management functions */
/* get ENVM operation flag */
FlagStatus nvmc_envm_flag_get(uint32_t envm_flag);
/* clear ENVM pending flag */
void nvmc_envm_flag_clear(uint32_t envm_flag);
/* enable ENVM interrupt */
void nvmc_envm_interrupt_enable(uint32_t envm_int);
/* disable ENVM interrupt */
void nvmc_envm_interrupt_disable(uint32_t envm_int);
/* get ENVM operation interrupt flag */
FlagStatus nvmc_envm_interrupt_flag_get(uint8_t envm_int_flag);
/* clear ENVM operation interrupt flag */
void nvmc_envm_interrupt_flag_clear(uint8_t envm_int_flag);
/* get ENVM repair pending flag */
FlagStatus nvmc_envm_repair_flag_get(uint32_t envm_repair_flag);
/* clear ENVM repair pending flag */
void nvmc_envm_repair_flag_clear(uint32_t envm_repair_flag);
/* enable ENVM repair interrupt */
void nvmc_envm_repair_interrupt_enable(uint32_t envm_repair_int);
/* disable ENVM repair interrupt */
void nvmc_envm_repair_interrupt_disable(uint32_t envm_repair_int);
/* get ENVM repair operation interrupt flag */
FlagStatus nvmc_envm_repair_interrupt_flag_get(uint8_t envm_repair_int_flag);
/* clear ENVM repair operation interrupt flag */
void nvmc_envm_repair_interrupt_flag_clear(uint8_t envm_repair_int_flag);
/* get the number of 2 bit error occurrences without repair during the self-check */
uint8_t nvmc_envm_ecc2_cnt_without_repair_get(void);
/* get the number of 1 bit error occurrences without repair during the self-check */
uint8_t nvmc_envm_ecc1_cnt_without_repair_get(void);
/* get the number of error count fail to repair during self-check */
uint8_t nvmc_envm_robbcnt_get(void);
/* get the error status during self-check */
uint8_t nvmc_envm_repair_error_status_get(void);
/* clear the number of 2 bit error occurrences without repair during self-check */
void nvmc_envm_ecc2_cnt_without_repair_clear(void);
/* clear the number of 1 bit error occurrences without repair during self-check */
void nvmc_envm_ecc1_cnt_without_repair_clear(void);
/* clear the number of error count fail to repair during self-check */
void nvmc_envm_robbcnt_clear(void);
/* clear error status during self-check */
void nvmc_envm_repair_error_status_clear(void);
/* clear the result of repair during self-check, including counter, error address, and status */
void nvmc_envm_self_check_repair_flag_clear(void);
/* clear the result of repair during ENVM axi read, including counter, error address, and status */
void nvmc_envm_axi_read_repair_flag_clear(void);

/* flash operation functions */
/* unlock NVMC_FLASH_CTL */
void nvmc_flash_unlock(void);
/* lock NVMC_FLASH_CTL */
void nvmc_flash_lock(void);
/* flash program a double-word at the corresponding address */
flash_state_enum nvmc_flash_doubleword_program(uint32_t address, uint64_t data);
/* flash program 32 bytes at the corresponding address */
flash_state_enum nvmc_flash_32bytes_program(uint32_t address, uint64_t data[]);
/* flash erase page */
flash_state_enum nvmc_flash_page_erase(uint32_t address);
/* flash erase sector */
flash_state_enum nvmc_flash_sector_erase(uint32_t address);
/* flash erase whole chip */
flash_state_enum nvmc_flash_mass_erase(void);

/* flash interrupts and flags management functions */
/* get flash operation flag */
FlagStatus nvmc_flash_flag_get(uint32_t flash_flag);
/* clear flash pending flag */
void nvmc_flash_flag_clear(uint32_t flash_flag);
/* enable flash interrupt */
void nvmc_flash_interrupt_enable(uint32_t flash_int);
/* disable flash interrupt */
void nvmc_flash_interrupt_disable(uint32_t flash_int);
/* get flash interrupt operation flag */
FlagStatus nvmc_flash_interrupt_flag_get(uint8_t flash_int_flag);
/* clear flash interrupt pending flag */
void nvmc_flash_interrupt_flag_clear(uint8_t flash_int_flag);

/* OTP operation functions */
otp_state_enum nvmc_otp_halfword_program(uint32_t address, uint16_t data);
/* program all AES KEY parameter */
otp_state_enum nvmc_otp_aes_key_all_program(uint16_t buf[]);
/* program all debug password parameter */
otp_state_enum nvmc_otp_dp_all_program(uint16_t buf[]);
/* program all user data parameter */
otp_state_enum nvmc_otp_user_data_all_program(uint16_t buf[]);
/* start OTP repair */
otp_state_enum nvmc_otp_repair(void);

/* OTP interrupts and flags management functions */
/* get OTP operation flag */
FlagStatus nvmc_otp_flag_get(uint32_t otp_flag);
/* clear OTP pending flag */
void nvmc_otp_flag_clear(uint32_t otp_flag);
/* enable OTP interrupt */
void nvmc_otp_interrupt_enable(uint32_t otp_int);
/* disable OTP interrupt */
void nvmc_otp_interrupt_disable(uint32_t otp_int);
/* get OTP interrupt operation flag */
FlagStatus nvmc_otp_interrupt_flag_get(uint8_t otp_int_flag);
/* clear OTP interrupt pending flag */
void nvmc_otp_interrupt_flag_clear(uint8_t otp_int_flag);

/* NVMC option bytes operation functions */
/* unlock the option byte operation */
void nvmc_ob_unlock(void);
/* lock the option byte operation */
void nvmc_ob_lock(void);
/* send option byte change command */
nvmc_ob_state_enum nvmc_ob_start(void);
/* modified option byte to factory value */
nvmc_ob_state_enum nvmc_ob_factory_value_config(void);
/* program the option bytes OBSTAT0 */
nvmc_ob_state_enum nvmc_ob_obstat0_config(uint32_t ob_obstat0, uint32_t ob_obstat0_mask);
/* enable secure access mode */
nvmc_ob_state_enum nvmc_ob_secure_access_mode_enable(void);
/* disable secure access mode */
nvmc_ob_state_enum nvmc_ob_secure_access_mode_disable(void);
/* configure the option byte security protection */
nvmc_ob_state_enum nvmc_ob_security_protection_config(uint32_t ob_spc);
/* program low power related option byte configuration */
nvmc_ob_state_enum nvmc_ob_low_power_write(uint32_t ob_deepsleep, uint32_t ob_stdby, uint32_t ob_fwdg_suspend_deepsleep,
        uint32_t ob_fwdg_suspend_standby);
/* configure option byte BOR threshold value */
nvmc_ob_state_enum nvmc_ob_bor_threshold_config(uint32_t ob_bor_th);
/* enable hardware watchdog */
nvmc_ob_state_enum nvmc_ob_hardware_watchdog_enable(void);
/* disable hardware watchdog */
nvmc_ob_state_enum nvmc_ob_hardware_watchdog_disable(void);
/* enable OTA check */
nvmc_ob_state_enum nvmc_ob_ota_check_enable(void);
/* disable OTA check */
nvmc_ob_state_enum nvmc_ob_ota_check_disable(void);
/* enable firmware update */
nvmc_ob_state_enum nvmc_ob_firmware_update_enable(void);
/* disable firmware update */
nvmc_ob_state_enum nvmc_ob_firmware_update_disable(void);
/* configure option byte bank0 as backup of ENVM */
nvmc_ob_state_enum nvmc_ob_flash_bank0_backup_envm(void);
/* configure option byte bank1 as backup of ENVM */
nvmc_ob_state_enum nvmc_ob_flash_bank1_backup_envm(void);
/* enable ecc */
nvmc_ob_state_enum nvmc_ob_ecc_enable(uint32_t ecc_type);
/* disable ecc */
nvmc_ob_state_enum nvmc_ob_ecc_disable(uint32_t ecc_type);
/* enable pipeline mode */
nvmc_ob_state_enum nvmc_ob_pipemode_enable(void);
/* disable pipeline mode */
nvmc_ob_state_enum nvmc_ob_pipemode_disable(void);

/* program the option bytes OBSTAT1 */
nvmc_ob_state_enum nvmc_ob_obstat1_config(uint32_t ob_obstat1, uint32_t ob_obstat1_mask);
/* program the option bytes user data */
nvmc_ob_state_enum nvmc_ob_data_config(uint32_t user_data);
/* configure option byte ITCM size in ITCM/DTCM shared RAM */
nvmc_ob_state_enum nvmc_ob_itcm_dtcm_shared_ram_config(uint32_t itcm_size);
/* configure option byte ITCM size in ITCM/AXI shared RAM */
nvmc_ob_state_enum nvmc_ob_itcm_axi_shared_ram_config(uint32_t itcm_size);
/* configure option byte CPUBUSTO */
nvmc_ob_state_enum nvmc_ob_cpubusto_reset_value_config(uint32_t cpubusto_value);

/* configure boot address */
nvmc_ob_state_enum nvmc_ob_boot_address_config(uint32_t boot_pin, uint16_t boot_address);
/* configure DCRP area */
nvmc_ob_state_enum nvmc_ob_dcrp_area_config(uint32_t dcrp_en, uint32_t dcrp_eren, uint32_t dcrp_start, uint32_t dcrp_end);
/* configure secure area */
nvmc_ob_state_enum nvmc_ob_secure_area_config(uint32_t scr_en, uint32_t scr_eren, uint32_t scr_start, uint32_t scr_end);
/* enable write protect */
nvmc_ob_state_enum nvmc_ob_write_protection_enable(wp_area_enum wp_area, uint64_t ob_wp);
/* disable write protect */
nvmc_ob_state_enum nvmc_ob_write_protection_disable(wp_area_enum wp_area, uint64_t ob_wp);

/* configure repair control */
nvmc_ob_state_enum nvmc_ob_repair_control_config(uint32_t ob_repctl, uint32_t ob_repctl_mask);
/* configure copy size during OTA update */
nvmc_ob_state_enum nvmc_ob_copysz_config(uint32_t value);
/* configure two bit error counter interrupt threshold during self-check */
nvmc_ob_state_enum nvmc_ob_ecc2cntth_config(uint32_t err2_cnt_threshold);
/* configure one bit error counter interrupt threshold during self-check */
nvmc_ob_state_enum nvmc_ob_ecc1cntth_config(uint32_t err1_cnt_threshold);
/* configure sync area size */
nvmc_ob_state_enum nvmc_ob_sync_area_size_config(uint32_t value);
/* enable power-on reset self-check repair   */
nvmc_ob_state_enum nvmc_ob_power_on_self_check_enable(void);
/* disable power-on reset self-check repair   */
nvmc_ob_state_enum nvmc_ob_power_on_self_check_disable(void);
/* enable system reset self-check repair   */
nvmc_ob_state_enum nvmc_ob_system_reset_check_enable(void);
/* disable system reset self-check repair   */
nvmc_ob_state_enum nvmc_ob_system_reset_check_disable(void);
/* enable 2 bits error automatic repair  */
nvmc_ob_state_enum nvmc_ob_ecc2_auto_repair_enable(void);
/* disable 2 bits error automatic repair  */
nvmc_ob_state_enum nvmc_ob_ecc2_auto_repair_disable(void);
/* enable 1 bit error automatic repair  */
nvmc_ob_state_enum nvmc_ob_ecc1_auto_repair_enable(void);
/* disable 1 bit error automatic repair  */
nvmc_ob_state_enum nvmc_ob_ecc1_auto_repair_disable(void);
/* enable bad data replacement in ENVM with flash content  */
nvmc_ob_state_enum nvmc_ob_envm_bad_data_replace_enable(void);
/* disable bad data replacement in ENVM with flash content  */
nvmc_ob_state_enum nvmc_ob_envm_bad_data_replace_disable(void);
/* enable repair data with two bits error during self check  */
nvmc_ob_state_enum nvmc_ob_two_bits_error_repair_enable(void);
/* disable repair data with two bits error during self check  */
nvmc_ob_state_enum nvmc_ob_two_bits_error_repair_disable(void);
/* enable repair data with one bit error during self check  */
nvmc_ob_state_enum nvmc_ob_one_bit_error_repair_enable(void);
/* disable repair data with one bit error during self check  */
nvmc_ob_state_enum nvmc_ob_one_bit_error_repair_disable(void);
/* enable bootloader self-check  */
nvmc_ob_state_enum nvmc_ob_bootloader_self_check_enable(void);
/* disable bootloader self-check  */
nvmc_ob_state_enum nvmc_ob_bootloader_self_check_disable(void);
/* enable synchronization mode  */
nvmc_ob_state_enum nvmc_ob_sync_mode_enable(void);
/* disable synchronization mode  */
nvmc_ob_state_enum nvmc_ob_sync_mode_disable(void);
/* enable hardware self-check mode */
nvmc_ob_state_enum nvmc_ob_hardware_self_check_enable(void);
/* disable hardware self-check mode */
nvmc_ob_state_enum nvmc_ob_hardware_self_check_disable(void);
/* configure repair address */
nvmc_ob_state_enum nvmc_ob_repair_address_config(uint32_t repair_start, uint32_t repair_end);
/* configure bad block address */
nvmc_ob_state_enum nvmc_ob_bad_block_address_config(uint32_t index, uint32_t bad_block_valid, uint32_t bad_block_address);

/* get option byte hardware watchdog configuration */
FlagStatus nvmc_ob_hardware_watchdog_get(void);
/* get option byte OTA check configuration */
FlagStatus nvmc_ob_ota_check_get(void);
/* get option byte firmware update configuration */
FlagStatus nvmc_ob_firmware_update_get(void);
/* get option byte ENVM backup bank number */
uint8_t nvmc_ob_envm_backup_bank_get(void);
/* get option byte secure mode */
FlagStatus nvmc_ob_secure_mode_get(void);
/* get option byte ecc configuration */
FlagStatus nvmc_ob_ecc_get(ecc_type_enum ecc_type);
/* get option byte security protection value */
FlagStatus nvmc_ob_security_protection_flag_get(void);
/* get option byte BOR threshold value */
uint32_t nvmc_ob_bor_threshold_get(void);

/* get option byte ITCM size in ITCM/AXI shared RAM */
uint32_t nvmc_ob_itcm_axi_shared_ram_itcm_size_get(void);
/* get option byte ITCM size in ITCM/DTCM shared RAM */
uint32_t nvmc_ob_itcm_dtcm_shared_ram_itcm_size_get(void);

/* get ENVM DCRP area configuration */
uint8_t nvmc_ob_envm_dcrp_area_get(uint32_t *dcrp_erase_option, uint32_t *dcrp_area_en, uint32_t *dcrp_area_start_addr, uint32_t *dcrp_area_end_addr);
#ifndef NVM_VER_CASE4
/* get flash bank0 DCRP area configuration */
uint8_t nvmc_ob_fb0_dcrp_area_get(uint32_t *dcrp_erase_option, uint32_t *dcrp_area_en, uint32_t *dcrp_area_start_addr, uint32_t *dcrp_area_end_addr);
#ifndef NVM_VER_CASE2
/* get flash bank1 DCRP area configuration */
uint8_t nvmc_ob_fb1_dcrp_area_get(uint32_t *dcrp_erase_option, uint32_t *dcrp_area_en, uint32_t *dcrp_area_start_addr, uint32_t *dcrp_area_end_addr);
#endif /* NVM_VER_CASE2 */
#endif /* NVM_VER_CASE4 */
/* get ENVM secure area configuration */
uint8_t nvmc_ob_envm_secure_area_get(uint32_t *secure_erase_option, uint32_t *scr_area_en, uint32_t *scr_area_start_addr,
                                     uint32_t *scr_area_end_addr);
#ifndef NVM_VER_CASE4
/* get flash bank0 secure area configuration */
uint8_t nvmc_ob_fb0_secure_area_get(uint32_t *secure_erase_option, uint32_t *scr_area_en, uint32_t *scr_area_start_addr, uint32_t *scr_area_end_addr);
#ifndef NVM_VER_CASE2
/* get flash bank1 secure area configuration */
uint8_t nvmc_ob_fb1_secure_area_get(uint32_t *secure_erase_option, uint32_t *scr_area_en, uint32_t *scr_area_start_addr, uint32_t *scr_area_end_addr);
#endif /* NVM_VER_CASE2 */
#endif /* NVM_VER_CASE4 */
/* get option byte write protection configuration */
uint64_t nvmc_ob_write_protection_get(wp_area_enum wp_area);
/* get boot area address */
void nvmc_ob_boot_address_get(uint32_t *boot_address_0, uint32_t *boot_address_1);

/* get option byte one bit error unrepaired counter interrupt threshold configuration */
uint32_t nvmc_ob_err1_cnt_threshold_get(void);
/* get option byte two bit error unrepaired counter interrupt threshold configuration */
uint32_t nvmc_ob_err2_cnt_threshold_get(void);
/* get option byte power-on reset self-check repair configuration */
FlagStatus nvmc_ob_power_on_self_check_get(void);
/* get option byte system reset self-check repair configuration */
FlagStatus nvmc_ob_system_reset_check_get(void);
/* get option byte 2 bits error automatic repair configuration */
FlagStatus nvmc_ob_ecc2_auto_repair_get(void);
/* get option byte 1 bit error automatic repair configuration */
FlagStatus nvmc_ob_ecc1_auto_repair_get(void);
/* get option byte bad data replacement in ENVM with flash content */
FlagStatus nvmc_ob_envm_bad_data_replace_get(void);
/* get option byte repair data with two bits error during self check */
FlagStatus nvmc_ob_two_bits_error_repair_get(void);
/* get option byte repair data with one bit error during self check */
FlagStatus nvmc_ob_one_bit_error_repair_get(void);
/* get option byte bootloader self-check */
FlagStatus nvmc_ob_bootloader_self_check_get(void);
/* get option byte synchronization mode */
FlagStatus nvmc_ob_sync_mode_get(void);
/* get option byte synchronization area size */
uint32_t nvmc_ob_sync_area_size_get(void);
/* get option byte hardware self-check mode */
FlagStatus nvmc_ob_hardware_self_check_get(void);
/* get repair address */
uint8_t nvmc_ob_repair_address_get(uint32_t *repair_start_addr, uint32_t *repair_end_addr);
/* get bad block address */
uint8_t nvmc_ob_bad_block_address_get(uint32_t index, uint32_t *bad_block_address);

/* get option byte operation flag */
FlagStatus nvmc_ob_flag_get(uint32_t ob_flag);
/* clear option byte pending flag */
void nvmc_ob_flag_clear(uint32_t ob_flag);
/* enable option byte interrupt */
void nvmc_ob_interrupt_enable(void);
/* disable option byte interrupt */
void nvmc_ob_interrupt_disable(void);
/* get option byte interrupt operation flag */
FlagStatus nvmc_ob_interrupt_flag_get(uint8_t ob_int_flag);
/* clear option byte interrupt pending flag */
void nvmc_ob_interrupt_flag_clear(uint8_t ob_int_flag);

#endif /* GD32H77X_NVMC_H */