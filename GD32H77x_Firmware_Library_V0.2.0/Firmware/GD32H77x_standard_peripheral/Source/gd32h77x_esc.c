/*!
    \file    gd32h77x_esc.c
    \brief   ESC driver

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

#include "gd32h77x_esc.h"
#include <string.h>

#define ESC_ADDR_MASK           (uint16_t)0x0FFFU
#define ESC_TIMEOUT_CNT_MASK    (uint32_t)0x000001FFU

#define ESC_DELAY               ((uint32_t)0x01FFFFFFU)     /* Timeout for ESC operations */

/*!
    \brief      pdi direct access read esc register (API_ID(0x0001U))
    \param[in]  readbuffer: data read buffer
    \param[in]  addr: esc register address offset
    \param[in]  count: the number to read from esc register
    \param[out] none
    \retval     none
*/
void pdi_direct_read_reg(uint8_t readbuffer[], uint16_t addr, uint16_t count)
{
    uint16_t address = (addr & ESC_ADDR_MASK);
    uint16_t i;
#ifdef FW_DEBUG_ERR_REPORT
    /* check parameter */
    if(NOT_VALID_POINTER(readbuffer)) {
        fw_debug_report_err(ESC_MODULE_ID, API_ID(0x0001U), ERR_PARAM_POINTER);
    } else
#endif /* FW_DEBUG_ERR_REPORT */
    {
        switch(count) {
        case 0x1U:
            *(uint8_t *)readbuffer = REG8(ESC_BASE + address);
            break;
        case 0x2U:
            *(uint16_t *)readbuffer = REG16(ESC_BASE + address);
            break;
        case 0x4U:
            *(uint32_t *)readbuffer = REG32(ESC_BASE + address);
            break;
        default:
            for(i = 0U; i < count; i++) {
                readbuffer[i] = REG8(ESC_BASE + address + i);
            }
            break;
        }
    }
}

/*!
    \brief      pdi indirect access read esc register (API_ID(0x0002U))
    \param[in]  readbuffer: data read buffer
    \param[in]  addr: esc register address offset
    \param[in]  count: the number to read from esc register
    \param[out] none
    \retval     none
    \note       This function includes timeout exit scenarios.
                Modify according to the user's actual usage scenarios.
*/
void pdi_indirect_read_reg(uint8_t readbuffer[], uint16_t addr, uint16_t count)
{
    uint32_val param32_1 = {0U};
    uint8_t i = 1U;
    uint16_t address = (uint16_t)(addr & ESC_ADDR_MASK);
    uint32_t timeout;
    uint16_t buffer_index = 0U;

#ifdef FW_DEBUG_ERR_REPORT
    /* check parameter */
    if(NOT_VALID_POINTER(readbuffer)) {
        fw_debug_report_err(ESC_MODULE_ID, API_ID(0x0002U), ERR_PARAM_POINTER);
    } else
#endif /* FW_DEBUG_ERR_REPORT */
    {
        /* loop for all bytes to be read */
        while(count > 0U) {
            /* reset/abort any previous commands */
            param32_1.val = CCTL_STOP;
            ESC_CCTL_CMD = param32_1.val;

            /* wait with timeout */
            timeout = ESC_DELAY;
            do {
                param32_1.val = ESC_CCTL_CMD;
                timeout--;
            } while((0U != timeout) && (param32_1.val & CCTL_BUSY));

            param32_1.val = MAKE_UINT32_FROM_BYTES(GET_LOW_BYTE(address), GET_HIGH_BYTE(address), i, ESC_READ_BYTE);

            ESC_CCTL_CMD = param32_1.val;

            /* wait with timeout */
            timeout = ESC_DELAY;
            do {
                param32_1.val = ESC_CCTL_CMD;
                timeout--;
            } while((0U != timeout) && (param32_1.val & CCTL_BUSY));

            param32_1.val = ESC_CCTL_DATA;

            switch(address & (uint16_t)0x0003U) {
            case 0x0000U:
                readbuffer[buffer_index] = GET_BYTE0(param32_1.val);
                break;
            case 0x0001U:
                readbuffer[buffer_index] = GET_BYTE1(param32_1.val);
                break;
            case 0x0002U:
                readbuffer[buffer_index] = GET_BYTE2(param32_1.val);
                break;
            case 0x0003U:
                readbuffer[buffer_index] = GET_BYTE3(param32_1.val);
                break;
            default:
                /* this should not occur under normal conditions */
                break;
            }

            count -= i;
            buffer_index += i;
            address += i;
        }
    }
}

