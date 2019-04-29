/*
 * adc.h
 *
 * Created: 2/3/2019 3:06:04 PM
 *  Author: dkyle
 */ 


#ifndef adc
#define adc

#include "spi.h"

void InitADCEx();
uint16_t ReadADCEx(uint8_t ch);

/********************************************************************

Function To Initialize the External ADC Chip

Arguments:
		NONE

Retuen Value:
		NONE

NOTE:
		Must be called before performing a conversion.
		Call it at program startup.

********************************************************************/
void InitADCEx()
{
	SPIInit();
}



/********************************************************************

Requests the ADC to perform conversion and send the result.

Arguments:
		uint8_t ch : Channel Number
		For MCP3204 ch is between 0-3 (Total 4 channels)
		For MCP3208 ch is between 0-7 (Total 8 channels)

Return Value:(TYPE uint16_t, i.e a 16bit unsigned int)
		The digital equivalent of analog input on selected channel.
		Since the ADCs are 12 bit the return value is between
		0-4095 (Including both)

********************************************************************/
uint16_t ReadADCEx(uint8_t ch)
{
	uint8_t byte,data_high,data_low;

	byte=0b00000110;

	if(ch>3)
		byte|=0b00000001;

	CS_LOW();
	
	SPIWrite(byte);

	byte=ch<<6;

	data_high=SPIWrite(byte);

	data_high&=0b00001111;

	data_low=SPIWrite(0xFF);

	CS_HIGH();

	return ((data_high<<8)|data_low);
}

#endif

