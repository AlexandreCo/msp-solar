/*
 * timer.c
 *
 *  Created on: 16 avr. 2013
 *      Author: alexandre.github@gmail.com
 */
#include <msp430g2553.h>
#include "timer.h"

void vtimer_wait(uint16_t uiDelay)
{
	__enable_interrupt();                     // Enable interrupts.
	TACCR0 = uiDelay;                              // Delay to allow Ref to settle
	TACCTL0 |= CCIE;                          // Compare-mode interrupt.
	TACTL = TASSEL_2 | MC_1;                  // TACLK = SMCLK, Up mode.
	LPM0;									  // 85uA
	TACCTL0 &= ~CCIE;                         // Disable timer Interrupt
	__disable_interrupt();
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void ta0_isr(void)					//pour sortir du wait
{
	TACTL = 0;
	LPM0_EXIT;                                // Exit LPM0 on return
}
