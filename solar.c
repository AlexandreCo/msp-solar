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
#define WAITING_TIME_SEC	300			/// 300s =5min

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

static uint16_t _uiFlashAddr=cSegD_startAddr;

void rxCallback(uint8_t ucCar)
{
	if(ucCar=='R'){
		_uiFlashAddr=cSegD_startAddr;
		vFlash_EraseInfoSeg(cSegD);
		vFlash_EraseInfoSeg(cSegC);
		vFlash_EraseInfoSeg(cSegB);
		vUart_puts("Raz...\n\r");
	}
}

int16_t aiRamTab[32];


void vLed_Init(void)
{
	P1DIR |= 0x41;                            // P1DIR is a register that configures the direction (DIR) of a port pin as an output or an input.
	P1OUT = 0x01;
}

void vLed_Toogle(void)
{
	P1OUT ^= 0x41;                        // Toggle P1.0 using exclusive-OR operation (^=)
}
void vAffiche(int iTemp,int iAdc)
{
	vUart_puts("T=");
	vUart_puti(iTemp,10);
	vUart_puts(" p=");
	vUart_puti(iAdc,10);
	vUart_puts("\n\r");
}
void vSaveData(int iTemp,int iAdc)
{
	uint16_t uiCpt;

	if(_uiFlashAddr==cSegA_startAddr)
	{
		_uiFlashAddr=cSegD_startAddr;
	}

	if(_uiFlashAddr==cSegD_startAddr)
	{
		uiCpt= (* (uint16_t *)cSegD_startAddr)+1;
		vFlash_EraseInfoSeg(cSegD);
		vFlase_vWriteInfoSeg(_uiFlashAddr, (char*)&uiCpt, sizeof(uiCpt));
		_uiFlashAddr+=2;
		_uiFlashAddr+=2;	//2 bytes free
	}

	if(_uiFlashAddr==cSegC_startAddr)
	{
		uiCpt= (* (uint16_t *)cSegC_startAddr)+1;
		vFlash_EraseInfoSeg(cSegC);
		vFlase_vWriteInfoSeg(_uiFlashAddr, (char*)&uiCpt, sizeof(uiCpt));
		_uiFlashAddr+=2;
		_uiFlashAddr+=2;	//2 bytes free
	}

	if(_uiFlashAddr==cSegB_startAddr)
	{
		uiCpt= (* (uint16_t *)cSegB_startAddr)+1;
		vFlash_EraseInfoSeg(cSegB);
		vFlase_vWriteInfoSeg(_uiFlashAddr, (char*)&uiCpt, sizeof(uiCpt));
		_uiFlashAddr+=2;
		_uiFlashAddr+=2;	//2 bytes free
	}


	vFlase_vWriteInfoSeg(_uiFlashAddr, (char*)&iTemp, sizeof(iTemp));
	_uiFlashAddr+=2;
	vFlase_vWriteInfoSeg(_uiFlashAddr, (char*)&iAdc, sizeof(iAdc));
	_uiFlashAddr+=2;

}
int main(void)
{
	int16_t iTemp=0;
	int16_t iAdc=0;
	uint16_t uiSampleTime;

	WDG_STOP;
	vClock_Init();
	vUart_init(rxCallback);
	vtimer_wait(30);
	vFlash_init();
	vLed_Init();
	_uiFlashAddr=cSegD_startAddr;
	while(1)
	{
		/// get temperature
		vAdc_initTemp();
		iTemp=lAdc_GetTemp();
						/// get port 1.5
		vAdc_init(5);
		iAdc=iAdc_GetADC();

		vSaveData(iTemp,iAdc);
		vAffiche(iTemp,iAdc);

		vLed_Toogle();

		for(uiSampleTime=0;uiSampleTime<WAITING_TIME_SEC;uiSampleTime++)
		{
			vTimer_1s();
		}
	}
}


