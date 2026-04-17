/*!
    \file    gdesc.c
    \brief   gdesc driver basic configuration

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

#include <string.h>
#include "ecat_def.h"
#include "ecatslv.h"
#include "ecatappl.h"
#include "systick.h"
#include "gdesc.h"

UALEVENT         EscALEvent;            //contains the content of the ALEvent register (0x220), this variable is updated on each Access to the Esc
UINT16           nAlEventMask;          // current ALEventMask (content of register 0x204:0x205)

/*!
    \brief      The function retrieves the content of the AL_Event register.
                The first two bytes of an access to the EtherCAT ASIC always deliver the AL_Event register (0x220).
                It will be saved in the global "EscALEvent"
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void GetInterruptRegister(void)
{

    HW_EscReadIsr((MEM_ADDR *)&EscALEvent.Word, 0x220, 2);
}

/*!
    \brief      The function retrieves the content of the AL_Event register from an interrupt context.
                Shall be implemented if interrupts are supported else this function is equal to "GetInterruptRegister()"
                The first two bytes of an access to the EtherCAT ASIC always deliver the AL_Event register (0x220).
                It will be saved in the global "EscALEvent"
    \param[in]  none
    \param[out] none
    \retval     none
*/
#if !INTERRUPTS_SUPPORTED
#define ISR_GetInterruptRegister GetInterruptRegister
#else
static void ISR_GetInterruptRegister(void)
{
    HW_EscReadIsr((MEM_ADDR *)&EscALEvent.Word, 0x220, 2);
}
#endif /* !INTERRUPTS_SUPPORTED */

/*!
    \brief      configure the esc gpio peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void esc_gpio_config(void)
{
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOF);
    rcu_periph_clock_enable(RCU_GPIOG);

    /* PG12: ESC_IRQ->PG12 */   
    gpio_mode_set(GPIOG, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_12);
    gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_100_220MHZ, GPIO_PIN_12);
    gpio_af_set(GPIOG, GPIO_AF_0, GPIO_PIN_12);

    /* PF7: ESC_SYNC0->PF7 ESC_SYNC0->PF8 */
    gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_7 | GPIO_PIN_8);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_100_220MHZ, GPIO_PIN_7 | GPIO_PIN_8);
    gpio_af_set(GPIOF, GPIO_AF_3, GPIO_PIN_7 | GPIO_PIN_8);
    /* PF9: ESC_LATCH0 */
    gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);
    gpio_output_options_set(GPIOF,GPIO_OTYPE_PP,GPIO_OSPEED_100_220MHZ,GPIO_PIN_9);
    gpio_af_set(GPIOF, GPIO_AF_3, GPIO_PIN_9);
    /* PF10: ESC_LATCH1 */
    gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_10);
    gpio_output_options_set(GPIOF,GPIO_OTYPE_PP,GPIO_OSPEED_100_220MHZ,GPIO_PIN_10);
    gpio_af_set(GPIOF, GPIO_AF_3, GPIO_PIN_10);

    /*****ESC_LEDRUN/LEDERR********/
    /* PG13: ESC_LEDRUN */
    gpio_mode_set(GPIOG, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_13);
    gpio_output_options_set(GPIOG,GPIO_OTYPE_PP,GPIO_OSPEED_100_220MHZ,GPIO_PIN_13);
    gpio_af_set(GPIOG, GPIO_AF_4, GPIO_PIN_13);
    /* PG14: ESC_LEDERR */
    gpio_mode_set(GPIOG, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_14);
    gpio_output_options_set(GPIOG,GPIO_OTYPE_PP,GPIO_OSPEED_100_220MHZ,GPIO_PIN_14);
    gpio_af_set(GPIOG, GPIO_AF_4, GPIO_PIN_14);

    /*****ESC_IIC_EEPROM********/
    /* PC0: ESC_EEPROM_SIZE */
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_0);
    gpio_output_options_set(GPIOC,GPIO_OTYPE_PP,GPIO_OSPEED_100_220MHZ,GPIO_PIN_0);
    gpio_af_set(GPIOC, GPIO_AF_2, GPIO_PIN_0);
    /* PF5: ESC_EEPROM_SDA */
    gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_5);
    gpio_output_options_set(GPIOF,GPIO_OTYPE_PP,GPIO_OSPEED_100_220MHZ,GPIO_PIN_5);
    gpio_af_set(GPIOF, GPIO_AF_2, GPIO_PIN_5);
    /* PF4: ESC_EEPROM_SCL */
    gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_4);
    gpio_output_options_set(GPIOF,GPIO_OTYPE_PP,GPIO_OSPEED_100_220MHZ,GPIO_PIN_4);
    gpio_af_set(GPIOF, GPIO_AF_2, GPIO_PIN_4);

    /* PG10: LINKACT0 */
    gpio_mode_set(GPIOG, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_10);
    gpio_output_options_set(GPIOG,GPIO_OTYPE_PP,GPIO_OSPEED_100_220MHZ,GPIO_PIN_10);
    gpio_af_set(GPIOG, GPIO_AF_4, GPIO_PIN_10);
    /* PG11: LINKACT1 */
    gpio_mode_set(GPIOG, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_11);
    gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_100_220MHZ, GPIO_PIN_11);
    gpio_af_set(GPIOG, GPIO_AF_4, GPIO_PIN_11);
}

