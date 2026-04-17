/*!
    \file  readme.txt
    \brief description of the GPSI example
    
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

    This demo is based on the  GD32H77EI-EVAL-V1.0 board， it shows how to use GPSI to
transfer and receive data by DMA.
    Firstly, for the board acting as the transmitter, #define the TRANSMIT_BOARD macro. 
For the board acting as the receiver, #undef TRANSMIT_BOARD macro. Secondly, both the 
transmitter and receiver need to use the same clock source. It is recommended to use a 
custom clock, toggling via PF14 for better control. Once the receiver receives the data, 
it compares it with the transmitted data, and if they match, the LED will light up.


The PA9,PA10,PB13,PC9,PE4,PB6,PB8,PB9,PB7,PH8 pins of the two boards need to be connected.
The PF14 or PA8 is clock send pin, PA6 of two board both need to connect to PF14 or PA8.