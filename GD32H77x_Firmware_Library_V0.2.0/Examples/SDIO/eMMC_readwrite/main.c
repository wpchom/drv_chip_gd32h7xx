/*!
    \file    main.c
    \brief   eMMC read and write demo

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
#include "emmc.h"
#include <stdio.h>
#include "gd32h77ei_eval.h"

#define SDR_BUSMODE_1BIT  0
#define SDR_BUSMODE_4BIT  1
#define SDR_BUSMODE_8BIT  2
#define DDR_BUSMODE_4BIT  3
#define DDR_BUSMODE_8BIT  4

/* config SDIO bus mode, select: SDR_BUSMODE_1BIT/SDR_BUSMODE_4BIT/SDR_BUSMODE_8BIT/DDR_BUSMODE_4BIT/DDR_BUSMODE_8BIT */
#define SDIO_BUSMODE       SDR_BUSMODE_4BIT
/* config data transfer mode, select: EMMC_POLLING_MODE/EMMC_DMA_MODE */
#define SDIO_DTMODE        EMMC_POLLING_MODE

//#define DATA_PRINT                                          /* uncomment the macro to print out the data */

uint32_t __attribute__((aligned(32))) buf_write[512];       /* store the data written to the card */
uint32_t __attribute__((aligned(32))) buf_read[512];        /* store the data read from the card */

void nvic_config(void);
emmc_error_enum emmc_io_init(void);
void cache_enable(void);
void card_info_get(void);

int main()
{
    emmc_error_enum emmc_error;
    uint16_t i = 5;
    emmc_rca = 1U;
#ifdef DATA_PRINT
    uint8_t *pdata;
#endif /* DATA_PRINT */

    /* enable the CPU Cache */
    cache_enable();

    /* configure the NVIC and USART */
    nvic_config();
    gd_eval_com_init(EVAL_COM);
    gd_eval_led_init(LED1);
    gd_eval_led_init(LED2);

    /* turn off all the LEDs */
    gd_eval_led_off(LED1);
    gd_eval_led_off(LED2);

    /* initialize the eMMC */
    do {
        emmc_error = emmc_io_init();
    } while((EMMC_OK != emmc_error) && (--i));

    if(i) {
        printf("\r\n eMMC init success!\r\n");
    } else {
        printf("\r\n eMMC init failed!\r\n");
        /* turn on LED1, LED2 */
        gd_eval_led_on(LED1);
        gd_eval_led_on(LED2);
        while(1) {
        }
    }

    /* get the information of the card and print it out by USART */
    card_info_get();

    /* init the write buffer */
    for(i = 0; i < 512; i++) {
        buf_write[i] = i;
    }

    /* clean and invalidate buffer in D-Cache */
    SCB_CleanInvalidateDCache_by_Addr(buf_write, 512 * 4);

    printf("\r\n\r\n eMMC test:");

    /* single block operation test */
    emmc_error = emmc_block_write(buf_write, 100, 512);
    if(EMMC_OK != emmc_error) {
        printf("\r\n Block write fail!");
        /* turn on LED1, LED2 */
        gd_eval_led_on(LED1);
        gd_eval_led_on(LED2);
        while(1) {
        }
    } else {
        printf("\r\n Block write success!");
    }

    emmc_error = emmc_block_read(buf_read, 100, 512);
    if(EMMC_OK != emmc_error) {
        printf("\r\n Block read fail!");
        /* turn on LED1, LED2 */
        gd_eval_led_on(LED1);
        gd_eval_led_on(LED2);
        while(1) {
        }
    } else {
        printf("\r\n Block read success!");
#ifdef DATA_PRINT
        SCB_CleanInvalidateDCache_by_Addr(buf_read, 512 * 4);
        pdata = (uint8_t *)buf_read;
        /* print data by USART */
        printf("\r\n");
        for(i = 0; i < 128; i++) {
            printf(" %3d %3d %3d %3d ", *pdata, *(pdata + 1), *(pdata + 2), *(pdata + 3));
            pdata += 4;
            if(0 == (i + 1) % 4) {
                printf("\r\n");
            }
        }
#endif /* DATA_PRINT */
    }

    /* multiple blocks operation test */
    emmc_error = emmc_multiblocks_write(buf_write, 200, 512, 3);
    if(EMMC_OK != emmc_error) {
        printf("\r\n Multiple block write fail!");
        /* turn on LED1, LED2 */
        gd_eval_led_on(LED1);
        gd_eval_led_on(LED2);
        while(1) {
        }
    } else {
        printf("\r\n Multiple block write success!");
    }

    emmc_error = emmc_multiblocks_read(buf_read, 200, 512, 3);
    if(EMMC_OK != emmc_error) {
        printf("\r\n Multiple block read fail!");
        /* turn on LED1, LED2 */
        gd_eval_led_on(LED1);
        gd_eval_led_on(LED2);
        while(1) {
        }
    } else {
        printf("\r\n Multiple block read success!");
#ifdef DATA_PRINT
        SCB_CleanInvalidateDCache_by_Addr(buf_read, 512 * 4);
        pdata = (uint8_t *)buf_read;
        /* print data by USART */
        printf("\r\n");
        for(i = 0; i < 512; i++) {
            printf(" %3d %3d %3d %3d ", *pdata, *(pdata + 1), *(pdata + 2), *(pdata + 3));
            pdata += 4;
            if(0 == (i + 1) % 4) {
                printf("\r\n");
            }
        }
#endif /* DATA_PRINT */
    }

    printf("\r\n eMMC test successfully!");
    while(1) {};
}

