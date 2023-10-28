
/*
 * ANALOG COMPRARATOR ADC PIN.cpp
 * Created: 4/2/2020 3:15:29 PM
 * ATmega328P_or ATMega8 Analog Comparator.
 * Author : GEORGIADIS GEORGE
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

int main()   
{
	DDRD |= 0x01;  //Ο ακροδέκτης (1) της PORTD γίνεται Έξοδος.Make pin (1) of PORTD as output. 
	ADCSRA &= (1<<ADEN); // Disable ADC. Απενεργοποίηση του  ADC.
	ADMUX = 0x00; //Επιλογή των ακίδων του καναλιού ADC0 ως αρνητική Έισοδο.          .
                                           //Select ADC0 as a -ve pin comparator.
	SFIOR |= (1<<ACME); //Ενεργοποίηση του Αναλογικού Συγκριτή.Enable analog comparator.
    ACSR = 0x00;       // καθαρισμός του καταχωρητή ACSR. Clear ACSR resister.
	
	while(1)
	{
		if (ACSR & (1<<ACO)) // Έλεγχος του ACO bit του ACSR καταχωρητή. 
                                                                 //Check ACO bit of ACSR register.
		PORTD = 0x01; // Ενεργοποίηση του ακροδέκτη PD1. Turn ON PD1 pin. 
		else		  // Εάν το Bit ACO είναι (0). If ACO bit is zero.
		PORTD = 0x00; // Απενεργοποίηση του ακροδέκτη PD1. Then turn OFF PD1 pin.     						
	}	
return 0;
}
