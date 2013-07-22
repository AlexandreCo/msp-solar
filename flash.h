/*
 * flash.h
 *
 *  Created on: 21 juil. 2013
 *      Author: alexandre.github@gmail.com
 */


#ifndef FLASH_H_
#define FLASH_H_
#include <msp430g2553.h>
#include <stdint.h>

#define cSegA_startAddr	0x10C0
#define cSegB_startAddr	0x1080
#define cSegC_startAddr	0x1040
#define cSegD_startAddr	0x1000

#define cInfoSeg_startAddr	0x1000
#define cInfoSeg_NbMax		0x04
#define cInfoSeg_Size		0x40

enum{
	cSegD=0,
	cSegC,
	cSegB,
	cSegA,
};


void vFlash_init(void);

void vFlash_EraseInfoSeg(uint8_t ucNumSeg);
void vFlase_vWriteInfoSeg(uint16_t uiAddr, char * acData, char cNbByte);


void write_SegA (char *pcValue);
void write_SegB (char *pcValue);
void write_SegC (char *pcValue);
void write_SegD (char *pcValue);

void write_Seg (char * pcAddr,char * acTab);

#endif /* TIMER_H_ */
