/*
 * ANALOG COMRARATOR WITH INT-ISP.cpp
 *
 * Created: 4/3/2020 6:40:11 PM
 * Author : ΓΕΩΡΓΙΑΔΗΣ ΓΕΩΡΓΙΟΣ
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

void initComparator(void)
{
	cli();               //Καθαρισμός τών διακοπών του συστήματος 
	ACSR |= (1<<ACIE);	//Ενεργοποίηση τής διακοπής του Αναλογικού Συγκριτή.
	                    //Enable analog comparator interrupt
	sei(); //Ενεργοποίηση τών διακοπών του συστήματος
}

void initIO(void){
	DDRD = 0b00000001;	//Ο ακροδέκτης (0) τής PORTD γίνεται Έξοδος.PortD as Outputs.
	//PORTD = 0b00000001;	//Η ακίδα έξόδου 0 με ύψηλό λογικό (1).PORTD Pin (0) outputs high.
}

int main(void)
{
	initIO();			//Initialize I/O
	initComparator();	//Αρχικοποιηση Initialize the analog comparator.
	
	while(1)
{  
	
	}
}

ISR (ANA_COMP_vect) //Υπηρεσία ρουτίνας διακοπής λειτουργίας.(Interrupt Service Routine).
{
                   //Check for rising or falling edge.Ελέγξτε γιά άνοδο ή πτώση άκρων.
	if (ACSR & (1<<ACO)) PORTD = 0x00;
	else PORTD = 0b00000001;
}