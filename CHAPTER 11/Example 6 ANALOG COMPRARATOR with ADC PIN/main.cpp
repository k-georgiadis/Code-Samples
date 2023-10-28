
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
	DDRD |= 0x01;  //� ���������� (1) ��� PORTD ������� ������.Make pin (1) of PORTD as output. 
	ADCSRA &= (1<<ADEN); // Disable ADC. �������������� ���  ADC.
	ADMUX = 0x00; //������� ��� ������ ��� �������� ADC0 �� �������� ������.          .
                                           //Select ADC0 as a -ve pin comparator.
	SFIOR |= (1<<ACME); //������������ ��� ���������� ��������.Enable analog comparator.
    ACSR = 0x00;       // ���������� ��� ���������� ACSR. Clear ACSR resister.
	
	while(1)
	{
		if (ACSR & (1<<ACO)) // ������� ��� ACO bit ��� ACSR ����������. 
                                                                 //Check ACO bit of ACSR register.
		PORTD = 0x01; // ������������ ��� ��������� PD1. Turn ON PD1 pin. 
		else		  // ��� �� Bit ACO ����� (0). If ACO bit is zero.
		PORTD = 0x00; // �������������� ��� ��������� PD1. Then turn OFF PD1 pin.     						
	}	
return 0;
}
