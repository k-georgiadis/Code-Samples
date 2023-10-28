/*
 * Interrupt External.c
 * Program to use External (Hardware) Interrupts.
 * Created: 7/21/2020 
 * Author : √≈Ÿ—√…¡ƒ«” √≈Ÿ—√…œ”
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*Interrupt Service Routine for INT0*/
ISR(INT0_vect)
{
	PORTB =~PORTB;		/* Toggle PORTB */
	_delay_ms(100);  	/* Software denouncing control delay */
	
}

int main(void)
{
	
	DDRB=0xFF;		    /* Make PORTB as output PORT*/
	PORTB=0;
	DDRD= (1<<PIND2);	/* PORTD as input */
	PORTD= (1<<PIND2);	/* Make pull up high */
	
	   GICR = (1 << INT0);    /* Enable INT0*/  
	   MCUCR |= (1<<ISC01 | 1<<ISC00); /* Trigger INT0 on rising edge */
	
	sei();			/* Enable Global Interrupt */
	
	while(1);
}


