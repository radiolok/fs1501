#include <msp430.h> 


#include "driver/uart.h"

#define TIMER_1USEC (16)
#define TIMER_NORMAL_TICK (TIMER_1USEC*1000)

void uartCallback(uint8_t cmd){
    UartSendByte(cmd);
}


uint8_t readStatus = 0;


inline void sendStart()
{
    P1DIR |= BIT0;
    P1OUT |= BIT0;
    _delay_cycles(TIMER_NORMAL_TICK>>2);
    P1OUT &= ~BIT0;
}


inline void sendStop()
{
    P1DIR |= BIT4;
    P1OUT |= BIT4;
    _delay_cycles(TIMER_NORMAL_TICK);
    P1OUT &= ~BIT4;
}

/*
uint8_t symbol = 0x20;
void toggle(void)
{
    if (readStatus)
    {
        UartSendByte(symbol++);
        if (symbol > 0x7f)
        {
            symbol = 0x20;
        }
    }


}
*/

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
    P1OUT |= BIT3;
    P1REN |= BIT3 + BIT6 + BIT7;

    P2REN |= BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5;

    P2OUT &= ~(0x3F);

    P1IE |= BIT7;

    UartInit();

    UartAddCallback(uartCallback);

    TimerStart();


    while(1)
    {
        __bis_SR_register(LPM1_bits + GIE);
    }
}


void __attribute__ ((interrupt(PORT1_VECTOR))) GetData(void)
{
    if (P1IFG & BIT7)
    {
        //read data
        uint8_t data = P2IN & 0x3f;
        data |= P1IN & BIT6;
        UartSendByte(data);
    }

}

void __attribute__ ((interrupt(TIMER0_A1_VECTOR))) TAIFTick(void)
{
    if (TA0IV & TA0IV_TAIFG)
    {
        if (readStatus != (P1IN & BIT3))
        {
            readStatus = (P1IN & BIT3);
            if (readStatus)
            {
                sendStart();
            }
            else
            {
                sendStop();
            }
        }
        //TA0R = TIMER_NORMAL_TICK;
    }
}

