/*
 * AVMeter.c
 *
 * Created: 2/3/2019 3:00:00 PM
 * Author : dkyle
 * Program: avrdude -c usbtiny -p m328 -U flash:w:AVMeter.hex:i
 */ 


#define F_CPU 1000000UL			/* Define CPU Frequency e.g. here 8MHz */
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>			/* Include AVR std. library file */
#include <util/delay.h>			/* Include Delay header file */
#include "lcd.h"
#include "adc.h"
#include "spi.h"

void DisplayVolts(uint16_t volts, char row, char pos);
void DisplayAmps(uint16_t apms);

int main()
{
	LCD_Init();			/* Initialization of LCD*/
	InitADCEx();
	uint16_t channel1Result;
	
	while(1)
	{
		_delay_ms(100);
		channel1Result=ReadADCEx(5); //1 bit = 0.005 Volts
		DisplayVolts(channel1Result, 0, 9);
		
	}
}

void DisplayVolts(uint16_t channelResult, char row, char pos)

{
	float channelVolts;
	char channelVolts_str[15];
	char startPos;
	
	LCD_Clear();
	
	channelVolts = (float)channelResult*.005f;
	dtostrf(channelVolts,6,3,channelVolts_str);
	startPos = pos - strlen(channelVolts_str);
	
	if (row == 0 && startPos<16)
		LCD_Command((startPos & 0x0F)|0x80);	/* Command of first row and required position<16 */
	else if (row == 1 && startPos<16)
		LCD_Command((startPos & 0x0F)|0xC0);	/* Command of second row and required position<16 */
		
	LCD_String(channelVolts_str);		/* Call LCD string function */
	LCD_String_xy(0,pos+1,"V");

}


	
	
	
	



