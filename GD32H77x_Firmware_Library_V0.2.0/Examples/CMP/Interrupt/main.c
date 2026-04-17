/*!
    \file    main.c
    \brief   CMP Interrupt demo

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
#include "systick.h"
#include "gd32h77ei_eval.h"

/* configure RCU peripheral */
void rcu_config(void);
/* configure GPIO peripheral */
void gpio_config(void);
/* configure CMP peripheral */
void cmp_config(void);
/* configure EXTI peripheral */
void exti_config(void);
/* enable the CPU cache */
void cache_enable(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* enable the CPU cache */
    cache_enable();
    /* configure systick */
    systick_config();

    /* initialize LED2 */
    gd_eval_led_init(LED2);

    /* configure RCU peripheral */
    rcu_config();
    /* configure GPIO peripheral */
    gpio_config();
    /* configure EXTI peripheral */
    exti_config();
    /* configure CMP peripheral */
    cmp_config();

    while(1) {
    }
}

/*!
    \brief      configure RCU peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_config(void)
{
    /* enable GPIOE clock */
    rcu_periph_clock_enable(RCU_GPIOE);
    /* enable CMP clock */
    rcu_periph_clock_enable(RCU_CMP);
}

/*!
    \brief      configure GPIO peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_config(void)
{
    /* configure PE9 as CMP1 input */
    gpio_mode_set(GPIOE, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_9);
}

/*!
    \brief      configure CMP peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void cmp_config(void)
{
    /* initialize CMP1 */
    cmp_deinit(CMP1);
    /* when using VREFINT as inverting input, the voltage bridge and voltage scaler should be enable */
    cmp_voltage_scaler_enable(CMP1);
    cmp_scaler_bridge_enable(CMP1);

    /* configure CMP1 mode */
    cmp_mode_init(CMP1, CMP_MODE_HIGHSPEED, CMP_INVERTING_INPUT_VREFINT, CMP_HYSTERESIS_NO);
    /* configure CMP1 non-inverting input */
    cmp_noninverting_input_select(CMP1, CMP_NONINVERTING_INPUT_PB0_PE9);
    /* configure CMP1 output */
    cmp_output_init(CMP1, CMP_OUTPUT_POLARITY_NONINVERTED);

    /* enable CMP1 */
    cmp_enable(CMP1);
    /* delay 1ms */
    delay_1ms(1);
}

/*!
    \brief      configure EXTI peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void exti_config(void)
{
    /* clear exti line21 interrupt flag */
    exti_flag_clear(EXTI_21);
    /* initialize exti line21 */
    exti_init(EXTI_21, EXTI_INTERRUPT, EXTI_TRIG_BOTH);
    /* enable exti line21 interrupt */
    exti_interrupt_enable(EXTI_21);

    /* configure CMP NVIC */
    nvic_irq_enable(CMP0_1_IRQn, 0, 0);
}

/*!
    \brief      enable the CPU cache
    \param[in]  none
    \param[out] none
    \retval     none
*/
void cache_enable(void)
{
    /* enable i-cache */
    SCB_EnableICache();
    /* enable d-cache */
    SCB_EnableDCache();
}
