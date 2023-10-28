/*
 * Generate Different Pulses.c
 *
 * Created: 7/3/2020 10:40:21 AM
 * Author : цеыяциадгс цеыяциос
 */ 

#include <avr/io.h>


int main(void)

{
	DDRB |= (1<<1);		/*PB1 (OC1A) as output */
	while (1)
	{
		OCR1A = 139;
		TCCR1A = 0xC0;	/* CTC, set on match, no Prescaler */
		TCCR1B = 0x09;
		while ((TIFR&(1<<OCF1A)) == 0);  /* monitor OCF1A flag */
		TIFR = (1<<OCF1A);/* Clear OCF1A by writing 1 */
		OCR1A = 169;
		TCCR1A = 0x80;	/* CTC, clear on match, no Prescaler */
		TCCR1B = 0x09;
		while ((TIFR&(1<<OCF1A)) == 0);
		TIFR = (1<<OCF1A);/* Clear OCF1A by writing 1 */
	}
}
