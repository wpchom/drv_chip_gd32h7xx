/*!
    \file    main.c
    \brief   PKCAU modular exponentiation of fast mode
    
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
#include <string.h>
#include <stdio.h>
#include "gd32h77ei_eval.h"

#define ME_A_SIZE   128
#define ME_E_SIZE   128
#define ME_MOD_SIZE 128
#define ME_RES_SIZE 128

#define MOD_EXP_FAST

/* operand A */
uint8_t oprd_a[ME_A_SIZE]         = {0xfb, 0x26, 0x2c, 0x69, 0xd4, 0x09, 0xb5, 0x5b, 0x06, 0x0a, 0xba, 0x06, 0x34, 0x08, 0x4a, 0xd7,
                                     0x09, 0x3a, 0xa5, 0x20, 0xb6, 0xa0, 0x8b, 0xb0, 0xdc, 0x0e, 0xcf, 0x52, 0xc9, 0xe7, 0x62, 0x65,
                                     0x68, 0x23, 0x3c, 0xaa, 0xe0, 0xd0, 0xe3, 0xeb, 0xee, 0xe9, 0x73, 0x30, 0xea, 0xb2, 0x11, 0x59,
                                     0xe7, 0x17, 0x5e, 0xe0, 0x71, 0xb9, 0x91, 0x2f, 0xdb, 0xfc, 0x61, 0xdd, 0x22, 0xd2, 0x21, 0x58,
                                     0x74, 0xec, 0xd2, 0x7a, 0x62, 0xf5, 0xcb, 0x58, 0xb8, 0x19, 0x63, 0x1e, 0x39, 0x81, 0x80, 0xca,
                                     0x2f, 0xc6, 0x78, 0xa1, 0x89, 0x6c, 0x54, 0xbf, 0xe1, 0xce, 0x12, 0x87, 0x0e, 0x75, 0xce, 0x2e,
                                     0x99, 0x46, 0xb0, 0x49, 0xcd, 0xe5, 0xcf, 0xc5, 0x73, 0x61, 0x81, 0x63, 0x5a, 0x93, 0xe6, 0x60,
                                     0x27, 0x5a, 0xa0, 0xc0, 0x45, 0x2d, 0x13, 0xcc, 0x90, 0xf5, 0xdd, 0xc1, 0xd2, 0x99, 0x5d, 0x15
                                    };

/* operand B */
uint8_t exp_e[ME_E_SIZE]          = {0xe0, 0xd5, 0x42, 0xef, 0xbc, 0x1d, 0x35, 0x95, 0xc4, 0x3, 0x8b, 0xcb, 0x6d, 0x49, 0x2c, 0x7f,
                                     0x7c, 0xc4, 0x57, 0xee, 0x5a, 0xec, 0x6d, 0xe6, 0xfb, 0x85, 0xf3, 0x74, 0xf2, 0xe2, 0x99, 0xd4,
                                     0xbe, 0xe5, 0x05, 0x98, 0x2a, 0x87, 0x97, 0x49, 0xdb, 0x81, 0x9a, 0x7e, 0x68, 0xee, 0xad, 0x5a,
                                     0x6d, 0xa7, 0xbb, 0xf2, 0xd8, 0x55, 0x59, 0x89, 0xf9, 0x99, 0xde, 0x72, 0x15, 0x98, 0x31, 0x93,
                                     0xd2, 0xd5, 0x43, 0x93, 0x71, 0xb7, 0x6f, 0x8b, 0xe3, 0xee, 0x6c, 0x12, 0xde, 0xb5, 0xba, 0xf7,
                                     0x00, 0xd1, 0x02, 0x28, 0x4c, 0x68, 0xf7, 0x25, 0x70, 0x3f, 0xaa, 0x6f, 0xe4, 0x15, 0x81, 0x3b,
                                     0x39, 0xc8, 0x7a, 0x5f, 0x19, 0x40, 0xc4, 0xee, 0x32, 0xdb, 0xd4, 0xc3, 0x6a, 0x59, 0x19, 0x88,
                                     0xb9, 0x9b, 0xd2, 0x9d, 0x72, 0x5f, 0xa5, 0x6d, 0x8f, 0x0f, 0xa6, 0x66, 0xf9, 0x83, 0x38, 0xb1
                                    };

