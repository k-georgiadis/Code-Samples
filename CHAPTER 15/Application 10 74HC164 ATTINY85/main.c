/*
 * 74HC164 ATTINY85.cpp
 *
 * Created: 11/7/2022 1:15:21 μμ
 * Author : Κοσμάς Γεωργιάδης
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

char digit_out[2], cur_dig;

char buf[] = 
{     // πίνακας με αριθμούς
	0b00000011, //0
	0b10011111, //1
	0b00100101, //2
	0b00001101, //3
	0b10011001, //4
	0b01001001, //5
	0b01000001, //6
	0b00011111, //7
	0b00000001, //8
	0b00001001, //9
	0b11111111
};

void hc164_wr(unsigned char dig)
{  
	//register output subroutines
	unsigned char a;
	
	for(a = 0; a < 8; a++)
	{      
		unsigned char x = buf[dig] & 1 << a; //εξάγετε το τρέχον bit στη γραμμή DATA.
		
		PORTB |= (1 << PINB1);
		
		//loop για την έξοδο 8 bit δεδομένων από έναν πίνακα‎
		if(x)
			PORTB |= (1 << PINB0); 
		else
			PORTB &= ~(1 << PINB0);
			
		PORTB &= ~(1 << PINB1); //Παραλείψτε τη γραμμή CLK 
		PORTB |= (1 << PINB1); //Σηκώστε τη γραμμή CLK 
	}
	
	return;
}

ISR(TIMER0_OVF_vect)
{
	switch (cur_dig)
	{
		case 0:
			PORTB &= ~(1 << PINB3);
			PORTB &= ~(1 << PINB2);
			hc164_wr(digit_out[cur_dig]);
			PORTB |= (1 << PINB2);
			break;  //πρώτο ψηφίο
		case 1:
			PORTB &= ~(1 << PINB3);
			PORTB &= ~(1 << PINB2);
			hc164_wr(digit_out[cur_dig]);
			PORTB |= (1 << PINB3);
			break;  //δεύτερο ψηφίο
	}
	cur_dig++;
	
	if(cur_dig == 2)
		cur_dig = 0;
		
}

void main(void)
{
	unsigned char f;
	
	// Port B initialization
	PORTB = 0;//Αρχικοποίηση θύρας Β‎
	DDRB = 0xFF;
	
	// Timer/Counter 0 initialization
	TCCR0A = 0; //‎Αρχικοποίηση χρονοδιακόπτη/μετρητή 0‎
	TCCR0B = 0x03;
	
	// Timer/Counter 0 Interrupt(s) initialization
	
	TIMSK = 0x02; //Αρχικοποίηση διακοπών χρονοδιακόπτη/μετρητή 0‎
	
	// Global enable interrupts
	sei(); //‎Διακοπές καθολικής ενεργοποίησης‎

	while (1)	
	{
		for(f = 0; f < 100; f++)
		{			
			_delay_ms(100); //καθυστέρηση 
			
			digit_out[0]=f/10;
			digit_out[1]=f%10;
		}
	}
}