/*!
    \brief      configure ESC MII GPIO
    \param[in]  none
    \param[out] none
    \retval     none
*/
void esc_mii_gpio_config(void)
{   
    /* enable GPIO clock for all used GPIO ports */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_GPIOF);
    rcu_periph_clock_enable(RCU_GPIOG);

    /*****MII0********/
    /* PA10: ESC_MII0_TXD0 */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_10);
    gpio_output_options_set(GPIOA,GPIO_OTYPE_PP,GPIO_OSPEED_100_220MHZ,GPIO_PIN_10);
    gpio_af_set(GPIOA, GPIO_AF_6, GPIO_PIN_10);
    /* PA9: ESC_MII0_TXD1 */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);
    gpio_output_options_set(GPIOA,GPIO_OTYPE_PP,GPIO_OSPEED_100_220MHZ,GPIO_PIN_9);
    gpio_af_set(GPIOA, GPIO_AF_6, GPIO_PIN_9);
    /* PA8: ESC_MII0_TXD2 */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_8);
    gpio_output_options_set(GPIOA,GPIO_OTYPE_PP,GPIO_OSPEED_100_220MHZ,GPIO_PIN_8);
    gpio_af_set(GPIOA, GPIO_AF_6, GPIO_PIN_8);
    /* PC9: ESC_MII0_TXD3 */
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);
    gpio_output_options_set(GPIOC,GPIO_OTYPE_PP,GPIO_OSPEED_100_220MHZ,GPIO_PIN_9);
    gpio_af_set(GPIOC, GPIO_AF_6, GPIO_PIN_9);

    /* PD4: ESC_MII0_RXD0 */
    gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_4);
    gpio_output_options_set(GPIOD,GPIO_OTYPE_PP,GPIO_OSPEED_100_220MHZ,GPIO_PIN_4);
    gpio_af_set(GPIOD, GPIO_AF_4, GPIO_PIN_4);
    /* PD5: ESC_MII0_RXD1 */
    gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_5);
    gpio_output_options_set(GPIOD,GPIO_OTYPE_PP,GPIO_OSPEED_100_220MHZ,GPIO_PIN_5);
    gpio_af_set(GPIOD, GPIO_AF_4, GPIO_PIN_5);
    /* PD6: ESC_MII0_RXD2 */
    gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6);
    gpio_output_options_set(GPIOD,GPIO_OTYPE_PP,GPIO_OSPEED_100_220MHZ,GPIO_PIN_6);
    gpio_af_set(GPIOD, GPIO_AF_0, GPIO_PIN_6);
    /* PD7: ESC_MII0_RXD3 */
    gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_7);
    gpio_output_options_set(GPIOD,GPIO_OTYPE_PP,GPIO_OSPEED_100_220MHZ,GPIO_PIN_7);
    gpio_af_set(GPIOD, GPIO_AF_4, GPIO_PIN_7);

    /* PC8: ESC_MII0_TXC */
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_8);
    gpio_output_options_set(GPIOC,GPIO_OTYPE_PP,GPIO_OSPEED_100_220MHZ,GPIO_PIN_8);
    gpio_af_set(GPIOC, GPIO_AF_5, GPIO_PIN_8);
    /* PC7: ESC_MII0_TXEN */
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_7);
    gpio_output_options_set(GPIOC,GPIO_OTYPE_PP,GPIO_OSPEED_100_220MHZ,GPIO_PIN_7);
    gpio_af_set(GPIOC, GPIO_AF_5, GPIO_PIN_7);

    /* PD3: ESC_MII0_RXER */
    gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_3);
    gpio_output_options_set(GPIOD,GPIO_OTYPE_PP,GPIO_OSPEED_100_220MHZ,GPIO_PIN_3);
    gpio_af_set(GPIOD, GPIO_AF_4, GPIO_PIN_3);
    /* PD2: ESC_MII0_RXDV */
    gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_2);
    gpio_output_options_set(GPIOD,GPIO_OTYPE_PP,GPIO_OSPEED_100_220MHZ,GPIO_PIN_2);
    gpio_af_set(GPIOD, GPIO_AF_5, GPIO_PIN_2);

    /* PC12: ESC_MII0_RXC */
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_12);
    gpio_output_options_set(GPIOC,GPIO_OTYPE_PP,GPIO_OSPEED_100_220MHZ,GPIO_PIN_12);
    gpio_af_set(GPIOC, GPIO_AF_4, GPIO_PIN_12);

    /*****MII1********/
    /* PF11: ESC_MII1_TXD0 */
    gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_11);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_100_220MHZ, GPIO_PIN_11);
    gpio_af_set(GPIOF, GPIO_AF_2, GPIO_PIN_11);
    /* PF12: ESC_MII1_TXD1 */
    gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_12);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_100_220MHZ, GPIO_PIN_12);
    gpio_af_set(GPIOF, GPIO_AF_2, GPIO_PIN_12);
    /* PF13: ESC_MII1_TXD2 */
    gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_13);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_100_220MHZ, GPIO_PIN_13);
    gpio_af_set(GPIOF, GPIO_AF_2, GPIO_PIN_13);
    /* PF14: ESC_MII1_TXD3 */
    gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_14);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_100_220MHZ, GPIO_PIN_14);
    gpio_af_set(GPIOF, GPIO_AF_2, GPIO_PIN_14);

    /* PC4: ESC_MII1_RXD0 */
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_4);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_100_220MHZ, GPIO_PIN_4);
    gpio_af_set(GPIOC, GPIO_AF_6, GPIO_PIN_4);
    /* PC5: ESC_MII1_RXD1 */
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_5);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_100_220MHZ, GPIO_PIN_5);
    gpio_af_set(GPIOC, GPIO_AF_6, GPIO_PIN_5);
    /* PB0: ESC_MII1_RXD2 */
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_0);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_100_220MHZ, GPIO_PIN_0);
    gpio_af_set(GPIOB, GPIO_AF_7, GPIO_PIN_0);
    /* PB1: ESC_MII1_RXD3 */
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_1);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_100_220MHZ, GPIO_PIN_1);
    gpio_af_set(GPIOB, GPIO_AF_7, GPIO_PIN_1);

    /* PF15: ESC_MII1_TXC */
    gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_15);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_100_220MHZ, GPIO_PIN_15);
    gpio_af_set(GPIOF, GPIO_AF_2, GPIO_PIN_15);
    /* PB11: ESC_MII1_TXEN */
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_11);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_100_220MHZ, GPIO_PIN_11);
    gpio_af_set(GPIOB, GPIO_AF_2, GPIO_PIN_11);

    /* PB10: ESC_MII1_RXER */
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_10);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_100_220MHZ, GPIO_PIN_10);
    gpio_af_set(GPIOB, GPIO_AF_2, GPIO_PIN_10);
    /* PA7: ESC_MII1_RXDV */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_7);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_100_220MHZ, GPIO_PIN_7);
    gpio_af_set(GPIOA, GPIO_AF_4, GPIO_PIN_7);

    /* PA1: ESC_MII1_RXC */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_1);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_100_220MHZ, GPIO_PIN_1);
    gpio_af_set(GPIOA, GPIO_AF_3, GPIO_PIN_1);

    /*****MII_LINK********/
    /* PA15: ESC_MII0_LINK */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_15);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_100_220MHZ, GPIO_PIN_15);
    gpio_af_set(GPIOA, GPIO_AF_2, GPIO_PIN_15);
    /* PA0: ESC_MII1_LINK */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_0);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_100_220MHZ, GPIO_PIN_0);
    gpio_af_set(GPIOA, GPIO_AF_14, GPIO_PIN_0);

    /*****ESC MDIO/MCLK********/
     /* PF2: ESC_MDIO */
    gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_2);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_100_220MHZ, GPIO_PIN_2);
    gpio_af_set(GPIOF, GPIO_AF_2, GPIO_PIN_2);
    /* PF3: ESC_MCLK */
    gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_3);
    gpio_output_options_set(GPIOF,GPIO_OTYPE_PP,GPIO_OSPEED_100_220MHZ,GPIO_PIN_3);
    gpio_af_set(GPIOF, GPIO_AF_2, GPIO_PIN_3);

    /*****ESC_MII_CLK_25M********/
     /* PB12: ESC_MII_CLK_25M (PHY1) */
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_12);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_100_220MHZ, GPIO_PIN_12);
    gpio_af_set(GPIOB, GPIO_AF_0, GPIO_PIN_12);
    /* PB9: ESC_MII_CLK_25M (PHY2) */
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_100_220MHZ, GPIO_PIN_9);
    gpio_af_set(GPIOB, GPIO_AF_16, GPIO_PIN_9);
}

