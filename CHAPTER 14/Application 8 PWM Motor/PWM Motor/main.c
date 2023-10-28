/*
 * PWM Motor.c
 * Phase and Frequency Correct PWM Mode.
 * Created: 21/1/2021 13:25:19 μμ
 * Author : ΚΟΣΜΑΣ ΓΕΩΡΓΙΑΔΗΣ
 */ 
 
 #define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t changed = 0;
uint8_t duty_cycle;

int main(void)
{
	DDRB = 0x2; //Make PINB1 as output (OC1A).
	DDRD = 0x7; //PD0, PD1, as output (motor direction control)
	DDRC = 0xB; //Make buttons as output.

	PORTC = 0xB; //Set buttons high (1).
	PORTD = 0x1; //Default direction is right.

	TCCR1A |= (1 << COM1A1) | (1 << COM1A0); //Set inverted mode. (low at bottom, high on match)
	TCCR1A |= (1 << WGM13) | (1 << WGM10); //10bit phase corrected and frequency PWM Mode.
	TIMSK |= (1 << OCIE1A); //Enable compare-match interrupt.

	//PWM_frequency = clock_speed / (2 * Prescaler_value * TOP)
	//OCR1A = F_CPU / (2 * 1024 * TOP);

	duty_cycle = 50; //Default duty cycle (%).
	OCR1A = 255 - (duty_cycle / 100.0) * 255; //Set OCR1A value.
	
	TCCR1B |= (1 << CS12) | (0 << CS11) | (1 << CS10) ; //Set prescaler to 1024 and start PWM.
	sei(); //Enable global interrupts.

    while (1) 
    {
		//Increase duty cycle.
		if((PINC & (1 << PINC0)) == 0) //right button PC0
		{
			if(duty_cycle < 100)
			{
				duty_cycle++;

				changed = 1;
				_delay_ms(100);
			}
		}
		//Decrease duty cycle.
		else if((PINC & (1 << PINC3)) == 0) //left button PC3
		{
			if(duty_cycle > 0)
			{
				duty_cycle--;

				changed = 1;
				_delay_ms(100);
			}	
		}
		//Change direction.
		else if((PINC & (1 << PINC1)) == 0) //middle button PC1
		{
			_delay_ms(100);

			//Toggle motor ON/OFF status if button is pressed for enough time.
			if((PINC & (1 << PINC1)) == 0)
			{
				_delay_ms(500);

				if((PINC & (1 << PINC1)) == 0)
				{
					PORTD &= 1;
					PORTD ^= 1;
				}
			}
			else if((PORTD & 3) != 0) //Toggle direction.
				PORTD ^= 0x3;
		}
		//PORTB = ((duty_cyle / 10) << 2); //Display duty cycle on leds.
    }
}

ISR(TIMER1_COMPA_vect)
{
	if(changed)
	{
		PORTD |= 4;
		changed = 0;
		TCNT1 = 0;
		OCR1A = 255 - (duty_cycle / 100.0) * 255; //Change TOP value.
	}
}