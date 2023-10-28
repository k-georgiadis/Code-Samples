/*
 * Normal Compare Match.c
 * ATmega8
 * Created: 6/24/2020 7:40:41 PM
 * Author : цеыяциадгс йослас
 */

#include "avr/io.h"

int main()
{
	DDRB |= (1 << PINB1); //PB1 (OC1а) as output.
	
	//Normal mode, Compare Mode.
	TCCR1A = (1 << COM1A0);
	TCCR1B = (1 << CS10); //No prescaler.

	//Compare value.
	OCR1A = 0xFFFF;
	
	while (1)
	{}
	
	return 0;
}


