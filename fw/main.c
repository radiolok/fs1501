#include <msp430.h> 


#include "driver/uart.h"

#define TIMER_1USEC (16)
#define TIMER_NORMAL_TICK (TIMER_1USEC*1000)

void uartCallback(uint8_t cmd){
    UartSendByte(cmd);
}

void toggle(void)
{
      if (P1IN & BIT0)
    {
        P1OUT &= ~(BIT0);
    }
    else
    {
        P1OUT |= BIT0 ;
    }

}


void TimerStart()
{
    //TA0R = TIMER_NORMAL_TICK;
    TA0CCR0 = TIMER_NORMAL_TICK;
    TA0CTL |= TASSEL1 + MC0 +TAIE + TAIFG;// + ID1 + ID0;
}

void TimerStop()
{
    TA0CTL = 0x00;
}


void main(void)
{
   WDTCTL = WDTPW | WDTHOLD;    // stop watchdog timer

    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;
    //BCSCTL2 = DIVS1 + DIVS0;


    P1DIR |= BIT0;

        UartInit();

        UartAddCallback(uartCallback);

        TimerStart();


    while(1)
    {
        __bis_SR_register(LPM1_bits + GIE);
    }
}

void __attribute__ ((interrupt(TIMER0_A1_VECTOR))) TAIFTick(void)
{
    if (TA0IV & TA0IV_TAIFG)
    {
        toggle();
        //TA0R = TIMER_NORMAL_TICK;
    }
}