/*!
    \brief      pdi direct access write esc register (API_ID(0x0003U))
    \param[in]  writebuffer: data write buffer
    \param[in]  addr: esc register address offset
    \param[in]  count: the number to write to esc register
    \param[out] none
    \retval     none
*/
void pdi_direct_write_reg(uint8_t writebuffer[], uint16_t addr, uint16_t count)
{
    uint16_t address = (addr & ESC_ADDR_MASK);
    uint16_t i;
#ifdef FW_DEBUG_ERR_REPORT
    /* check parameter */
    if(NOT_VALID_POINTER(writebuffer)) {
        fw_debug_report_err(ESC_MODULE_ID, API_ID(0x0003U), ERR_PARAM_POINTER);
    } else
#endif /* FW_DEBUG_ERR_REPORT */
    {
        switch(count) {
        case 0x0001U:
            REG8(ESC_BASE + address) = *(uint8_t *)writebuffer;
            break;
        case 0x0002U:
            REG16(ESC_BASE + address) = *(uint16_t *)writebuffer;
            break;
        case 0x0004U:
            REG32(ESC_BASE + address) = *(uint32_t *)writebuffer;
            break;
        default:
            for(i = 0U; i < count; i++) {
                REG8(ESC_BASE + address + i) = writebuffer[i];
            }
            break;
        }
    }
}

/*!
    \brief      pdi indirect access write esc register (API_ID(0x0004U))
    \param[in]  writebuffer: data write buffer
    \param[in]  addr: esc register address offset
    \param[in]  count: the number to write to esc register
    \param[out] none
    \retval     none
    \note       This function includes timeout exit scenarios.
                Modify according to the user's actual usage scenarios.
*/
void pdi_indirect_write_reg(uint8_t writebuffer[], uint16_t addr, uint16_t count)
{
    uint32_val param32_1 = {0U};
    uint8_t i = 1U;
    uint16_t address = (uint16_t)(addr & ESC_ADDR_MASK);
    uint32_t timeout;
    uint16_t buffer_index = 0U;

#ifdef FW_DEBUG_ERR_REPORT
    /* check parameter */
    if(NOT_VALID_POINTER(writebuffer)) {
        fw_debug_report_err(ESC_MODULE_ID, API_ID(0x0004U), ERR_PARAM_POINTER);
    } else
#endif /* FW_DEBUG_ERR_REPORT */
    {
        while(count > 0U) {
            /* reset/abort any previous commands */
            param32_1.val = CCTL_STOP;
            ESC_CCTL_CMD = param32_1.val;

            /* wait with timeout */
            timeout = ESC_DELAY;
            do {
                param32_1.val = ESC_CCTL_CMD;
                timeout--;
            } while((0U != timeout) && (param32_1.val & CCTL_BUSY));

            param32_1.val = 0U;
            switch(address & (uint16_t)0x0003U) {
            case 0x0000U:
                param32_1.val = SET_BYTE0(param32_1.val, writebuffer[buffer_index]);
                break;
            case 0x0001U:
                param32_1.val = SET_BYTE1(param32_1.val, writebuffer[buffer_index]);
                break;
            case 0x0002U:
                param32_1.val = SET_BYTE2(param32_1.val, writebuffer[buffer_index]);
                break;
            case 0x0003U:
                param32_1.val = SET_BYTE3(param32_1.val, writebuffer[buffer_index]);
                break;
            default:
                /* this should not occur under normal conditions */
                break;
            }

            ESC_CCTL_DATA = param32_1.val;

            param32_1.val = MAKE_UINT32_FROM_BYTES(GET_LOW_BYTE(address), GET_HIGH_BYTE(address), i, ESC_WRITE_BYTE);

            ESC_CCTL_CMD = param32_1.val;

            /* wait with timeout */
            timeout = ESC_DELAY;
            do {
                param32_1.val = ESC_CCTL_CMD;
                timeout--;
            } while((0U != timeout) && (param32_1.val & CCTL_BUSY));

            count -= i;
            buffer_index += i;
            address += i;
        }
    }
}

