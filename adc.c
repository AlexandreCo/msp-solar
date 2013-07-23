/*
 * adc.c
 *
 *  Created on: 16 avr. 2013
 *      Author: alexandre.github@gmail.com
 */
#include <legacymsp430.h>
#include "adc.h"
/// param in channel : INCH_10
void vAdc_initTemp(void)
{
	ADC10CTL1 = INCH_10 + ADC10DIV_3;         // Temp Sensor ADC10CLK/4
	ADC10CTL0 = SREF_1 + ADC10SHT_3 + REFON + ADC10ON + ADC10IE;

	ADC10AE0 |= 0x10;   // PA.4 ADC option select

}

void vAdc_init(int8_t channel)
{
	ADC10CTL1 = channel*INCH_1 + ADC10DIV_3;                       // input A1
	ADC10CTL0 = SREF_0 + ADC10SHT_3 + REFON  + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled

	ADC10AE0 |= (1<<channel);                         // PA.5 ADC option select

}


int32_t lAdc_GetTemp(void)
{
	int32_t lTemp;
	int32_t lDegC;
	ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
	__bis_SR_register(CPUOFF + GIE);        // LPM0 with interrupts enabled

	// oC = ((A10/1024)*1500mV)-986mV)*1/3.55mV = A10*423/1024 - 278
	lTemp = ADC10MEM;
	lDegC = ((lTemp - 673) * 4230) / 1024;
	return lDegC;
}



int16_t iAdc_GetADC(void)
{
	int32_t lTemp;
	ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
	__bis_SR_register(CPUOFF + GIE);        // LPM0 with interrupts enabled

	// oC = ((A10/1024)*1500mV)-986mV)*1/3.55mV = A10*423/1024 - 278
	lTemp = ADC10MEM;

	return lTemp;
}

// ADC10 interrupt service routine
//#pragma vector=ADC10_VECTOR
//__interrupt void ADC10_ISR (void)				//sortir du wait sur fin de conversion
interrupt (ADC10_VECTOR)
ADC10_ISR (void)
{
	__bic_SR_register_on_exit(CPUOFF);        // Clear CPUOFF bit from 0(SR)
}
