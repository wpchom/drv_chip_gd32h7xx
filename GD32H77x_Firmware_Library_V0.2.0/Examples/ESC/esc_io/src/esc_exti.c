/*!
    \file    esc_exti.c
    \brief   esc exti configuration

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
#include "esc_exti.h"



/*!
    \brief      configure EXTI IRQ
    \param[in]  none
    \param[out] none
    \retval     none
*/
void EXTI_IRQ_Configuration(void)
{

    /** Enable the GPIOH Clock */
    rcu_periph_clock_enable(RCU_GPIOH);
    rcu_periph_clock_enable(RCU_SYSCFG);

    /** Configure pin as input floating */
    gpio_mode_set(GPIOH, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_6);

    /* enable and set key EXTI interrupt priority */
    nvic_irq_enable(EXTI5_9_IRQn, 2U, 2U);

    /* connect  EXTI line to button GPIO pin */
    syscfg_exti_line_config(EXTI_SOURCE_GPIOH, EXTI_SOURCE_PIN6);

    /* Configure EXTI Line6 */
    exti_init(EXTI_6, EXTI_INTERRUPT, EXTI_TRIG_RISING);
    exti_interrupt_flag_clear(EXTI_6);

}

/*!
    \brief      configure EXTI SYNC0
    \param[in]  none
    \param[out] none
    \retval     none
*/
void EXTI_SYNC0_Configuration(void)
{
    /** Enable the GPIOE Clock */
    rcu_periph_clock_enable(RCU_GPIOE);
    rcu_periph_clock_enable(RCU_SYSCFG);

    /** Configure pin as input floating */
    gpio_mode_set(GPIOE, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_3);

    /* enable and set key EXTI interrupt priority */
    nvic_irq_enable(EXTI3_IRQn, 2U, 2U);

    /* connect  EXTI line to button GPIO pin */
    syscfg_exti_line_config(EXTI_SOURCE_GPIOE, EXTI_SOURCE_PIN3);

    /* Configure EXTI Line3 */
    exti_init(EXTI_3, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(EXTI_3);
}

/*!
    \brief      configure EXTI SYNC1
    \param[in]  none
    \param[out] none
    \retval     none
*/
void EXTI_SYNC1_Configuration(void)
{
    /** Enable the GPIOD Clock */
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_SYSCFG);

    /** Configure pin as input floating */
    gpio_mode_set(GPIOD, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_0);


    /* enable and set key EXTI interrupt priority */
    nvic_irq_enable(EXTI0_IRQn, 2U, 2U);

    /* connect  EXTI line to button GPIO pin */
    syscfg_exti_line_config(EXTI_SOURCE_GPIOD, EXTI_SOURCE_PIN0);

    /* Configure EXTI Line0 */
    exti_init(EXTI_0, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(EXTI_0);
}

/*!
    \brief      disable global int
    \param[in]  none
    \param[out] none
    \retval     none
*/
void disable_global_int(void)
{
    NVIC_DisableIRQ(EXTI5_9_IRQn);

    NVIC_DisableIRQ(EXTI3_IRQn);

    NVIC_DisableIRQ(TIMER2_IRQn);

}

/*!
    \brief      enable global int
    \param[in]  none
    \param[out] none
    \retval     none
*/
void enable_global_int(void)
{
    NVIC_EnableIRQ(EXTI5_9_IRQn);

    NVIC_EnableIRQ(EXTI3_IRQn);

    NVIC_EnableIRQ(TIMER2_IRQn);
}