/* modulus value n */
uint8_t modulus_n[ME_MOD_SIZE]    = {0xfe, 0xff, 0x26, 0xb3, 0x53, 0xd4, 0x22, 0x72, 0xb5, 0xa6, 0xea, 0x7d, 0xf4, 0x90, 0xbe, 0xf0,
                                     0x96, 0x2a, 0xdc, 0x11, 0x88, 0xa3, 0x81, 0xf1, 0xb8, 0xf7, 0x29, 0x96, 0x54, 0xcc, 0xc8, 0xd6,
                                     0x5a, 0x39, 0x22, 0xba, 0xf1, 0x0a, 0xbb, 0xe9, 0x35, 0xb2, 0xfc, 0x2c, 0x66, 0x6f, 0x26, 0x88,
                                     0xa0, 0xe4, 0x01, 0xd3, 0xd1, 0x83, 0xb0, 0x3c, 0x9a, 0x2a, 0x13, 0x4d, 0xef, 0x18, 0x0e, 0x6e,
                                     0xa8, 0xf8, 0xf4, 0xae, 0xca, 0x61, 0xc3, 0x9d, 0x75, 0x25, 0x2e, 0x73, 0x48, 0xc6, 0xeb, 0x97,
                                     0x5c, 0x7d, 0x24, 0x5f, 0xfa, 0x2e, 0xdc, 0x4c, 0x58, 0x38, 0xaf, 0xf5, 0x51, 0x15, 0xff, 0x09,
                                     0x1f, 0x21, 0x75, 0x61, 0x33, 0x23, 0x55, 0xca, 0x4b, 0xdf, 0x6c, 0xfe, 0x32, 0x06, 0xfa, 0x9e,
                                     0x5e, 0x38, 0x2b, 0x2f, 0x26, 0x21, 0xe2, 0xbe, 0xe3, 0x5e, 0xc0, 0x55, 0x70, 0xd2, 0xfd, 0x39
                                    };

/* expected result of modular exponentiation */
uint8_t expected_res[ME_RES_SIZE] = {0xc2, 0x68, 0xcf, 0x8e, 0x95, 0x01, 0x29, 0x4b, 0xc0, 0x49, 0x18, 0x26, 0x4d, 0x8a, 0xa6, 0xfa,
                                     0x0b, 0xa8, 0x84, 0x63, 0x4f, 0x07, 0x4f, 0x69, 0x8b, 0x33, 0xb6, 0x15, 0x64, 0xea, 0xfd, 0x19,
                                     0x0d, 0x3c, 0x52, 0xc0, 0x50, 0xe2, 0x53, 0x29, 0x29, 0x52, 0x16, 0x45, 0xc3, 0x1e, 0x99, 0xc2,
                                     0xd4, 0xcd, 0x7a, 0x16, 0x64, 0xbf, 0x9f, 0xd9, 0xf1, 0x71, 0x79, 0xa5, 0x2f, 0xf8, 0xea, 0xfb,
                                     0xdc, 0xe9, 0x39, 0xef, 0x45, 0x9c, 0xec, 0x46, 0x4d, 0x6c, 0xaf, 0x11, 0x69, 0xe1, 0x3a, 0xa3,
                                     0x75, 0x77, 0xa6, 0xea, 0x1f, 0x57, 0x11, 0x66, 0xc4, 0xe1, 0xba, 0xdb, 0x76, 0x7c, 0xfd, 0x99,
                                     0x0d, 0x5e, 0xf7, 0x91, 0x34, 0x08, 0xba, 0x4e, 0xc0, 0x3d, 0xfc, 0xb0, 0xdd, 0xf8, 0x4f, 0x5d,
                                     0x0f, 0x37, 0x83, 0x46, 0x4d, 0xf9, 0xa1, 0x6f, 0xdc, 0xa6, 0xa3, 0xaa, 0x60, 0xe4, 0x3e, 0x8b
                                    };

/* montgomery parameter */
uint8_t mont_para[ME_MOD_SIZE]    = {0};

/* configure parameters of modular exponentiation */
void pkcau_modular_exp_config(uint8_t *results);
/* configure exponentiation parameter operation */
void pkcau_mont_para_config(uint8_t *results);

/*!
    \brief      enable the CPU Chache
    \param[in]  none
    \param[out] none
    \retval     none
*/
void cache_enable(void)
{
    /* Enable I-Cache */
    SCB_EnableICache();

    /* Enable D-Cache */
    SCB_EnableDCache();
}

