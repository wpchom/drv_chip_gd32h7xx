/*!
    \file    gdesc.h
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

#ifndef GDESC_H
#define GDESC_H

#include "gd32h77x.h"
#include  "esc_exti.h"
#include  "esc_timer.h"
#include  "esc.h"

#define ESE_EEPROM_INTERNAL                         0
#define ESE_EEPROM_EXTERNAL                         1
#define ESC_PDI_INDIRECT                            0
#define ESC_PDI_DIRECT                              1

#if ESE_EEPROM_INTERNAL
#define EEPROM_INTERNAL_ADDRESS        0x24080100
#endif


#define ECAT_TIMER_INC_P_MS                         1000

#define    DISABLE_GLOBAL_INT                       disable_global_int();
#define    ENABLE_GLOBAL_INT                        enable_global_int();
#define    DISABLE_AL_EVENT_INT                     DISABLE_GLOBAL_INT
#define    ENABLE_AL_EVENT_INT                      ENABLE_GLOBAL_INT

#ifndef DISABLE_ESC_INT
#define    DISABLE_ESC_INT()                        NVIC_DisableIRQ(EXTI5_9_IRQn)
#endif
#ifndef ENABLE_ESC_INT
#define    ENABLE_ESC_INT()                         NVIC_EnableIRQ(EXTI5_9_IRQn)
#endif

#if AL_EVENT_ENABLED
#define    INIT_ESC_INT                            EXTI_IRQ_Configuration();
#define    EcatIsr                                 EXTI5_9_IRQHandler
#define    ACK_ESC_INT                             exti_interrupt_flag_clear(EXTI_6);

#endif  /* #if AL_EVENT_ENABLED */

#if DC_SUPPORTED
#define    INIT_SYNC0_INT                         EXTI_SYNC0_Configuration();
#define    Sync0Isr                               EXTI3_IRQHandler
#define    DISABLE_SYNC0_INT                      NVIC_DisableIRQ(EXTI3_IRQn);
#define    ENABLE_SYNC0_INT                       NVIC_EnableIRQ(EXTI3_IRQn);
#define    ACK_SYNC0_INT                          exti_interrupt_flag_clear(EXTI_3);

#define    INIT_SYNC1_INT                         EXTI_SYNC1_Configuration();
#define    Sync1Isr                               EXTI0_IRQHandler
#define    DISABLE_SYNC1_INT                      NVIC_DisableIRQ(EXTI0_IRQn);
#define    ENABLE_SYNC1_INT                       NVIC_EnableIRQ(EXTI0_IRQn);
#define    ACK_SYNC1_INT                          exti_interrupt_flag_clear(EXTI_0);

#endif /* if DC_SUPPORTED */

#if ECAT_TIMER_INT
#define    ECAT_TIMER_ACK_INT                    timer_interrupt_flag_clear(TIMER2 , TIMER_INT_FLAG_UP);
#define    TimerIsr                              TIMER2_IRQHandler
#define    ENABLE_ECAT_TIMER_INT                 NVIC_EnableIRQ(TIMER2_IRQn) ;
#define    DISABLE_ECAT_TIMER_INT                NVIC_DisableIRQ(TIMER2_IRQn) ;

#define INIT_ECAT_TIMER                          ESC_TIMER_Configuration();        /*!< config timer interrupt */
#define STOP_ECAT_TIMER                          DISABLE_ECAT_TIMER_INT;           /*!< disable timer interrupt */
#define START_ECAT_TIMER                         ENABLE_ECAT_TIMER_INT             /*!< enable timer */
#else /* if ECAT_TIMER_INT */
#define INIT_ECAT_TIMER                          ESC_TIMER_Configuration();        /*!< config timer */
#define STOP_ECAT_TIMER                          timer_disable(TIMER2);            /*!< disable timer */
#define START_ECAT_TIMER                         timer_enable(TIMER2);             /*!< enable timer */
#endif /* else if ECAT_TIMER_INT */

#ifndef HW_GetTimer
#define HW_GetTimer()                               timer_counter_read(TIMER2);             /*!< Access to the hardware timer */
#endif

#ifndef HW_ClearTimer
#define HW_ClearTimer()                             timer_counter_value_config(TIMER2,0);   /*!< Clear the hardware timer */
#endif