/*!
    \brief      pdi direct access read esc pdram (API_ID(0x0005U))
    \param[in]  readbuffer: data read buffer
    \param[in]  addr: esc pdram address offset
    \param[in]  count: the number to read from esc pdram
    \param[out] none
    \retval     none
*/
void pdi_direct_read_pdram(uint8_t readbuffer[], uint16_t addr, uint16_t count)
{
    uint32_t address = (uint32_t)addr;
    uint16_t i;

#ifdef FW_DEBUG_ERR_REPORT
    /* check parameter */
    if(NOT_VALID_POINTER(readbuffer)) {
        fw_debug_report_err(ESC_MODULE_ID, API_ID(0x0005U), ERR_PARAM_POINTER);
    } else
#endif /* FW_DEBUG_ERR_REPORT */
    {
        /* read data byte by byte using register access macros to avoid type conversion */
        for(i = 0U; i < count; i++) {
            readbuffer[i] = REG8(ESC_BASE + address + (uint32_t)i);
        }
    }
}

/*!
    \brief      pdi indirect access read esc pdram (API_ID(0x0006U))
    \param[in]  readbuffer: data read buffer
    \param[in]  addr: esc pdram address offset
    \param[in]  count: the number to read from esc pdram
    \param[out] none
    \retval     none
    \note       This function includes timeout exit scenarios.
                Modify according to the user's actual usage scenarios.
*/
void pdi_indirect_read_pdram(uint8_t readbuffer[], uint16_t addr, uint16_t count)
{
    uint32_val param32_1 = {0U};
    uint16_t i = 0U, length, byteposition;
    uint16_t address = (uint16_t)addr;
    uint32_t timeout;
    uint8_t temp_bytes[4];

#ifdef FW_DEBUG_ERR_REPORT
    /* check parameter */
    if(NOT_VALID_POINTER(readbuffer)) {
        fw_debug_report_err(ESC_MODULE_ID, API_ID(0x0006U), ERR_PARAM_POINTER);
    } else
#endif /* FW_DEBUG_ERR_REPORT */
    {
        /* reset/abort any previous commands */
        param32_1.val = PRAM_STOP_READ;

        ESC_PRAM_CR = param32_1.val;
        /* the host should not modify this field unless the pdram read busy bit is 0 */
        timeout = ESC_DELAY;
        do {
            param32_1.val = ESC_PRAM_CR;
            timeout--;
        } while((0U != timeout) && (param32_1.val & PRAM_BUSY_READ));

        /* write address and length in the esc pdram read address and length register */
        param32_1.val = MAKE_UINT32_FROM_WORDS(address, count);
        ESC_PRAM_ALR = param32_1.val;

        /* set pdram read busy bit to start read operation */
        ESC_PRAM_CR = PRAM_BUSY_READ;
        timeout = ESC_DELAY;
        do {
            param32_1.val = ESC_PRAM_CR;
            timeout--;
        } while((0U != timeout) && (param32_1.val & PRAM_BUSY_READ));

        /* get the length for first read */
        param32_1.val = ESC_PRAM_FIFO_DR;
        byteposition = (address & (uint16_t)0x0003U);
        length = ((uint16_t)4U - byteposition) > count ? count : ((uint16_t)4U - byteposition);

        /* extract bytes using macros */
        temp_bytes[0] = GET_BYTE0(param32_1.val);
        temp_bytes[1] = GET_BYTE1(param32_1.val);
        temp_bytes[2] = GET_BYTE2(param32_1.val);
        temp_bytes[3] = GET_BYTE3(param32_1.val);

        memcpy(&readbuffer[i], &temp_bytes[byteposition], (size_t)length);
        count -= length;
        i += length;

        while(count) {
            param32_1.val = ESC_PRAM_FIFO_DR;
            length = count > (uint16_t)4U ? (uint16_t)4U : count;

            /* extract bytes using macros */
            temp_bytes[0] = GET_BYTE0(param32_1.val);
            temp_bytes[1] = GET_BYTE1(param32_1.val);
            temp_bytes[2] = GET_BYTE2(param32_1.val);
            temp_bytes[3] = GET_BYTE3(param32_1.val);

            memcpy(&readbuffer[i], temp_bytes, (size_t)length);
            i += length;
            count -= length;
        }
    }
}

/*!
    \brief      pdi direct access write esc pdram (API_ID(0x0007U))
    \param[in]  writebuffer: data write buffer
    \param[in]  addr: esc pdram address offset
    \param[in]  count: the number to write to esc pdram
    \param[out] none
    \retval     none
*/
void pdi_direct_write_pdram(uint8_t writebuffer[], uint16_t addr, uint16_t count)
{
    uint32_t address = (uint32_t)addr;
    uint16_t i;

#ifdef FW_DEBUG_ERR_REPORT
    /* check parameter */
    if(NOT_VALID_POINTER(writebuffer)) {
        fw_debug_report_err(ESC_MODULE_ID, API_ID(0x0007U), ERR_PARAM_POINTER);
    } else
#endif /* FW_DEBUG_ERR_REPORT */
    {
        /* write data byte by byte using register access macros to avoid type conversion */
        for(i = 0U; i < count; i++) {
            REG8(ESC_BASE + address + (uint32_t)i) = writebuffer[i];
        }
    }
}

