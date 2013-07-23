/*
 * uart.c
 *
 *  Created on: 16 avr. 2013
 *      Author: alexandre.github@gmail.com
 */

#include <legacymsp430.h>
#include "uart.h"

#define USE_RXINT
#define USE_TXINT

rx_t  tRx;

#ifdef USE_TXINT
static int8_t ucStart=0;
static int8_t ucEnd=0;
static uint8_t ucBufTx[TX_BUF_SIZE];
#endif

void vUart_puti(int16_t iNum,int8_t cBase)
{
	int16_t i;
	char acNum[6];

	itoa(iNum, acNum, cBase);

	for(i=0;i<strlen(acNum);i++)
	{
		vUart_putc(acNum[i]);
	}
}

void vUart_puts(char *str)
{
	while(*str) vUart_putc(*str++);		//Advance though string till end
}

void vUart_putc(uint8_t ucChar)
{
#ifdef USE_TXINT
	ucBufTx[ucEnd++]=ucChar;
	if(ucEnd>(TX_BUF_SIZE-1))
		ucEnd=0;
	enable_txint();
#else
	 while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
	 UCA0TXBUF = ucChar;                    // TX -> RXed character
#endif
}


void vUart_init(rx_t rx)
{
	tRx=rx;
	P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
	P1SEL2 = BIT1 + BIT2 ;                    // P1.1 = RXD, P1.2=TXD
	UCA0CTL1 |= UCSSEL_2;                     // SMCLK

	UCA0BR0 = 104;                            // 1MHz/9600 high
	UCA0BR1 = 0;                              // 1MHz/9600 low
	UCA0MCTL = UCBRS0;                        // use MCLK (1MHz) as baud rate clock
	UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
#ifdef USE_RXINT
	enable_rxint();                          // Enable USCI_A0 RX interrupt
#endif
	disable_txint();						  // Enable USCI_A0 TX interrupt
}

#ifdef USE_RXINT
// reveil le CPU
//#pragma vector=USCIAB0RX_VECTOR
//__interrupt void USCI0RX_ISR(void)
interrupt (USCIAB0RX_VECTOR)
USCI0RX_ISR(void)
{
	tRx(UCA0RXBUF);
	LPM0_EXIT;
}
#endif
#ifdef USE_TXINT
//#pragma vector=USCIAB0TX_VECTOR
//__interrupt void USCI0TX_ISR(void)
interrupt (USCIAB0TX_VECTOR)
USCI0TX_ISR(void)
{
	UCA0TXBUF = ucBufTx[ucStart++];                 // TX next character
	if(ucStart>(TX_BUF_SIZE-1))
		ucStart=0;
	if (ucStart == ucEnd)              // TX over?
		disable_txint();                       // Disable USCI_A0 TX interrupt
}
#endif
