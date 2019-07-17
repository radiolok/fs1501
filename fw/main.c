#include <msp430.h> 
#include "HAL/HAL_PMM.h"



#include "driver/uart.h"

void ClockInit(){

    /*MCLK and SMCLK = DCO frequency about 2MHz*/
    UCSCTL1 |= DCORSEL2;//DCORSEL = 4
    UCSCTL0 |= DCO0 + DCO1;

#ifdef TABLO
    UCSCTL5 |= DIVS1;
#endif
    // ---------------------------------------------------------------------
    // Enable 32kHz ACLK
    P5SEL |= 0x03;              // Select XIN, XOUT on P5.0 and P5.1
    UCSCTL6 &= ~XT1OFF;         // XT1 On, Highest drive strength
    UCSCTL6 |= XCAP_3;          // Internal load cap

    UCSCTL3 = SELA__XT1CLK;     // Select XT1 as FLL reference
    UCSCTL4 = SELA__XT1CLK | SELS__DCOCLKDIV | SELM__DCOCLKDIV;

    // ---------------------------------------------------------------------
    // Configure CPU clock for 12MHz
    _BIS_SR(SCG0);              // Disable the FLL control loop
    UCSCTL0 = 0x0000;           // Set lowest possible DCOx, MODx
    UCSCTL1 = DCORSEL_5;        // Select suitable range
    UCSCTL2 = FLLD_1 + 0x16E;   // Set DCO Multiplier
    _BIC_SR(SCG0);              // Enable the FLL control loop

    // Worst-case settling time for the DCO when the DCO range bits have been
    // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
    // UG for optimization.
    // 32 x 32 x 12 MHz / 32,768 Hz = 375000 = MCLK cycles for DCO to settle
    __delay_cycles(375000);

    // Loop until XT1 & DCO stabilizes, use do-while to insure that
    // body is executed at least once
    do
    {
        UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG  + DCOFFG);
        SFRIFG1 &= ~OFIFG;      // Clear fault flags
    }
    while (0);
}


void uartCallback(uint8_t cmd){
    UartSendByte(cmd);
}

void toggle(void)
{
      if (P4IN & BIT7)
    {
        P4OUT &= ~(BIT7 + BIT3);
    }
    else
    {
        P4OUT |= BIT7 + BIT3;
    }

}

/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    // ---------------------------------------------------------------------
    // Configure PMM
    SetVCore(3);

    // Set global high power request enable
    PMMCTL0_H = 0xA5;
    PMMCTL0_L |= PMMHPMRE;
    PMMCTL0_H = 0x00;

    ClockInit();//24MHz on ACLK 12 MHz on SMCLK


	UCSCTL4 |= SELA0 + SELA1 + SELS0 + SELS2 + SELM0 + SELM2;// ACLK = XT2CLK
	//UCSCTL5 |= DIVPA2 + DIVPA0;
	//UCSCTL6 |= XT2DRIVE1 + XT2DRIVE0;

	/*TA0CTL |= TASSEL0 + MC1 + TAIE + TAIFG;//ACLK*/
	P4DIR |= BIT7 + BIT3;
	P4OUT = 0;

    TA0CCR0 = 34000;//ticks to 1ms

	 UartInit();
	 UartAddCallback(uartCallback);

    TA0CTL |= TASSEL0 + MC0 + ID1 + ID0 + TAIE + TAIFG;
   while(1)
   {

       __bis_SR_register(LPM2_bits + GIE);

   }

	return 0;
}

void __attribute__ ((interrupt(TIMER1_A1_VECTOR))) Timer1Tick1_Isr(void){

 }//not needed

//1ms interrupt
void __attribute__ ((interrupt(TIMER0_A1_VECTOR))) Timer1Tick0_Isr(void){

    if (TA0IV & TA0IV_TA0IFG)
    {
        toggle();
    }
 }

