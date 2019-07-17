/*Copyright (c) 2015, Artem Kashkanov
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list
 of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this
list of conditions and the following disclaimer in the documentation and/or other
materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#include <inttypes.h>
#include <msp430.h>

#include "tmanager.h"
#include "stdlib.h"

#ifndef NULL
#define NULL
#endif


//TManager Time Slots

#define SLOT_QUANTITY			(8)
#define SLOT_BUTTONS 			0x00
#define SLOT_SEMAPHORES			0x01
#define SLOT_LED_BLINK 			0x02
#define SLOT_SEGMENT_BLINK 		0x03
#define SLOT_SOUND_BLINK 		0x04
#define RF_BIND_SLOT 			0x05
#define PAIR_SLOT				0x06

//Semaphores slot:
#define SLOT_RF_SEMAPHORE		0x00
#define SLOT_UART_SEMAPHORE		0x01

#define BUTTON_SECOND_FUNCTION  0x100


#define BUTTON_UPDATE_TIME 	(100)//100 ms update time


#define SUCCESS    1
#define FAIL       0

#endif // DEFINITIONS_H_
