/*
 * Timer Interrupt.c
 *
 * Created: 5/28/2020 6:49:44 PM
 * Author : √≈Ÿ—√…¡ƒ«” √≈Ÿ—√…œ”
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

/* timer0 overflow interrupt */
ISR(TIMER0_OVF_vect)
{
	PORTB ^=0xff;					/*toggle PORTB */
	


int main( void )
{
	DDRB=0xFF;						/*make port B as output*/

	sei();
	TIMSK=(1<<TOIE0);				/*Enable Timer0 overflow interrupts */
	
	TCNT0 = 0x0;					/*load TCNT0, count for 10ms*/
	TCCR0 = (1<<CS02) | (1<<CS00);  /* start timer0 with /1024 prescaler*/
	while(1);
}
