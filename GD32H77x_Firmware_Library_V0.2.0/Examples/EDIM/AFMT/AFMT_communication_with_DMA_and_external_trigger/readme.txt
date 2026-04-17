/*!
    \file    readme.txt
    \brief   description of EDIM_AFMT communication with DMA and external trigger example

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

  This demo is based on the GD32H77EI-EVAL-V1.0 board, It shows how to communicate
 with the AFMT encoder and use DMA to transfer encoder data to the rdata_buffer array.

  The frequency of TIMER0 is fixed at 300MHz, and the counter period is 1kHz. 
It is used to trigger the EDIM_AFMT to communicate with the encoder.

  The EDIM_AFMT sends a command to the encoder requesting 40-bit absolute data, and 
the data returned by the encoder is stored in the EDIM_AFMT_ENC0RDATA0/1/2 registers 
and transferred to the rdata_buffer array via DMA.
 
  The baud rate of the EDIM_AFMT is configured to 2.5 Mbits/s, and the t2 time is set to 1 μs. 
The EDIM_AFMT communication is triggered by TIMER0 with a period of 1 kHz.

  The rdata_buffer array is printed via USART.
