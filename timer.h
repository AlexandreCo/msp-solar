/*
 * timer.h
 *
 *  Created on: 16 avr. 2013
 *      Author: alexandre.github@gmail.com
 */

#ifndef TIMER_H_
#define TIMER_H_
#include <msp430g2553.h>
#include <stdint.h>
void vtimer_wait(uint16_t uiDelay);
void vTimer_1s(void);
#define WAIT_1SEC	20			/// 20 ~ 1s

#endif /* TIMER_H_ */
