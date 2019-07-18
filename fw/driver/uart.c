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
#include <uart.h>

void (*uartcallback)(uint8_t);

void UartInit(void){

	uartcallback = 0;


    P1SEL |= BIT1 + BIT2 ; // P1.1 = RXD, P1.2=TXD
    P1SEL2 |= BIT1 + BIT2 ; // P1.1 = RXD, P1.2=TXD

	 /*Baud rate 38400*/
	    UCA0CTL0 = 0x00;
	    UCA0CTL1 = UCSSEL1|UCSWRST;
	    /*See table 15-4*/
	    UCA0BR0 = (416-256);
	    UCA0BR1 = 416/256;
	    UCA0MCTL = UCBRS2|UCBRS1;
	    UCA0CTL1 &= ~UCSWRST;

	    IE2 |= UCA0TXIE + UCA0RXIE;
}

void UartAddCallback(void (*f)(uint8_t)){
	uartcallback = f;
}

void UartReleaseCallback(void){
	uartcallback = 0;
}

void UartSendByte(uint8_t data){
	UCA0TXBUF = data;
}



void __attribute__ ((interrupt(USCIAB0TX_VECTOR))) TxEnd(void)
{
    if (IFG2&UCA0TXIFG)
    {
        IFG2 &= ~UCA0TXIFG;
    }
}

void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) RxGet(void)
{
    if (IFG2&UCA0RXIFG)
    {
        uint8_t symbol = UCA0RXBUF;
          if (uartcallback)
              uartcallback(symbol);
    }

}
