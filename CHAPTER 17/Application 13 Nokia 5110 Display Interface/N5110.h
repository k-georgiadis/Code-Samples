/*
 * N5110.h
 *
 * Created: 22/12/2021 17:03:15
 *  Author: œ”Ã¡” √≈Ÿ—√…¡ƒ«”
 */ 
 
#ifndef N5110_H_
#define N5110_H_

#include <util/delay.h>
#include <stdlib.h>
#include <string.h>

#include "char_map.h" //Library with standard and custom ASCII characters/symbols.

//Check if definitions don't exist.
#ifndef DC

	//Default N5110 definitions for ATmega8/328. Define these before loading the library.
	#define RS			PINB0
	#define DC			PINB1
	#define CE			PINB2 //This pin is active low pin.

	//SPI definitions.
	#define SS			PINB2
	#define MOSI		PINB3
	#define MISO		PINB4
	#define SCK			PINB5

#endif

#define MAX_X			84
#define MAX_Y			6	//Maximum number of banks. Each bank contains 8 * MAX_X pixels.
#define MAX_LINE_CHAR	(uint8_t)MAX_X / CHAR_WIDTH //Max characters in a row.

struct N5110_dev
{
	uint8_t cursorX;
	uint8_t cursorY;
};
typedef struct N5110_dev N5110_device;

N5110_device display;

void SPI_Init();
void SPI_Write(char data);
char SPI_Read();

void N5110_Init();
void N5110_Reset();

void N5110_Cmd(unsigned char data);
void N5110_Data(char *data);

void N5110_Clear();

void SPI_Init()
{
	DDRB |= (1 << MOSI) | (1 << SCK) | (1 << SS); //Make MOSI, SCK, SS as Output.
	DDRB &= ~(1 << MISO); //Make MISO as input.

	PORTB &= ~((1 << MOSI) | (1 << SCK)); //Make MOSI and SCK low (0).
	
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0)| (1 << SPR1); //Enable SPI in master mode with Fosc / 4.
	PORTB |= (1 << SS); //Make SS high (1).

	return;
}
void SPI_Write(char data)
{
	char flush_buffer;
	SPDR = data; //Write data to SPI data register.

	while(!(SPSR & (1 << SPIF))); //Wait till transmission complete.

	flush_buffer = SPDR; //Flush received data.
	//Note: SPIF flag is cleared by first reading SPSR (with SPIF set) and then accessing SPDR hence flush buffer used here to access SPDR after SPSR read.

	return;
}
char SPI_Read()
{
	SPDR = 0xFF; //Dummy data.

	while(!(SPSR & (1<<SPIF))); //Wait till transmission complete.

	return(SPDR); //Return received data.
}

void N5110_Init()
{	
	DDRB = (1 << RS) | (1 << DC) | (1 << CE); //Set RS, DC, CE pins as Output.
	
	SPI_Init();
	N5110_Reset();

	N5110_Cmd(0x21); //Use extended instruction set.
	N5110_Cmd(0xA0); //Set the voltage by sending Vop = A0 => VLCD = 5V --> VLCD = a + Vop * b[V], (a = 3.06 and b = 0.06)
	N5110_Cmd(0x07); //Set the temp coefficient to 3.
	N5110_Cmd(0x13); //Set Bias Voltage value for MUX RATE = 1 : 48
	N5110_Cmd(0x20); //Use basic instruction set.
	N5110_Cmd(0x0C); //Display in normal mode.
	
	display.cursorX = 0;
	display.cursorY = 0;

	return;
}
void N5110_Reset()
{
	PORTB &= ~(1 << RS); //Set RST low (0).
	_delay_ms(100); //100 ns minimum. (1 microsecond = 1000 nanoseconds)
	PORTB |= (1 << RS); //Set RST high (1).

	return;
}

void N5110_Cmd(unsigned char data)
{
	uint8_t idle = PORTB & (1 << CE); //Are we in the middle of a transmission?

	PORTB &= ~(1 << DC); //Make DC low (0) for command operation.
	PORTB &= ~(1 << CE); //Make CE  low (0) to start communication with the N5110.

	SPI_Write(data); //Send cmd data on data register.

	PORTB |= (1 << DC); //Make DC high (1) for data transmission.

	if(idle)
		PORTB |= (1 << CE); //Make CE high (1) - communication has ended.
	
	return;
}
void N5110_Data(char *data)
{
	PORTB |= (1 << DC); //Make DC high (1) for data transmission.
	PORTB &= ~(1 << CE); //Make CE low (0) to start communication.

	uint8_t i;

	while(*data)
	{
		//Move cursor to next line if necessary.
		if(*data == '\r' || *data == '\n' || display.cursorX == MAX_LINE_CHAR)
		{
			display.cursorX = 0; //Go back to start.
			N5110_Cmd(0x80);
			
			display.cursorY++; //Next line.

			if(display.cursorY == MAX_Y)
				display.cursorY = 0; //Go back to top.

			N5110_Cmd(0x40 | display.cursorY);
		}
		else
		{
			for(i = 0; i < CHAR_WIDTH; i++)
			{
				uint8_t ram_byte = pgm_read_byte(CHAR_MAP[*data - 1] + i);
				SPI_Write(ram_byte);
			}

			display.cursorX++; //Next position.
		}

		data++; //Next char.
	}

	PORTB |= (1 << CE); //Make CE high (1) - communication has ended.
	return;
}

void N5110_Clear()
{
	PORTB |= (1 << DC); //Make DC high (1) for data transmission.
	PORTB &= ~(1 << CE); //Make CE low (0) to start communication.

	uint16_t i;

	//Clear pixels.
	for(i = 0; i < MAX_X * MAX_Y; i++)
		SPI_Write(0);

	PORTB |= (1 << CE); //Make CE high (1) - communication has ended.

	//Go back to start.
	N5110_Cmd(0x80);
	N5110_Cmd(0x40);

	return;
}

#endif /* N5110_H_ */