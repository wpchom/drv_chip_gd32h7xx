/*!
    \file    dsi_lcd_cfg.h
    \brief   the header file of the DSI LCD
    
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

#ifndef DSI_TEST_CFG_H
#define DSI_TEST_CFG_H

#include "dsi_lcd.h"

#define LCM_TEST
#define LCM_VD480x800_TEST

#ifdef LCM_VD480x800_TEST
#define LCM_VIDEO_MODE_TEST
#define HORIZONTAL_SYNCHRONOUS_PULSE  2
#define HORIZONTAL_BACK_PORCH         60
#define ACTIVE_WIDTH                  480
#define HORIZONTAL_FRONT_PORCH        10
#define VERTICAL_SYNCHRONOUS_PULSE    2
#define VERTICAL_BACK_PORCH           20
#define ACTIVE_HEIGHT                 800
#define VERTICAL_FRONT_PORCH          10
#define PIXEL_CLK                     (13000000U)       /* TLI pixel clock */
#define DATA_LANE_NUM                  TWO_DATA_LANE
#define LP_CLK                        (12500000U)       /* DSI clock configuration */
#define HS_RATE                       (700000000U)      /* MIPI DSI rate */
#endif /* LCM_VD480x800_TEST */

/* pixel data format */
#define DSI_FMT_RGB565              (1U << 7)
#define DSI_FMT_RGB666              (2U << 7)
#define DSI_FMT_RGB666_PACKED       (3U << 7)
#define DSI_FMT_RGB888              (4U << 7)
#define PIXEL_FOMART                DSI_FMT_RGB565

#ifdef DEBUG
#define LOG(fmt, ...)                     printf(fmt, ##__VA_ARGS__)
#else
#define LOG(fmt, ...)
#endif /* DEBUG */

#endif /* DSI_TEST_CFG_H */
