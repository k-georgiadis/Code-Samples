/*
 * GccApplication1.cpp
 * Created: 7/10/2018 6:32:33 PM
 * Author: Γεωργιάδης Γεώργιος
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>                 /* Defines pins, ports, etc.*/
#include <util/delay.h>             /* Functions to waste time */

int main(void)
{
    DDRD |= 0b11111111; 	// Data Direction Register D: writing
				// a one to the bits enables output.

    // Όλοι οι ακροδέκτες της Θύρας PORTD γίνονται Έξοδος λογικό(1)
    while (1) // ’πειρος βρόχος.
    {
        PORTD |=0b00000010; // Set (PD1) as HIGH.
 				  // Ο ακροδέκτης (1) τής Θύρας PORTD
 				  // γίνεται λογικό (1) υψηλό δυναμικό
        _delay_ms(200);     // Περιμένει 200 χιλ. του δευτερολέπτου

        PORTD = 0b00000000; // Απενεργοποιούνται όλοι οι ακροδέκτες 
		                    //της Θύρας PORTD, γίνονται λογικό (0)  				   
        /* Turn off all D pins, including LED */ 
        _delay_ms (200);    // Περιμένει 200 χιλ. του δευτερολέπτου
    }                       /* Τερματισμός του Βρόγχου */
    return 0; 		  /* This line is never reached */
}