/*!
    \brief      pdi indirect access write esc pdram (API_ID(0x0008U))
    \param[in]  writebuffer: data write buffer
    \param[in]  addr: esc pdram address offset
    \param[in]  count: the number to write to esc pdram
    \param[out] none
    \retval     none
    \note       This function includes timeout exit scenarios.
                Modify according to the user's actual usage scenarios.
*/
void pdi_indirect_write_pdram(uint8_t writebuffer[], uint16_t addr, uint16_t count)
{
    uint32_val param32_1 = {0U};
    uint16_t i = 0U, length, byteposition;
    uint16_t address = (uint16_t)addr;
    uint32_t timeout;
    uint8_t temp_bytes[4];

#ifdef FW_DEBUG_ERR_REPORT
    /* check parameter */
    if(NOT_VALID_POINTER(writebuffer)) {
        fw_debug_report_err(ESC_MODULE_ID, API_ID(0x0008U), ERR_PARAM_POINTER);
    } else
#endif /* FW_DEBUG_ERR_REPORT */
    {
        /* reset or abort any previous commands */
        param32_1.val = PRAM_STOP_WRITE;

        ESC_PRAM_CW = param32_1.val;
        /* make sure there is no previous write is pending bit is a 0 */
        timeout = ESC_DELAY;
        do {
            param32_1.val = ESC_PRAM_CW;
            timeout--;
        } while((0U != timeout) && (param32_1.val & PRAM_BUSY_WRITE));

        /* write address and length register with the starting address and length */
        param32_1.val = MAKE_UINT32_FROM_WORDS(address, count);
        ESC_PRAM_ALW = param32_1.val;
        param32_1.val = PRAM_BUSY_WRITE;
        ESC_PRAM_CW = param32_1.val;

        /* get the byte length for first write */
        byteposition = (address & (uint16_t)0x0003U);
        length = ((uint16_t)4U - byteposition) > count ? count : ((uint16_t)4U - byteposition);

        /* clear temp bytes and copy data */
        temp_bytes[0] = 0U;
        temp_bytes[1] = 0U;
        temp_bytes[2] = 0U;
        temp_bytes[3] = 0U;
        memcpy(&temp_bytes[byteposition], &writebuffer[i], (size_t)length);
        /* create uint32 from bytes using macro */
        param32_1.val = MAKE_UINT32_FROM_BYTES(temp_bytes[0], temp_bytes[1], temp_bytes[2], temp_bytes[3]);
        ESC_PRAM_FIFO_DW = param32_1.val;
        count -= length;
        i += length;

        while(count) {
            length = count > 4U ? 4U : count;
            /* clear temp bytes and copy data */
            temp_bytes[0] = 0U;
            temp_bytes[1] = 0U;
            temp_bytes[2] = 0U;
            temp_bytes[3] = 0U;
            memcpy(temp_bytes, &writebuffer[i], (size_t)length);
            /* create uint32 from bytes using macro */
            param32_1.val = MAKE_UINT32_FROM_BYTES(temp_bytes[0], temp_bytes[1], temp_bytes[2], temp_bytes[3]);
            ESC_PRAM_FIFO_DW = param32_1.val;
            i += length;
            count -= length;
        }
    }
}

/*!
    \brief      esc pdi reset (API_ID(0x0009U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pdi_reset(void)
{
    uint8_t wbuffer[1] = {0U};
    wbuffer[0] = PDI_RESET_R; /* R */
    pdi_direct_write_reg(wbuffer, ESC_RST_PDI_OFFSET, 0x0001U);
    wbuffer[0] = PDI_RESET_E; /* E */
    pdi_direct_write_reg(wbuffer, ESC_RST_PDI_OFFSET, 0x0001U);
    wbuffer[0] = PDI_RESET_S; /* S */
    pdi_direct_write_reg(wbuffer, ESC_RST_PDI_OFFSET, 0x0001U);
}

