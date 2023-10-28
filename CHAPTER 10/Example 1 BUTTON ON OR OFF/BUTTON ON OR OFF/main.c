/*
 * BUTTON ON OR OFF.c
 *
 * Created: 12/24/2019 2:32:36 PM
 * Author : ���������� ��������
 */ 


#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
//#include <asf.h>
 
    
int main(void) /* Replace with your application code */
{
   
DDRC = 0b00000001; //PC0 as output.� ���������� PC0 ��� PORTC ������� �������
PORTC = 0b00000001; //Set button (PC0) as HIGH.� ���������� (0) ��� ����� PORTC ������� ������ (1)

DDRD = 0b00000001; //PD0 as output.
//PORTD = 0b00000001; //Turn OFF LED (PD0).

while(1) //infinite loop
{
	
	
	if(PINC & (1<<PINC0)) //If switch is released
	PORTD &= ~(1<<PIND0); //Turns OFF LED
	else //If switch is pressed.
	PORTD |= (1<<PIND0); //Turns ON LED
}
}