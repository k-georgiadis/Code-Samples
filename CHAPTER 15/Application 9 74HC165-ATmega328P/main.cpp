/*
 * 74HC165-ATmega328P.cpp
 *
 * Created: 6/7/2022 11:49:13 πμ
 * Author : thalis
 */ 


#include <avr/io.h>
/*74HC165 read function*/
volatile char readInput(void)
{
	char reader=0;
	/*PINB2 as input*/
	DDRB=0x03;
	/*Raise LD pin*/
	PORTB|=(1<<1);
	for (int i=0;i<8;i++)
	{
		reader<<=1;   /*Get a one-bit digital input*/
		reader|=((PINB&0x04)?1:0);
		/*Clock the register*/
		PORTB&=~(1<<0);
		for(int i=0;i<200;i++); //0.1 ms = 0.0001 s (8MHz)
		PORTB|=(1<<0);
	}
	/*Low the LD pin*/
	PORTB&=~(1<<1);
	
	return reader;	
}
int main(void)
{
	DDRD=0xFF;
	while (1)
	{
		PORTD=readInput();
	}
}