/*!
    \brief      Enable EtherCAT Slave Controller (ESC) function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void esc_enable(void)
{    
    rcu_esc_clock_config(RCU_ESCSRC_AHB);
    
    rcu_esc_clock_div_config(RCU_ESC_DIV3);
    /* Enable  clock */
    rcu_periph_clock_enable(RCU_PMU);
    rcu_periph_clock_enable(RCU_SYSCFG);
    rcu_periph_clock_enable(RCU_ESC);

    pmu_esc_power_enable();

    pmu_esc_output_isolation_enable();
    
    /* Enable ESC peripheral clock */
    rcu_periph_reset_enable(RCU_ESCRST);

    /* Select ESC to use MII interface mode (instead of RMII mode)
     * MII (Media Independent Interface) provides a standard interface
     * connecting MAC and PHY, supporting 100Mbps data transfer
     */
    syscfg_esc_mii_rmii_sel(ESC_MII);
    
#if ESE_EEPROM_INTERNAL

#endif
#if ESE_EEPROM_EXTERNAL
    /* Select external EEPROM */
    syscfg_esc_external_eeprom_sel();
#endif

    /* Configure EtherCAT link signal polarity  */
    syscfg_esc_link_pol_sel(ESC_LINK0_POL_LOW,ESC_LINK1_POL_LOW,ESC_LINK2_POL_HIGH);

    /* Configure EtherCAT link/activity LED polarity (active high) */
    syscfg_esc_linkactled_pol_sel(ESC_LINKACTLED0_POL_HIGH,ESC_LINKACTLED1_POL_HIGH,ESC_LINKACTLED2_POL_HIGH);

    /* Configure EtherCAT error LED polarity (active high) */
    syscfg_esc_lederr_pol_sel(ESC_LEDERR_POL_HIGH);

    /* Configure EtherCAT run LED polarity (active high) */
    syscfg_esc_ledrun_pol_sel(ESC_LEDRUN_POL_HIGH);

    /* Set PHY address offset to 1 */
    syscfg_esc_phyaddress_offset_set(ESC_PHY_ADDRESS_OFFSET_0, 1);
    
    /* Configure ESC general GPIO pins */
    esc_gpio_config();

    /* Configure ESC MII interface related GPIO pins */
    esc_mii_gpio_config();
    
    /* Disable ESC reset signal to let ESC start normal operation */
    rcu_periph_reset_disable(RCU_ESCRST);

    delay_ms(2);

}

