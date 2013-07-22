/*
 * timer.c
 *
 *  Created on: 16 avr. 2013
 *      Author: alexandre.github@gmail.com
 */
#include <msp430g2553.h>
#include "flash.h"

void vFlash_init()
{
	FCTL2 = FWKEY + FSSEL0 + FN1;             // MCLK/3 for Flash Timing Generator
}


void vFlash_EraseInfoSeg(uint8_t ucNumSeg)
{
	char *Flash_ptr;
	Flash_ptr = (char *) cSegD_startAddr+ucNumSeg*cInfoSeg_Size;              // Initialize Flash pointer
	FCTL1 = FWKEY + ERASE;                    // Set Erase bit
	FCTL3 = FWKEY;                            // Clear Lock bit
	*Flash_ptr = 0;                           // Dummy write to erase Flash segment

}



void vFlase_vWriteInfoSeg(uint16_t uiAddr, char * acData, char cNbByte)
{
	char *Flash_ptr;
	unsigned int i;
	Flash_ptr = (char *)uiAddr;
	FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation
	FCTL3 = FWKEY;                             // Clear Lock bit

	for (i=0; i< cNbByte; i++){
	   *Flash_ptr++ = *acData++;                   // Write value to flash
	}
	FCTL1 = FWKEY;                            // Clear WRT bit
	FCTL3 = FWKEY + LOCK;                     // Set LOCK bit

}