#define HW_EscReadIsr                               HW_EscRead
#define HW_EscReadByte(ByteValue,Address)           HW_EscRead(((MEM_ADDR *)&(ByteValue)),((UINT16)(Address)),1)  /*!< 8Bit ESC read access */
#define HW_EscReadWord(WordValue, Address)          HW_EscRead(((MEM_ADDR *)&(WordValue)),((UINT16)(Address)),2)  /*!< 16Bit ESC read access */
#define HW_EscReadDWord(DWordValue, Address)        HW_EscRead(((MEM_ADDR *)&(DWordValue)),((UINT16)(Address)),4) /*!< 32Bit ESC read access */
#define HW_EscReadMbxMem(pData,Address,Len)         HW_EscRead(((MEM_ADDR *)(pData)),((UINT16)(Address)),(Len))   /*!< The mailbox data is stored in the local uC memory therefore the default read function is used */

#define HW_EscReadByteIsr(ByteValue,Address)        HW_EscReadIsr(((MEM_ADDR *)&(ByteValue)),((UINT16)(Address)),1)   /*!< Interrupt specific 8Bit ESC read access */
#define HW_EscReadWordIsr(WordValue, Address)       HW_EscReadIsr(((MEM_ADDR *)&(WordValue)),((UINT16)(Address)),2)   /*!< Interrupt specific 16Bit ESC read access */
#define HW_EscReadDWordIsr(DWordValue, Address)     HW_EscReadIsr(((MEM_ADDR *)&(DWordValue)),((UINT16)(Address)),4)  /*!< Interrupt specific 32Bit ESC read access */

#define HW_EscWriteIsr                              HW_EscWrite
#define HW_EscWriteByte(WordValue, Address)         HW_EscWrite(((MEM_ADDR *)&(WordValue)),((UINT16)(Address)),1)   /*!< 8Bit ESC write access */
#define HW_EscWriteWord(WordValue, Address)         HW_EscWrite(((MEM_ADDR *)&(WordValue)),((UINT16)(Address)),2)   /*!< 16Bit ESC write access */
#define HW_EscWriteDWord(DWordValue, Address)       HW_EscWrite(((MEM_ADDR *)&(DWordValue)),((UINT16)(Address)),4)  /*!< 32Bit ESC write access */
#define HW_EscWriteMbxMem(pData,Address,Len)        HW_EscWrite(((MEM_ADDR *)(pData)),((UINT16)(Address)),(Len))    /*!< The mailbox data is stored in the local uC memory therefore the default write function is used */

#define HW_EscWriteWordIsr(WordValue, Address)      HW_EscWriteIsr(((MEM_ADDR *)&(WordValue)),((UINT16)(Address)),2)  /*!< Interrupt specific 16Bit ESC write access */
#define HW_EscWriteDWordIsr(DWordValue, Address)    HW_EscWriteIsr(((MEM_ADDR *)&(DWordValue)),((UINT16)(Address)),4) /*!< Interrupt specific 32Bit ESC write access */

typedef union {
    unsigned short    Word;
    unsigned char    Byte[2];
} UBYTETOWORD;

typedef union {
    UINT8           Byte[2];
    UINT16          Word;
}
UALEVENT;


extern UINT8 HW_Init(void);
extern void HW_Release(void);
extern UINT16 HW_GetALEventRegister(void);

extern UINT16 HW_GetALEventRegister_Isr(void);

extern void HW_ResetALEventMask(UINT16 intMask);
extern void HW_SetALEventMask(UINT16 intMask);
extern void HW_SetLed(UINT8 RunLed, UINT8 ErrLed);

extern void HW_EscRead(MEM_ADDR *pData, UINT16 Address, UINT16 Len);
extern void HW_EscReadIsr(MEM_ADDR *pData, UINT16 Address, UINT16 Len);

extern void HW_EscWrite(MEM_ADDR *pData, UINT16 Address, UINT16 Len);
extern void HW_EscWriteIsr(MEM_ADDR *pData, UINT16 Address, UINT16 Len);

/* configure esc gpio peripheral */
void esc_gpio_config(void);
/* configure esc mii gpio peripheral */
void esc_mii_gpio_config(void);
/* enable esc func */
void esc_enable(void);

#endif  /* GDESC_H */
