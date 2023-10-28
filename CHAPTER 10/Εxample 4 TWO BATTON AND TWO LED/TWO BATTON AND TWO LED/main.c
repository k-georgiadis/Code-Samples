/*
 * BUTTON ON-OFF-LED.c
 *
 * Created: 29/08/2023 7:20:04 PM
 * Author : ΓΕΩΡΓΙΑΔΗΣ ΚΟΣΜΑΣ
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	
	DDRC=0b00000011;
	DDRD=0b00000011;
	PORTC |= (1<<PINC0) | (1<<PINC1);

	char button1_pressed = 0;
	char button2_pressed = 0;

	while(1) //infinite loop
	{
		if(PINC & (1<<PINC0)) //If Switch1 is released.
		{
			if(button1_pressed)
				PORTD ^= (1<<PIND0); //Toggle LED1.
		
			button1_pressed = 0;
		}
		else //If Switch1 is pressed.
			button1_pressed = 1;

		if(PINC & (1<<PINC1)) //If Switch2 is released.
		{
			if(button2_pressed)
				PORTD ^= (1<<PIND1); //Toggle LED2.
		
			button2_pressed = 0;
		}
		else //If Switch2 is pressed.
			button2_pressed = 1;
	}
	return 0;
}
