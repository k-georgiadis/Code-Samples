/*
 * TIMER DEMO.c
 *
 * Created: 5/23/2020 6:48:17 PM
 * Author : GEORGIADIS GEORGE
 */ 
#define F_CPU 1000000UL
#include <avr/io.h>

void delay();

int main(void)
{
	DDRB = 0xFF;		/* PORTB as output*/
	//TCNT0 = 0x25;
	//TCCR0 = 0x01;
	
	while(1)  		/* Repeat forever*/
	{
		PORTB = 0x55;
		delay();  	/* Give some delay */
		PORTB = 0xAA;
		delay();
	}
}
//ISR(TIMER0_OVF_vect)
//{
	//TCNT0 = 0x25;
	//PORTB ^= 0x01; //toggle bit 0.
//}
void delay()
{
	TCNT0 =   		/* Load TCNT0*/
	TCCR0 = 0x01;  		/* Timer0, normal mode, no pre-scalar */
	
	while((TIFR & 0x01)==0);  /* Wait for TOV0 to roll over */
	TCCR0 = 0;
    TIFR = 0x01;  		/* Clear TOV0 flag*/
	return;
}