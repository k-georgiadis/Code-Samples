
/*
 * ATmega8 Input Capture Mode.c
 * Use With Timer/Counter1
 * Created: 5/27/2020 6:03:43 PM
 * Author: ���������� ��������
 */

#include "avr/io.h"
int main ()
{
    unsigned int t;
    DDRC = 0xFF;	/*����C �� ������*/
    DDRB = 0xFF;	/*����� �� ������*/
    TCCR1A = 0;
    TIFR = (1<<ICF1); /* clear input capture flag */
    TCCR1B = 0x41; /* ������� ���� ����� capture on rising edge */ 

    /* �������������� ��� ���� ��������� - monitor for capture */ 
    while ((TIFR&(1<<ICF1)) == 0); 

    t = ICR1;
    TIFR = (1<<ICF1); /*���������� ��� ������� clear capture flag */

    /* monitor for next rising edge capture */
    while ((TIFR&(1<<ICF1)) == 0); 

    t = ICR1 - t; 	/* period = recent capture - previous capture */
    PORTC = t; 	/* put period count on PORTC & PORTB */	
    PORTB = t>>8;	/* ������� �� �������� 8 �����, �� ����� Byte*/ 
 			/* ��� �������� �����������.*/ 
    while (1);    
    return 0;
}