/*!
    \brief      PDI read phy (API_ID(0x000AU))
    \param[in]  phy_addr: phy address
    \param[in]  phy_reg_addr: phy register address
    \retval     res: 16-bit PHY register data read from the specified address
    \note       This function includes timeout exit scenarios.
                Modify according to the user's actual usage scenarios.
*/
uint16_t pdi_phy_read(uint8_t phy_addr, uint8_t phy_reg_addr)
{
    uint8_t temp        = 0U;
    uint8_t value       = 0U;
    uint8_t readbuf[2]  = {0U};
    uint16_t res        = 0U;
    uint32_t timeout;

    /* PDI access MDIO allowed */
    temp = PDI_ACCESS_MII_MANAGEMENT_ENABLE;
    pdi_direct_write_reg(&temp, MIIM_PDI_ACC_STAT_OFFSET, 0x0001U);

    /*1. check mdio idle state */
    timeout = ESC_DELAY;
    do {
        pdi_direct_read_reg(&temp, ESC_MII_MNG_STAT_OFFSET, 0x0001U);
        timeout--;
    } while((0U != timeout) && (temp & 0x80U));

    /*2. write PHY address and PHY register */
    value = phy_addr ;
    pdi_direct_write_reg(&value, ESC_PHY_ADDR_OFFSET, 0x0001U);
    value = phy_reg_addr ;
    pdi_direct_write_reg(&value, ESC_PHY_REG_ADDR_OFFSET, 0x0001U);

    /* 3. write PHY command */
    value = READ_PHY_CMD;
    pdi_direct_write_reg(&value, ESC_MII_MNG_STAT_OFFSET, 0x0001U);

    /* 4. wait for mdio idle and get data */
    timeout = ESC_DELAY;
    do {
        pdi_direct_read_reg(&temp, ESC_MII_MNG_STAT_OFFSET, 0x0001U);
        timeout--;
    } while((0U != timeout) && (temp & 0x80U));

    /* 5. check mdio idle state */
    pdi_direct_read_reg(readbuf, ESC_PHY_DATA_OFFSET, 0x0002U);

    res = (uint16_t)((((uint32_t)readbuf[1]) << 8U) | (readbuf[0]));
    return res;
}

/*!
    \brief      PDI write phy (API_ID(0x000BU))
    \param[in]  phy_addr: phy address
    \param[in]  phy_reg_addr: phy register address
    \param[in]  phy_data: phy data
    \retval     none
    \note       This function includes timeout exit scenarios.
                Modify according to the user's actual usage scenarios.
*/
void pdi_phy_write(uint8_t phy_addr, uint8_t phy_reg_addr, uint16_t phy_data)
{
    uint8_t temp         = 0U;
    uint8_t value        = 0U;
    uint8_t writebuf[2]  = {0U};
    uint32_t timeout;

    writebuf[0] = (uint8_t)phy_data;
    writebuf[1] = (uint8_t)(phy_data >> 8U);

    /* PDI access MDIO allowed */
    temp = PDI_ACCESS_MII_MANAGEMENT_ENABLE;
    pdi_direct_write_reg(&temp, MIIM_PDI_ACC_STAT_OFFSET, 0x0001U);


    /* 1. check mdio idle state */
    timeout = ESC_DELAY;
    do {
        pdi_direct_read_reg(&temp, ESC_MII_MNG_STAT_OFFSET, 0x0001U);
        timeout--;
    } while((0U != timeout) && (temp & 0x80U));

    /* 2. write PHY address and PHY register */
    value = phy_addr;
    pdi_direct_write_reg(&value, ESC_PHY_ADDR_OFFSET, 0x0001U);
    value = phy_reg_addr;
    pdi_direct_write_reg(&value, ESC_PHY_REG_ADDR_OFFSET, 0x0001U);

    /* 3. write PHY data */
    pdi_direct_write_reg(writebuf, ESC_PHY_DATA_OFFSET, 0x0002U);

    /* 4. write PHY command and enable */
    value = WRITE_PHY_ENABLE;
    pdi_direct_write_reg(&value, ESC_MII_MNG_CTL_OFFSET, 0x0001U);
    value = WRITE_PHY_CMD;
    pdi_direct_write_reg(&value, ESC_MII_MNG_STAT_OFFSET, 0x0001U);

    /* 5. wait for mdio idle */
    timeout = ESC_DELAY;
    do {
        pdi_direct_read_reg(&temp, ESC_MII_MNG_STAT_OFFSET, 0x0001U);
        timeout--;
    } while((0U != timeout) && (temp & 0x80U));
}