/*!
    \brief      configure the MPU attributes
    \param[in]  none
    \param[out] none
    \retval     none
*/
void mpu_config(void)
{
    mpu_region_init_struct mpu_init_struct;
    mpu_region_struct_para_init(&mpu_init_struct);

    /* disable the MPU */
    ARM_MPU_Disable();
    ARM_MPU_SetRegion(0, 0);

    /* configure the MPU attributes for the entire 4GB area, Reserved, no access */
    /* This configuration is highly recommended to prevent Speculative Prefetching of external memory,
       which may cause CPU read locks and even system errors */
    mpu_init_struct.region_base_address  = 0x0;
    mpu_init_struct.region_size          = MPU_REGION_SIZE_4GB;
    mpu_init_struct.access_permission    = MPU_AP_NO_ACCESS;
    mpu_init_struct.access_bufferable    = MPU_ACCESS_NON_BUFFERABLE;
    mpu_init_struct.access_cacheable     = MPU_ACCESS_NON_CACHEABLE;
    mpu_init_struct.access_shareable     = MPU_ACCESS_SHAREABLE;
    mpu_init_struct.region_number        = MPU_REGION_NUMBER0;
    mpu_init_struct.subregion_disable    = 0x87;
    mpu_init_struct.instruction_exec     = MPU_INSTRUCTION_EXEC_NOT_PERMIT;
    mpu_init_struct.tex_type             = MPU_TEX_TYPE0;
    mpu_region_config(&mpu_init_struct);
    mpu_region_enable();

    /* enable the MPU */
    ARM_MPU_Enable(MPU_MODE_PRIV_DEFAULT);
}

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/

int main(void)
{
    uint8_t mod_exp_res[ME_MOD_SIZE];

    /* enable the CPU Cache */
    cache_enable();
    mpu_config();

    /* initialize LED1 and LED2 */
    gd_eval_led_init(LED1);
    gd_eval_led_init(LED2);
    /* turn off LED1 and LED2 */
    gd_eval_led_off(LED1);
    gd_eval_led_off(LED2);
    /* enable PKCAU clock */
    rcu_periph_clock_enable(RCU_PKCAU);
    /* enable PKCAU power */
    pmu_pkcau_power_enable();
    /* enable PKCAU output isolation */
    pmu_pkcau_output_isolation_enable();

    /* configure montgomery parameter operation */
    pkcau_mont_para_config(mont_para);
    /* configure modular exponentiation operation */
    pkcau_modular_exp_config(mod_exp_res);
    /* if success, LED1 is on */
    if(memcmp(mod_exp_res, expected_res, ME_MOD_SIZE)) {
        gd_eval_led_on(LED2);
    } else {
        gd_eval_led_on(LED1);
    }
    while(1) {
    }
}

/*!
    \brief      configure parameters of modular exponentiation of fast mode
    \param[in]  none
    \param[out] results: output buffer
    \retval     none
*/
void pkcau_modular_exp_config(uint8_t *results)
{
    /* modular parameter structure */
    pkcau_mod_exp_parameter_struct pkcau_mod_exp_parameter;

    /* initialize the PKCAU modular parameter structure */
    pkcau_mod_exp_struct_para_init(&pkcau_mod_exp_parameter);
    /* initialize the modular parameters */
    pkcau_mod_exp_parameter.oprd_a        = (uint8_t *)oprd_a;
    pkcau_mod_exp_parameter.oprd_a_len    = sizeof(oprd_a);
    pkcau_mod_exp_parameter.exp_e         = (uint8_t *)exp_e;
    pkcau_mod_exp_parameter.e_len         = ME_E_SIZE;
    pkcau_mod_exp_parameter.modulus_n     = (uint8_t *)modulus_n;
    pkcau_mod_exp_parameter.modulus_n_len = ME_MOD_SIZE;
    pkcau_mod_exp_parameter.mont_para     = (uint8_t *)mont_para;
    pkcau_mod_exp_parameter.mont_para_len = sizeof(mont_para);
    /* execute modular exponentiation of fast mode operation */
    pkcau_mod_exp_operation(&pkcau_mod_exp_parameter, PKCAU_MODE_MOD_EXP_FAST, results);
}

/*!
    \brief      configure montgomery parameter operation
    \param[in]  none
    \param[out] results: output buffer
    \retval     none
*/
void pkcau_mont_para_config(uint8_t *results)
{
    /* montgomery parameter structure */
    pkcau_mont_parameter_struct pkcau_mont_parameter;

    /* initialize the PKCAU montgomery parameter structure */
    pkcau_mont_struct_para_init(&pkcau_mont_parameter);
    /* initialize the montgomery parameters */
    pkcau_mont_parameter.modulus_n     = (uint8_t *)modulus_n;
    pkcau_mont_parameter.modulus_n_len = ME_MOD_SIZE;
    /* execute montgomery parameter operation */
    pkcau_mont_param_operation(&pkcau_mont_parameter, results);
}