/*!
    \brief      configure the NVIC
    \param[in]  none
    \param[out] none
    \retval     none
*/
void nvic_config(void)
{
    nvic_irq_enable(SDIO0_IRQn, 2U, 0);
}

/*!
    \brief      initialize the card, get the card information, set the bus mode and transfer mode
    \param[in]  none
    \param[out] none
    \retval     emmc_error_enum
*/
emmc_error_enum emmc_io_init(void)
{
    emmc_error_enum status = EMMC_OK;

    status = emmc_init();
    if(EMMC_OK == status) {
        status = emmc_card_select_deselect(emmc_rca);
    }

    if(EMMC_OK == status) {
        status = emmc_card_extcsd_get();
    }

    if(EMMC_OK == status) {
        /* set bus mode */
#if (SDIO_BUSMODE == DDR_BUSMODE_8BIT)
        status = emmc_bus_mode_config(SDIO_BUSMODE_8BIT, SDIO_DATA_RATE_DDR);
#elif (SDIO_BUSMODE == DDR_BUSMODE_4BIT)
        status = emmc_bus_mode_config(SDIO_BUSMODE_4BIT, SDIO_DATA_RATE_DDR);
#elif (SDIO_BUSMODE == SDR_BUSMODE_8BIT)
        status = emmc_bus_mode_config(SDIO_BUSMODE_8BIT, SDIO_DATA_RATE_SDR);
#elif (SDIO_BUSMODE == SDR_BUSMODE_4BIT)
        status = emmc_bus_mode_config(SDIO_BUSMODE_4BIT, SDIO_DATA_RATE_SDR);
#else
        status = emmc_bus_mode_config(SDIO_BUSMODE_1BIT, SDIO_DATA_RATE_SDR);
#endif
    }

    if(EMMC_OK == status) {
        /* set data transfer mode */
        status = emmc_transfer_mode_config(SDIO_DTMODE);
    }
    return status;
}

/*!
    \brief      get the eEMMC information and print it out by USRAT
    \param[in]  none
    \param[out] none
    \retval     none
*/
void card_info_get(void)
{
    uint32_t device_size = 0,i = 0;

    printf("\r\n## eMMC CID Regiseter: ");
    printf("\r\n %08x %08x %08x %08x \r\n", emmc_cid[0], emmc_cid[1], emmc_cid[2], emmc_cid[3]);

    printf("\r\n## eMMC CSD Regiseter: ");
    printf("\r\n %08x %08x %08x %08x \r\n", emmc_csd[0], emmc_csd[1], emmc_csd[2], emmc_csd[3]);

    /* print data by USART */
    printf("\r\n## EXT_CSD Regiseter(From low to high): ");
    printf("\r\n");
    for(i = 0; i < 512; i++) {
        printf(" %02x", emmc_extcsd[i]);
        if(0 == (i + 1) % 16) {
            printf("\r\n");
        }
    }
    device_size = (((uint32_t)emmc_extcsd[212] & 0x000000FF) | ((uint32_t)emmc_extcsd[213] << 8U & 0x0000FF00) 
                  | ((uint32_t)emmc_extcsd[214] << 16U & 0x00FF0000) | ((uint32_t)emmc_extcsd[215] << 24U & 0xFF000000)) / 2;
    printf("\r\n## Device size is %dKB ##", device_size);
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
