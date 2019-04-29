/*
 * spi.h
 *
 * Created: 2/3/2019 3:06:21 PM
 *  Author: dkyle
 */ 


#include <avr/io.h>

#ifndef spi
#define spi

//SPI PIN CONFIGURATIONS
#define SPI_PORT 	PORTB
#define SPI_DDR 	DDRB

//PINS
#define SCK_POS 	PB5
#define MISO_POS 	PB4
#define MOSI_POS	PB3



//CS : Chip Select
#define CS_PORT PORTC
#define CS_DDR	DDRC
#define CS_POS PC0

#define CS_HIGH() CS_PORT|=(1<<CS_POS)
#define CS_LOW() CS_PORT&=(~(1<<CS_POS))


void SPIInit();
void SPIClose();
uint8_t SPIWrite(uint8_t data);


/********************************************************************

Function To Initialize the Internal SPI Hardware

********************************************************************/
void SPIInit()
{
	//Set up SPI I/O Ports
	SPI_DDR|=((1<<MOSI_POS)|(1<<SCK_POS));

	//Chip Select PIN is initialize as HIGH i.e. Idle
	//CS PIN is always Active Low (inverted)
	CS_DDR|=(1<<CS_POS);
	CS_HIGH();

	/*

	SPI BUS CONFIGURATION
	---------------------
	*Master Mode
	*MSB first
	*CPOL=0
	*CPHA=0
	*Above two implies SPI MODE0
	*SCK Freq = FCPU/16 i.e. 1MHz

	*/

	SPCR|=(1<<SPE)|(1<<MSTR)|(1<<SPR0);
}



/********************************************************************

Function To Disable the Internal SPI Hardware

********************************************************************/
void SPIClose()
{
	SPCR&=(~(1<<SPE));
	SPI_DDR&=(~((1<<MOSI_POS)|(1<<SCK_POS)));
}



/********************************************************************

Function for SPI Transaction.

Arguments:
	uint8_t data : Data to write

Return Value
	uint8_t : Data Read from BUS

********************************************************************/
uint8_t SPIWrite(uint8_t data)
{
	/* Start transmission */
	SPDR = data;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));

	return (SPDR);
}

#endif
