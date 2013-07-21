/*
 * timer.c
 *
 *  Created on: 16 avr. 2013
 *      Author: alexandre.github@gmail.com
 */
#include <msp430g2553.h>
#include "flash.h"




char cRead_Seg(char * pcAddr);
void erase_Seg (char * pcAddr);


void init_Flash()
{
	FCTL2 = FWKEY + FSSEL0 + FN1;             // MCLK/3 for Flash Timing Generator
}


void write_Seg (char * pcAddr,char * acTab)
{
	char *Flash_ptr;                          // Flash pointer
	unsigned int i;
	Flash_ptr = (char *) pcAddr;              // Initialize Flash pointer
	FCTL1 = FWKEY + ERASE;                    // Set Erase bit
	FCTL3 = FWKEY;                            // Clear Lock bit
	*Flash_ptr = 0;                           // Dummy write to erase Flash segment
	FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation
	for (i=0; i< 64; i++){
	   *Flash_ptr++ = *acTab++;                   // Write value to flash
	}
	FCTL1 = FWKEY;                            // Clear WRT bit
	FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
}



void write_SegA (char *pcValue)
{
	write_Seg ((char *)cSegA_startAddr,pcValue);
}

void write_SegB (char *pcValue)
{
	write_Seg ((char *)cSegB_startAddr,pcValue);
}
void write_SegC (char *pcValue)
{
	write_Seg ((char *)cSegC_startAddr,pcValue);
}
void write_SegD (char *pcValue)
{
	write_Seg ((char *)cSegD_startAddr,pcValue);
}


char cRead_SegC (char cIndex)
{
	return cRead_Seg((char *)(cSegD_startAddr+cIndex));
}

char cRead_Seg(char * pcAddr)
{
	char * Flash_ptr= (char *)0x10C0;
	return Flash_ptr[0];
}






void ReadA(){
	char *FlashC;
	FlashC = (char *) 0x1040;
	FCTL1 = FWKEY + ERASE;
	FCTL3 = FWKEY;
	FCTL1 = FWKEY + WRT;
	char i = 0;
	for(i = 0; i < 64; i++) {
		*FlashC++; //data ici
	}
	FCTL1 = FWKEY;
	FCTL3 = FWKEY + LOCK;
}



void WriteA(char *acTab){
	char *Flash_ptr;                          // Flash pointer
	unsigned int i;
	Flash_ptr = (char *) 0x1040;              // Initialize Flash pointer
	FCTL1 = FWKEY + ERASE;                    // Set Erase bit
	FCTL3 = FWKEY;                            // Clear Lock bit
	*Flash_ptr = 0;                           // Dummy write to erase Flash segment
	FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation
	for (i=0; i< 64; i++){
	   *Flash_ptr++ = *acTab++;                   // Write value to flash
	}
	FCTL1 = FWKEY;                            // Clear WRT bit
	FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
}







