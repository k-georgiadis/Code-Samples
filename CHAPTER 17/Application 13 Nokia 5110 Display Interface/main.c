/*
 * Nokia 5110 Display Interface.c
 *
 * Created: 22/12/2021 14:17:55
 * Author :ΚΟΣΜΑΣ ΓΕΩΡΓΙΑΔΗΣ (CosmaOne)
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//N5110 definitions for ATmega8/328. Define these before loading the library.
#define RS	PINB0
#define DC	PINB1
#define CE	PINB2 //This pin is active low pin.

//SPI definitions. Define these before loading the library.
#define SS			PINB2
#define MOSI		PINB3
#define MISO		PINB4
#define SCK			PINB5

#include "N5110.h" //Our library for implementing the SPI protocols and communication functions for the N5110 display.

int main(void)
{
	N5110_Init(); //Initialize Nokia5110 display.
	N5110_Clear(); //Clear screen.
	

	char str[] = "N5110 Display\nInterface\n\nG&K\nElectronics \xFE\xFF"; //Build string and add our flag at the end.
	N5110_Data(str); //Send and display string.

	while (1) 
    { }
}