/*!
    \brief      This function initializes the Process Data Interface (PDI) and the host controller.
    \param[in]  none
    \param[out] none
    \retval     0 if initialization was successful
*/
UINT8 HW_Init(void)
{
    UINT32 intMask;

    esc_enable();
    
   do {
        intMask = 0x0093;
        HW_EscWriteDWord(intMask, ESC_AL_EVENTMASK_OFFSET);
        intMask = 0x02;
        HW_EscReadDWord(intMask, ESC_AL_EVENTMASK_OFFSET);
    } while(intMask != 0x0093);
    

#if AL_EVENT_ENABLED
    INIT_ESC_INT;
    HW_ResetALEventMask(0x0);
    ENABLE_ESC_INT();
#endif

#if DC_SUPPORTED
    INIT_SYNC0_INT;
    INIT_SYNC1_INT;

    ENABLE_SYNC0_INT;
    ENABLE_SYNC1_INT;
#endif

    INIT_ECAT_TIMER;
    START_ECAT_TIMER;

    return 0;
}

/*!
    \brief      This function shall be implemented if hardware resources need to be released
                when the sample application stops
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HW_Release(void)
{
    /* TODO: Add implementation to release hardware resources when needed */
}

/*!
    \brief      This function updates the EtherCAT run and error led
    \param[in]  RunLed            desired EtherCAT Run led state
    \param[in]  ErrLed            desired EtherCAT Error led state
    \param[out] none
    \retval     none
*/
void HW_SetLed(UINT8 RunLed, UINT8 ErrLed)
{
    /* TODO: Add implementation to control EtherCAT Run and Error LEDs */
}

