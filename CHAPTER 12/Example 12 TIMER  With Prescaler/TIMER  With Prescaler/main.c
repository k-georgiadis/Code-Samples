/*
 * TIMER  With Prescaler.c
 *
 * Created: 5/24/2020 6:03:43 PM
 * Author : √≈Ÿ—√…¡ƒ«” √≈Ÿ—√…œ”
 */ 


#define F_CPU 8000000UL
#include <avr/io.h>

void delay();

int main(void) /* Replace with your application code */
{
     DDRB=0xFF;	
     PORTB=0; 
	 
 while(1) 
{
   PORTB= ~ PORTB;
   delay();
    }
}
void delay()
{
    TCCR0 =(1<<CS02)|(1<<CS00);
	TCNT0 =0x00;
while((TIFR & 0x01)==0); 
    TCCR0 = 0;
	TIFR =0x01;

}

