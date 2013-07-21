/*
 * adc.h
 *
 *  Created on: 16 avr. 2013
 *      Author: alexandre.github@gmail.com
 */

#ifndef ADC_H_
#define ADC_H_
#include <msp430g2553.h>
#include <stdint.h>

void vAdc_init(int8_t channel);
int32_t lAdc_GetTemp(void);
int16_t iAdc_GetADC(void);
void vAdc_initTemp(void);
#endif /* ADC_H_ */