/*!
    \brief      This function sets AL Event register mask
    \param[in]  intMask          interrupt mask to set (enables specific interrupts)
    \param[out] none
    \retval     none
*/
void HW_SetALEventMask(UINT16 intMask)
{
    HW_EscWriteWord(intMask, ESC_AL_EVENTMASK_OFFSET);
}

/*!
    \brief      Read data from the EtherCAT Slave Controller (ESC)
    \param[in]  pData       pointer to a byte array which receives the read data.
    \param[in]  Address     EtherCAT ASIC address (upper limit is 0x1FFF) for access.
    \param[in]  Len         Access size in Bytes.
    \param[out] none
    \retval     none
*/
void HW_EscRead(MEM_ADDR *pData, UINT16 Address, UINT16 Len)
{
    if(Address >= 0x1000U) {
#if ESC_PDI_DIRECT
        pdi_direct_read_pdram(pData, Address, Len);
#endif
#if ESC_PDI_INDIRECT
        pdi_indirect_read_pdram(pData, Address, Len);
#endif
    } else {
#if ESC_PDI_DIRECT
        pdi_direct_read_reg(pData, Address, Len);
#endif
#if ESC_PDI_INDIRECT
        pdi_indirect_read_reg(pData, Address, Len);
#endif
    }
}

