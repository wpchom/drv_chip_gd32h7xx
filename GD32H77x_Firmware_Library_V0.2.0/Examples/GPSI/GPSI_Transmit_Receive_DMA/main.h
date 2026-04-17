/*!
    \file    main.h
    \brief   GPSI data transfer by dma

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

#ifndef MAIN_H
#define MAIN_H

/* Definition for GPSI Pins */
#define  GPSI_PCLK_GPIO_CLK         RCU_GPIOA
#define  GPSI_VSYNC_RDY_GPIO_CLK    RCU_GPIOB
#define  GPSI_HSYNC_DE_GPIO_CLK     RCU_GPIOH

#define  GPSI_D0_GPIO_CLK        RCU_GPIOA
#define  GPSI_D1_GPIO_CLK        RCU_GPIOA
#define  GPSI_D2_GPIO_CLK        RCU_GPIOB
#define  GPSI_D3_GPIO_CLK        RCU_GPIOC
#define  GPSI_D4_GPIO_CLK        RCU_GPIOE
#define  GPSI_D5_GPIO_CLK        RCU_GPIOB
#define  GPSI_D6_GPIO_CLK        RCU_GPIOB
#define  GPSI_D7_GPIO_CLK        RCU_GPIOB
#define  GPSI_D8_GPIO_CLK        RCU_GPIOH
#define  GPSI_D9_GPIO_CLK        RCU_GPIOH
#define  GPSI_D10_GPIO_CLK       RCU_GPIOB
#define  GPSI_D11_GPIO_CLK       RCU_GPIOD
#define  GPSI_D12_GPIO_CLK       RCU_GPIOD
#define  GPSI_D13_GPIO_CLK       RCU_GPIOD
#define  GPSI_D14_GPIO_CLK       RCU_GPIOA
#define  GPSI_D15_GPIO_CLK       RCU_GPIOC

#define  GPSI_PCLK_PIN           GPIO_PIN_6
#define  GPSI_VSYNC_RDY_PIN      GPIO_PIN_7
#define  GPSI_HSYNC_DE_PIN       GPIO_PIN_8

#define  GPSI_D0_PIN             GPIO_PIN_9
#define  GPSI_D1_PIN             GPIO_PIN_10
#define  GPSI_D2_PIN             GPIO_PIN_13
#define  GPSI_D3_PIN             GPIO_PIN_9
#define  GPSI_D4_PIN             GPIO_PIN_4
#define  GPSI_D5_PIN             GPIO_PIN_6
#define  GPSI_D6_PIN             GPIO_PIN_8
#define  GPSI_D7_PIN             GPIO_PIN_9
#define  GPSI_D8_PIN             GPIO_PIN_6
#define  GPSI_D9_PIN             GPIO_PIN_7
#define  GPSI_D10_PIN            GPIO_PIN_5
#define  GPSI_D11_PIN            GPIO_PIN_2
#define  GPSI_D12_PIN            GPIO_PIN_12
#define  GPSI_D13_PIN            GPIO_PIN_13
#define  GPSI_D14_PIN            GPIO_PIN_5
#define  GPSI_D15_PIN            GPIO_PIN_4

#define  GPSI_PCLK_GPIO_PORT         GPIOA
#define  GPSI_VSYNC_RDY_GPIO_PORT    GPIOB
#define  GPSI_HSYNC_DE_GPIO_PORT     GPIOH

#define  GPSI_D0_GPIO_PORT       GPIOA
#define  GPSI_D1_GPIO_PORT       GPIOA
#define  GPSI_D2_GPIO_PORT       GPIOB
#define  GPSI_D3_GPIO_PORT       GPIOC
#define  GPSI_D4_GPIO_PORT       GPIOE
#define  GPSI_D5_GPIO_PORT       GPIOB
#define  GPSI_D6_GPIO_PORT       GPIOB
#define  GPSI_D7_GPIO_PORT       GPIOB
#define  GPSI_D8_GPIO_PORT       GPIOH
#define  GPSI_D9_GPIO_PORT       GPIOH
#define  GPSI_D10_GPIO_PORT      GPIOB
#define  GPSI_D11_GPIO_PORT      GPIOD
#define  GPSI_D12_GPIO_PORT      GPIOD
#define  GPSI_D13_GPIO_PORT      GPIOD
#define  GPSI_D14_GPIO_PORT      GPIOA
#define  GPSI_D15_GPIO_PORT      GPIOC

#define  GPSI_PCLK_AF            GPIO_AF_13
#define  GPSI_VSYNC_RDY_AF       GPIO_AF_13
#define  GPSI_HSYNC_DE_AF        GPIO_AF_13
#define  GPSI_D0_AF              GPIO_AF_13
#define  GPSI_D1_AF              GPIO_AF_13
#define  GPSI_D2_AF              GPIO_AF_13
#define  GPSI_D3_AF              GPIO_AF_13
#define  GPSI_D4_AF              GPIO_AF_13
#define  GPSI_D5_AF              GPIO_AF_13
#define  GPSI_D6_AF              GPIO_AF_13
#define  GPSI_D7_AF              GPIO_AF_13
#define  GPSI_D8_AF              GPIO_AF_13
#define  GPSI_D9_AF              GPIO_AF_13
#define  GPSI_D10_AF             GPIO_AF_13
#define  GPSI_D11_AF             GPIO_AF_13
#define  GPSI_D12_AF             GPIO_AF_13
#define  GPSI_D13_AF             GPIO_AF_13
#define  GPSI_D14_AF             GPIO_AF_13
#define  GPSI_D15_AF             GPIO_AF_13

#endif /* MAIN_H */
