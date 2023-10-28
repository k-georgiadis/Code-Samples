/*
 * Timer Compare Match CTC.c
 * Created: 6/30/2020 2:43:26 PM
 * Author:цеыяциадгс йослас
 */ 

#include "avr/io.h"

int main()
{
	DDRB |= (1 << PINB1); //PB1 (OC1а) as output.
	
	//Mode 4, CTC mode with OCR1A.
	TCCR1A = (1 << COM1A0);
	TCCR1B = (1 << WGM12);
	
	//No prescaler.
	TCCR1B |= (1 << CS10);

    OCR1A = 200; //Compare value.
	
	while (1)
	{}
	
	return 0;
}