/*!
    \brief      Write data to the EtherCAT Slave Controller (ESC)
    \param[in]  pData       pointer to a byte array which contains data to write.
    \param[in]  Address     EtherCAT ASIC address (upper limit is 0x1FFF) for access.
    \param[in]  Len         Access size in Bytes.
    \param[out] none
    \retval     none
*/
void HW_EscWrite(MEM_ADDR *pData, UINT16 Address, UINT16 Len)
{
    if(Address >= 0x1000U) {
#if ESC_PDI_DIRECT
        pdi_direct_write_pdram(pData, Address, Len);
#endif
#if ESC_PDI_INDIRECT
        pdi_indirect_write_pdram(pData, Address, Len);
#endif
    } else {
#if ESC_PDI_DIRECT
        pdi_direct_write_reg(pData, Address, Len);
#endif
#if ESC_PDI_INDIRECT
        pdi_indirect_write_reg(pData, Address, Len);
#endif
    }
}

/*!
    \brief      This function gets the current content of ALEvent register
    \param[in]  none
    \param[out] none
    \retval     first two Bytes of ALEvent register (0x220)
*/
UINT16 HW_GetALEventRegister(void)
{
    GetInterruptRegister();
    return EscALEvent.Word;
}

#if INTERRUPTS_SUPPORTED
/*!
    \brief      This function gets the current content of ALEvent register from interrupt context.
                The behavior is equal to "HW_GetALEventRegister()"
    \param[in]  none
    \param[out] none
    \retval     first two Bytes of ALEvent register (0x220)
*/
UINT16 HW_GetALEventRegister_Isr(void)
{
    ISR_GetInterruptRegister();
    return EscALEvent.Word;
}
#endif /* INTERRUPTS_SUPPORTED */

/*!
    \brief      This function makes a logical AND with the AL Event Mask register (0x204)
    \param[in]  intMask          interrupt mask (disabled interrupt shall be zero)
    \param[out] none
    \retval     none
*/
void HW_ResetALEventMask(UINT16 intMask)
{
    UINT16 mask = 0;

    HW_EscReadWord(mask, ESC_AL_EVENTMASK_OFFSET);

    mask &= intMask;

    HW_EscWriteWord(mask, ESC_AL_EVENTMASK_OFFSET);
    HW_EscReadWord(nAlEventMask, ESC_AL_EVENTMASK_OFFSET);
}

#if AL_EVENT_ENABLED
/*!
    \brief      Interrupt service routine for the PDI interrupt from the EtherCAT Slave Controller
    \param[in]  none
    \param[out] none
    \retval     none
*/
void EcatIsr(void)
{
    if(exti_interrupt_flag_get(EXTI_6) != RESET) {
        PDI_Isr();
        ACK_ESC_INT;
    }
}
#endif /* AL_EVENT_ENABLED */

#if DC_SUPPORTED
/*!
    \brief      Interrupt service routine for the interrupts from SYNC0
    \param[in]  none
    \param[out] none
    \retval     none
*/
void Sync0Isr(void)
{
    if(exti_interrupt_flag_get(EXTI_3) != RESET) {
        Sync0_Isr();
        ACK_SYNC0_INT;
    }
}

/*!
    \brief      Interrupt service routine for the interrupts from SYNC1
    \param[in]  none
    \param[out] none
    \retval     none
*/
void Sync1Isr(void)
{
    if(exti_interrupt_flag_get(EXTI_0) != RESET) {
        Sync1_Isr();
        ACK_SYNC1_INT;
    }
}
#endif /* DC_SUPPORTED */

#if ECAT_TIMER_INT
/*!
    \brief      Timer 2 ISR (0.1ms) - Handles timer-based tasks for EtherCAT
    \param[in]  none
    \param[out] none
    \retval     none
*/
void TimerIsr(void)
{
    if(timer_interrupt_flag_get(TIMER2, TIMER_INT_FLAG_UP) != RESET) {
        ECAT_CheckTimer();
        ECAT_TIMER_ACK_INT;
    }
}
#endif /* ECAT_TIMER_INT */