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
#include <string.h>
#include <avr/io.h>			/* Include AVR std. library file */
#include <util/delay.h>			/* Include Delay header file */
#include <stdbool.h>
#include "lcd_8bit.h"
#include "adc.h"
#include "spi.h"

void Display(uint16_t adcVoltageResult, uint16_t adcAmpsResult, char row, int channel, bool negative);

int main()
{
	unsigned char charOne[8] = { 0x1B, 0x13, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x11 };
	unsigned char charTwo[8] = { 0x1F, 0x11, 0x1D, 0x1D, 0x11, 0x17, 0x17, 0x11 };
    unsigned char charThree[8] = { 0x1F, 0x11, 0x1D, 0x1D, 0x11, 0x1D, 0x1D, 0x11 };
		
	uint16_t channel1Amps;  // positive adjustable channel
	uint16_t channel1Volts; // positive adjustable channel
	uint16_t channel2Amps;  // 5 Volt non-adjustable channel
	uint16_t channel2Volts; // 5 Volt non-adjustable channel
	uint16_t channel3Amps;  // negative adjustable channel
	uint16_t channel3Volts; // negative adjustable channel
	
	LCD_Init();			/* Initialization of LCD*/
	InitADCEx();
	
	uint16_t adcTestResult = 0;
	
	/* Loads custom characters into LCD memory */
	LCD_Custom_Char(0, charOne);  
	LCD_Custom_Char(1, charTwo);
	LCD_Custom_Char(2, charThree);
	
	while(1)
	{
		_delay_ms(100);
		
		channel1Amps = ReadADCEx(0);
		channel1Volts = ReadADCEx(1);
		channel2Amps = ReadADCEx(2);
		channel2Volts = ReadADCEx(3);
		channel3Amps = ReadADCEx(4);
		channel3Volts = ReadADCEx(5);
		
		adcTestResult++;
		Display(channel1Volts, channel1Amps, 0, 1, false);
		Display(channel3Volts, channel3Amps, 1, 3, true);
	}
}

void Display(uint16_t adcVoltageResult,  uint16_t adcAmpsResult, char row, int channel, bool negative)
{
	float volts;
	float amps;
	char voltsStartPos;
	char ampsStartPos;
	char voltsBuffer[9];
	char ampsBuffer[9];
	
	/* Builds Voltage Output string to be displayed on LCD */
	
	if (adcVoltageResult > 199) {
		if (negative) {
			volts = (float)adcVoltageResult*.005f*-1;
			sprintf(voltsBuffer, "%6.2f V", volts); //builds buffer with 6 total characters with precision 2
		}
		else {
			volts = (float)adcVoltageResult*.005f;
			sprintf(voltsBuffer, "%6.2f V", volts); //builds buffer with 6 total characters with precision 2
		}
	}
	
	else {
		if (negative) {
			volts = (float)adcVoltageResult*.005f*1000*-1;
			sprintf(voltsBuffer, "%5.0f mV", volts); //builds buffer with 5 total characters with precision 0
		} 
		else {
			volts = (float)adcVoltageResult*.005f*1000;
			sprintf(voltsBuffer, "%5.0f mV", volts); //builds buffer with 5 total characters with precision 0
		}
	}
	
	if (adcAmpsResult > 199) {
		amps = (float)adcAmpsResult*.001f;
		sprintf(ampsBuffer, "%4.2f A", amps); //builds buffer with 6 total characters with precision 2	
	}	
	else {
		amps = (float)adcAmpsResult;
		sprintf(ampsBuffer, "%3.0f mA", amps); //builds buffer with 5 total characters with precision 0
	}
	
	voltsStartPos = 1;
	ampsStartPos = 10;
	
	if (row == 0) {
		LCD_Command(0x80);
		LCD_Char(channel-1);
		LCD_Command((voltsStartPos & 0x0F)|0x80);	/* Command of first row and required position<16 */
		LCD_String(voltsBuffer);
		LCD_Command((ampsStartPos & 0x0F)|0x80);	/* Command of first row and required position<16 */
		LCD_String(ampsBuffer);
	}
	else if (row == 1) {
		LCD_Command(0xC0);
		LCD_Char(channel-1);
		LCD_Command((voltsStartPos & 0x0F)|0xC0);	/* Command of second row and required position<16 */
		LCD_String(voltsBuffer);
		LCD_Command((ampsStartPos & 0x0F)|0xC0);	/* Command of first row and required position<16 */
		LCD_String(ampsBuffer);
	}


}


	
	
	
	