/*!
    \brief      ESC brp enable (API_ID(0x000CU))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void esc_brp_enable(void)
{
    ESC_CTL |= BRP;
}

/*!
    \brief      ESC brp disable (API_ID(0x000DU))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void esc_brp_disable(void)
{
    ESC_CTL &= ~BRP;
}

/*!
    \brief      ESC timeout enable (API_ID(0x000EU))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void esc_timeout_enable(void)
{
    ESC_CTL |= TOEN;
}

/*!
    \brief      ESC timeout disable (API_ID(0x000FU))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void esc_timeout_disable(void)
{
    ESC_CTL &= ~TOEN;
}

/*!
    \brief      ESC timeout counter (API_ID(0x0010U))
    \param[in]  cnt:0-511
    \param[out] none
    \retval     none
*/
void esc_timeout_cnt(uint32_t cnt)
{
    ESC_CTL &= ~(TO_CNT);
    ESC_CTL |= ESC_TO_CNT(cnt & ESC_TIMEOUT_CNT_MASK);
}

/*!
    \brief      enable the on_chip_bus interrupt (API_ID(0x0011U))
    \param[in]  int_flag: refer to esc_on_chip_bus_interrupt_enum
                  only one parameter can be selected which is shown as below:
      \arg        ESC_I2CIE: Internal I2C overrun interrupt enable
      \arg        ESC_CCTLIVIE: CCTL size & addr illegal value interrupt enable
      \arg        ESC_WEIE: write error interrupt enable
      \arg        ESC_TOIE: time out interrupt enable
      \arg        ESC_WDLIE: write data lost interrupt enable
    \param[out] none
    \retval     none
*/
void esc_interrupt_enable(esc_on_chip_bus_interrupt_enum interrupt)
{
    ESC_REG_VAL(interrupt) |= BIT(ESC_BIT_POS(interrupt));
}

/*!
    \brief      disable the on_chip_bus interrupt (API_ID(0x0012U))
    \param[in]  int_flag: refer to esc_on_chip_bus_interrupt_enum
                  only one parameter can be selected which is shown as below:
      \arg        ESC_I2CIE: Internal I2C overrun interrupt enable
      \arg        ESC_CCTLIVIE: CCTL size & addr illegal value interrupt enable
      \arg        ESC_WEIE: write error interrupt enable
      \arg        ESC_TOIE: time out interrupt enable
      \arg        ESC_WDLIE: write data lost interrupt enable
    \param[out] none
    \retval     none
*/
void esc_interrupt_disable(esc_on_chip_bus_interrupt_enum interrupt)
{
    ESC_REG_VAL(interrupt) &= ~BIT(ESC_BIT_POS(interrupt));
}

/*!
    \brief      get esc on_chip_bus flag (API_ID(0x0013U))
    \param[in]  int_flag: refer to esc_on_chip_bus_flag_enum
                only one parameter can be selected which is shown as below:
      \arg        ESC_FLAG_I2CF: Internal I2C communication error flag
      \arg        ESC_FLAG_CCTLIVF: CCTL size & addr illegal value flag
      \arg        ESC_FLAG_WEF: write error flag
      \arg        ESC_FLAG_TOF: time out flag
      \arg        ESC_FLAG_WDLF: write data lost flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus esc_interrupt_flag_get(esc_on_chip_bus_flag_enum int_flag)
{
    FlagStatus reval = RESET;

    /* get the ESC on_chip_bus interrupt flag */
    if(RESET != (ESC_REG_VAL(int_flag) & BIT(ESC_BIT_POS(int_flag)))) {
        reval = SET;
    }
    return reval;
}

/*!
    \brief      clear esc on_chip_bus flag (API_ID(0x0014U))
    \param[in]  int_flag: refer to esc_on_chip_bus_flag_enum
                  only one parameter can be selected which is shown as below:
      \arg        ESC_FLAG_I2CF: Internal I2C communication error flag
      \arg        ESC_FLAG_CCTLIVF: CCTL size & addr illegal value flag
      \arg        ESC_FLAG_WEF: write error flag
      \arg        ESC_FLAG_TOF: time out flag
      \arg        ESC_FLAG_WDLF: write data lost flag
    \param[out] none
    \retval     none
*/
void esc_interrupt_flag_clear(esc_on_chip_bus_flag_enum int_flag)
{
    ESC_REG_VAL(int_flag) |= BIT(ESC_BIT_POS(int_flag));
}
