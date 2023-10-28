/*
 * ANALOG COMPARATOR.c
 * Created: 4/19/2022 11:54:01 AM
 * Author: GEORGIADIS GEORGE
 * ATmega328P_Analog_Comparator.
 */ 
#define F_CPU 12000000UL
#include <avr/io.h>
#include <util/delay.h>

int main (){
    DDRD |= 0x01; 		// Make pin (0) of PORTD is output.
    ADCSRB &= (1<<ACME); 	// Enable AIN1 for -ve input.
    ACSR &= 0x00;	 	// Clear ACSR register.

    while (1)
    {
        if (ACSR & (1<<ACO)) // Check ACO bit of ACSR register.
            PORTD = 0x01; 	   // Turn ON PD0 pin.
        else	 		   // If ACO bit is zero.
            PORTD = 0x00; 	   //Then turn OFF PD0 pin.
    }
}
