//******************************************************************************
//   USCI_A0, 9600 UART Echo ISR, DCO SMCLK
//
//   Description: RX and TX ISR used. Normal mode is LPM0.
//   Baud rate divider with 1MHz = 1MHz/9600 = ~104.2
//   ACLK = n/a, MCLK = SMCLK = CALxxx_1MHZ = 1MHz
//
//                MSP430G2xx3
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |     P1.2/UCA0TXD|------------>
//            |                 | 9600 - 8N1
//            |     P1.1/UCA0RXD|<------------
//
//******************************************************************************
#include <msp430g2553.h>
#include "uart.h"
#include "adc.h"
#include "timer.h"
#include "flash.h"

#define WDG_STOP (WDTCTL = WDTPW + WDTHOLD)                 // Stop WDT
#define STOP_WAITING	20			/// 20 ~ 1s
#define SAMPLE_TIME_SEC	300			/// 300s =5min

uint8_t ucWait=0;

#define cNbMaxSample	30
void vClock_Init()
{
	if (CALBC1_1MHZ==0xFF)					// If calibration constant erased
	{
		while(1);                               // do not load, trap CPU!!
	}
	DCOCTL = 0;                               // Select lowest DCOx and MODx settings
	BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
	DCOCTL = CALDCO_1MHZ;

}



void rxCallback(uint8_t ucCar)
{
	ucWait=STOP_WAITING;
}

int16_t aiRamTab[32];

int main(void)
{
	int8_t cIndex;
	int32_t lTempDec=0;
	int32_t lAdcDec=0;
	int16_t uiSegAddr=cSegD_startAddr;
	int8_t cTabIndex=2;
	uint32_t ulUptime=0;
	uint16_t uiSampleTime;
	char *FlashC;

	WDG_STOP;
	vClock_Init();
	vUart_init(rxCallback);
	vtimer_wait(30);
	init_Flash();
	P1DIR |= 0x41;                            // P1DIR is a register that configures the direction (DIR) of a port pin as an output or an input.
	P1OUT = 0x01;


	vUart_puts("Uptime=");
	FlashC = (char *) 0x1000;
	lTempDec= *((uint32_t *) FlashC);
	uiSampleTime=(lTempDec>>16)&0xFFFF;
	vUart_puti(uiSampleTime,10);
	vUart_puts(".");
	uiSampleTime=(lTempDec)&0xFFFF;
	vUart_puti(uiSampleTime,10);
	vUart_puts("\n\r");

	while(1)
	{
		/// get temperature
		vAdc_initTemp();
		lTempDec=0;
		for(cIndex=0;cIndex<10;cIndex++)
			lTempDec+=lAdc_GetTemp();
		vUart_puts("T=");
		vUart_puti(lTempDec/10,10);
		vUart_puts("\n\r");

		aiRamTab[cTabIndex++]=(int16_t)lTempDec;

		if(cTabIndex>30){
			//ecriture de l'entete (uptime)
			*((uint32_t *) &aiRamTab[0])=ulUptime;
			ulUptime++;
			cTabIndex=2;
			write_Seg ((char *)uiSegAddr,(char *)aiRamTab);
			uiSegAddr+=0x40;
			if(uiSegAddr>=cSegA_startAddr){
				uiSegAddr=cSegD_startAddr;
			}
		}

		for(uiSampleTime=0;uiSampleTime<SAMPLE_TIME_SEC;uiSampleTime++)
		{
			for(ucWait=0;ucWait<STOP_WAITING;ucWait++)
				vtimer_wait(60000);
		}

		P1OUT ^= 0x41;                        // Toggle P1.0 using exclusive-OR operation (^=)


		/*
		vAdc_init(5);
		lAdcDec=0;
		for(cIndex=0;cIndex<10;cIndex++)
			lAdcDec+=iAdc_GetADC();

		vUart_puts("; p1.5=");
		vUart_puti(lAdcDec/10,10);
		vUart_puts("\n\r");

		for(ucWait=0;ucWait<STOP_WAITING;ucWait++)
			vtimer_wait(60000);
		P1OUT ^= 0x41;                        // Toggle P1.0 using exclusive-OR operation (^=)

*/

	}
}


