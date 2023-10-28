/*
 * ANALOG COMRARATOR WITH INT-ISP.cpp
 *
 * Created: 4/3/2020 6:40:11 PM
 * Author : ���������� ��������
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

void initComparator(void)
{
	cli();               //���������� ��� �������� ��� ���������� 
	ACSR |= (1<<ACIE);	//������������ ��� �������� ��� ���������� ��������.
	                    //Enable analog comparator interrupt
	sei(); //������������ ��� �������� ��� ����������
}

void initIO(void){
	DDRD = 0b00000001;	//� ���������� (0) ��� PORTD ������� ������.PortD as Outputs.
	//PORTD = 0b00000001;	//� ����� ������ 0 �� ����� ������ (1).PORTD Pin (0) outputs high.
}

int main(void)
{
	initIO();			//Initialize I/O
	initComparator();	//������������ Initialize the analog comparator.
	
	while(1)
{  
	
	}
}

ISR (ANA_COMP_vect) //�������� �������� �������� �����������.(Interrupt Service Routine).
{
                   //Check for rising or falling edge.������� ��� ����� � ����� �����.
	if (ACSR & (1<<ACO)) PORTD = 0x00;
	else PORTD = 0b00000001;
}