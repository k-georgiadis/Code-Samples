/*
 * GccApplication1.cpp
 * Created: 7/10/2018 6:32:33 PM
 * Author: ���������� ��������
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>                 /* Defines pins, ports, etc.*/
#include <util/delay.h>             /* Functions to waste time */

int main(void)
{
    DDRD |= 0b11111111; 	// Data Direction Register D: writing
				// a one to the bits enables output.

    // ���� �� ���������� ��� ����� PORTD �������� ������ ������(1)
    while (1) // ������� ������.
    {
        PORTD |=0b00000010; // Set (PD1) as HIGH.
 				  // � ���������� (1) ��� ����� PORTD
 				  // ������� ������ (1) ����� ��������
        _delay_ms(200);     // ��������� 200 ���. ��� �������������

        PORTD = 0b00000000; // ����������������� ���� �� ���������� 
		                    //��� ����� PORTD, �������� ������ (0)  				   
        /* Turn off all D pins, including LED */ 
        _delay_ms (200);    // ��������� 200 ���. ��� �������������
    }                       /* ����������� ��� ������� */
    return 0; 		  /* This line is never reached */
}
