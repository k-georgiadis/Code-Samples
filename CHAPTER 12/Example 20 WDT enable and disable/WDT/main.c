/*
 * ATmega8_watchdog_timer WDT.c
 * Watchdog timer enables with typical timeout period 2.1second.
 * Created: 7/21/2020 1:29:49 PM
 * Author : √≈Ÿ—√…¡ƒ«” √≈Ÿ—√…œ”
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/wdt.h>

#define LED_DDR		DDRD
#define LED_PORT	PORTD

void WDT_ON()
{
	WDTCR = (1 << WDCE) | (1 << WDE); //Enable WDT system reset.
	WDTCR |= (1 << WDP2) | (1 << WDP1) | (1 << WDP0); //Set timeout = 2.1s
}

void WDT_OFF()
{
	/*
		This function is used for disabling the watchdog timer.
	*/
	WDTCR = (1 << WDCE) | (1 << WDE);
	WDTCR = 0x00;
}

int main(void)
{
	LED_DDR = (1 << PIND7);	//PD7 as output.
	
	WDT_ON(); //Enable the watchdog timer.
				
	LED_PORT = (1 << PIND7); //Set PD7 pin to logic high.
	_delay_ms(1000); //Wait for 1 second.				
	LED_PORT &= ~(1 << PIND7); //Clear PD7 pin.
	
    while(1){}
}
