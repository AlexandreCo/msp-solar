/*
 * uart.h
 *
 *  Created on: 16 avr. 2013
 *      Author: alexandre.github@gmail.com
 */

#ifndef _UART
#define _UART
#include <msp430g2553.h>
#include <stdint.h>

typedef void (*rx_t)(uint8_t);

#define disable_txint() IE2 &= ~UCA0TXIE
#define enable_txint() IE2 |= UCA0TXIE

#define disable_rxint() IE2 &= ~UCA0RXIE
#define enable_rxint() IE2 |= UCA0RXIE
#define TX_BUF_SIZE 8

void vUart_putc(uint8_t cChar);
void vUart_puts(char *str);
void vUart_puti(int16_t iNum,int8_t cBase);
void vUart_init(rx_t rx);
#endif
