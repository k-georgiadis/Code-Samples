/*
 *PWM FAST TIMER1 16BIT.c
 * Duty Cycle Control από 1% έως 100%
 * Created: 22/5/2022 7:24:45 μμ
 * Author : Γεωργιάδης Γεωργιάδης
 */ 


#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

void FPWM_init()
{
 /* ρύθμιση σε γρήγορη λειτουργία PWM μη μη ανεστραμμένη έξοδο. (set fast PWM mode with   
   non-inverted output) */
TCCR1A = (1<<WGM10) | (0<<WGM11) | (0<<COM1A0) | (1<<COM1A1);
TCCR1B = (1<<WGM12) | (0<<WGM13) | (0<<CS12) | (0<<CS11)|(1<<CS10);
DDRB|=(1<<PINB1);  /* PinB OC1A ώς έξοδος. Set OC1A pin as output*/

}

int main ()
{
	int16_t duty;

	FPWM_init();
	while (1)
	{
		  for(duty=0; duty<=255; duty++)
		{
			OCR1A=duty; /* αύξηση του κύκλου λειτουργία (increase Duty Cycle) */
			_delay_ms(40);
		}
		  for(duty=255; duty>=0; duty--)
		{
			OCR1A=duty;  /*Μείωση του κύκλου εργασίας (decrease Duty Cycle)*/
			_delay_ms(60);
		}
	
    }
}

